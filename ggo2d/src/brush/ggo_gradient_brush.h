#ifndef __GGO_RGB_GRADIENT_BRUSH__
#define __GGO_RGB_GRADIENT_BRUSH__

#include <ggo_brush_abc.h>
#include <ggo_ease.h>

//////////////////////////////////////////////////////////////
namespace ggo
{
  template <typename T>
  struct gradient_brush : public brush_abc<T>
  {
    T get(float x, float y, const ggo::paintable_shape2d_abc<float> & shape, int width, int height) const override;
      
    T                   _value1;
    T                   _value2;
    ggo::point2d_float  _pos1;
    ggo::point2d_float  _pos2;
  };
}

//////////////////////////////////////////////////////////////
namespace ggo
{
  using rgb_gradient_brush = gradient_brush<ggo::color>;
  using gray_gradient_brush = gradient_brush<float>;
  using opacity_gradient_brush = gradient_brush<float>;
}

//////////////////////////////////////////////////////////////
namespace ggo
{
  template <typename T>
  T gradient_brush<T>::get(float x, float y, const ggo::paintable_shape2d_abc<float> & shape, int width, int height) const
  {
    ggo::rect_float bounding_rect(shape.get_bounding_rect());
    
    float x_f = x - bounding_rect.left();
    float y_f = y - bounding_rect.bottom();

    ggo::vector2d_float diff1(x_f - _pos1.x(), y_f - _pos1.y());
    ggo::vector2d_float diff2(_pos2 - _pos1);
      
    float dot_prod = ggo::dot(diff1, diff2);
      
    if (dot_prod <= 0)
    {
      return _value1;
    }
    else
    {
      float hypot = diff2.get_hypot();
      if (dot_prod >= hypot)
      {
        return _value2;
      }
      else
      {
        float v2 = ggo::ease_inout(dot_prod / hypot);
        float v1 = 1 - v2;
        return v1 * _value1 + v2 * _value2;
      }
    }
  }
}

#endif

