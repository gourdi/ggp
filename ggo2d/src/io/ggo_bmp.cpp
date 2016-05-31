#include "ggo_bmp.h"
#include <ggo_kernel.h>
#include <ggo_memory.h>
#include <ggo_array.h>
#include <fstream>

namespace ggo
{
  //////////////////////////////////////////////////////////////
  bool save_bmp(const std::string & filename, const uint8_t * rgb, int width, int height)
  {
    uint8_t			  header[40];
    std::ofstream stream(filename.c_str(), std::ios_base::out | std::ios_base::binary);

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

    stream.write(reinterpret_cast<char*>(header), 14);

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

    stream.write(reinterpret_cast<char*>(header), 40);

    // Write pixels (note that pixels are stored BGR from bottom to top.
    for (int y = 0; y < height; ++y)
    {
      for (int x = 0; x < width; ++x)
      {
        stream.write(reinterpret_cast<const char*>(rgb + 2), 1);
        stream.write(reinterpret_cast<const char*>(rgb + 1), 1);
        stream.write(reinterpret_cast<const char*>(rgb + 0), 1);
        rgb += 3;
      }
    }
    if (!stream)
    {
      return false;
    }

    return true;
  }
}
