#ifndef __GGO_BLENDER__
#define __GGO_BLENDER__

#include <ggo_color.h>

namespace ggo
{
  template <typename color_t>
  struct overwrite_blender
  {
    const color_t & operator()(int x, int y, const color_t & bkgd_color, const color_t & brush_color) const { return brush_color; }
  };
  
  template <typename color_t>
  struct alpha_blender { };

  template <>
  struct alpha_blender<ggo::color_8u>
  {
    alpha_blender(float opacity) :
      _weight_brush(ggo::to<uint32_t>(0x10000 * opacity)),
      _weight_bkgd(0x10000 - _weight_brush)
    {
    }     

    ggo::color_8u operator()(int x, int y, const ggo::color_8u & bkgd_color, const ggo::color_8u & brush_color) const
    {
      const ggo::color_32u bkgd_color_32u(bkgd_color._r, bkgd_color._g, bkgd_color._b);
      const ggo::color_32u brush_color_32u(brush_color._r, brush_color._g, brush_color._b);
      const ggo::color_32u output_color_32u(ggo::fixed_point_div<16>(_weight_bkgd * bkgd_color_32u + _weight_brush * brush_color_32u));

      return ggo::color_8u(output_color_32u._r, output_color_32u._g, output_color_32u._b);
    }

    const uint32_t _weight_brush;
    const uint32_t _weight_bkgd;
  };
}

#endif

