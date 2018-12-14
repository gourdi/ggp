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
    using processing_t = uint16_t;

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
  struct gaussian_blur2d_helper<ggo::rgb_8u>
  {
    using processing_t = rgb_16u;

    static std::vector<uint16_t> build_kernel(const float stddev)
    {
      return ggo::build_fixed_point_gaussian_kernel<uint16_t, float>(stddev, 0.001f, 8);
    }

    static ggo::rgb_16u convert(const ggo::rgb_8u & c)
    {
      return ggo::rgb_16u(uint16_t(c.r()), uint16_t(c.g()), uint16_t(c.b()));
    }

    static ggo::rgb_8u convert(const ggo::rgb_16u & c)
    {
      return ggo::rgb_8u(
        static_cast<uint8_t>(ggo::fixed_point_div<8>(c.r())),
        static_cast<uint8_t>(ggo::fixed_point_div<8>(c.g())),
        static_cast<uint8_t>(ggo::fixed_point_div<8>(c.b())));
    }
  };

  template <>
  struct gaussian_blur2d_helper<float>
  {
    using processing_t = float;

    static std::vector<float> build_kernel(const float stddev)
    {
      return ggo::build_gaussian_kernel<float>(stddev, 0.001f);
    }

    static float convert(float c)
    {
      return c;
    }
  };

  template <>
  struct gaussian_blur2d_helper<double>
  {
    using processing_t = double;

    static std::vector<double> build_kernel(const float stddev)
    {
      return ggo::build_gaussian_kernel<double>(stddev, 0.001);
    }

    static double convert(double c)
    {
      return c;
    }
  };

  template <ggo::image_format format>
  struct gaussian_blur2d_data_traits
  {
    using format_traits = image_format_traits<format>;
    using data_t = typename format_traits::color_t;
    using processing_t = typename gaussian_blur2d_helper<data_t>::processing_t;

    static  constexpr size_t  input_item_byte_size = format_traits::pixel_byte_size;
    static  constexpr size_t  output_item_byte_size = format_traits::pixel_byte_size;

    static  data_t  read(const void * ptr) { return format_traits::read(ptr); }
    static  void    write(void * ptr, const data_t & c) { format_traits::write(ptr, c); }

    static  processing_t  from_data_to_processing(const data_t & v) { return typename gaussian_blur2d_helper<data_t>::convert(v); }
    static  data_t        from_processing_to_data(const processing_t & v) { return typename gaussian_blur2d_helper<data_t>::convert(v); }
  };

  ////////////////////////////////////////////////////////////////////
  template <image_format format, ggo::border_mode border_mode = border_mode::mirror>
  void gaussian_blur2d(void * buffer, int line_byte_step, const ggo::size & size, float stddev, ggo::rect_int clipping)
  {
    using format_traits = image_format_traits<format>;
    using gaussian_blur_helper = gaussian_blur2d_helper<typename format_traits::color_t>;
    using gaussian_blur_data_traits = gaussian_blur2d_data_traits<format>;

    auto kernel = gaussian_blur_helper::build_kernel(stddev);
    if (kernel.size() <= 1)
    {
      return;
    }

    ggo::apply_symetric_kernel_2d<format_traits::lines_order, border_mode, gaussian_blur_data_traits>(
      buffer, line_byte_step, buffer, line_byte_step,
      size, clipping, kernel.data(), kernel.size());
  }

  ////////////////////////////////////////////////////////////////////
  template <image_format format, ggo::border_mode border_mode = border_mode::mirror>
  void gaussian_blur2d(void * buffer, int line_byte_step, const ggo::size & size, float stddev)
  {
    gaussian_blur2d<format, border_mode>(buffer, line_byte_step, size, stddev, ggo::rect_int::from_size(size));
  }
}

#endif
