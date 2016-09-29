#ifndef __GGO_INTERSECTION__
#define __GGO_INTERSECTION__

#include <ggo_shapes2d.h>

namespace ggo
{
  // The output normal is the one of the first shape parameter.

  bool test_collision(const ggo::half_plane<float> & half_plane, const ggo::oriented_box<float> & box,
                      ggo::pos2f & pos, ggo::vec2f & normal);

  bool test_collision(const ggo::oriented_box<float> & box1, const ggo::oriented_box<float> & box2,
                      ggo::pos2f & pos, ggo::vec2f & normal);
}

#endif
