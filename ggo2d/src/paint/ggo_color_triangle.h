#ifndef __GGO_COLOR_TRIANGLE__
#define __GGO_COLOR_TRIANGLE__

#include <ggo_shapes2d.h>
#include <ggo_triangle_interpolation.h>

namespace ggo
{
  template <typename blender_t, typename color_t>
  struct color_triangle
  {
    using floating_point_color_t = typename color_traits<color_t>::floating_point_t;
    using real_t = typename color_traits<floating_point_color_t>::sample_t;

    static_assert(std::is_floating_point<real_t>::value, "expecting floating point type");

    color_triangle(const ggo::triangle2d<real_t> & triangle,
      const color_t & color1, const color_t & color2, const color_t & color3,
      const blender_t & blender) :
      _triangle(triangle),
      _color1(ggo::convert_color_to<floating_point_color_t>(color1)),
      _color2(ggo::convert_color_to<floating_point_color_t>(color2)),
      _color3(ggo::convert_color_to<floating_point_color_t>(color3)),
      _blender(blender)
    { }

    color_triangle(const ggo::triangle2d<real_t> & triangle,
      const floating_point_color_t & color1, const floating_point_color_t & color2, const floating_point_color_t & color3,
      const blender_t & blender) :
      _triangle(triangle),
      _color1(color1),
      _color2(color2),
      _color3(color3),
      _blender(blender)
    { }

    // Geometry.
    rect_data<real_t> get_bounding_rect() const { return _triangle.get_bounding_rect(); }
    rect_intersection	get_rect_intersection(const rect_data<real_t> & rect_data) const { return _triangle.get_rect_intersection(rect_data); }
    bool              is_point_inside(const ggo::pos2<real_t> & p) const { return _triangle.is_point_inside(p); }

    // Brush.
    color_t brush(int x, int y) const
    {
      floating_point_color_t color(0);
      const ggo::pos2<real_t> p(static_cast<real_t>(x), static_cast<real_t>(y));
      ggo::triangular_interpolation<real_t, floating_point_color_t, false>(_triangle.v1(), _color1, _triangle.v2(), _color2, _triangle.v3(), _color3, p, color);
      return ggo::convert_color_to<color_t>(color);
    }

    // Blend.
    color_t blend(int x, int y, const color_t & bkgd_color, const color_t & brush_color) const
    { 
      return _blender(x, y, bkgd_color, brush_color);
    }

    ggo::triangle2d<real_t> _triangle;
    floating_point_color_t  _color1;
    floating_point_color_t  _color2;
    floating_point_color_t  _color3;
    blender_t               _blender;
  };
}

namespace ggo
{
  template <typename color_t>
  struct alpha_color_triangle
  {
    using floating_point_color_t = typename color_traits<color_t>::floating_point_t;
    using real_t = typename color_traits<floating_point_color_t>::sample_t;
    using rgba_t = ggo::vec<real_t, 4, none_t>;

    static_assert(std::is_floating_point<real_t>::value, "expecting floating point type");
    static_assert(ggo::color_traits<floating_point_color_t>::samples_count == 3, "only rgb colors supported");

    // For performance reasons, color are expecting to be of a floating point type,
    // even if the class render integer colors.
    alpha_color_triangle(const ggo::triangle2d<real_t> & triangle,
      const color_t & color1, const real_t & opacity1, 
      const color_t & color2, const real_t & opacity2,
      const color_t & color3, const real_t & opacity3) :
      _triangle(triangle)
    {
       auto color1_fp = ggo::convert_color_to<floating_point_color_t>(color1);
       _color1.template set<0>(color1_fp.r());
       _color1.template set<1>(color1_fp.g());
       _color1.template set<2>(color1_fp.b());
       _color1.template set<3>(opacity1);

       auto color2_fp = ggo::convert_color_to<floating_point_color_t>(color2);
       _color2.template set<0>(color2_fp.r());
       _color2.template set<1>(color2_fp.g());
       _color2.template set<2>(color2_fp.b());
       _color2.template set<3>(opacity2);

       auto color3_fp = ggo::convert_color_to<floating_point_color_t>(color3);
       _color3.template set<0>(color3_fp.r());
       _color3.template set<1>(color3_fp.g());
       _color3.template set<2>(color3_fp.b());
       _color3.template set<3>(opacity3);
    }

    // Geometry.
    rect_data<real_t> get_bounding_rect() const { return _triangle.get_bounding_rect(); }
    rect_intersection	get_rect_intersection(const rect_data<real_t> & rect_data) const { return _triangle.get_rect_intersection(rect_data); }
    bool              is_point_inside(const ggo::pos2<real_t> & p) const { return _triangle.is_point_inside(p); }

    // Brush.
    rgba_t brush(int x, int y) const
    {
      rgba_t color(0);
      const ggo::pos2<real_t> p(static_cast<real_t>(x), static_cast<real_t>(y));
      ggo::triangular_interpolation<real_t, rgba_t, false>(_triangle.v1(), _color1, _triangle.v2(), _color2, _triangle.v3(), _color3, p, color);
      return color;
    }

    // Blend.
    color_t blend(int x, int y, const color_t & bkgd_color, const rgba_t & brush_color) const
    {
      const floating_point_color_t rgb_brush_color(brush_color.template get<0>(), brush_color.template get<1>(), brush_color.template get<2>());
      const real_t opacity = brush_color.template get<3>();

      const floating_point_color_t output_color_fp = opacity * rgb_brush_color + (1 - opacity) * convert_color_to<floating_point_color_t>(bkgd_color);

      return ggo::convert_color_to<color_t>(output_color_fp);
    }

    ggo::triangle2d<real_t> _triangle;
    rgba_t                  _color1;
    rgba_t                  _color2;
    rgba_t                  _color3;
  };
}

#endif
