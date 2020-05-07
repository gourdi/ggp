#pragma once

#include <2d/ggo_color.h>
#include <2d/blend/ggo_alpha_blend.h>

// Structs.
namespace ggo
{
  template <typename color_t>
  struct overwrite_blender
  {
    color_t operator()(int x, int y, const color_t & bkgd_color, const color_t & brush_color) const
    {
      if constexpr (color_traits<color_t>::has_alpha == false)
      {
        return brush_color;
      }
      else
      {
        return alpha_blend(bkgd_color, brush_color);
      }
    }
  };
}
