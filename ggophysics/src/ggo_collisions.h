#ifndef __GGO_INTERSECTION__
#define __GGO_INTERSECTION__

#include <ggo_shapes2d.h>

namespace ggo
{
  // The output normal is the one of the first shape parameter.

  //////////////////////////////////////////////////////////////////
  // Oriented box.
  bool test_collision(const ggo::half_plane<float> & half_plane,
                      const ggo::oriented_box<float> & box, const ggo::vector2d_float & linear_velocity, const float angular_velocity,
                      ggo::point2d_float & pos, ggo::vector2d_float & normal, float & dt);
}

#endif
