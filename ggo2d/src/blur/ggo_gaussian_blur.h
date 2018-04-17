#ifndef __GGO_2D_GAUSSIAN_BLUR__
#define __GGO_2D_GAUSSIAN_BLUR__

#include <ggo_image_format.h>
#include <ggo_color.h>
#include <ggo_memory_layouts.h>
#include <math/signal_proc/ggo_convolution.h>
#include <math/signal_proc/ggo_gaussian_blur.h>

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
  struct gaussian_blur2d_helper<float>
  {
    using color_t = float;

    static std::vector<float> build_kernel(const float stddev)
    {
      return ggo::build_gaussian_kernel<float>(stddev, 0.001f);
    }

    static float convert(const float & c)
    {
      return c;
    }
  };

  template <>
  struct gaussian_blur2d_helper<double>
  {
    using color_t = double;

    static std::vector<double> build_kernel(const double stddev)
    {
      return ggo::build_gaussian_kernel<double>(stddev, 0.001);
    }

    static double convert(const double & c)
    {
      return c;
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
      return ggo::color_16u(uint16_t(c.r()), uint16_t(c.g()), uint16_t(c.b()));
    }

    static ggo::color_8u convert(const ggo::color_16u & c)
    {
      return ggo::color_8u(
        static_cast<uint8_t>(ggo::fixed_point_div<8>(c.r())),
        static_cast<uint8_t>(ggo::fixed_point_div<8>(c.g())),
        static_cast<uint8_t>(ggo::fixed_point_div<8>(c.b())));
    }
  };

  template <ggo::image_format format>
  struct gaussian_accessor
  {
    using format_traits = image_format_traits<format>;
    using gauss = gaussian_blur2d_helper<typename format_traits::color_t>;
    using input_t = typename gauss::color_t;
    using output_t = typename gauss::color_t;

    static typename gauss::color_t read(const void * ptr)
    {
      return gauss::convert(format_traits::read(ptr));
    }

    static void write(void * ptr, typename gauss::color_t c)
    {
      return format_traits::write(ptr, gauss::convert(c));
    }
  };

  ////////////////////////////////////////////////////////////////////
  template <image_format format, typename left_t, typename right_t, typename bottom_t, typename top_t>
  void gaussian_blur2d(void * buffer, const int width, const int height, const int line_byte_step,
    const float stddev, left_t left, right_t right, bottom_t bottom, top_t top)
  {
    using format_traits = image_format_traits<format>;
    using memory_layout = format_traits::memory_layout_t;
    using gaussian_helper = gaussian_blur2d_helper<typename format_traits::color_t>;

    std::vector<uint8_t> tmp(line_byte_step * height);

    auto kernel = gaussian_helper::build_kernel(stddev);
    if (kernel.size() <= 1)
    {
      return;
    }

    // First horizontal pass.
    {
      auto input_line_iterator = [&](int y) {
        return memory_layout::make_horizontal_read_only_iterator<gaussian_accessor<format>>(buffer, y, height, line_byte_step);
      };

      auto output_line_iterator = [&](int y) {
        return memory_layout::make_horizontal_write_only_iterator<gaussian_accessor<format>>(tmp.data(), y, height, line_byte_step);
      };

      ggo::apply_symetric_kernel_2d_horz(input_line_iterator, output_line_iterator,
        [&](int x, int y) { return left(x, y, buffer); },
        [&](int x, int y) { return right(x, y, buffer); },
        width, height, kernel.data(), kernel.size());
    }

    // Second vertical pass.
    {
      auto input_column_iterator = [&](int x) {
        return memory_layout::make_vertical_read_only_iterator<gaussian_accessor<format>>(tmp.data(), x, height, line_byte_step);
      };

      auto output_column_iterator = [&](int x) {
        return memory_layout::make_vertical_write_only_iterator<gaussian_accessor<format>>(buffer, x, height, line_byte_step);
      };

      ggo::apply_symetric_kernel_2d_vert(input_column_iterator, output_column_iterator,
        [&](int x, int y){ return bottom(x, y, tmp.data()); },
        [&](int x, int y){ return top(x, y, tmp.data()); },
        width, height, kernel.data(), kernel.size());
    }
  }

  ////////////////////////////////////////////////////////////////////
  template <image_format format>
  void gaussian_blur2d_mirror(void * buffer, const int width, const int height, const int line_byte_step, const float stddev)
  {
    using format_traits = image_format_traits<format>;
    using gaussian_helper = gaussian_blur2d_helper<typename format_traits::color_t>;

    auto left = [&](int x, int y, void * buf) {
      return gaussian_helper::convert(format_traits::read(ggo::get_pixel_ptr<format>(buf, 0, y, height, line_byte_step)));
    };

    auto right = [&](int x, int y, void * buf) {
      return gaussian_helper::convert(format_traits::read(ggo::get_pixel_ptr<format>(buf, width - 1, y, height, line_byte_step)));
    };

    auto bottom = [&](int x, int y, void * buf) {
      return gaussian_helper::convert(format_traits::read(ggo::get_pixel_ptr<format>(buf, x, 0, height, line_byte_step)));
    };

    auto top = [&](int x, int y, void * buf) {
      return gaussian_helper::convert(format_traits::read(ggo::get_pixel_ptr<format>(buf, x, height - 1, height, line_byte_step)));
    };

    gaussian_blur2d<format>(buffer, width, height, line_byte_step, stddev, left, right, bottom, top);
  }

  ////////////////////////////////////////////////////////////////////
  template <image_format format>
  void gaussian_blur2d_loop(void * buffer, const int width, const int height, const int line_byte_step, const float stddev)
  {
    using format_traits = image_format_traits<format>;
    using gaussian_helper = gaussian_blur2d_helper<typename format::color_t>;

    auto horz = [&](int x, int y, void * buf) {
      x = loop_index(x, width);
      return gaussian_helper::convert(format_traits::read(ggo::get_pixel_ptr<format>(buf, x, y, height, line_byte_step)));
    };

    auto vert = [&](int x, int y, void * buf) {
      y = loop_index(y, height);
      return gaussian_helper::convert(format_traits::read(ggo::get_pixel_ptr<format>(buf, x, y, height, line_byte_step)));
    };

    gaussian_blur2d<format>(buffer, width, height, line_byte_step, stddev, horz, horz, vert, vert);
  }
}

#endif