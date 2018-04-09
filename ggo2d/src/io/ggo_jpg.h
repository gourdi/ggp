#ifndef __GGO_JPG__
#define __GGO_JPG__

#include <string>
#include <ggo_pixel_buffer.h>

namespace ggo
{
  bool          is_jpg(const std::string & filename);
  pixel_buffer  load_jpg(const std::string & filename);
  bool          save_jpg(const std::string & filename, int quality, const void * buffer, pixel_buffer_format pbf, int width, int height, int line_byte_step);
}

#endif


