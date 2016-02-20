#include <ggo_intersections.h>

namespace ggo
{
  //////////////////////////////////////////////////////////////////
  bool test_intersection(const ggo::disc<float> & disc1, const ggo::disc<float> & disc2, ggo::vector2d_float & normal)
  {
    float squared_radius1 = ggo::square(disc1.radius());
    float squared_radius2 = ggo::square(disc2.radius());
    float hypot = ggo::hypot(disc1.center(), disc2.center());

    if (hypot > squared_radius1 + squared_radius2)
    {
      return false;
    }

    normal = disc2.center() - disc1.center();
    normal.normalize();

    return true;
  }

  //////////////////////////////////////////////////////////////////
  bool test_intersection(const ggo::half_plane<float> & half_plane, const ggo::disc<float> & disc, ggo::vector2d_float & normal)
  {
    float dist = half_plane.dist_to_point(disc.center());

    if (dist > disc.radius())
    {
      return false;
    }

    normal = half_plane.normal();

    return true;
  }
}