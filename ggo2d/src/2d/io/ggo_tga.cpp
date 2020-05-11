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

  //////////////////////////////////////////////////////////////
  // Write pixels (note that pixels are stored BGR from top to bottom).
  struct write_pixels
  {
    template <ggo::pixel_type pixel_type, ggo::lines_order memory_lines_order>
    static void call(std::ofstream & ofs, const void * buffer, int width, int height, int line_byte_step)
    {
      ggo::const_image_t<pixel_type, memory_lines_order> img(buffer, { width, height }, line_byte_step);

      for (int y = 0; y < height; ++y)
      {
        // PIxels are stored top down.
        const void * ptr = img.line_ptr(img.height() - y - 1);

        for (int x = 0; x < width; ++x)
        {
          auto c = ggo::pixel_type_traits<pixel_type>::read(ptr);
          ggo::rgb_8u rgb = ggo::convert_color_to<ggo::rgb_8u>(c);
          ofs.write(reinterpret_cast<char*>(&rgb.b()), 1);
          ofs.write(reinterpret_cast<char*>(&rgb.g()), 1);
          ofs.write(reinterpret_cast<char*>(&rgb.r()), 1);
          ptr = ggo::move_ptr<ggo::pixel_type_traits<pixel_type>::pixel_byte_size>(ptr);
        }
      }
    }
  };
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

    const lines_order memory_lines_order = (header._image_descriptor & (1 << 5)) ? lines_order::down : lines_order::down;

    ggo::image image({ header._width, header._height }, pixel_type::bgr_8u, memory_lines_order);
    ifs.read(reinterpret_cast<char *>(image.data()), image.size().height() * image.line_byte_step());

    return image;
  }

  //////////////////////////////////////////////////////////////
  bool save_tga(const std::string & filename, const void * buffer, ggo::pixel_type pixel_type, ggo::lines_order memory_lines_order, int width, int height, int line_byte_step)
  {
    std::ofstream ofs(filename.c_str(), std::ios_base::binary);

    // Header.
    header header;
    memset(&header, 0, sizeof(header));

    header._image_type = 2;
    header._width = width;
    header._height = height;
    header._bpp = 24;
    header._image_descriptor = (1 << 5); // Always down.

    ofs.write(reinterpret_cast<char*>(&header), sizeof(header));

    dispatch_image_format<write_pixels>(pixel_type, memory_lines_order, ofs, buffer, width, height, line_byte_step);

    if (!ofs)
    {
      return false;
    }

    return true;
  }
}

