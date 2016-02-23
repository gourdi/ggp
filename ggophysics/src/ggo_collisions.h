#ifndef __GGO_COLLISIONS__
#define __GGO_COLLISIONS__

#include <ggo_shapes2d.h>

namespace ggo
{
  // The normal must be the one of the first object. 
  void resolve_collision(float restitution, ggo::vector2d_float & velocity1, float mass1, ggo::vector2d_float & velovity2, float mass2, const ggo::vector2d_float & normal);
}

#endif
