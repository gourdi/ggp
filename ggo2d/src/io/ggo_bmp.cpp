#include "ggo_bmp.h"
#include <ggo_kernel.h>
#include <ggo_memory.h>
#include <ggo_color.h>
#include <fstream>

namespace
{
  //////////////////////////////////////////////////////////////
  // Write pixels (note that pixels are stored BGR from bottom to top.
  template <ggo::pixel_buffer_format pbf>
  void write_pixels(std::ofstream & ofs, const void * buffer, int width, int height, int line_step)
  {
    for (int y = 0; y < height; ++y)
    {
      const void * ptr = ggo::get_line_ptr<pbf>(buffer, y, height, line_step);

      for (int x = 0; x < width; ++x)
      {
        auto c = ggo::read_pixel<pbf>(ptr);
        ggo::color_8u rgb = ggo::convert_color_to<ggo::color_8u>(c);
        ofs.write(reinterpret_cast<char*>(&rgb.b()), 1);
        ofs.write(reinterpret_cast<char*>(&rgb.g()), 1);
        ofs.write(reinterpret_cast<char*>(&rgb.r()), 1);
        ptr = ggo::ptr_offset(ptr, ggo::pixel_buffer_format_info<pbf>::pixel_byte_size);
      }
    }
  }
}

namespace ggo
{
  //////////////////////////////////////////////////////////////
  bool save_bmp(const std::string & filename, const void * buffer, pixel_buffer_format pbf, int width, int height, int line_step)
  {
    uint8_t			  header[40];
    std::ofstream ofs(filename.c_str(), std::ios_base::out | std::ios_base::binary);

    int line_size	= ggo::pad(3 * width, 4);
    int filesize	= 14 + 40 + 3 * line_size * height;

    // File header.
    ggo::mem_zero(header, sizeof(header));

    header[ 0]	= 'B';
    header[ 1]	= 'M';

    header[ 2]	= (filesize&0x000000FF)>>0;
    header[ 3]	= (filesize&0x0000FF00)>>8;
    header[ 4]	= (filesize&0x00FF0000)>>16;
    header[ 5]	= (filesize&0xFF000000)>>24;

    header[10]	= 14+40; // Offset.

    ofs.write(reinterpret_cast<char*>(header), 14);

    // Info header.
    ggo::mem_zero(header, sizeof(header));

    header[ 0] = 40; // sizeof info header

    header[ 4] = (width&0x000000FF)>>0;
    header[ 5] = (width&0x0000FF00)>>8;
    header[ 6] = (width&0x00FF0000)>>16;
    header[ 7] = (width&0xFF000000)>>24;

    header[ 8] = (height&0x000000FF)>>0;
    header[ 9] = (height&0x0000FF00)>>8;
    header[10] = (height&0x00FF0000)>>16;
    header[11] = (height&0xFF000000)>>24;

    header[12] = 1;		// Planes.
    header[14] = 24;	// Bpp.

    ofs.write(reinterpret_cast<char*>(header), 40);

    switch (pbf)
    {
    case ggo::y_8u_yu:
      write_pixels<ggo::y_8u_yu>(ofs, buffer, width, height, line_step);
      break;
    case ggo::rgb_8u_yu:
      write_pixels<ggo::rgb_8u_yu>(ofs, buffer, width, height, line_step);
      break;
    case ggo::bgra_8u_yd:
      write_pixels<ggo::bgra_8u_yd>(ofs, buffer, width, height, line_step);
      break;
    }

    if (!ofs)
    {
      return false;
    }

    return true;
  }
}
