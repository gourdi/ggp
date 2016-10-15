#ifndef __GGO_BMP_CODEC__
#define __GGO_BMP_CODEC__

#include <string>
#include <stdint.h>
#include <ggo_pixel_buffer.h>

namespace ggo
{
  bool save_bmp(const std::string & filename, const void * rgb, pixel_buffer_format pbf, int width, int height, int line_step);
}

#endif


