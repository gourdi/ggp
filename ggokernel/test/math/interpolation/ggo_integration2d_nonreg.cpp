#include <kernel/nonreg/ggo_nonreg.h>
#include <kernel/math/interpolation/ggo_integration2d.h>

/////////////////////////////////////////////////////////////////////
GGO_TEST(integration2d, make_surface)
{
  auto s = ggo::integration2d_surface<float>::make<float>(2, 3, 6, 7, 1, 4, 3, 2);

  GGO_CHECK_FLOAT_EQ(s.eval(2.f, 6.f), 1.f);
  GGO_CHECK_FLOAT_EQ(s.eval(3.f, 6.f), 4.f);
  GGO_CHECK_FLOAT_EQ(s.eval(2.f, 7.f), 3.f);
  GGO_CHECK_FLOAT_EQ(s.eval(3.f, 7.f), 2.f);
  GGO_CHECK_FLOAT_EQ(s.eval(2.5f, 6.f), 2.5f);
  GGO_CHECK_FLOAT_EQ(s.eval(2.5f, 7.f), 2.5f);
  GGO_CHECK_FLOAT_EQ(s.eval(2.f, 6.5f), 2.f);
  GGO_CHECK_FLOAT_EQ(s.eval(3.f, 6.5f), 3.f);
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(integration2d, integrate_constant)
{
  // f(x, y) = 1
  ggo::integration2d_surface<float> s{ 1.f, 0.f, 0.f, 0.f };

  GGO_CHECK_FLOAT_EQ(s.integrate(0.f, 1.f, 0.f, 1.f), 1.f);
  GGO_CHECK_FLOAT_EQ(s.integrate(2.f, 3.f, 6.f, 7.f), 1.f);
  GGO_CHECK_FLOAT_EQ(s.integrate(2.f, 4.f, 6.f, 8.f), 4.f);
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(integration2d, integrate_constant_slope_x)
{
  // f(x, y) = 2 + x
  ggo::integration2d_surface<float> s{ 2.f, 1.f, 0.f, 0.f };

  GGO_CHECK_FLOAT_EQ(s.integrate(0.f, 1.f, 0.f, 1.f), 2.5f);
  GGO_CHECK_FLOAT_EQ(s.integrate(0.f, 1.f, 5.f, 6.f), 2.5f);
  GGO_CHECK_FLOAT_EQ(s.integrate(1.f, 2.f, 5.f, 6.f), 3.5f);
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(integration2d, integrate_constant_slope_y)
{
  // f(x, y) = 2 + y
  ggo::integration2d_surface<float> s{ 2.f, 0.f, 1.f, 0.f };

  GGO_CHECK_FLOAT_EQ(s.integrate(0.f, 1.f, 0.f, 1.f), 2.5f);
  GGO_CHECK_FLOAT_EQ(s.integrate(5.f, 6.f, 0.f, 1.f), 2.5f);
  GGO_CHECK_FLOAT_EQ(s.integrate(5.f, 6.f, 1.f, 2.f), 3.5f);
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(integration2d, integrate_generic)
{
  // f(x, y) = 2 + x - 3 * y + x * y
  ggo::integration2d_surface<float> s{ 2.f, 1.f, -3.f, 1.f };

  GGO_CHECK_FLOAT_EQ(s.integrate(0.f, 1.f, 0.f, 1.f), 1.25f);
  GGO_CHECK_FLOAT_EQ(s.integrate(1.f, 2.f, 5.f, 7.f), -11.f);
  GGO_CHECK_FLOAT_EQ(s.integrate(-1.f, 2.f, -2.f, 5.f), -26.25f);
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(integration2d, linear_integration2d_constant)
{
  const float input[] = { 1.f, 1.f, 1.f, 1.f, 1.f, 1.f };

  GGO_CHECK_FLOAT_EQ(ggo::linear_integration2d(input, 3, 2, 0.f, 1.f, 0.f, 1.f), 1.f);
  GGO_CHECK_FLOAT_EQ(ggo::linear_integration2d(input, 3, 2, 0.25f, 0.75f, 0.25f, 0.75f), 0.25f);
  GGO_CHECK_FLOAT_EQ(ggo::linear_integration2d(input, 3, 2, 0.5f, 1.5f, 0.f, 1.f), 1.f);
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(integration2d, linear_integration2d_non_constant)
{
  const float input[] = {
    1.f, 1.f, 1.f, 2.f,
    1.f, 1.f, 1.f, 2.f,
    2.f, 2.f, 2.f, 2.f };

  GGO_CHECK_FLOAT_EQ(ggo::linear_integration2d(input, 4, 3, 0.f, 1.f, 1.f, 2.f), 1.5f);
  GGO_CHECK_FLOAT_EQ(ggo::linear_integration2d(input, 4, 3, 0.f, 1.f, 0.5f, 2.f), 2.f);
  GGO_CHECK_FLOAT_EQ(ggo::linear_integration2d(input, 4, 3, 2.f, 3.f, 1.f, 2.f), 1.75f);
}





