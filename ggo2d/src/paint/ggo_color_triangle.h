#ifndef __GGO_COLOR_TRIANGLE__
#define __GGO_COLOR_TRIANGLE__

#include <ggo_shapes2d.h>
#include <ggo_triangle_interpolation.h>

namespace ggo
{
  template <typename blender_t, typename color_t, typename color_real_t, typename real_type>
  struct color_triangle
  {
    using real_t = real_type;

    rect_data<real_type>  get_bounding_rect() const { return _triangle.get_bounding_rect(); }
    rect_intersection	    get_rect_intersection(const rect_data<real_type> & rect_data) const { return _triangle.get_rect_intersection(rect_data); }
    bool                  is_point_inside(real_type x_f, real_type y_f) const { return _triangle.is_point_inside(x_f, y_f); }

    color_t brush(int x, int y) const
    {
      color_real_t color(0);
      const ggo::pos2<real_t> p(static_cast<real_type>(x), static_cast<real_type>(y));
      ggo::triangular_interpolation<real_type, color_real_t, false>(_triangle.v1(), _color1, _triangle.v2(), _color2, _triangle.v3(), _color3, p, color);
      return ggo::convert_color_to<color_t>(color);
    }

    color_t blend(const color_t & bkgd_color, const color_t & brush_color) const
    { 
      return _blender.blend(bkgd_color, brush_color);
    }

    ggo::triangle2d<real_t> _triangle;
    color_real_t            _color1;
    color_real_t            _color2;
    color_real_t            _color3;
    blender_t               _blender;
  };
}

#endif
