#ifndef __GGO_TGA__
#define __GGO_TGA__

#include <ggo_image.h>

namespace ggo
{
  image load_tga(const std::string & filename);
  bool  save_tga(const std::string & filename, const void * buffer, image_format format, int width, int height, int line_step);
}

#endif
