#ifndef __GGO_COLLISIONS__
#define __GGO_COLLISIONS__

#include <ggo_shapes2d.h>

namespace ggo
{
  struct oriented_box_body;

  float compute_impulse(const ggo::oriented_box_body & body,
                        const ggo::vector2d_float & pos,
                        const ggo::vector2d_float & normal);

  float compute_impulse(const ggo::oriented_box_body & body1,
                        const ggo::oriented_box_body & body2,
                        const ggo::vector2d_float & pos,
                        const ggo::vector2d_float & normal);
}

#endif
