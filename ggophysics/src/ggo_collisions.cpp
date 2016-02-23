#include <ggo_collisions.h>

namespace ggo
{
  //////////////////////////////////////////////////////////////////
  // The normal must be the one of the first object. 
  void resolve_collision(float restitution, ggo::vector2d_float & velocity1, float mass1, ggo::vector2d_float & velovity2, float mass2, const ggo::vector2d_float & normal)
  {
    float proj_velocity1 = ggo::dot(normal, velocity1);
    float proj_velocity2 = ggo::dot(normal, velovity2);

    float proj_next_velocity1 = (restitution * mass2 * (proj_velocity2 - proj_velocity1) + mass1 * proj_velocity1 + mass2 * proj_velocity2) / (mass1 + mass2);
    float proj_next_velocity2 = (restitution * mass1 * (proj_velocity1 - proj_velocity2) + mass1 * proj_velocity1 + mass2 * proj_velocity2) / (mass1 + mass2);

    velocity1 += (proj_next_velocity1 - proj_velocity1) * normal;
    velovity2 += (proj_next_velocity2 - proj_velocity2) * normal;
  }
}
