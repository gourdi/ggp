#include "ggo_bmp.h"
#include <kernel/ggo_kernel.h>
#include <kernel/ggo_file_helpers.h>
#include <kernel/memory/ggo_array.h>
#include <fstream>

namespace
{
#pragma pack(push, 1)
  struct file_header
  {
    char _b;
    char _m;
    uint32_t _file_size;
    uint16_t _reserved1;
    uint16_t _reserved2;
    uint32_t _offset;
  };

  struct info_header
  {
    uint32_t _size;
    uint32_t _width;
    uint32_t _height;
    uint16_t _planes;
    uint16_t _bpp;
    uint32_t _compression;
    uint32_t _image_size;
    uint32_t _horizontal_resolution;
    uint32_t _vertical_resolution;
    uint32_t _palette_size;
    uint32_t _important;
  };
#pragma pack(pop)

  static_assert(sizeof(file_header) == 14);
  static_assert(sizeof(info_header) == 40);

  //////////////////////////////////////////////////////////////
  // Write pixels (note that pixels are stored BGR from bottom to top).
  struct write_pixels
  {
    template <ggo::image_format format>
    static void call(std::ofstream & ofs, const void * buffer, int width, int height, int line_byte_step, int padded_line_size)
    {
      using format_traits = ggo::image_format_traits<format>;

      ggo::array_8u padded_line(padded_line_size);

      for (int y = 0; y < height; ++y)
      {
        const void * in_ptr = ggo::get_line_ptr<format>(buffer, y, height, line_byte_step);
        uint8_t * out_ptr = padded_line.data();

        for (int x = 0; x < width; ++x)
        {
          auto c = ggo::read_pixel<format>(in_ptr);
          ggo::color_8u rgb = ggo::convert_color_to<ggo::color_8u>(c);

          *out_ptr++ = rgb.b();
          *out_ptr++ = rgb.g();
          *out_ptr++ = rgb.r();

          in_ptr = ggo::ptr_offset<format_traits::pixel_byte_size>(in_ptr);
        }

        ofs.write(reinterpret_cast<char *>(padded_line.data()), padded_line_size);
      }
    }
  };
}

namespace ggo
{
  //////////////////////////////////////////////////////////////
  image load_bmp(const std::string & filename)
  {
    std::ifstream ifs(filename.c_str(), std::ios_base::binary);

    // File header.
    file_header file_header;
    ifs.read(reinterpret_cast<char *>(&file_header), sizeof(file_header));

    if (file_header._b != 'B' || file_header._m != 'M')
    {
      throw std::runtime_error("invalid header");
    }
    if (file_header._offset != 54)
    {
      throw std::runtime_error("invalid offset");
    }

    // Info header.
    info_header info_header;
    ifs.read(reinterpret_cast<char *>(&info_header), sizeof(info_header));

    if (info_header._size != 40)
    {
      throw std::runtime_error("invalid bitmap info header size");
    }
    if (info_header._planes != 1 || info_header._bpp != 24)
    {
      throw std::runtime_error("invalid bpp");
    }
    if (info_header._compression != 0)
    {
      throw std::runtime_error("unsupported compressed bitmap");
    }
    if (info_header._palette_size)
    {
      throw std::runtime_error("unsupported color mapped bitmap");
    }

    // Pixels.
    int line_byte_size = ggo::pad(3 * info_header._width, 4);
    image image(info_header._width, info_header._height, line_byte_size, ggo::bgr_8u_yu);

    ifs.read(reinterpret_cast<char *>(image.data()), info_header._height * line_byte_size);

    // Everything went well?
    if (!ifs)
    {
      throw std::runtime_error("invalid bitmap file");
    }

    return image;
  }

  //////////////////////////////////////////////////////////////
  bool save_bmp(const std::string & filename, const void * buffer, image_format format, int width, int height, int line_byte_step)
  {
    std::ofstream ofs(filename.c_str(), std::ios_base::binary);

    int padded_line_size	= ggo::pad(3 * width, 4);

    // File header.
    file_header file_header;
    memset(&file_header, 0, sizeof(file_header));

    file_header._b = 'B';
    file_header._m = 'M';
    file_header._offset = sizeof(file_header) + sizeof(info_header);
    file_header._file_size = file_header._offset + 3 * padded_line_size * height;

    ofs.write(reinterpret_cast<char*>(&file_header), sizeof(file_header));

    // Info header.
    info_header info_header;
    memset(&info_header, 0, sizeof(info_header));

    info_header._size = sizeof(info_header);
    info_header._width = width;
    info_header._height = height;
    info_header._planes = 1;
    info_header._bpp = 24;

    ofs.write(reinterpret_cast<char*>(&info_header), sizeof(info_header));

    ggo::dispatch_image_format<write_pixels>(format, ofs, buffer, width, height, line_byte_step, padded_line_size);

    if (!ofs)
    {
      return false;
    }

    return true;
  }
}