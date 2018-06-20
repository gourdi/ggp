#ifndef __GGO_2D_GAUSSIAN_BLUR__
#define __GGO_2D_GAUSSIAN_BLUR__

#include <kernel/math/signal_proc/ggo_convolution2d.h>
#include <kernel/math/signal_proc/ggo_gaussian_blur.h>
#include <2d/ggo_image_format.h>
#include <2d/ggo_color.h>

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

  ////////////////////////////////////////////////////////////////////
  template <image_format format, ggo::border_mode border_mode = border_mode::mirror>
  void gaussian_blur2d(void * buffer, int line_byte_step, const ggo::size & size, float stddev)
  {
    using format_traits = image_format_traits<format>;
    using gaussian_helper = gaussian_blur2d_helper<typename format_traits::color_t>;

    std::vector<uint8_t> tmp(line_byte_step * size.height());
    memcpy(tmp.data(), buffer, tmp.size());

    auto kernel = gaussian_helper::build_kernel(stddev);
    if (kernel.size() <= 1)
    {
      return;
    }

    auto read = [](const void * ptr) {
      return gaussian_helper::convert(format_traits::read(ptr));
    };

    auto write = [](void * ptr, const typename gaussian_helper::color_t & pixel) {
      format_traits::write(ptr, gaussian_helper::convert(pixel));
    };

    ggo::apply_symetric_kernel_2d_horz<border_mode>(buffer, format_traits::pixel_byte_size, line_byte_step, read,
      tmp.data(), format_traits::pixel_byte_size, line_byte_step, write,
      size, rect_int::from_size(size), kernel.data(), kernel.size());

    ggo::apply_symetric_kernel_2d_vert<border_mode>(tmp.data(), format_traits::pixel_byte_size, line_byte_step, read,
      buffer, format_traits::pixel_byte_size, line_byte_step, write,
      size, rect_int::from_size(size), kernel.data(), kernel.size());
  }
}

#endif
