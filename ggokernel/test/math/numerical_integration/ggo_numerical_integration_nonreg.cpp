#include <kernel/nonreg/ggo_nonreg.h>
#include <kernel/ggo_vec.h>
#include <kernel/math/numerical_integration/ggo_numerical_integration.h>

////////////////////////////////////////////////////////////////////
GGO_TEST(numerical_integration, explicit_euler_1d)
{
  auto velocity = [](float x) { return -x; };

  float x0 = 2.f;
  float x1 = ggo::explicit_euler(x0, 0.5f, velocity);
  float x2 = ggo::explicit_euler(x1, 0.5f, velocity);
  float x3 = ggo::explicit_euler(x2, 0.5f, velocity);
  GGO_CHECK_FLOAT_EQ(x1, 1.f);
  GGO_CHECK_FLOAT_EQ(x2, 0.5f);
  GGO_CHECK_FLOAT_EQ(x3, 0.25f);
}

////////////////////////////////////////////////////////////////////
GGO_TEST(numerical_integration, runge_kutta_1d)
{
  auto velocity = [](float x) { return -x; };

  float x0 = 2.f;
  float x1 = ggo::runge_kutta(x0, 0.5f, velocity);
  float x2 = ggo::runge_kutta(x1, 0.5f, velocity);
  float x3 = ggo::runge_kutta(x2, 0.5f, velocity);
  GGO_CHECK_FLOAT_EQ(x1, 1.21354175f);
  GGO_CHECK_FLOAT_EQ(x2, 0.736341715f);
  GGO_CHECK_FLOAT_EQ(x3, 0.446790665f);
}

////////////////////////////////////////////////////////////////////
GGO_TEST(numerical_integration, explicit_euler_freefall)
{
  // Here v.x == vertical position, v.y == speed
  // Therefore, dv/dt = (velocity, accleration) == (v.y, F/m). 
  auto velocity = [](ggo::vec2f v) { return ggo::vec2f(v.y(), -1.f); };

  ggo::vec2f v0 = { 0.f, 1.f }; // Starting at 0 altitude, with a upward speed of 1.
  ggo::vec2f v1 = ggo::explicit_euler(v0, 0.5f, velocity);
  ggo::vec2f v2 = ggo::explicit_euler(v1, 0.5f, velocity);
  ggo::vec2f v3 = ggo::explicit_euler(v2, 0.5f, velocity);
  ggo::vec2f v4 = ggo::explicit_euler(v3, 0.5f, velocity);

  // The particule goes up because of initial speed, then falls.
  GGO_CHECK_FLOAT_EQ(v1.x(), 0.5f);
  GGO_CHECK_FLOAT_EQ(v1.y(), 0.5f);
  GGO_CHECK_FLOAT_EQ(v2.x(), 0.75f);
  GGO_CHECK_FLOAT_EQ(v2.y(), 0.f);
  GGO_CHECK_FLOAT_EQ(v3.x(), 0.75f);
  GGO_CHECK_FLOAT_EQ(v3.y(), -0.5f);
  GGO_CHECK_FLOAT_EQ(v4.x(), 0.5f);
  GGO_CHECK_FLOAT_EQ(v4.y(), -1.f);
}

////////////////////////////////////////////////////////////////////
GGO_TEST(numerical_integration, runge_kutta_freefall)
{
  // Here v.x == vertical position, v.y == speed
  // Therefore, dv/dt = (velocity, accleration) == (v.y, F/m). 
  auto velocity = [](ggo::vec2f v) { return ggo::vec2f(v.y(), -1.f); };

  ggo::vec2f v0 = { 0.f, 1.f }; // Starting at 0 altitude, with a upward speed of 1.
  ggo::vec2f v1 = ggo::runge_kutta(v0, 0.5f, velocity);
  ggo::vec2f v2 = ggo::runge_kutta(v1, 0.5f, velocity);
  ggo::vec2f v3 = ggo::runge_kutta(v2, 0.5f, velocity);
  ggo::vec2f v4 = ggo::runge_kutta(v3, 0.5f, velocity);

  // The particule goes up because of initial speed, then falls.
  GGO_CHECK_FLOAT_EQ(v1.x(), 0.375f);
  GGO_CHECK_FLOAT_EQ(v1.y(), 0.5f);
  GGO_CHECK_FLOAT_EQ(v2.x(), 0.5f);
  GGO_CHECK_FLOAT_EQ(v2.y(), 0.f);
  GGO_CHECK_FLOAT_EQ(v3.x(), 0.375f);
  GGO_CHECK_FLOAT_EQ(v3.y(), -0.5f);
  GGO_CHECK_FLOAT_EQ(v4.x(), 0.f);
  GGO_CHECK_FLOAT_EQ(v4.y(), -1.f);
}


