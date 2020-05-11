#pragma once

#include <string>
#include <2d/ggo_image.h>

namespace ggo
{
  image load_bmp(const std::string & filename);
  bool  save_bmp(const std::string & filename, const void * buffer, ggo::pixel_type pixel_type, ggo::lines_order memory_lines_order, int width, int height, int line_byte_step);
}

namespace ggo
{
  template <typename size_t>
  bool save_bmp(const std::string & filename, const void * buffer, ggo::pixel_type pixel_type, ggo::lines_order memory_lines_order, size_t width, size_t height, size_t line_byte_step)
  {
    static_assert(std::is_integral<size_t>::value);
    return save_bmp(filename, buffer, pixel_type, memory_lines_order, int(width), int(height), int(line_byte_step));
  }

  template <typename image_t>
  bool save_bmp(const std::string & filename, const image_t & image)
  {
    return save_bmp(filename, image.data(), image.pixel_type(), image.memory_lines_order(), image.width(), image.height(), image.line_byte_step());
  }
}
