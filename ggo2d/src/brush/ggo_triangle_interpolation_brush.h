#ifndef __GGO_TRIANGLE_INTERPOLATION_BRUSH__
#define __GGO_TRIANGLE_INTERPOLATION_BRUSH__

#include <ggo_triangle_interpolation.h>
#include <ggo_brush_abc.h>

namespace ggo
{
  template <typename color_t, typename real_t>
  struct triangle_interpolation_brush : public brush_abc<color_t, real_t>
  {
    triangle_interpolation_brush(
      const ggo::pos2f & p1, const color_t & value1,
      const ggo::pos2f & p2, const color_t & value2,
      const ggo::pos2f & p3, const color_t & value3)
      :
      _p1(p1), _value1(value1),
      _p2(p2), _value2(value2),
      _p3(p3), _value3(value3) {}

    color_t get(real_t x, real_t y, const ggo::paintable_shape2d_abc<real_t> & shape, int width, int height) const override
    {
      color_t value(0);
      ggo::triangular_interpolation(_p1, _value1, _p2, _value2, _p3, _value3, ggo::pos2<real_>(x, y), value);
      return value;
    }

    ggo::pos2<real_t> _p1;
    ggo::pos2<real_t> _p2;
    ggo::pos2<real_t> _p3;
    color_t           _value1;
    color_t           _value2;
    color_t           _value3;
  };
}

#endif
