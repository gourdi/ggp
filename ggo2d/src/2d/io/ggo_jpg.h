#ifndef __GGO_JPG__
#define __GGO_JPG__

#include <string>
#include <2d/ggo_image.h>

namespace ggo
{
  image load_jpg(const std::string & filename);
  bool  save_jpg(const std::string & filename, int quality, const void * buffer, ggo::pixel_type pixel_type, ggo::lines_order memory_lines_order, int width, int height, int line_byte_step);
}

#endif


