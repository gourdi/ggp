#ifndef __GGO_COLLISIONS__
#define __GGO_COLLISIONS__

#include <ggo_shapes2d.h>

namespace ggo
{
  struct oriented_box_body;

  inline float ortho_dot(const ggo::vector2d_float & v1, const ggo::vector2d_float & v2) { return v1.y() * v2.x() - v1.x() * v2.y(); }

  float compute_static_impulse(const ggo::oriented_box_body & body,
                               const ggo::vector2d_float & pos,
                               const ggo::vector2d_float & normal);
}

#endif
