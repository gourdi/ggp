#ifndef __GGO_PNG__
#define __GGO_PNG__

#include <string>
#include <2d/ggo_image.h>

namespace ggo
{
  image load_png(const std::string & filename);
  bool  save_png(const std::string & filename, const void * buffer, image_format format, int width, int height, int line_byte_step);
}

#endif


