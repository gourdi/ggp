#include "ggo_linear_fog.h"
#include <ggo_interpolation1d.h>

namespace ggo
{
  //////////////////////////////////////////////////////////////
  linear_fog::linear_fog(const ggo::color_32f & color, float far)
  :
  _color(color),
  _far(far)
  {
  }

  //////////////////////////////////////////////////////////////
  ggo::color_32f linear_fog::process_segment(const ggo::pos3f & p1, const ggo::pos3f & p2, const ggo::color_32f & color) const
  {
    float dist = ggo::distance(p1, p2);

    if (dist > _far)
    {
      return _color;
    }
    else
    {
      return ggo::linear_interpolation(0.f, color, _far, _color, dist);
    }
  }

  //////////////////////////////////////////////////////////////
  ggo::color_32f linear_fog::process_background_ray(const ggo::ray3d_float & ray, const ggo::color_32f & color) const
  {
    return _color;
  }
}
