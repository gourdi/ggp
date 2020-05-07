#pragma once

#include <kernel/ggo_kernel.h>
#include <2d/ggo_color.h>

// Functions.
namespace ggo
{
  inline uint8_t additive_blend(uint8_t bkgd_color, uint8_t brush_color)
  {
    return uint8_t(std::min<uint32_t>(0xff, uint32_t(bkgd_color) + uint32_t(brush_color)));
  }

  inline ggo::rgb_8u additive_blend(const ggo::rgb_8u & bkgd_color, const ggo::rgb_8u & brush_color)
  {
    return {
      additive_blend(bkgd_color.r(), brush_color.r()),
      additive_blend(bkgd_color.g(), brush_color.g()),
      additive_blend(bkgd_color.b(), brush_color.b()) };
  }

  inline float additive_blend(float bkgd_color, float brush_color)
  {
    return std::min(1.f, bkgd_color + brush_color);
  }

  inline rgb_32f additive_blend(const rgb_32f & bkgd_color, const rgb_32f & brush_color)
  {
    return {
      additive_blend(bkgd_color.r(), brush_color.r()),
      additive_blend(bkgd_color.g(), brush_color.g()),
      additive_blend(bkgd_color.b(), brush_color.b()) };
  }
}

// Basic additive blending (without opacity). Just add the pixels, and use the alpha channel if available.
namespace ggo
{
  template <typename bkgd_color_t, typename brush_color_t = bkgd_color_t>
  struct additive_blender
  {
    bkgd_color_t operator()(const bkgd_color_t & bkgd_color, const brush_color_t & brush_color) const
    {
      return additive_blend(bkgd_color, brush_color);
    }
  };
}
