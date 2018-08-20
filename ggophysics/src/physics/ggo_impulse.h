#ifndef __GGO_COLLISIONS__
#define __GGO_COLLISIONS__

#include <kernel/math/shapes_2d/ggo_shapes2d.h>

namespace ggo
{
  struct oriented_box_body;

  float compute_impulse(const ggo::oriented_box_body & body,
                        const ggo::vec2f & pos,
                        const ggo::vec2f & normal);

  float compute_impulse(const ggo::oriented_box_body & body1,
                        const ggo::oriented_box_body & body2,
                        const ggo::vec2f & pos,
                        const ggo::vec2f & normal);
}

#endif
