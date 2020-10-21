#pragma once

#include <string>
#include <kernel/memory/ggo_memory_layouts.h>
#include <2d/ggo_image.h>

namespace ggo
{
  image load_png(const std::string & filename);
  bool  save_png(const std::string & filename, const void * buffer, ggo::pixel_type pixel_type, const ggo::memory_layout & mem_layout);
}

