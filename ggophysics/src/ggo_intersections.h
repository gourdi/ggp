#ifndef __GGO_INTERSECTION__
#define __GGO_INTERSECTION__

#include <ggo_shapes2d.h>

namespace ggo
{
  // The output normal is the one of the first shape parameter.

  //////////////////////////////////////////////////////////////////
  // Oriented box.
  bool test_intersection(const ggo::half_plane<float> & half_plane, const ggo::oriented_box<float> & box, ggo::vector2d_float & normal, float & penetration);

  //////////////////////////////////////////////////////////////////
  // Disc.
  bool test_intersection(const ggo::disc<float> & disc1, const ggo::disc<float> & disc2, ggo::vector2d_float & normal, float & penetration);
  bool test_intersection(const ggo::half_plane<float> & half_plane, const ggo::disc<float> & disc, ggo::vector2d_float & normal, float & penetration);
}

#endif
