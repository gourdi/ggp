#ifndef __GGO_BMP__
#define __GGO_BMP__

#include <string>
#include <ggo_pixel_buffer.h>

namespace ggo
{
  pixel_buffer  load_bmp(const std::string & filename);
  bool          save_bmp(const std::string & filename, const void * buffer, pixel_buffer_format pbf, int width, int height, int line_byte_step);
}

#endif


