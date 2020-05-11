#pragma once

#include <kernel/ggo_kernel.h>
#include <2d/ggo_color.h>

/////////////////////////////////////////////////////////////////////
// cf. https://en.wikipedia.org/wiki/Alpha_compositing
/////////////////////////////////////////////////////////////////////

// Functions.
namespace ggo
{
  // Specialized functions with background with no alpha channel.
  inline constexpr uint8_t alpha_blend(uint8_t bkgd_color, ggo::ya_8u brush_color)
  {
    return ggo::round_div<uint32_t>(brush_color.a() * brush_color.y() + (0xff - brush_color.a()) * bkgd_color, 0xff);
  }

  inline constexpr uint16_t alpha_blend(uint16_t bkgd_color, ggo::ya_16u brush_color)
  {
    return ggo::round_div<uint32_t>(brush_color.a() * brush_color.y() + (0xffff - brush_color.a()) * bkgd_color, 0xffff);
  }

  inline constexpr uint32_t alpha_blend(uint32_t bkgd_color, ggo::ya_32u brush_color)
  {
    uint64_t bkgd_color_64 = static_cast<uint64_t>(bkgd_color);
    uint64_t brush_color_y_64 = static_cast<uint64_t>(brush_color.y());
    uint64_t brush_color_a_64 = static_cast<uint64_t>(brush_color.a());

    return static_cast<uint32_t>(ggo::round_div<uint64_t>(brush_color_a_64 * brush_color_y_64 + (0xffffffff - brush_color_a_64) * bkgd_color_64, 0xffffffff));
  }

  inline constexpr float alpha_blend(float bkgd_color, ggo::ya_32f brush_color)
  {
    return brush_color.a() * brush_color.y() + (1.f - brush_color.a()) * bkgd_color;
  }

  inline constexpr ggo::rgb_8u alpha_blend(const ggo::rgb_8u & bkgd_color, const ggo::rgba_8u & brush_color)
  {
    uint32_t inv_opacity = 0xff - brush_color.a();

    return ggo::rgb_8u(
      ggo::round_div<uint32_t>(brush_color.a() * brush_color.r() + inv_opacity * bkgd_color.r(), 0xff),
      ggo::round_div<uint32_t>(brush_color.a() * brush_color.g() + inv_opacity * bkgd_color.g(), 0xff),
      ggo::round_div<uint32_t>(brush_color.a() * brush_color.b() + inv_opacity * bkgd_color.b(), 0xff));
  }

  inline constexpr ggo::rgb_16u alpha_blend(const ggo::rgb_16u & bkgd_color, const ggo::rgba_16u & brush_color)
  {
    uint32_t inv_opacity = 0xffff - brush_color.a();

    return ggo::rgb_16u(
      ggo::round_div<uint32_t>(brush_color.a() * brush_color.r() + inv_opacity * bkgd_color.r(), 0xffff),
      ggo::round_div<uint32_t>(brush_color.a() * brush_color.g() + inv_opacity * bkgd_color.g(), 0xffff),
      ggo::round_div<uint32_t>(brush_color.a() * brush_color.b() + inv_opacity * bkgd_color.b(), 0xffff));
  }

  inline constexpr ggo::rgb_32u alpha_blend(const ggo::rgb_32u & bkgd_color, const ggo::rgba_32u & brush_color)
  {
    uint64_t bkgd_color_t_64 = static_cast<uint64_t>(bkgd_color.r());
    uint64_t bkgd_color_g_64 = static_cast<uint64_t>(bkgd_color.g());
    uint64_t bkgd_color_b_64 = static_cast<uint64_t>(bkgd_color.b());

    uint64_t brush_color_r_64 = static_cast<uint64_t>(brush_color.r());
    uint64_t brush_color_g_64 = static_cast<uint64_t>(brush_color.g());
    uint64_t brush_color_b_64 = static_cast<uint64_t>(brush_color.b());
    uint64_t brush_color_a_64 = static_cast<uint64_t>(brush_color.a());

    uint64_t inv_opacity_64 = 0xffffffff - brush_color_a_64;

    return ggo::rgb_32u(
      static_cast<uint32_t>(ggo::round_div<uint64_t>(brush_color_a_64 * brush_color_r_64 + inv_opacity_64 * bkgd_color_t_64, 0xffffffff)),
      static_cast<uint32_t>(ggo::round_div<uint64_t>(brush_color_a_64 * brush_color_g_64 + inv_opacity_64 * bkgd_color_g_64, 0xffffffff)),
      static_cast<uint32_t>(ggo::round_div<uint64_t>(brush_color_a_64 * brush_color_b_64 + inv_opacity_64 * bkgd_color_b_64, 0xffffffff)));
  }

  inline constexpr ggo::rgb_32f alpha_blend(const ggo::rgb_32f & bkgd_color, const ggo::rgba_32f & brush_color)
  {
    float inv_opacity = 1.f - brush_color.a();

    return ggo::rgb_32f(
      brush_color.a() * brush_color.r() + inv_opacity * bkgd_color.r(),
      brush_color.a() * brush_color.g() + inv_opacity * bkgd_color.g(),
      brush_color.a() * brush_color.b() + inv_opacity * bkgd_color.b());
  }

  // Specialized functions with background with an alpha channel.
  inline constexpr ggo::ya_8u alpha_blend(const ggo::ya_8u & bkgd_color, const ggo::ya_8u & brush_color)
  {
    uint32_t cropped_bkgd_a = ggo::round_div<uint32_t>((0xff - brush_color.a()) * bkgd_color.a(), 0xff);
    uint32_t a = brush_color.a() + cropped_bkgd_a;

    if (a == 0x00)
    {
      return { 0x00, 0x00 };
    }
    else
    {
      uint32_t y = ggo::round_div<uint32_t>(brush_color.y() * brush_color.a() + bkgd_color.y() * cropped_bkgd_a, a);
      return { static_cast<uint8_t>(y), static_cast<uint8_t>(a) };
    }
  }

  //inline ggo::ya_16u alpha_blend(const ggo::ya_16u & bkgd_color, const ggo::ya_16u & brush_color)
  //{
  //  throw std::runtime_error("unsupported alpha blending");
  //  return bkgd_color; // TODO
  //}

  //inline ggo::ya_32u alpha_blend(const ggo::ya_32u & bkgd_color, const ggo::ya_32u & brush_color)
  //{
  //  throw std::runtime_error("unsupported alpha blending");
  //  return bkgd_color; // TODO
  //}

  inline ggo::ya_32f alpha_blend(const ggo::ya_32f & bkgd_color, const ggo::ya_32f & brush_color)
  {
    float cropped_bkgd_a = (1.f - brush_color.a()) * bkgd_color.a();
    float a = brush_color.a() + cropped_bkgd_a;

    if (a == 0.f)
    {
      return { 0.f, 0.f };
    }
    else
    {
      return { (brush_color.y() * brush_color.a() + bkgd_color.y() * cropped_bkgd_a) / a, a };
    }
  }

  inline ggo::rgba_8u alpha_blend(const ggo::rgba_8u & bkgd_color, const ggo::rgba_8u & brush_color)
  {
    uint32_t cropped_bkgd_a = ggo::round_div<uint32_t>((0xff - brush_color.a()) * bkgd_color.a(), 0xff);
    uint32_t a = brush_color.a() + cropped_bkgd_a;

    if (a == 0x00)
    {
      return { 0x00, 0x00, 0x00, 0x00 };
    }
    else
    {
      uint32_t r = ggo::round_div<uint32_t>(brush_color.r() * brush_color.a() + bkgd_color.r() * cropped_bkgd_a, a);
      uint32_t g = ggo::round_div<uint32_t>(brush_color.g() * brush_color.a() + bkgd_color.g() * cropped_bkgd_a, a);
      uint32_t b = ggo::round_div<uint32_t>(brush_color.b() * brush_color.a() + bkgd_color.b() * cropped_bkgd_a, a);

      return { static_cast<uint8_t>(r), static_cast<uint8_t>(g), static_cast<uint8_t>(b), static_cast<uint8_t>(a) };
    }
  }

  //inline ggo::rgba_16u alpha_blend(const ggo::rgba_16u & bkgd_color, const ggo::rgba_16u & brush_color)
  //{
  //  throw std::runtime_error("unsupported alpha blending");
  //  return bkgd_color; // TODO
  //}

  //inline ggo::rgba_32u alpha_blend(const ggo::rgba_32u & bkgd_color, const ggo::rgba_32u & brush_color)
  //{
  //  throw std::runtime_error("unsupported alpha blending");
  //  return bkgd_color; // TODO
  //}

  inline ggo::rgba_32f alpha_blend(const ggo::rgba_32f & bkgd_color, const ggo::rgba_32f & brush_color)
  {
    float cropped_bkgd_a = (1.f - brush_color.a()) * bkgd_color.a();
    float a = brush_color.a() + cropped_bkgd_a;

    if (a == 0.f)
    {
      return { 0.f, 0.f, 0.f, 0.f };
    }
    else
    {
      return {
        (brush_color.r() * brush_color.a() + bkgd_color.r() * cropped_bkgd_a) / a,
        (brush_color.g() * brush_color.a() + bkgd_color.g() * cropped_bkgd_a) / a,
        (brush_color.b() * brush_color.a() + bkgd_color.b() * cropped_bkgd_a) / a,
        a };
    }
  }

  // Generic function, uses the specialized functions.
  template <typename bkgd_color_t, typename brush_color_t>
  constexpr bkgd_color_t alpha_blend(const bkgd_color_t & bkgd_color, const brush_color_t & brush_color)
  {
    constexpr color_space bkgd_color_space  = color_traits<bkgd_color_t>::color_space;

    // The brush does not have an alpha channel, so it just overwrite the background.
    if constexpr (has_alpha_v<brush_color_t> == false)
    {
      return ggo::convert_color_to<bkgd_color_t>(brush_color);
    }
    else
    {
      if constexpr (has_alpha_v<bkgd_color_t> == true)
      {
        // Here both brush and background have an alpha channel, so we just convert the brush pixel type
        // to the background pixel type and then use specialized functions above.
        return alpha_blend(bkgd_color, ggo::convert_color_to<bkgd_color_t>(brush_color));
      }
      else
      {
        // Here the brush has alpha but the background does not. So we must convert the brush
        // to the pixel type of the background with an alpha channel.
        using convert_brush_color_t = color_type_t<add_alpha(bkgd_color_space), sample_type_t<bkgd_color_t>>;

        return alpha_blend(bkgd_color, ggo::convert_color_to<convert_brush_color_t>(brush_color));
      }
    }
  }
}

// Struct calling the above functions.
namespace ggo
{
  template <typename bkgd_color_t, typename brush_color_t>
  struct alpha_blender
  {
    bkgd_color_t operator()(bkgd_color_t bkgd_color, brush_color_t brush_color) const
    {
      return alpha_blend(bkgd_color, brush_color);
    }
  };
}


