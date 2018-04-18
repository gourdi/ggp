#ifndef __GGO_BMP__
#define __GGO_BMP__

#include <string>
#include <2d/ggo_image.h>

namespace ggo
{
  image load_bmp(const std::string & filename);
  bool  save_bmp(const std::string & filename, const void * buffer, image_format format, int width, int height, int line_byte_step);
}

namespace ggo
{
  template <typename size_t>
  bool save_bmp(const std::string & filename, const void * buffer, image_format format, size_t width, size_t height, size_t line_byte_step)
  {
    static_assert(std::is_integral<size_t>::value);
    return save_bmp(filename, buffer, format, int(width), int(height), int(line_byte_step));
  }
}

#endif


