#ifndef __GGO_TGA__
#define __GGO_TGA__

#include <ggo_pixel_buffer.h>

namespace ggo
{
  pixel_buffer	load_tga(const std::string & filename);
  bool			    save_tga(const std::string & filename, const void * buffer, pixel_buffer_format pbf, int width, int height, int line_step);
}

#endif
