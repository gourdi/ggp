#include "ggo_linear_fog.h"
#include <kernel/math/interpolation/ggo_interpolation1d.h>

namespace ggo
{
  //////////////////////////////////////////////////////////////
  linear_fog::linear_fog(const ggo::rgb_32f & color, float near, float far)
  :
  _color(color),
  _near(near),
  _far(far)
  {
    GGO_ASSERT_LT(_near, _far);
  }

  //////////////////////////////////////////////////////////////
  ggo::rgb_32f linear_fog::process_segment(const ggo::pos3f & p1, const ggo::pos3f & p2, const ggo::rgb_32f & color) const
  {
    float dist = ggo::distance(p1, p2);

    if (dist > _far)
    {
      return _color;
    }
    else if (dist < _near)
    {
      return color;
    }
    else
    {
      return ggo::linear_interpolation(_near, color, _far, _color, dist);
    }
  }

  //////////////////////////////////////////////////////////////
  ggo::rgb_32f linear_fog::process_background_ray(const ggo::ray3d_float & ray, const ggo::rgb_32f & color) const
  {
    return _color;
  }
}
