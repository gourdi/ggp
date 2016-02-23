#include <ggo_intersections.h>

namespace ggo
{
  //////////////////////////////////////////////////////////////////
  bool test_intersection(const ggo::disc<float> & disc1, const ggo::disc<float> & disc2, ggo::vector2d_float & normal, float & penetration)
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

    penetration = disc1.radius() + disc2.radius() - std::sqrt(hypot);

    return true;
  }

  //////////////////////////////////////////////////////////////////
  bool test_intersection(const ggo::half_plane<float> & half_plane, const ggo::disc<float> & disc, ggo::vector2d_float & normal, float & penetration)
  {
    float dist = half_plane.dist_to_point(disc.center());

    if (dist > disc.radius())
    {
      return false;
    }

    normal = half_plane.normal();

    penetration = disc.radius() - dist;

    return true;
  }
}
