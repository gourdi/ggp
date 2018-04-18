#ifndef __GGO_JPG__
#define __GGO_JPG__

#include <string>
#include <2d/ggo_image.h>

namespace ggo
{
  image load_jpg(const std::string & filename);
  bool  save_jpg(const std::string & filename, int quality, const void * buffer, image_format format, int width, int height, int line_byte_step);
}

#endif


