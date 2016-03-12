#ifndef __GGO_INTERSECTION__
#define __GGO_INTERSECTION__

#include <ggo_shapes2d.h>

namespace ggo
{
  // The output normal is the one of the first shape parameter.

  //////////////////////////////////////////////////////////////////
  // Oriented box.
  bool test_collision(const ggo::half_plane<float> & half_plane, const ggo::oriented_box<float> & box,
                      ggo::point2d_float & pos, ggo::vector2d_float & normal, ggo::vector2d_float & correction);
}

#endif
