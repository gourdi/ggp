#ifndef __GGO_2D_GAUSSIAN_BLUR__
#define __GGO_2D_GAUSSIAN_BLUR__

#include <kernel/ggo_size.h>
#include <kernel/ggo_borders.h>
#include <kernel/math/signal_processing/ggo_gaussian_blur.h>
#include <2d/ggo_color.h>
#include <2d/ggo_pixel_type.h>
#include <2d/processing/ggo_local_processing.h>

// Details.
namespace ggo
{
  namespace gaussian_blur_details
  {
    constexpr unsigned int bit_shift = 16;

    inline auto build_gaussian_kernel_fp(float stddev)
    {
      auto kernel_float = ggo::build_gaussian_kernel<float>(stddev);

      std::vector<uint32_t> kernel;
      for (float k : kernel_float)
      {
        kernel.emplace_back(ggo::round_to<uint32_t>(k * (1 << bit_shift)));
      }

      return kernel;
    }

    template <typename color_t>
    struct helpers
    {
    };

    template <>
    struct helpers<float>
    {
      static auto build_kernel(float stddev) { return ggo::build_gaussian_kernel<float>(stddev); }
      static float convert(float v) { return v; }
    };

    template <>
    struct helpers<double>
    {
      static auto build_kernel(double stddev) { return ggo::build_gaussian_kernel<double>(stddev); }
      static double convert(double v) { return v; }
    };

    template <>
    struct helpers<uint8_t>
    {
      static auto build_kernel(float stddev) { return build_gaussian_kernel_fp(stddev); }
      static uint32_t convert(uint8_t c) { return static_cast<uint32_t>(c); }
      static uint8_t convert(uint32_t c) { return ggo::fixed_point_div<gaussian_blur_details::bit_shift>(c); }
    };

    template <>
    struct helpers<rgb_8u>
    {
      static auto build_kernel(float stddev) { return build_gaussian_kernel_fp(stddev); }
      static rgb_32u convert(rgb_8u c) {
        return {
          static_cast<uint32_t>(c.r()),
          static_cast<uint32_t>(c.g()),
          static_cast<uint32_t>(c.b()) };
      }
      static rgb_8u convert(rgb_32u c) {
        return {
          static_cast<uint8_t>(ggo::fixed_point_div<gaussian_blur_details::bit_shift>(c.r())),
          static_cast<uint8_t>(ggo::fixed_point_div<gaussian_blur_details::bit_shift>(c.g())),
          static_cast<uint8_t>(ggo::fixed_point_div<gaussian_blur_details::bit_shift>(c.b())) };
      }
    };

    template <pixel_type pt, typename image_t, typename cache_t>
    void gaussian_blur(image_t & image, cache_t & cache, float stddev, border_mode border_mode)
    {
      using helpers = gaussian_blur_details::helpers<ggo::pixel_type_traits<pt>::color_t>;

      const auto kernel = helpers::build_kernel(stddev);
      if (kernel.size() <= 1)
      {
        return;
      }

      int processing_size = static_cast<int>(kernel.size() - 1);

      auto processing = [&](const auto & neighborhood)
      {
        auto out = kernel[0] * helpers::convert(neighborhood(0));
        for (int i = 1; i < kernel.size(); ++i)
        {
          out += kernel[i] * (helpers::convert(neighborhood(-i)) + helpers::convert(neighborhood(i)));
        }
        return helpers::convert(out);
      };

      switch (border_mode)
      {
      case ggo::border_mode::mirror:
        apply_horizontal_processing<ggo::border_mode::mirror>(image, cache, processing_size, processing_size, processing);
        apply_vertical_symmetric_processing<ggo::border_mode::mirror>(cache, image, processing_size, processing);
        break;
      default:
        throw std::runtime_error("unsupported border mode");
      }
    }
  }
}

// Static images.
namespace ggo
{
  template <pixel_type pt, lines_order lo, bool owns_buffer>
  void gaussian_blur(image_base_t<pt, lo, void *, owns_buffer> & image, float stddev, border_mode border_mode = border_mode::mirror)
  {
    ggo::image_base_t<pt, lo, void *, true> cache(image.size());

    gaussian_blur_details::gaussian_blur<pt>(image, cache, stddev, border_mode);
  }
}

// Dynamic images.
namespace ggo
{
  struct gaussian_blur_functor
  {
    template <ggo::lines_order memory_lines_order, ggo::pixel_type pixel_type>
    static void call(ggo::size size, void * image_buffer, int image_line_byte_step, void * cache_buffer,int cache_line_byte_step,
      float stddev, border_mode border_mode)
    {
      image_view_t<pixel_type, memory_lines_order> image(image_buffer, size, image_line_byte_step);
      image_view_t<pixel_type, memory_lines_order> cache(cache_buffer, size, cache_line_byte_step);

      gaussian_blur_details::gaussian_blur<pixel_type>(image, cache, stddev, border_mode);
    }
  };

  template <bool owns_buffer>
  void gaussian_blur(image_base<void *, owns_buffer> & image, float stddev, border_mode border_mode = border_mode::mirror)
  {
    ggo::image cache(image.size(), image.pixel_type(), image.memory_lines_order());

    dispatch_image_format<gaussian_blur_functor>(image.pixel_type(), image.memory_lines_order(),
      image.size(), image.data(), image.line_byte_step(), cache.data(), cache.line_byte_step(), stddev, border_mode);
  }
}

#endif
