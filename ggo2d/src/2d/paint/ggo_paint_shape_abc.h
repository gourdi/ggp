#ifndef __GGO_PAINT_SHAPE__
#define __GGO_PAINT_SHAPE__

#include <2d/paint/ggo_blend.h>

namespace ggo
{
  // Abstract base class paint shape.
  template <typename data_type, typename color_type, typename brush_color_type = color_type>
  struct paint_shape_abc
  {
    using data_t = typename data_type;
    using color_t = typename color_type;
    using brush_color_t = typename brush_color_type;

    static_assert(std::is_floating_point<data_t>::value == true);

    virtual rect_data<data_t> get_bounding_rect() const = 0;
    virtual rect_intersection	get_rect_intersection(const rect_data<data_t> & rect_data) const = 0;
    virtual bool              is_point_inside(const ggo::pos2<data_t> & p) const = 0;

    virtual brush_color_t brush(int x, int y) const = 0;
    virtual color_t blend(int x, int y, const color_t & bkgd_color, const brush_color_t & brush_color) const = 0;
  };

  // Solid color shape.
  template <typename shape_t, typename color_t, typename blender_t = ggo::overwrite_blender<color_t, color_t>>
  struct solid_color_shape : public paint_shape_abc<typename shape_t::data_t, color_t, color_t>
  {
    using data_t = typename shape_t::data_t;

    shape_t _shape;
    color_t _color;
    blender_t _blender;

    solid_color_shape(const shape_t & shape, const color_t & color, const blender_t & blender = blender_t()) : _shape(shape), _color(color), _blender(blender) {}

    rect_data<data_t> get_bounding_rect() const override { return _shape.get_bounding_rect(); }
    rect_intersection	get_rect_intersection(const rect_data<data_t> & rect_data) const override { return _shape.get_rect_intersection(rect_data); }
    bool              is_point_inside(const ggo::pos2<data_t> & p) const override { return _shape.is_point_inside(p); }

    color_t brush(int x, int y) const override { return _color; }
    color_t blend(int x, int y, const color_t & bkgd_color, const color_t & brush_color) const override { return _blender(x, y, bkgd_color, brush_color); }
  };
}

#endif