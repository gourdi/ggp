#include <ggo_nonreg.h>
#include <ggo_nonreg.h>
#include <ggo_intersections.h>
#include <ggo_collisions.h>

//////////////////////////////////////////////////////////////////
GGO_TEST(collision, ortho_dot)
{
  GGO_CHECK_FABS(ggo::ortho_dot({ 1.f, 2.f }, { 3.f, 4.f }), -2.f);
  GGO_CHECK_FABS(ggo::ortho_dot({ 2.f, 1.f }, { 5.f, 4.f }), 3.f);
}

//////////////////////////////////////////////////////////////////
GGO_TEST(collision, static)
{
  // An object falling with a center of mass aligned with the collision point and the normal.
  // The restituion is 0, so the impulse should just cancel the momentum (and therefore the velocity).
  {
    const float restitution = 0.f;
    const ggo::vector2d_float center_of_mass{ 0.f, 1.0f };
    const ggo::vector2d_float normal{ 0.f, 1.f };
    const ggo::vector2d_float velocity{ 0.f, -1.f };
    const float mass = 1.f;
    const float moment_of_intertia = 1.f;

    //float impulse = ggo::compute_static_impulse(restitution, center_of_mass, normal, velocity, mass, moment_of_intertia);
    //GGO_CHECK_FABS(impulse, 1.f);

    //auto new_velocity = ggo::update_linear_velocity(velocity, normal, impulse, mass);
    //GGO_CHECK_FABS(new_velocity.x(), 0.f);
    //GGO_CHECK_FABS(new_velocity.y(), 0.f);
  }

  // An object falling with a center of mass aligned with the collision point and the normal.
  // The restituion is 1, so the impulse should reverse the momentum (and therefore the velocity).
  {
    const float restitution = 1.f;
    const ggo::vector2d_float center_of_mass{ 0.f, 1.0f };
    const ggo::vector2d_float normal{ 0.f, 1.f };
    const ggo::vector2d_float velocity{ 0.f, -1.f };
    const float mass = 1.f;
    const float moment_of_intertia = 1.f;

    //float impulse = ggo::compute_static_impulse(restitution, center_of_mass, normal, velocity, mass, moment_of_intertia);
    //GGO_CHECK_FABS(impulse, 2.f);

    //auto new_velocity = ggo::update_linear_velocity(velocity, normal, impulse, mass);
    //GGO_CHECK_FABS(new_velocity.x(), 0.f);
    //GGO_CHECK_FABS(new_velocity.y(), 1.f);
  }

  // 2x1 rectangle, 45° rotated CCW.
  {
    const float restitution = 1.f;
    const ggo::vector2d_float center_of_mass{ 0.5f / std::sqrt(2.f), 1.5f / std::sqrt(2.f) }; // Not aligned with the collision point and the normal.
    const ggo::vector2d_float normal{ 0.f, 1.0f };
    const ggo::vector2d_float velocity{ 0.f, -1.f };
    const float mass = 1.f;
    const float moment_of_intertia = 1.f;

    //float impulse = ggo::compute_static_impulse(restitution, center_of_mass, normal, velocity, mass, moment_of_intertia);
    //GGO_CHECK_FABS(impulse, 1.777777f);

    //auto new_velocity = ggo::update_linear_velocity(velocity, normal, impulse, mass);
    //GGO_CHECK_FABS(new_velocity.x(), 0.f);
    //GGO_CHECK_FABS(new_velocity.y(), 0.777777f);
  }
}

//////////////////////////////////////////////////////////////////
GGO_TEST(collision, oriented_box_vs_half_plane)
{
  const ggo::half_plane<float> half_plane({ 0.f, 1.f }, 2.0f);
  const ggo::oriented_box<float> box({ 5.f, 6.f }, { 1.f, 0.f }, 2, 1);

  ggo::point2d_float pos{ 0.f, 0.f };
  ggo::vector2d_float normal{ 0.f, 0.f };
  float dt = 1.f;

  GGO_CHECK(ggo::test_collision(half_plane, box, { 0.f, -1.f }, 0.f, pos, normal, dt) == false);

  dt = 4.f;
  GGO_CHECK(ggo::test_collision(half_plane, box, { 0.f, -1.f }, 0.f, pos, normal, dt) == true);
  GGO_CHECK_FABS(pos.x(), 5.0f);
  GGO_CHECK_FABS(pos.y(), 2.0f);
  GGO_CHECK_FABS(normal.x(), 0.0f);
  GGO_CHECK_FABS(normal.y(), 1.0f);
  GGO_CHECK(std::abs(dt - 3.f) < 0.1f); // There is a small imprecision with 'dt'.
}

//////////////////////////////////////////////////////////////////
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

