#ifndef __GGO_TRIANGLE_INTERPOLATION_BRUSH__
#define __GGO_TRIANGLE_INTERPOLATION_BRUSH__

#include <ggo_triangle_interpolation.h>
#include <ggo_brush_abc.h>

namespace ggo
{
  template <typename T>
  struct triangle_interpolation_brush : public brush_abc<T>
  {
    triangle_interpolation_brush(const ggo::point2d_float & p1, const T & value1,
                                 const ggo::point2d_float & p2, const T & value2,
                                 const ggo::point2d_float & p3, const T & value3)
                                 :
                                 _p1(p1), _value1(value1),
                                 _p2(p2), _value2(value2),
                                 _p3(p3), _value3(value3) {}
  
    T get(float x, float y, const ggo::paintable_shape2d_abc<float> & shape, int width, int height) const override
    {
      T value(0);
      ggo::triangular_interpolation(_p1, _value1, _p2, _value2, _p3, _value3, ggo::point2d_float(x, y), value);
      return value;
    }
      
    ggo::point2d_float _p1;
    ggo::point2d_float _p2; 
    ggo::point2d_float _p3;
    T _value1;
    T _value2;
    T _value3;
  };
  
  using rgb_triangle_interpolation_brush = triangle_interpolation_brush<ggo::color>;
  using gray_triangle_interpolation_brush = triangle_interpolation_brush<float>;
  using opacity_triangle_interpolation_brush = triangle_interpolation_brush<float>;
}

#endif
