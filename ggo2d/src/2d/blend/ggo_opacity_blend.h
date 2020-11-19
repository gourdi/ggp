#pragma once

#include <2d/ggo_color.h>
#include <2d/blend/ggo_alpha_blend.h>

namespace ggo
{
  /////////////////////////////////////////////////////////////////////
  template <typename bkgd_color_t, typename blend_t>
  struct opacity_blender
  {
  };

  /////////////////////////////////////////////////////////////////////
  template <typename blend_t>
  struct opacity_blender<float, blend_t>
  {
    const float _opacity;
    const float _inv_opacity;
    const blend_t _blend;

    opacity_blender(float opacity, blend_t blend = blend_t())
      :
      _opacity(opacity),
      _inv_opacity(1 - opacity),
      _blend(blend)
    {
    }

    template <typename brush_color_t>
    float operator()(float bkgd_color, brush_color_t brush_color) const
    {
      return _inv_opacity * bkgd_color + _opacity * _blend(bkgd_color, brush_color);
    }
  };

  /////////////////////////////////////////////////////////////////////
  template <typename blend_t>
  struct opacity_blender<uint8_t, blend_t>
  {
    static constexpr uint32_t bit_shift = 8;
    static constexpr uint32_t one = 1 << bit_shift;

    const uint32_t _opacity;
    const uint32_t _inv_opacity;
    const blend_t _blend;

    template <typename scalar_t>
    opacity_blender(scalar_t opacity, blend_t blend = blend_t())
      :
      _opacity(ggo::round_to<uint32_t>(opacity* one)),
      _inv_opacity(ggo::round_to<uint32_t>((1 - opacity)* one)),
      _blend(blend)
    {
      static_assert(std::is_floating_point_v<scalar_t> == true);
    }

    template <typename brush_color_t>
    uint8_t operator()(uint8_t bkgd_color, brush_color_t brush_color) const
    {
      return fixed_point_div<bit_shift>(_inv_opacity * bkgd_color + _opacity * _blend(bkgd_color, brush_color));
    }
  };

  /////////////////////////////////////////////////////////////////////
  template <typename blend_t>
  struct opacity_blender<ggo::rgb_8u, blend_t>
  {
    static constexpr uint32_t bit_shift = 8;
    static constexpr uint32_t one = 1 << bit_shift;

    const uint32_t _opacity;
    const uint32_t _inv_opacity;
    const blend_t _blend;

    template <typename scalar_t>
    opacity_blender(scalar_t opacity, blend_t blend = blend_t())
      :
      _opacity(ggo::round_to<uint32_t>(opacity * one)),
      _inv_opacity(ggo::round_to<uint32_t>((1 - opacity) * one)),
      _blend(blend)
    {
      static_assert(std::is_floating_point_v<scalar_t> == true);
    }

    template <typename brush_color_t>
    rgb_8u operator()(const rgb_8u & bkgd_color, const brush_color_t & brush_color) const
    {
      if constexpr (std::is_same_v<blend_t, alpha_blender<rgb_8u, rgb_8u>>)
      {
        return {
          static_cast<uint8_t>(fixed_point_div<bit_shift>(_inv_opacity * bkgd_color.r() + _opacity * brush_color.r())),
          static_cast<uint8_t>(fixed_point_div<bit_shift>(_inv_opacity * bkgd_color.g() + _opacity * brush_color.g())),
          static_cast<uint8_t>(fixed_point_div<bit_shift>(_inv_opacity * bkgd_color.b() + _opacity * brush_color.b())) };
      }
      else
      {
        rgb_8u blended_brush_color = _blend(bkgd_color, brush_color);

        return {
          static_cast<uint8_t>(fixed_point_div<bit_shift>(_inv_opacity * bkgd_color.r() + _opacity * blended_brush_color.r())),
          static_cast<uint8_t>(fixed_point_div<bit_shift>(_inv_opacity * bkgd_color.g() + _opacity * blended_brush_color.g())),
          static_cast<uint8_t>(fixed_point_div<bit_shift>(_inv_opacity * bkgd_color.b() + _opacity * blended_brush_color.b())) };
      }
    }
  };


#if 0 
  template <>
  struct alpha_blender<ggo::rgba_8u>
  {
    static constexpr uint32_t bit_shift = 8;
    static constexpr uint32_t one = 1 << bit_shift;

    const uint32_t _opacity;

    alpha_blender(float opacity)
      :
      _opacity(ggo::round_to<uint32_t>(opacity* one))
    {}

    rgba_8u operator()(int x, int y, const rgba_8u& bkgd_color, const rgba_8u& brush_color) const
    {
      THIS LOOKS REALLY WRONG
        return linerp(brush_color, bkgd_color, ggo::log2_fract<bit_shift>(_opacity));
    }

    rgba_8u operator()(const rgba_8u& bkgd_color, const rgba_8u& brush_color) const
    {
      return operator()(0, 0, bkgd_color, brush_color);
    }
  };
#endif
}

