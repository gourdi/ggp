#pragma once

#include <kernel/math/shapes_2d/ggo_shapes2d.h>
#include <kernel/math/ggo_discretization.h>
#include <kernel/math/interpolation/ggo_triangle_interpolation.h>
#include <2d/ggo_color.h>
#include <2d/paint/ggo_layer.h>
#include <2d/blend/ggo_alpha_blend.h>

namespace ggo
{
  template <typename bkgd_color_t, typename brush_color_t, typename vertex_color_t, typename blender_t = alpha_blender<bkgd_color_t, brush_color_t>, typename data_t = float>
  struct color_triangle : public layer<bkgd_color_t, data_t>
  {
    using shape_t = ggo::triangle2d<data_t>;

    static_assert(std::is_floating_point_v<data_t> == true);
    static_assert(std::is_floating_point_v<typename ggo::color_traits<vertex_color_t>::sample_t> == true);

    color_triangle(const ggo::triangle2d<data_t> & triangle,
      const vertex_color_t & color1,
      const vertex_color_t & color2,
      const vertex_color_t & color3) :
      _triangle(triangle),
      _color1(color1),
      _color2(color2),
      _color3(color3)
    { }

    // Geometry.
    rect_data<data_t> get_bounding_rect() const override { return _triangle.get_bounding_rect(); }
    rect_intersection	get_rect_intersection(const rect_data<data_t> & rect_data) const override { return _triangle.get_rect_intersection(rect_data); }
    bool              is_point_inside(const ggo::pos2<data_t> & p) const override { return _triangle.is_point_inside(p); }

    // Paint.
    bkgd_color_t      paint(int x, int y, const bkgd_color_t & bkgd_color) const override
    {
      const ggo::pos2<data_t> p = ggo::get_pixel_center<data_t>({ x, y });
      auto c = ggo::triangular_interpolation<data_t, vertex_color_t, false>(_triangle.v1(), _color1, _triangle.v2(), _color2, _triangle.v3(), _color3, p);
      if (c.has_value() == false)
      {
        throw std::runtime_error("triangular interpolation failed");
      }
      return _blender(bkgd_color, convert_color_to<brush_color_t>(*c));
    }

    ggo::triangle2d<data_t> _triangle;
    vertex_color_t          _color1;
    vertex_color_t          _color2;
    vertex_color_t          _color3;
    blender_t               _blender;
  };
}
