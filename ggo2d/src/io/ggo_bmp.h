#ifndef __GGO_BMP__
#define __GGO_BMP__

#include <string>
#include <ggo_pixel_buffer.h>

namespace ggo
{
  pixel_buffer  load_bmp(const std::string & filename);
  bool          save_bmp(const std::string & filename, const void * buffer, pixel_buffer_format pbf, int width, int height, int line_byte_step);
}

namespace ggo
{
  template <typename size_t>
  bool save_bmp(const std::string & filename, const void * buffer, pixel_buffer_format pbf, size_t width, size_t height, size_t line_byte_step)
  {
    static_assert(std::is_integral<size_t>::value);
    return save_bmp(filename, buffer, pbf, int(width), int(height), int(line_byte_step));
  }
}

#endif


