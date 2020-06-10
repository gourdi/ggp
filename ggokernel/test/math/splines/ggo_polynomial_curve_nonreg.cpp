#include <kernel/nonreg/ggo_nonreg.h>
#include <kernel/ggo_vec2.h>
#include <kernel/math/splines/ggo_polymial_curve.h>

////////////////////////////////////////////////////////////////////
GGO_TEST(polynomial_curve, linear1)
{
  const std::vector<ggo::pos2_f> points({ {0.f, 1.f }, {1.f, 1.f} });
  const std::vector<float> knots({ 0.f, 1.f });

  ggo::pos2_f p0 = ggo::polynomial_curve(points, knots, 0.0f);
  GGO_CHECK_FLOAT_EQ(p0.x(), 0.f);
  GGO_CHECK_FLOAT_EQ(p0.y(), 1.f);

  ggo::pos2_f p1 = ggo::polynomial_curve(points, knots, 0.5f);
  GGO_CHECK_FLOAT_EQ(p1.x(), 0.5f);
  GGO_CHECK_FLOAT_EQ(p1.y(), 1.f);
  
  ggo::pos2_f p2 = ggo::polynomial_curve(points, knots, 1.0f);
  GGO_CHECK_FLOAT_EQ(p2.x(), 1.f);
  GGO_CHECK_FLOAT_EQ(p2.y(), 1.f);
}

////////////////////////////////////////////////////////////////////
GGO_TEST(polynomial_curve, linear2)
{
  const std::vector<ggo::pos2_f> points({ {0.f, 1.f }, {1.f, 1.f} });
  const std::vector<float> knots({ 3.f, 5.f });

  ggo::pos2_f p0 = ggo::polynomial_curve(points, knots, 3.0f);
  GGO_CHECK_FLOAT_EQ(p0.x(), 0.f);
  GGO_CHECK_FLOAT_EQ(p0.y(), 1.f);

  ggo::pos2_f p1 = ggo::polynomial_curve(points, knots, 4.0f);
  GGO_CHECK_FLOAT_EQ(p1.x(), 0.5f);
  GGO_CHECK_FLOAT_EQ(p1.y(), 1.f);

  ggo::pos2_f p2 = ggo::polynomial_curve(points, knots, 5.0f);
  GGO_CHECK_FLOAT_EQ(p2.x(), 1.f);
  GGO_CHECK_FLOAT_EQ(p2.y(), 1.f);
}

////////////////////////////////////////////////////////////////////
GGO_TEST(polynomial_curve, quadratic1)
{
  const std::vector<ggo::pos2_f> points({ {0.f, 1.f }, {1.f, 1.f}, {1.f, 0.f} });
  const std::vector<float> knots({ 0.f, 1.f, 2.f });

  // The curve must go through the control points.
  ggo::pos2_f p0 = ggo::polynomial_curve(points, knots, 0.0f);
  GGO_CHECK_FLOAT_EQ(p0.x(), 0.f);
  GGO_CHECK_FLOAT_EQ(p0.y(), 1.f);

  ggo::pos2_f p1 = ggo::polynomial_curve(points, knots, 1.0f);
  GGO_CHECK_FLOAT_EQ(p1.x(), 1.f);
  GGO_CHECK_FLOAT_EQ(p1.y(), 1.f);

  ggo::pos2_f p2 = ggo::polynomial_curve(points, knots, 2.0f);
  GGO_CHECK_FLOAT_EQ(p2.x(), 1.f);
  GGO_CHECK_FLOAT_EQ(p2.y(), 0.f);

  // Check other points.
  ggo::pos2_f p3 = ggo::polynomial_curve(points, knots, 0.5f);
  GGO_CHECK_FLOAT_EQ(p3.x(), 0.625f);
  GGO_CHECK_FLOAT_EQ(p3.y(), 1.125f);

  ggo::pos2_f p4 = ggo::polynomial_curve(points, knots, 1.5f);
  GGO_CHECK_FLOAT_EQ(p4.x(), 1.125f);
  GGO_CHECK_FLOAT_EQ(p4.y(), 0.625f);
}

////////////////////////////////////////////////////////////////////
GGO_TEST(polynomial_curve, quadratic2)
{
  const std::vector<ggo::pos2_f> points({ {0.f, 1.f }, {1.f, 1.f}, {1.f, 0.f} });
  const std::vector<float> knots({ 0.f, 1.f, 5.f });

  // The curve must go through the control points.
  ggo::pos2_f p0 = ggo::polynomial_curve(points, knots, 0.0f);
  GGO_CHECK_FLOAT_EQ(p0.x(), 0.f);
  GGO_CHECK_FLOAT_EQ(p0.y(), 1.f);

  ggo::pos2_f p1 = ggo::polynomial_curve(points, knots, 1.0f);
  GGO_CHECK_FLOAT_EQ(p1.x(), 1.f);
  GGO_CHECK_FLOAT_EQ(p1.y(), 1.f);

  ggo::pos2_f p2 = ggo::polynomial_curve(points, knots, 5.0f);
  GGO_CHECK_FLOAT_EQ(p2.x(), 1.f);
  GGO_CHECK_FLOAT_EQ(p2.y(), 0.f);

  // Check other points.
  ggo::pos2_f p3 = ggo::polynomial_curve(points, knots, 0.5f);
  GGO_CHECK_FLOAT_EQ(p3.x(), 0.55f);
  GGO_CHECK_FLOAT_EQ(p3.y(), 1.0125f);

  ggo::pos2_f p4 = ggo::polynomial_curve(points, knots, 3.0f);
  GGO_CHECK_FLOAT_EQ(p4.x(), 1.8f);
  GGO_CHECK_FLOAT_EQ(p4.y(), 0.7f);
}


