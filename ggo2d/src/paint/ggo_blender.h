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
}

namespace ggo
{  
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
      const ggo::color_32u bkgd_color_32u(static_cast<uint32_t>(bkgd_color.r()), static_cast<uint32_t>(bkgd_color.g()), static_cast<uint32_t>(bkgd_color.b()));
      const ggo::color_32u brush_color_32u(static_cast<uint32_t>(brush_color.r()), static_cast<uint32_t>(brush_color.g()), static_cast<uint32_t>(brush_color.b()));
      const ggo::color_32u output_color_32u(ggo::fixed_point_div<16>(_weight_bkgd * bkgd_color_32u + _weight_brush * brush_color_32u));

      return ggo::color_8u(static_cast<uint8_t>(output_color_32u.r()), static_cast<uint8_t>(output_color_32u.g()), static_cast<uint8_t>(output_color_32u.b()));
    }

    const uint32_t _weight_brush;
    const uint32_t _weight_bkgd;
  };

  template <>
  struct alpha_blender<float>
  {
    alpha_blender(float opacity) : _weight_brush(opacity), _weight_bkgd(1.f - opacity) { }

    float operator()(int x, int y, const float & bkgd_color, const float & brush_color) const
    {
      return _weight_brush * brush_color + _weight_bkgd * bkgd_color;
    }

    const float _weight_brush;
    const float _weight_bkgd;
  };

  template <>
  struct alpha_blender<ggo::color_32f>
  {
    alpha_blender(float opacity) : _weight_brush(opacity), _weight_bkgd(1.f - opacity) { }

    ggo::color_32f operator()(int x, int y, const ggo::color_32f & bkgd_color, const ggo::color_32f & brush_color) const
    {
      return _weight_brush * brush_color + _weight_bkgd * bkgd_color;
    }

    const float _weight_brush;
    const float _weight_bkgd;
  };

  using alpha_blender_rgb8u = alpha_blender<ggo::color_8u>;
  using alpha_blender_y32f = alpha_blender<float>;
  using alpha_blender_rgb32f = alpha_blender<ggo::color_32f>;
}

namespace ggo
{
  template <typename color_t>
  struct additive_blender { };

  template <>
  struct additive_blender<ggo::color_32f>
  {
    ggo::color_32f operator()(int x, int y, const ggo::color_32f & bkgd_color, const ggo::color_32f & brush_color) const
    {
      return brush_color + bkgd_color;
    }
  };
}

#endif

