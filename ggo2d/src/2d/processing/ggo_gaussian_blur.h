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

    template <>
    struct helpers<float>
    {
      static auto build_kernel(float stddev) { return ggo::build_gaussian_kernel<float>(stddev); }
      static float convert(float v) { return v; }
    };

    template <typename image_t, typename cache_t>
    void gaussian_blur(image_t & image, cache_t & cache, float stddev, border_mode border_mode)
    {
      static_assert(std::is_same_v<image_t::color_t, cache_t::color_t>);

      using helpers = gaussian_blur_details::helpers<image_t::color_t>;

      const auto kernel = helpers::build_kernel(stddev);
      if (kernel.size() <= 1)
      {
        return;
      }

      int processing_size = static_cast<int>(kernel.size() - 1);

      auto processing = [&](auto && neighborhood)
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
        apply_vertical_processing<ggo::border_mode::mirror>(cache, image, processing_size, processing_size, processing);
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
    ggo::image_t<pt, lo> cache(image.size());

    gaussian_blur_details::gaussian_blur(image, cache, stddev, border_mode);
  }
}

// Dynamic images.
namespace ggo
{
  template <pixel_type pt, bool owns_buffer>
  void gaussian_blur_aux(image_base<void *, owns_buffer> & image, float stddev, border_mode border_mode)
  {
    GGO_ASSERT_EQ(pt, image.pixel_type());

    switch (image.memory_lines_order())
    {
    case ggo::lines_order::up:
    {
      ggo::image_view_t<pt, ggo::lines_order::up> view(image.data(), image.size(), image.line_byte_step());

      gaussian_blur(view, stddev, border_mode);
    }
    break;
    case ggo::lines_order::down:
    {
      ggo::image_view_t<pt, ggo::lines_order::down> view(image.data(), image.size(), image.line_byte_step());

      gaussian_blur(view, stddev, border_mode);
    }
    break;
    default:
      throw std::runtime_error("invalid lines order");
      break;
    }
  }

  template <bool owns_buffer>
  void gaussian_blur(image_base<void *, owns_buffer> & image, float stddev, border_mode border_mode = border_mode::mirror)
  {
    // Limited dispatch for now, see later if I should implement all color types.
    switch (image.pixel_type())
    {
    case ggo::pixel_type::y_8u:
      gaussian_blur_aux<ggo::pixel_type::y_8u>(image, stddev, border_mode);
      break;
    case ggo::pixel_type::y_32f:
      gaussian_blur_aux<ggo::pixel_type::y_32f>(image, stddev, border_mode);
      break;
    case ggo::pixel_type::rgb_8u:
      gaussian_blur_aux<ggo::pixel_type::rgb_8u>(image, stddev, border_mode);
      break;
    case ggo::pixel_type::bgr_8u:
      gaussian_blur_aux<ggo::pixel_type::bgr_8u>(image, stddev, border_mode);
      break;
    case ggo::pixel_type::bgrx_8u:
      gaussian_blur_aux<ggo::pixel_type::bgrx_8u>(image, stddev, border_mode);
      break;
    }
  }
}

#endif
