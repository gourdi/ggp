#include "ggo_linear_fog.h"

namespace ggo
{
  //////////////////////////////////////////////////////////////
  linear_fog::linear_fog(const ggo::color & color, float far)
  :
  _color(color),
  _far(far)
  {
  }

  //////////////////////////////////////////////////////////////
  ggo::color linear_fog::process_segment(const ggo::point3d_float & p1, const ggo::point3d_float & p2, const ggo::color & color) const
  {
    float dist = ggo::distance(p1, p2);

    if (dist > _far)
    {
      return _color;
    }
    else
    {
      return ggo::color::linear_interpolate(0, color, _far, _color, dist);
    }
  }

  //////////////////////////////////////////////////////////////
  ggo::color linear_fog::process_background_ray(const ggo::ray3d_float & ray, const ggo::color & color) const
  {
    return _color;
  }
}
