#include <2d/io/ggo_tga.h>
#include <fstream>

namespace
{
#pragma pack(push, 1)
  struct header
  {
    uint8_t _id_length;
    uint8_t _colormap_type;
    uint8_t _image_type;
    uint16_t _colormap_first;
    uint16_t _colormap_length;
    uint8_t _colormap_entry_size;
    uint16_t _x_origin;
    uint16_t _y_origin;
    uint16_t _width;
    uint16_t _height;
    uint8_t _bpp;
    uint8_t _image_descriptor;
  };
#pragma pack(pop)

  static_assert(sizeof(header) == 18);
}

namespace ggo
{
  //////////////////////////////////////////////////////////////
  image load_tga(const std::string & filename)
  {
    std::ifstream ifs(filename.c_str(), std::ios_base::binary);

    // Header.
    header header;
    ifs.read(reinterpret_cast<char *>(&header), sizeof(header));

    if (header._image_type != 2 || header._bpp != 24)
    {
      throw std::runtime_error("unsupported image format");
    }
    if (header._image_descriptor & (1 << 4)) // Right-to-left pixels is unsupported.
    {
      throw std::runtime_error("unsupported right-to-left tga file");
    }

    // Pixels.
    ifs.seekg(header._id_length, std::ios_base::cur);

    std::unique_ptr<memory_layout> mem_layout;
    if (header._image_descriptor & (1 << 5))
    {
      mem_layout.reset(new rows_memory_layout<3, vertical_direction::down>({ header._width, header._height }, 3 * header._width));
    }
    else
    {
      mem_layout.reset(new rows_memory_layout<3, vertical_direction::up>({ header._width, header._height }, 3 * header._width));
    }

    ggo::image image(pixel_type::bgr_8u, std::move(mem_layout));
    ifs.read(reinterpret_cast<char *>(image.data()), 3 * image.size().height() * image.size().width());

    return image;
  }

  //////////////////////////////////////////////////////////////
  bool save_tga(const std::string & filename, const void * buffer, ggo::pixel_type pixel_type, const memory_layout & mem_layout)
  {
    std::ofstream ofs(filename.c_str(), std::ios_base::binary);

    // Header.
    header header;
    memset(&header, 0, sizeof(header));

    header._image_type = 2;
    header._width = mem_layout.size().width();
    header._height = mem_layout.size().height();
    header._bpp = 24;
    header._image_descriptor = (1 << 5); // Always down.

    ofs.write(reinterpret_cast<char*>(&header), sizeof(header));

    for (int y = 0; y < mem_layout.size().height(); ++y)
    {
      for (int x = 0; x < mem_layout.size().width(); ++x)
      {
        const void * in_ptr = mem_layout.at(buffer, x, mem_layout.size().height() - y - 1); // Pixels are stored top down.

        rgb_8u rgb = ggo::read_rgb_8u(in_ptr, pixel_type);

        ofs.write(reinterpret_cast<char*>(&rgb.b()), 1);
        ofs.write(reinterpret_cast<char*>(&rgb.g()), 1);
        ofs.write(reinterpret_cast<char*>(&rgb.r()), 1);
      }
    }

    if (!ofs)
    {
      return false;
    }

    return true;
  }
}

