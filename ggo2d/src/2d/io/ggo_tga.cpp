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
    template <ggo::image_format format>
    static void call(std::ofstream & ofs, const void * buffer, int width, int height, int line_byte_step)
    {
      using format_traits = ggo::image_format_traits<format>;

      for (int y = 0; y < height; ++y)
      {
        const void * ptr = ggo::get_line_ptr<format_traits::lines_order>(buffer, height - y - 1, height, line_byte_step);

        for (int x = 0; x < width; ++x)
        {
          auto c = ggo::read_pixel<format>(ptr);
          ggo::rgb_8u rgb = ggo::convert_color_to<ggo::rgb_8u>(c);
          ofs.write(reinterpret_cast<char*>(&rgb.b()), 1);
          ofs.write(reinterpret_cast<char*>(&rgb.g()), 1);
          ofs.write(reinterpret_cast<char*>(&rgb.r()), 1);
          ptr = ggo::ptr_offset<format_traits::pixel_byte_size>(ptr);
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

    ggo::image_format format = (header._image_descriptor & (1 << 5)) ? bgr_8u_yd : bgr_8u_yu;

    ggo::image image(header._width, header._height, 3 * header._width, format);
    ifs.read(reinterpret_cast<char *>(image.data()), image.height() * image.line_byte_step());

    return image;
  }

  //////////////////////////////////////////////////////////////
  bool save_tga(const std::string & filename, const void * buffer, image_format format, int width, int height, int line_byte_step)
  {
    std::ofstream ofs(filename.c_str(), std::ios_base::binary);

    int line_size = ggo::pad(3 * width, 4);

    // Header.
    header header;
    memset(&header, 0, sizeof(header));

    header._image_type = 2;
    header._width = width;
    header._height = height;
    header._bpp = 24;
    header._image_descriptor = (1 << 5); // Always down.

    ofs.write(reinterpret_cast<char*>(&header), sizeof(header));

    ggo::dispatch_image_format<write_pixels>(format, ofs, buffer, width, height, line_byte_step);

    if (!ofs)
    {
      return false;
    }

    return true;
  }
}

