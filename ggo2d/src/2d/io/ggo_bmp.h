#pragma once

#include <string>
#include <kernel/memory/ggo_memory_layouts.h>
#include <2d/ggo_image.h>

namespace ggo
{
  image load_bmp(const std::string & filename);
  bool  save_bmp(const std::string & filename, const void * buffer, ggo::pixel_type pixel_type, const memory_layout & mem_layout);
}

namespace ggo
{
  template <typename image_t>
  bool save_bmp(const std::string & filename, const image_t & image)
  {
    return save_bmp(filename, image.data(), image.pixel_type(), image.memory_layout());
  }
}
