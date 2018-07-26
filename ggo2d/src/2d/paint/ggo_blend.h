#ifndef __GGO_BLENDER__
#define __GGO_BLENDER__

#include <kernel/ggo_kernel.h>
#include <2d/ggo_color.h>

/////////////////////////////////////////////////////////////////////
// Alpha blending.
namespace ggo
{
  // Functions.
  template <typename bkgd_color_t, typename brush_color_t>
  bkgd_color_t alpha_blend(const bkgd_color_t & bkgd_color, const brush_color_t & brush_color)
  {
    //static_assert(ggo::color_traits<brush_color_t>::has_alpha == false);
    GGO_ASSERT(ggo::color_traits<brush_color_t>::has_alpha == false); // Until all the functions are implemented...

    return ggo::convert_color_to<bkgd_color_t>(brush_color);
  }

  inline ggo::color_8u alpha_blend(const ggo::color_8u & bkgd_color, const ggo::alpha_color_8u & brush_color)
  {
    uint8_t inv_opacity = 0xff - brush_color.a();

    return ggo::color_8u(
      ggo::round_div(brush_color.a() * brush_color.r() + inv_opacity * bkgd_color.r(), 0xff),
      ggo::round_div(brush_color.a() * brush_color.g() + inv_opacity * bkgd_color.g(), 0xff),
      ggo::round_div(brush_color.a() * brush_color.b() + inv_opacity * bkgd_color.b(), 0xff));
  }

  inline ggo::alpha_color_8u alpha_blend(const ggo::alpha_color_8u & bkgd_color, const ggo::alpha_color_8u & brush_color)
  {
    return linerp(brush_color, bkgd_color, one_log2_fract);
  }

  template <typename real_t>
  ggo::color_8u alpha_blend(const ggo::color_8u & bkgd_color, const ggo::color_8u & brush_color, real_t opacity)
  {
    static_assert(std::is_floating_point<real_t>::value);

    uint8_t opacity_8u = ggo::round_to<uint8_t>(ggo::clamp(opacity, 0.f, 1.f) * 0xff);

    alpha_blend(bkgd_color, brush_color, opacity_8u);
  }

  // Structs.
  template <typename color_t>
  struct alpha_blender
  {
    static_assert(std::is_same<typename ggo::color_traits<color_t>::sample_t, float>::value == true);

    float _opacity;
    float _inv_opacity;

    alpha_blender(float opacity) : _opacity(opacity), _inv_opacity(1.f - opacity) {}

    color_t operator()(int x, int y, const color_t & bkgd_color, const color_t & brush_color) const
    {
      return _inv_opacity * bkgd_color + _opacity * brush_color;
    }
  };

  template <>
  struct alpha_blender<uint8_t>
  {
    static constexpr uint32_t bit_shift = 8;
    static constexpr uint32_t one = 1 << bit_shift;

    uint32_t _opacity;
    uint32_t _inv_opacity;

    alpha_blender(float opacity)
      :
      _opacity(ggo::round_to<uint32_t>(opacity * one)),
      _inv_opacity(ggo::round_to<uint32_t>((1.f - opacity) * one))
    {}

    uint8_t operator()(int x, int y, uint8_t bkgd_color, uint8_t brush_color) const
    {
      return fixed_point_div<bit_shift>(_inv_opacity * bkgd_color + _opacity * brush_color);
    }

    uint8_t operator()(uint8_t bkgd_color, uint8_t brush_color) const
    {
      return operator()(0, 0, bkgd_color, brush_color);
    }
  };

  template <>
  struct alpha_blender<ggo::color_8u>
  {
    static constexpr uint32_t bit_shift = 8;
    static constexpr uint32_t one = 1 << bit_shift;

    uint32_t _opacity;
    uint32_t _inv_opacity;

    alpha_blender(float opacity)
      :
      _opacity(ggo::round_to<uint32_t>(opacity * one)),
      _inv_opacity(ggo::round_to<uint32_t>((1.f - opacity) * one))
    {}

    color_8u operator()(int x, int y, const color_8u & bkgd_color, const color_8u & brush_color) const
    {
      return {
        static_cast<uint8_t>(fixed_point_div<bit_shift>(_inv_opacity * bkgd_color.r() + _opacity * brush_color.r())),
        static_cast<uint8_t>(fixed_point_div<bit_shift>(_inv_opacity * bkgd_color.g() + _opacity * brush_color.g())),
        static_cast<uint8_t>(fixed_point_div<bit_shift>(_inv_opacity * bkgd_color.b() + _opacity * brush_color.b())) };
    }

    color_8u operator()(const color_8u & bkgd_color, const color_8u & brush_color) const
    {
      return operator()(0, 0, bkgd_color, brush_color);
    }
  };

  template <>
  struct alpha_blender<ggo::alpha_color_8u>
  {
    static constexpr uint32_t bit_shift = 8;
    static constexpr uint32_t one = 1 << bit_shift;

    uint32_t _opacity;

    alpha_blender(float opacity)
    :
    _opacity(ggo::round_to<uint32_t>(opacity * one))
    {}

    alpha_color_8u operator()(int x, int y, const alpha_color_8u & bkgd_color, const alpha_color_8u & brush_color) const
    {
      return linerp(brush_color, bkgd_color, ggo::log2_fract<bit_shift>(_opacity));
    }

    alpha_color_8u operator()(const alpha_color_8u & bkgd_color, const alpha_color_8u & brush_color) const
    {
      return operator()(0, 0, bkgd_color, brush_color);
    }
  };

  using alpha_blender_y8u = ggo::alpha_blender<uint8_t>;
  using alpha_blender_rgb8u = ggo::alpha_blender<color_8u>;
  using alpha_blender_rgba8u = ggo::alpha_blender<alpha_color_8u>;
  using alpha_blender_y32f = ggo::alpha_blender<float>;
  using alpha_blender_rgb32f = ggo::alpha_blender<color_32f>;
}

/////////////////////////////////////////////////////////////////////
// Overwrite blending.
namespace ggo
{
  // Structs.
  template <typename color_t, typename brush_color_t = color_t>
  struct overwrite_blender
  {
    static_assert(color_traits<brush_color_t>::has_alpha == false);

    color_t operator()(int x, int y, const color_t & bkgd_color, const brush_color_t & brush_color) const
    {
      return convert_color_to<color_t>(brush_color);
    }
  };

  template <>
  struct overwrite_blender<alpha_color_8u, alpha_color_8u>
  {
    alpha_color_8u operator()(int x, int y, const alpha_color_8u & bkgd_color, const alpha_color_8u & brush_color) const
    {
      return alpha_blend(bkgd_color, brush_color);
    }
  };
}

/////////////////////////////////////////////////////////////////////
// Additive blending.
namespace ggo
{
  // Free functions.
  inline 
  uint8_t add_blend(uint8_t bkgd_color, uint8_t brush_color)
  {
    return uint8_t(std::min<uint32_t>(0xff, uint32_t(bkgd_color) + uint32_t(brush_color)));
  }

  inline
  ggo::color_8u add_blend(const ggo::color_8u & bkgd_color, const ggo::color_8u & brush_color)
  {
    return {
      add_blend(bkgd_color.r(), brush_color.r()),
      add_blend(bkgd_color.g(), brush_color.g()),
      add_blend(bkgd_color.b(), brush_color.b()) };
  }

  inline
  float add_blend(float bkgd_color, float brush_color)
  {
    return std::min(1.f, bkgd_color + brush_color);
  }

  inline
  color_32f add_blend(const color_32f & bkgd_color, const color_32f & brush_color)
  {
    return {
      add_blend(bkgd_color.r(), brush_color.r()),
      add_blend(bkgd_color.g(), brush_color.g()),
      add_blend(bkgd_color.b(), brush_color.b()) };
  }

  // Structs.
  template <typename color_t, typename brush_color_t = color_t>
  struct add_blender
  {
    color_t operator()(int x, int y, const color_t & bkgd_color, const brush_color_t & brush_color) const
    {
      return add_blend(bkgd_color, convert_color_to<color_t>(brush_color));
    }

    color_t operator()(const color_t & bkgd_color, const brush_color_t & brush_color) const
    {
      return operator()(0, 0, bkgd_color, convert_color_to<color_t>(brush_color));
    }
  };
}

#endif
