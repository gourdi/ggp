#ifndef __GGO_COLOR_TRIANGLE__
#define __GGO_COLOR_TRIANGLE__

#include <kernel/math/shapes_2d/ggo_shapes2d.h>
#include <kernel/math/ggo_coordinates_conversions.h>
#include <kernel/math/interpolation/ggo_triangle_interpolation.h>
#include <2d/ggo_color.h>
#include <2d/paint/ggo_blend.h>
#include <2d/paint/ggo_paint_shape_abc.h>

namespace ggo
{
  template <typename color_t, typename brush_color_t, typename data_t = float>
  struct color_triangle_abc : public paint_shape_abc<color_t, data_t>
  {
    using shape_t = ggo::triangle2d<data_t>;

    static_assert(std::is_floating_point_v<data_t> == true);
    static_assert(std::is_floating_point_v<ggo::color_traits<brush_color_t>::sample_t> == true);

    color_triangle_abc(const ggo::triangle2d<data_t> & triangle, 
      const brush_color_t & color1,
      const brush_color_t & color2,
      const brush_color_t & color3) :
      _triangle(triangle),
      _color1(color1),
      _color2(color2),
      _color3(color3)
    { }

    // Geometry.
    rect_data<data_t> get_bounding_rect() const override { return _triangle.get_bounding_rect(); }
    rect_intersection	get_rect_intersection(const rect_data<data_t> & rect_data) const override { return _triangle.get_rect_intersection(rect_data); }
    bool              is_point_inside(const ggo::pos2<data_t> & p) const override { return _triangle.is_point_inside(p); }

    // Brush.
    brush_color_t brush(int x, int y) const
    {
      const ggo::pos2<data_t> p = ggo::from_discrete_to_continuous<data_t>({ x, y });
      auto c = ggo::triangular_interpolation<data_t, brush_color_t, false>(_triangle.v1(), _color1, _triangle.v2(), _color2, _triangle.v3(), _color3, p);
      if (c.has_value() == false)
      {
        throw std::runtime_error("triangular interpolation failed");
      }
      return *c;
    }

    // Blend.
    virtual color_t blend(int x, int y, const color_t & bkgd_color, const brush_color_t & brush_color) const = 0;

    // Paint.
    color_t paint(int x, int y, const color_t & bkgd_color) const override
    {
      return blend(x, y, bkgd_color, brush(x, y));
    }

    ggo::triangle2d<data_t> _triangle;
    brush_color_t           _color1;
    brush_color_t           _color2;
    brush_color_t           _color3;
  };
}

namespace ggo
{
  template <typename color_t, typename blender_t = ggo::overwrite_blender<color_t>, typename data_t = float>
  struct solid_color_triangle : public color_triangle_abc<
    color_t,
    typename floating_point_color_traits<color_t, data_t>::floating_point_color_t,
    data_t>
  {
    using brush_color_t = typename floating_point_color_traits<color_t, data_t>::floating_point_color_t;

    static_assert(ggo::color_traits<brush_color_t>::has_alpha == false);
    static_assert(std::is_floating_point_v<ggo::color_traits<brush_color_t>::sample_t>);

    solid_color_triangle(const ggo::triangle2d<data_t> & triangle,
      const brush_color_t & color1, const brush_color_t & color2, const brush_color_t & color3)
      :
      color_triangle_abc(triangle, color1, color2, color3)
    { }

    // Blend.
    color_t blend(int x, int y, const color_t & bkgd_color, const brush_color_t & brush_color) const override
    { 
      return _blender(x, y, bkgd_color, convert_color_to<color_t>(brush_color));
    }

    blender_t _blender;
  };
}

namespace ggo
{
  template <typename color_t, typename data_t = float>
  struct alpha_color_triangle : public color_triangle_abc<
    color_t,
    typename ggo::color_traits<typename ggo::floating_point_color_traits<color_t, data_t>::floating_point_color_t>::alpha_color_t,
    data_t>
  {
    using brush_color_t = typename ggo::color_traits<typename ggo::floating_point_color_traits<color_t, data_t>::floating_point_color_t>::alpha_color_t;

    static_assert(ggo::color_traits<brush_color_t>::has_alpha == true);
    static_assert(std::is_floating_point_v<ggo::color_traits<brush_color_t>::sample_t>);

    alpha_color_triangle(const ggo::triangle2d<data_t> & triangle,
      const brush_color_t & color1,
      const brush_color_t & color2,
      const brush_color_t & color3) :
      color_triangle_abc(triangle, color1, color2, color3)
    {
    }

    // Blend.
    color_t blend(int x, int y, const color_t & bkgd_color, const brush_color_t & brush_color) const override
    {
      using no_alpha_color_t = ggo::color_traits<brush_color_t>::no_alpha_color_t;

      auto no_alpha_color = ggo::convert_color_to<no_alpha_color_t>(brush_color);
      auto opacity = brush_color.a();

      auto output_color = opacity * no_alpha_color + (1 - opacity) * convert_color_to<no_alpha_color_t>(bkgd_color);

      return ggo::convert_color_to<color_t>(output_color);
    }
  };
}

#endif
