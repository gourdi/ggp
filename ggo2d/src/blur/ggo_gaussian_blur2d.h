#ifndef __GGO_GAUSSIAN_BLUR2D__
#define __GGO_GAUSSIAN_BLUR2D__

#include <ggo_pixel_buffer.h>
#include <ggo_color.h>
#include <ggo_convolution.h>
#include <ggo_gaussian_blur.h>

namespace ggo
{
  ////////////////////////////////////////////////////////////////////
  template <pixel_buffer_format pbf>
  void gaussian_blur2d_8u(void * buffer, const int width, const int height, const int line_step, const float stddev)
  {
    static_assert(std::is_same<pixel_buffer_format_info<pbf>::sample_t, uint8_t>::value, "expecting 8u color type");

    using helper = ggo::color_conv<pixel_buffer_format_info<pbf>::color_t>;

    const int tmp_line_step = pixel_buffer_format_info<pbf>::samples_count * width;
    std::vector<uint8_t> tmp(tmp_line_step * height);

    auto kernel = ggo::build_fixed_point_gaussian_kernel<uint16_t, float>(stddev, 0.001f, 8);

    auto read = [](const void * ptr) {
      return helper::to_16u(ggo::get_pixel<pbf>(ptr));
    };
    auto write = [](void * ptr, const helper::color_t_16u & c) {
      ggo::set_pixel<pbf>(ptr, helper::to_8u(c));
    };

    // First horizontal pass.
    {
      auto left = [&](int x, int y) {
        return read(ggo::get_pixel_ptr<pbf>(buffer, 0, y, height, line_step));
      };
      auto right = [&](int x, int y) {
        return read(ggo::get_pixel_ptr<pbf>(buffer, width - 1, y, height, line_step));
      };

      ggo::apply_symetric_kernel_2d_horz(
        buffer, ggo::pixel_buffer_format_info<pbf>::pixel_byte_size, line_step, read,
        tmp.data(), ggo::pixel_buffer_format_info<pbf>::pixel_byte_size, tmp_line_step, write,
        left, right, width, height,
        kernel.data(), kernel.size());
    }

    // Second vertical pass.
    {
      auto bottom = [&](int x, int y) {
        return read(ggo::get_pixel_ptr<pbf>(tmp.data(), x, 0, height, line_step));
      };
      auto top = [&](int x, int y) {
        return read(ggo::get_pixel_ptr<pbf>(tmp.data(), x, height - 1, height, line_step));
      };

      ggo::apply_symetric_kernel_2d_vert<ggo::pixel_buffer_format_info<pbf>::y_dir>(
        tmp.data(), ggo::pixel_buffer_format_info<pbf>::pixel_byte_size, tmp_line_step, read,
        buffer, ggo::pixel_buffer_format_info<pbf>::pixel_byte_size, line_step, write,
        bottom, top, width, height,
        kernel.data(), kernel.size());
    }
  }
}

#endif