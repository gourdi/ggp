#include <kernel/nonreg/ggo_nonreg.h>
#include <kernel/math/numerical_integration/ggo_numerical_integration.h>

////////////////////////////////////////////////////////////////////
GGO_TEST(numerical_integration, explicit_euler)
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
GGO_TEST(numerical_integration, runge_kutta)
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

