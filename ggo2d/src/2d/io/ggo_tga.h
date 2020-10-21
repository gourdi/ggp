#pragma once

#include <2d/ggo_image.h>

namespace ggo
{
  image load_tga(const std::string & filename);
  bool  save_tga(const std::string & filename, const void * buffer, ggo::pixel_type pixel_type, const memory_layout & mem_layout);
}
