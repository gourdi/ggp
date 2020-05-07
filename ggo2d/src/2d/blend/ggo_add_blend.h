#pragma once

#include <kernel/ggo_kernel.h>
#include <2d/ggo_color.h>

// Functions.
namespace ggo
{
  inline uint8_t add_blend(uint8_t bkgd_color, uint8_t brush_color)
  {
    return uint8_t(std::min<uint32_t>(0xff, uint32_t(bkgd_color) + uint32_t(brush_color)));
  }

  inline ggo::rgb_8u add_blend(const ggo::rgb_8u & bkgd_color, const ggo::rgb_8u & brush_color)
  {
    return {
      add_blend(bkgd_color.r(), brush_color.r()),
      add_blend(bkgd_color.g(), brush_color.g()),
      add_blend(bkgd_color.b(), brush_color.b()) };
  }

  inline float add_blend(float bkgd_color, float brush_color)
  {
    return std::min(1.f, bkgd_color + brush_color);
  }

  inline rgb_32f add_blend(const rgb_32f & bkgd_color, const rgb_32f & brush_color)
  {
    return {
      add_blend(bkgd_color.r(), brush_color.r()),
      add_blend(bkgd_color.g(), brush_color.g()),
      add_blend(bkgd_color.b(), brush_color.b()) };
  }
}

// Structs.
namespace ggo
{
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
