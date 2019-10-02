#ifndef __GGO_TGA__
#define __GGO_TGA__

#include <2d/ggo_image.h>

namespace ggo
{
  image load_tga(const std::string & filename);
  bool  save_tga(const std::string & filename, const void * buffer, ggo::pixel_type pixel_type, ggo::lines_order memory_lines_order, int width, int height, int line_byte_step);
}

#endif
