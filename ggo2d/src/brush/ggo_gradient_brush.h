#ifndef __GGO_RGB_GRADIENT_BRUSH__
#define __GGO_RGB_GRADIENT_BRUSH__

#include <ggo_brush_abc.h>
#include <ggo_ease.h>

//////////////////////////////////////////////////////////////
namespace ggo
{
  template <typename color_type, bool global_coordinates = false>
  struct gradient_brush : public brush_abc<color_type>
  {
    color_type get(float x, float y, const ggo::paintable_shape2d_abc<float> & shape, int width, int height) const override;
      
    color_type          _value1;
    color_type          _value2;
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
  template <typename color_type, bool global_coordinates>
  color_type gradient_brush<color_type, global_coordinates>::get(float x, float y, const ggo::paintable_shape2d_abc<float> & shape, int width, int height) const
  {
    ggo::rect_float bounding_rect(shape.get_bounding_rect());
    
    float x_f = global_coordinates ? x : x - bounding_rect.left();
    float y_f = global_coordinates ? y : y - bounding_rect.bottom();

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

