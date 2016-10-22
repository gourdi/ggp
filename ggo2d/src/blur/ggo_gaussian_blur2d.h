#ifndef __GGO_GAUSSIAN_BLUR2D__
#define __GGO_GAUSSIAN_BLUR2D__

#include <ggo_pixel_buffer.h>
#include <ggo_color.h>
#include <ggo_buffer_iterator.h>
#include <ggo_convolution.h>
#include <ggo_gaussian_blur.h>

namespace ggo
{
  template <typename color_t>
  struct gaussian_blur2d_helper {};

  template <>
  struct gaussian_blur2d_helper<uint8_t>
  {
    using color_t = uint16_t;

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
    using color_t = color_16u;

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

  template <ggo::pixel_buffer_format pbf>
  struct gaussian_accessor
  {
    using format = pixel_buffer_format_info<pbf>;
    using gauss = gaussian_blur2d_helper<typename format::color_t>;
    using type = typename gauss::color_t;

    static typename gauss::color_t read(const void * ptr)
    {
      return gauss::convert(format::read(ptr));
    }

    static void write(void * ptr, typename gauss::color_t c)
    {
      return format::write(ptr, gauss::convert(c));
    }
  };

  ////////////////////////////////////////////////////////////////////
  template <pixel_buffer_format pbf>
  void gaussian_blur2d(void * buffer, const int width, const int height, const int line_step, const float stddev)
  {
    using format = pixel_buffer_format_info<pbf>;
    using gaussian_helper = gaussian_blur2d_helper<typename format::color_t>;

    std::vector<uint8_t> tmp(line_step * height);

    auto kernel = gaussian_helper::build_kernel(stddev);

    // First horizontal pass.
    {
      auto input_line_iterator = [&](int y) {
        const void * ptr = ggo::get_pixel_ptr<pbf>(buffer, 0, y, height, line_step);
        return ggo::const_buffer_iterator<format::pixel_byte_size, gaussian_accessor<pbf>>(ptr);
      };

      auto output_line_iterator = [&](int y) {
        void * ptr = ggo::get_pixel_ptr<pbf>(tmp.data(), 0, y, height, line_step);
        return ggo::buffer_iterator<format::pixel_byte_size, gaussian_accessor<pbf>>(ptr);
      };

      auto left = [&](int x, int y) {
        return gaussian_helper::convert(format::read(ggo::get_pixel_ptr<pbf>(buffer, 0, y, height, line_step)));
      };
      auto right = [&](int x, int y) {
        return gaussian_helper::convert(format::read(ggo::get_pixel_ptr<pbf>(buffer, width - 1, y, height, line_step)));
      };

      ggo::apply_symetric_kernel_2d_horz(input_line_iterator, output_line_iterator,
        left, right, width, height, kernel.data(), kernel.size());
    }

    // Second vertical pass.
    {
      auto input_column_iterator = [&](int x) {
        const void * ptr = ggo::get_pixel_ptr<pbf>(tmp.data(), x, 0, height, line_step);
        return ggo::const_buffer_iterator<0, gaussian_accessor<pbf>>(ptr, format::y_dir == y_up ? line_step : -line_step);
      };

      auto output_column_iterator = [&](int x) {
        void * ptr = ggo::get_pixel_ptr<pbf>(buffer, x, 0, height, line_step);
        return ggo::buffer_iterator<0, gaussian_accessor<pbf>>(ptr, format::y_dir == y_up ? line_step : -line_step);
      };

      auto bottom = [&](int x, int y) {
        return gaussian_helper::convert(format::read(ggo::get_pixel_ptr<pbf>(tmp.data(), x, 0, height, line_step)));
      };
      auto top = [&](int x, int y) {
        return gaussian_helper::convert(format::read(ggo::get_pixel_ptr<pbf>(tmp.data(), x, height - 1, height, line_step)));
      };

      ggo::apply_symetric_kernel_2d_vert(input_column_iterator, output_column_iterator,
        bottom, top, width, height, kernel.data(), kernel.size());
    }
  }
}

#endif