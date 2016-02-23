#include <ggo_nonreg.h>
#include <ggo_nonreg.h>
#include <ggo_collisions.h>

GGO_TEST(collision_resolution, opposite_velocities)
{
  // To object of mass 1 and moving with a velocity of 1. They move after collision in the opposition direction 
  // with a lower velocity because of the resitution factor.
  ggo::vector2d_float v1{  1.f, 0.f };
  ggo::vector2d_float v2{ -1.f, 0.f };
  ggo::resolve_collision(0.8f, v1, 1, v2, 1, { 1.f, 0.f });
  GGO_CHECK_FABS(v1.x(), -0.8f);
  GGO_CHECK_FABS(v1.y(),  0.0f);
  GGO_CHECK_FABS(v2.x(),  0.8f);
  GGO_CHECK_FABS(v2.y(),  0.0f);
}

GGO_TEST(collision_resolution, motionless)
{
  // A motionless object is hit by an object from the right.
  ggo::vector2d_float v1{  0.f, 0.f };
  ggo::vector2d_float v2{ -1.f, 0.f };
  ggo::resolve_collision(0.8f, v1, 1, v2, 1, { 1.f, 0.f });
  GGO_CHECK_FABS(v1.x(), -0.9f);
  GGO_CHECK_FABS(v1.y(),  0.0f);
  GGO_CHECK_FABS(v2.x(), -0.1f);
  GGO_CHECK_FABS(v2.y(),  0.0f);
}
