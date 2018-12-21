#ifndef __GGO_2D_GAUSSIAN_BLUR__
#define __GGO_2D_GAUSSIAN_BLUR__

#include <kernel/ggo_size.h>
#include <kernel/ggo_rect_int.h>
#include <kernel/ggo_borders.h>
#include <kernel/math/signal_proc/ggo_convolution1d.h>
#include <kernel/math/signal_proc/ggo_convolution2d.h>
#include <kernel/math/signal_proc/ggo_gaussian_blur.h>
#include <2d/ggo_image_format.h>
#include <2d/ggo_color.h>

namespace ggo
{
  namespace gaussian_blur_details
  {
    constexpr unsigned int bit_shift = 16;

    inline auto build_gaussian_kernel_fp(float stddev)
    {
      auto kernel_float = ggo::build_gaussian_kernel<float>(stddev, 0.001f);

      std::vector<uint32_t> kernel;
      for (float k : kernel_float)
      {
        kernel.emplace_back(ggo::round_to<uint32_t>(k * (1 << bit_shift)));
      }

      return kernel;
    }

    template <typename data_t>
    struct helper
    {
    };

    template <>
    struct helper<float>
    {
      using processing_t = float;

      static auto build_kernel(float stddev)
      {
        return ggo::build_gaussian_kernel<float>(stddev, 0.001f);
      }

      static processing_t convert(float c) { return c; }
    };

    template <>
    struct helper<double>
    {
      using processing_t = double;

      static auto build_kernel(float stddev)
      {
        return ggo::build_gaussian_kernel<float>(stddev, 0.001f);
      }

      static processing_t convert(double c) { return c; }
    };

    template <>
    struct helper<uint8_t>
    {
      using processing_t = uint32_t;

      static auto build_kernel(float stddev)
      {
        return build_gaussian_kernel_fp(stddev);
      }

      static uint32_t convert(uint8_t c) { return static_cast<uint32_t>(c); }
      static uint8_t  convert(uint32_t c) { return ggo::fixed_point_div<16>(c); }
    };

    template <>
    struct helper<rgb_8u>
    {
      using processing_t = ggo::rgb_32u;

      static auto build_kernel(float stddev)
      {
        return build_gaussian_kernel_fp(stddev);
      }

      static rgb_32u convert(rgb_8u c) { 
        return { static_cast<uint32_t>(c.r()), static_cast<uint32_t>(c.g()), static_cast<uint32_t>(c.b()) }; }

      static rgb_8u convert(rgb_32u c) {
        return {
          static_cast<uint8_t>(ggo::fixed_point_div<16>(c.r())),
          static_cast<uint8_t>(ggo::fixed_point_div<16>(c.g())),
          static_cast<uint8_t>(ggo::fixed_point_div<16>(c.b())) };
      }
    };
  }

  template <image_format format, border_mode border = border_mode::mirror>
  void gaussian_blur(void * buffer, const ggo::size & size, int line_byte_step, const ggo::rect_int & clipping, float stddev)
  {
    using color_t = image_format_traits<format>::color_t;
    using helper = gaussian_blur_details::helper<color_t>;
    using processing_t = helper::processing_t;

    auto kernel = helper::build_kernel(stddev);
    if (kernel.size() == 1)
    {
      return;
    }

    // Horizontal pass.
    {
      ggo::array<color_t, 1> line(size.width());

      for (int y = clipping.bottom(); y <= clipping.top(); ++y)
      {
        void * buffer_ptr = get_line_ptr<image_format_traits<format>::lines_order>(buffer, y, size.height(), line_byte_step);

        // Copy line.
        for (int x = 0; x < size.width(); ++x)
        {
          line[x] = ggo::read_pixel<format>(ggo::ptr_offset(buffer_ptr, x * image_format_traits<format>::pixel_byte_size));
        }

        // Convolution.
        auto read = [&](int x)
        { 
          return helper::convert(line[x]);
        };
        auto write = [&](int x, processing_t c)
        { 
          write_pixel<format>(ggo::ptr_offset(buffer_ptr, x * image_format_traits<format>::pixel_byte_size), helper::convert(c));
        };

        ggo::symmetric_convolution<border>(read, write, size.width(), clipping.left(), clipping.right(), kernel.data(), int(kernel.size()));
      }
    }

    // Vertical pass.
    {
      ggo::array<color_t, 1> column(size.height());

      for (int x = clipping.left(); x <= clipping.right(); ++x)
      {
        // Copy column.
        for (int y = 0; y < size.height(); ++y)
        {
          column[y] = read_pixel<format>(buffer, x, y, size.height(), line_byte_step);
        }

        // Convolution.
        auto read = [&](int y)
        {
          return helper::convert(column[y]);
        };
        auto write = [&](int y, processing_t c)
        {
          write_pixel<format>(buffer, x, y, size.height(), line_byte_step, helper::convert(c));
        };

        ggo::symmetric_convolution<border>(read, write, size.height(), clipping.bottom(), clipping.top(), kernel.data(), int(kernel.size()));
      }
    }
  }

  template <image_format format, border_mode border = border_mode::mirror>
  void gaussian_blur(void * buffer, const ggo::size & size, int line_byte_step, float stddev)
  {
    gaussian_blur<format, border>(buffer, size, line_byte_step, ggo::rect_int::from_size(size), stddev);
  }
}

#endif
