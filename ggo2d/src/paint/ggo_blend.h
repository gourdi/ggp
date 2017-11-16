#ifndef __GGO_BLENDER__
#define __GGO_BLENDER__

#include <ggo_color.h>

/////////////////////////////////////////////////////////////////////
// Free functions.

// Alpha blending.
namespace ggo
{
  inline uint8_t alpha_blend(uint8_t bkgd_color, uint8_t paint_color, uint8_t opacity)
  {
    return ((0xff - opacity) * bkgd_color + opacity * paint_color + 128) / 255;
  }

  inline ggo::color_8u alpha_blend(const ggo::color_8u & bkgd_color, const ggo::color_8u & paint_color, uint8_t opacity)
  {
    const uint8_t inv_opacity = 0xff - opacity;
    const uint32_t r = inv_opacity * bkgd_color.r() + opacity * paint_color.r();
    const uint32_t g = inv_opacity * bkgd_color.g() + opacity * paint_color.g();
    const uint32_t b = inv_opacity * bkgd_color.b() + opacity * paint_color.b();
    return ggo::color_8u(
      static_cast<uint8_t>((r + 128) / 255),
      static_cast<uint8_t>((g + 128) / 255),
      static_cast<uint8_t>((b + 128) / 255));
  }
}

// Additive blending.
namespace ggo
{
  inline ggo::color_8u additive_blend(const ggo::color_8u & bkgd_color, const ggo::color_8u & paint_color)
  {
    return ggo::color_8u(
      uint8_t(std::min<int>(0xff, int(bkgd_color.r()) + int(paint_color.r()))),
      uint8_t(std::min<int>(0xff, int(bkgd_color.g()) + int(paint_color.g()))),
      uint8_t(std::min<int>(0xff, int(bkgd_color.b()) + int(paint_color.b()))));
  }
}

/////////////////////////////////////////////////////////////////////
// Structs.

// Base class.
namespace ggo
{
  template <typename color_t, typename brush_color_t>
  struct blender_abc
  {
    virtual color_t operator()(int x, int y, const color_t & bkgd_color, const brush_color_t & brush_color) const = 0;
  };
}

// Overwrite blending.
namespace ggo
{
  template <typename color_t, typename brush_color_t = color_t>
  struct overwrite_blender : public blender_abc<color_t, brush_color_t>
  {
    color_t operator()(int x, int y, const color_t & bkgd_color, const brush_color_t & brush_color) const override { return ggo::convert_color_to<color_t>(brush_color); }
  };
}

// Alpha blending.
namespace ggo
{  
  template <typename color_t, typename brush_color_t>
  struct alpha_blender
  {
  };

  template <>
  struct alpha_blender<uint8_t, uint8_t> : public blender_abc<uint8_t, uint8_t>
  {
    alpha_blender(float opacity) :
      _weight_brush(ggo::round_to<uint32_t>(0x10000 * opacity)),
      _weight_bkgd(0x10000 - _weight_brush)
    {
    }

    uint8_t operator()(int x, int y, const uint8_t & bkgd_color, const uint8_t & brush_color) const override
    {
      const uint32_t bkgd_color_32u(static_cast<uint32_t>(bkgd_color));
      const uint32_t brush_color_32u(static_cast<uint32_t>(brush_color));
      const uint32_t output_color_32u(ggo::fixed_point_div<16>(_weight_bkgd * bkgd_color_32u + _weight_brush * brush_color_32u));

      return static_cast<uint8_t>(output_color_32u);
    }

    const uint32_t _weight_brush;
    const uint32_t _weight_bkgd;
  };

  template <>
  struct alpha_blender<float, float> : public blender_abc<float, float>
  {
    alpha_blender(float opacity) : _weight_brush(opacity), _weight_bkgd(1.f - opacity) { }

    float operator()(int x, int y, const float & bkgd_color, const float & brush_color) const override
    {
      return _weight_brush * brush_color + _weight_bkgd * bkgd_color;
    }

    const float _weight_brush;
    const float _weight_bkgd;
  };

  template <>
  struct alpha_blender<ggo::color_8u, ggo::color_8u> : public blender_abc<ggo::color_8u, ggo::color_8u>
  {
    alpha_blender(float opacity) :
      _weight_brush(ggo::round_to<uint32_t>(0x10000 * opacity)),
      _weight_bkgd(0x10000 - _weight_brush)
    {
    }     

    ggo::color_8u operator()(int x, int y, const ggo::color_8u & bkgd_color, const ggo::color_8u & brush_color) const override
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
  struct alpha_blender<ggo::color_32f, ggo::color_32f> : public blender_abc<ggo::color_32f, ggo::color_32f>
  {
    alpha_blender(float opacity) : _weight_bkgd(1.f - opacity), _weight_brush(opacity) { }

    ggo::color_32f operator()(int x, int y, const ggo::color_32f & bkgd_color, const ggo::color_32f & brush_color) const override
    {
      return _weight_bkgd * bkgd_color + _weight_brush * brush_color;
    }

    const float _weight_bkgd;
    const float _weight_brush;
  };

  using alpha_blender_y8u = alpha_blender<uint8_t, uint8_t>;
  using alpha_blender_y32f = alpha_blender<float, float>;
  using alpha_blender_rgb8u = alpha_blender<ggo::color_8u, ggo::color_8u>;
  using alpha_blender_rgb32f = alpha_blender<ggo::color_32f, ggo::color_32f>;
}

// Additive blending.
namespace ggo
{
  template <typename color_t>
  struct additive_blender { };

  template <>
  struct additive_blender<ggo::color_8u> : public blender_abc<ggo::color_8u, ggo::color_8u>
  {
    ggo::color_8u operator()(int x, int y, const ggo::color_8u & bkgd_color, const ggo::color_8u & brush_color) const override
    {
      return additive_blend(bkgd_color, brush_color);
    }
  };

  template <>
  struct additive_blender<ggo::color_32f> : public blender_abc<ggo::color_32f, ggo::color_32f>
  {
    ggo::color_32f operator()(int x, int y, const ggo::color_32f & bkgd_color, const ggo::color_32f & brush_color) const override
    {
      return brush_color + bkgd_color;
    }
  };
}

#endif

