#ifndef __GGO_GAUSSIAN_BLUR2D__
#define __GGO_GAUSSIAN_BLUR2D__

#include <ggo_pixel_buffer.h>
#include <ggo_color.h>
#include <ggo_convolution.h>
#include <ggo_gaussian_blur.h>

namespace ggo
{
  template <typename color_t>
  struct gaussian_blur2d_helper {};

  template <>
  struct gaussian_blur2d_helper<uint8_t>
  {
    using data_t = uint16_t;

    static std::vector<uint16_t> build_kernel(const float stddev)
    {
      return ggo::build_fixed_point_gaussian_kernel<uint16_t, float>(stddev, 0.001f, 8);
    }

    static uint16_t convert(const uint8_t & c)
    {
      return static_cast<uint16_t>(c);
    }

    static uint8_t convert(const uint16_t & c)
    {
      return static_cast<uint8_t>(ggo::fixed_point_div<8>(c));
    }
  };

  template <>
  struct gaussian_blur2d_helper<ggo::color_8u>
  {
    using data_t = color_16u;

    static std::vector<uint16_t> build_kernel(const float stddev)
    {
      return ggo::build_fixed_point_gaussian_kernel<uint16_t, float>(stddev, 0.001f, 8);
    }

    static ggo::color_16u convert(const ggo::color_8u & c)
    {
      return ggo::color_16u(c._r, c._g, c._b);
    }

    static ggo::color_8u convert(const ggo::color_16u & c)
    {
      return ggo::color_8u(
        static_cast<uint8_t>(ggo::fixed_point_div<8>(c._r)),
        static_cast<uint8_t>(ggo::fixed_point_div<8>(c._g)),
        static_cast<uint8_t>(ggo::fixed_point_div<8>(c._b)));
    }
  };

  ////////////////////////////////////////////////////////////////////
  template <pixel_buffer_format pbf>
  void gaussian_blur2d(void * buffer, const int width, const int height, const int line_step, const float stddev)
  {
    using helper = gaussian_blur2d_helper<pixel_buffer_format_info<pbf>::color_t>;

    std::vector<uint8_t> tmp(line_step * height);

    auto kernel = helper::build_kernel(stddev);

    auto read = [](const void * ptr) {
      return helper::convert(ggo::get_pixel<pbf>(ptr));
    };
    auto write = [](void * ptr, const helper::data_t & c) {
      ggo::set_pixel<pbf>(ptr, helper::convert(c));
    };

    // First horizontal pass.
    {
      auto left = [&](int x, int y) {
        return read(ggo::get_pixel_ptr<pbf>(buffer, 0, y, height, line_step));
      };
      auto right = [&](int x, int y) {
        return read(ggo::get_pixel_ptr<pbf>(buffer, width - 1, y, height, line_step));
      };

      ggo::apply_symetric_kernel_2d_horz<ggo::pixel_buffer_format_info<pbf>::y_dir>(
        buffer, ggo::pixel_buffer_format_info<pbf>::pixel_byte_size, line_step, read,
        tmp.data(), ggo::pixel_buffer_format_info<pbf>::pixel_byte_size, line_step, write,
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
        tmp.data(), ggo::pixel_buffer_format_info<pbf>::pixel_byte_size, line_step, read,
        buffer, ggo::pixel_buffer_format_info<pbf>::pixel_byte_size, line_step, write,
        bottom, top, width, height,
        kernel.data(), kernel.size());
    }
  }
}

#endif