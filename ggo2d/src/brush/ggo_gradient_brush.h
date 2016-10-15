#ifndef __GGO_RGB_GRADIENT_BRUSH__
#define __GGO_RGB_GRADIENT_BRUSH__

#include <ggo_brush_abc.h>
#include <ggo_ease.h>

//////////////////////////////////////////////////////////////
namespace ggo
{
  template <typename color_t, typename real_t, bool global_coordinates = false>
  struct gradient_brush : public brush_abc<color_t, real_t>
  {
    color_t get(real_t x, real_t y, const ggo::paintable_shape2d_abc<real_t> & shape, int width, int height) const override;
      
    color_t     _value1;
    color_t     _value2;
    ggo::pos2f  _pos1;
    ggo::pos2f  _pos2;
  };
}

//////////////////////////////////////////////////////////////
namespace ggo
{
  template <typename color_t, typename real_t, bool global_coordinates>
  color_t gradient_brush<color_t, real_t, global_coordinates>::get(real_t x, real_t y, const ggo::paintable_shape2d_abc<real_t> & shape, int width, int height) const
  {
    ggo::rect<real_t> bounding_rect(shape.get_bounding_rect());
    
    real_t x_f = global_coordinates ? x : x - bounding_rect.left();
    real_t y_f = global_coordinates ? y : y - bounding_rect.bottom();

    ggo::vec2<real_t> diff1(x_f - _pos1.get<0>(), y_f - _pos1.get<1>());
    ggo::vec2<real_t> diff2(_pos2 - _pos1);
      
    real_t dot_prod = ggo::dot(diff1, diff2);
      
    if (dot_prod <= 0)
    {
      return _value1;
    }
    else
    {
      real_t hypot = diff2.get_hypot();
      if (dot_prod >= hypot)
      {
        return _value2;
      }
      else
      {
        real_t v2 = ggo::ease_inout(dot_prod / hypot);
        real_t v1 = 1 - v2;
        return v1 * _value1 + v2 * _value2;
      }
    }
  }
}

#endif

