#pragma once

#include <2d/brush/ggo_solid_color_brush.h>
#include <2d/blend/ggo_alpha_blend.h>

namespace ggo
{
  // Abstract base class layer.
  template <typename color_t_, typename data_t_ = float>
  struct layer
  {
    using color_t = typename color_t_;
    using data_t = typename data_t_;

    static_assert(std::is_floating_point_v<data_t> == true);

    // Geometry.
    virtual rect_data<data_t> get_bounding_rect() const = 0;
    virtual rect_intersection	get_rect_intersection(const rect_data<data_t> & rect_data) const = 0;
    virtual bool              is_point_inside(const ggo::pos2<data_t> & p) const = 0;

    // Paint.
    virtual color_t           paint(int x, int y, const color_t & bkgd_color) const = 0;
  };

  // Static layer.
  template <typename shape_t, typename bkgd_color_t, typename brush_t = solid_color_brush<bkgd_color_t>, typename blender_t = ggo::alpha_blender<bkgd_color_t, typename brush_t::color_t>>
  struct layer_t : public layer<bkgd_color_t, typename shape_t::data_t>
  {
    using data_t = typename shape_t::data_t;

    shape_t _shape;
    brush_t _brush;
    blender_t _blender;

    layer_t(const shape_t & shape, const brush_t & brush, const blender_t & blender = blender_t()) : _shape(shape), _brush(brush), _blender(blender) {}

    rect_data<data_t> get_bounding_rect() const override { return _shape.get_bounding_rect(); }
    rect_intersection	get_rect_intersection(const rect_data<data_t> & rect_data) const override { return _shape.get_rect_intersection(rect_data); }
    bool              is_point_inside(const ggo::pos2<data_t> & p) const override { return _shape.is_point_inside(p); }

    bkgd_color_t      paint(int x, int y, const bkgd_color_t & bkgd_color) const override { return _blender(bkgd_color, _brush(x, y)); }
  };
}

