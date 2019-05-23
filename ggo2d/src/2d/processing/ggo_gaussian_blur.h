#ifndef __GGO_2D_GAUSSIAN_BLUR__
#define __GGO_2D_GAUSSIAN_BLUR__

#include <kernel/ggo_size.h>
#include <kernel/ggo_borders.h>
#include <kernel/memory/ggo_forward_input_cache.h>
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

      using memory_to_processing_convert_t = passthrough_conversion<float>;
      using processing_to_memory_convert_t = passthrough_conversion<float>;

      static auto build_kernel(float stddev)
      {
        return ggo::build_gaussian_kernel<float>(stddev, 0.001f);
      }
    };

    template <>
    struct helper<double>
    {
      using processing_t = double;

      using memory_to_processing_convert_t = passthrough_conversion<double>;
      using processing_to_memory_convert_t = passthrough_conversion<double>;

      static auto build_kernel(float stddev)
      {
        return ggo::build_gaussian_kernel<float>(stddev, 0.001f);
      }
    };

    template <>
    struct helper<uint8_t>
    {
      using processing_t = uint32_t;

      struct memory_to_processing_convert_t
      {
        using input_t = uint8_t;
        using output_t = uint32_t;

        static uint32_t convert(uint8_t c) { return static_cast<uint32_t>(c); }
      };

      struct processing_to_memory_convert_t
      {
        using input_t = uint32_t;
        using output_t = uint8_t;

        static uint8_t convert(uint32_t c) { return ggo::fixed_point_div<16>(c); }
      };

      static auto build_kernel(float stddev)
      {
        return build_gaussian_kernel_fp(stddev);
      }
    };

    template <>
    struct helper<rgb_8u>
    {
      using processing_t = ggo::rgb_32u;

      struct memory_to_processing_convert_t
      {
        using input_t = rgb_8u;
        using output_t = rgb_32u;

        static rgb_32u convert(rgb_8u c) { return {
            static_cast<uint32_t>(c.r()),
            static_cast<uint32_t>(c.g()),
            static_cast<uint32_t>(c.b()) };
        }
      };

      struct processing_to_memory_convert_t
      {
        using input_t = rgb_32u;
        using output_t = rgb_8u;

        static rgb_8u convert(rgb_32u c) { return {
          static_cast<uint8_t>(ggo::fixed_point_div<16>(c.r())),
          static_cast<uint8_t>(ggo::fixed_point_div<16>(c.g())),
          static_cast<uint8_t>(ggo::fixed_point_div<16>(c.b())) };
        }
      };

      static auto build_kernel(float stddev)
      {
        return build_gaussian_kernel_fp(stddev);
      }
    };
  }

  template <image_format format, border_mode border = border_mode::mirror>
  void gaussian_blur(void * buffer, const ggo::size & size, int line_byte_step, float stddev)
  {
    using color_t = image_format_traits<format>::color_t;
    using processing_t = gaussian_blur_details::helper<color_t>::processing_t;
    using memory_to_processing_convert_t = gaussian_blur_details::helper<color_t>::memory_to_processing_convert_t;
    using processing_to_memory_convert_t = gaussian_blur_details::helper<color_t>::processing_to_memory_convert_t;

    auto kernel = gaussian_blur_details::helper<color_t>::build_kernel(stddev);
    if (kernel.size() == 1)
    {
      return;
    }
    int kernel_half_size = static_cast<int>(kernel.size());
    int width = size.width();
    int height = size.height();

    // Horizontal pass.
    {
      forward_input_cache<processing_t, border, color_t, memory_to_processing_convert_t> forward_input(size.width(), kernel_half_size);

      for (int y = 0; y < size.height(); ++y)
      {
        // Set up forward input.
        const void * line_ptr = ggo::move_ptr(buffer, y * line_byte_step);
        auto read = [&](int x)
        {
          const void * pixel_ptr = ggo::move_ptr(line_ptr, x * image_format_traits<format>::pixel_byte_size);
          return ggo::read_pixel<format>(pixel_ptr);
        };
        forward_input.setup(read);

        // Convolution.
        void * output_ptr = ggo::move_ptr(buffer, y * line_byte_step);
        for (int x = 0; x < width; ++x)
        {
          processing_t c = kernel[0] * forward_input.read(0);
          for (int k = 1; k < kernel_half_size; ++k)
          {
            c += kernel[k] * (forward_input.read(-k) + forward_input.read(k));
          }
          write_pixel<format>(output_ptr, processing_to_memory_convert_t::convert(c));

          forward_input.move_to_next();
          output_ptr = ggo::move_ptr(output_ptr, image_format_traits<format>::pixel_byte_size);
        }
      }
    }

    // Vertical pass.
    {
      forward_input_cache<processing_t, border, color_t, memory_to_processing_convert_t> forward_input(size.height(), kernel_half_size);

      for (int x = 0; x < size.width(); ++x)
      {
        // Set up forward input.
        auto read = [&](int y)
        {
          const void * pixel_ptr = ggo::move_ptr(buffer, y * line_byte_step);
          pixel_ptr = ggo::move_ptr(pixel_ptr, x * image_format_traits<format>::pixel_byte_size);
          return ggo::read_pixel<format>(pixel_ptr);
        };
        forward_input.setup(read);

        // Convolution.
        void * output_ptr = ggo::move_ptr(buffer, x * image_format_traits<format>::pixel_byte_size);
        for (int y = 0; y < height; ++y)
        {
          processing_t c = kernel[0] * forward_input.read(0);
          for (int k = 1; k < kernel_half_size; ++k)
          {
            c += kernel[k] * (forward_input.read(-k) + forward_input.read(k));
          }
          write_pixel<format>(output_ptr, processing_to_memory_convert_t::convert(c));

          forward_input.move_to_next();
          output_ptr = ggo::move_ptr(output_ptr, line_byte_step);
        }
      }
    }
  }
}

#endif
