#include <kernel/nonreg/ggo_nonreg.h>
#include <kernel/ggo_vec2.h>
#include <kernel/math/splines/ggo_polymial_curve.h>

////////////////////////////////////////////////////////////////////
GGO_TEST(polynomial_curve, linear1)
{
  const std::vector<ggo::pos2_f> points({ {0.f, 1.f }, {1.f, 1.f} });
  const std::vector<float> knots({ 0.f, 1.f });

  GGO_CHECK_VEC2F_EQ(ggo::polynomial_curve(points, knots, 0.0f), ggo::pos2_f(0.0f, 1.0f));
  GGO_CHECK_VEC2F_EQ(ggo::polynomial_curve(points, knots, 0.5f), ggo::pos2_f(0.5f, 1.0f));
  GGO_CHECK_VEC2F_EQ(ggo::polynomial_curve(points, knots, 1.0f), ggo::pos2_f(1.0f, 1.0f));
}

////////////////////////////////////////////////////////////////////
GGO_TEST(polynomial_curve, linear2)
{
  const std::vector<ggo::pos2_f> points({ {0.f, 1.f }, {1.f, 1.f} });
  const std::vector<float> knots({ 3.f, 5.f });

  GGO_CHECK_VEC2F_EQ(ggo::polynomial_curve(points, knots, 3.0f), ggo::pos2_f(0.f, 1.f));
  GGO_CHECK_VEC2F_EQ(ggo::polynomial_curve(points, knots, 4.0f), ggo::pos2_f(0.5f, 1.f));
  GGO_CHECK_VEC2F_EQ(ggo::polynomial_curve(points, knots, 5.0f), ggo::pos2_f(1.f, 1.f));
}

////////////////////////////////////////////////////////////////////
GGO_TEST(polynomial_curve, quadratic1)
{
  const std::vector<ggo::pos2_f> points({ {0.f, 1.f }, {1.f, 1.f}, {1.f, 0.f} });
  const std::vector<float> knots({ 0.f, 1.f, 2.f });

  // The curve must go through the control points.
  GGO_CHECK_VEC2F_EQ(ggo::polynomial_curve(points, knots, 0.0f), ggo::pos2_f(0.f, 1.f));
  GGO_CHECK_VEC2F_EQ(ggo::polynomial_curve(points, knots, 1.0f), ggo::pos2_f(1.f, 1.f));
  GGO_CHECK_VEC2F_EQ(ggo::polynomial_curve(points, knots, 2.0f), ggo::pos2_f(1.f, 0.f));

  // Check other points.
  GGO_CHECK_VEC2F_EQ(ggo::polynomial_curve(points, knots, 0.5f), ggo::pos2_f(0.625f, 1.125f));
  GGO_CHECK_VEC2F_EQ(ggo::polynomial_curve(points, knots, 1.5f), ggo::pos2_f(1.125f, 0.625f));
}

////////////////////////////////////////////////////////////////////
GGO_TEST(polynomial_curve, quadratic2)
{
  const std::vector<ggo::pos2_f> points({ {0.f, 1.f }, {1.f, 1.f}, {1.f, 0.f} });
  const std::vector<float> knots({ 0.f, 1.f, 5.f });

  // The curve must go through the control points.
  GGO_CHECK_VEC2F_EQ(ggo::polynomial_curve(points, knots, 0.0f), ggo::pos2_f(0.f, 1.f));
  GGO_CHECK_VEC2F_EQ(ggo::polynomial_curve(points, knots, 1.0f), ggo::pos2_f(1.f, 1.f));
  GGO_CHECK_VEC2F_EQ(ggo::polynomial_curve(points, knots, 5.0f), ggo::pos2_f(1.f, 0.f));

  // Check other points.
  GGO_CHECK_VEC2F_EQ(ggo::polynomial_curve(points, knots, 0.5f), ggo::pos2_f(0.55f, 1.0125f));
  GGO_CHECK_VEC2F_EQ(ggo::polynomial_curve(points, knots, 3.0f), ggo::pos2_f(1.8f, 0.7f));
}


