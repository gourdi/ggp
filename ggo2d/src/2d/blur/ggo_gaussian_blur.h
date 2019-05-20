#ifndef __GGO_2D_GAUSSIAN_BLUR__
#define __GGO_2D_GAUSSIAN_BLUR__

#include <kernel/ggo_size.h>
#include <kernel/ggo_rect_int.h>
#include <kernel/ggo_borders.h>
#include <kernel/math/signal_proc/ggo_convolution1d.h>
#include <kernel/math/signal_proc/ggo_gaussian_blur.h>
#include <2d/ggo_image_format.h>
#include <2d/ggo_color.h>
#include <cassert>

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
  void gaussian_blur(const void * input, int input_line_byte_step, void * output, int output_line_byte_step, const ggo::size & size, float stddev)
  {
    using color_t = image_format_traits<format>::color_t;
    using helper = gaussian_blur_details::helper<color_t>;
    using processing_t = helper::processing_t;

    auto kernel = helper::build_kernel(stddev);
    if (kernel.size() == 1)
    {
      assert(input != output); // In-place not handled for now.
      return;
    }
    int kernel_half_size = static_cast<int>(kernel.size());

    // Horizontal pass.
    {
      ggo::array<color_t, 1> line(size.width() + 2 * (kernel_half_size - 1));

      for (int y = 0; y < size.height(); ++y)
      {
        // Copy line.
        {
          const void * input_ptr = get_line_ptr<image_format_traits<format>::lines_order>(input, y, size.height(), input_line_byte_step);
          color_t * line_ptr = line.data();
          int begin = -kernel_half_size + 1;
          int end = size.width() + kernel_half_size - 1;
          for (int x = begin; x < end; ++x)
          {
            int xx = index<border>(x, size.width());
            const void * pixel_ptr = ggo::move_ptr(input_ptr, xx * image_format_traits<format>::pixel_byte_size);
            *line_ptr++ = ggo::read_pixel<format>(pixel_ptr);
          }
          GGO_ASSERT(line_ptr == line.end());
        }

        // Convolution.
        {
          const color_t * line_ptr = line.data() + kernel_half_size - 1;
          void * output_ptr = get_line_ptr<image_format_traits<format>::lines_order>(output, y, size.height(), output_line_byte_step);

          auto read = [&](int x)
          {
            return helper::convert(line_ptr[x]);
          };
          auto write = [&](int x, processing_t c)
          {
            write_pixel<format>(ggo::move_ptr(output_ptr, x * image_format_traits<format>::pixel_byte_size), helper::convert(c));
          };

          ggo::symmetric_convolution<border_mode::in_memory>(read, write, size.width(), 0, size.width() - 1, kernel.data(), int(kernel.size()));
        }
      }
    }

    // Vertical pass.
    {
      ggo::array<color_t, 1> column(size.height() + 2 * (kernel_half_size - 1));

      for (int x = 0; x < size.width(); ++x)
      {
        // Copy column.
        {
          color_t * column_ptr = column.data();
          int begin = -kernel_half_size + 1;
          int end = size.height() + kernel_half_size - 1;
          for (int y = begin; y < end; ++y)
          {
            int yy = index<border>(y, size.height());
            *column_ptr++ = ggo::read_pixel<format>(output, x, yy, size.height(), output_line_byte_step);
          }
          GGO_ASSERT(column_ptr == column.end());
        }

        // Convolution.
        const color_t * column_ptr = column.data() + kernel_half_size - 1;

        auto read = [&](int y)
        {
          return helper::convert(column_ptr[y]);
        };
        auto write = [&](int y, processing_t c)
        {
          write_pixel<format>(output, x, y, size.height(), output_line_byte_step, helper::convert(c));
        };

        ggo::symmetric_convolution<border_mode::in_memory>(read, write, size.height(), 0, size.height() - 1, kernel.data(), int(kernel.size()));
      }
    }
  }

  template <image_format format, border_mode border = border_mode::mirror>
  void gaussian_blur(void * buffer, const ggo::size & size, int line_byte_step, float stddev)
  {
    gaussian_blur<format, border>(buffer, line_byte_step, buffer, line_byte_step, size, stddev);
  }
}

#endif
