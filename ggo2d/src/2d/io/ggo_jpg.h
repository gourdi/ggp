#pragma once

#include <string>
#include <kernel/memory/ggo_memory_layouts.h>
#include <2d/ggo_image.h>

namespace ggo
{
  image load_jpg(const std::string & filename);
  bool  save_jpg(const std::string & filename, int quality, const void * buffer, ggo::pixel_type pixel_type, const memory_layout & mem_layout);
}

