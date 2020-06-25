#include <kernel/nonreg/ggo_nonreg.h>
#include <kernel/ggo_vec2.h>
#include <kernel/math/splines/ggo_bspline.h>

////////////////////////////////////////////////////////////////////
GGO_TEST(bspline, uniform_bspline_linear)
{
  const std::vector<ggo::pos2_f> points({ { 0.f, 1.f }, { 1.f, 1.f }, { 1.f, 0.f }, { 2.f, 0.f } });

  GGO_CHECK_VEC2F_EQ(ggo::uniform_bspline_linear(points, 0.f),       ggo::pos2_f(0.0f, 1.0f));
  GGO_CHECK_VEC2F_EQ(ggo::uniform_bspline_linear(points, 1.f / 6.f), ggo::pos2_f(0.5f, 1.0f));
  GGO_CHECK_VEC2F_EQ(ggo::uniform_bspline_linear(points, 1.f / 3.f), ggo::pos2_f(1.0f, 1.0f));
  GGO_CHECK_VEC2F_EQ(ggo::uniform_bspline_linear(points, 1.f / 2.f), ggo::pos2_f(1.0f, 0.5f));
  GGO_CHECK_VEC2F_EQ(ggo::uniform_bspline_linear(points, 2.f / 3.f), ggo::pos2_f(1.0f, 0.0f));
  GGO_CHECK_VEC2F_EQ(ggo::uniform_bspline_linear(points, 5.f / 6.f), ggo::pos2_f(1.5f, 0.0f));
  GGO_CHECK_VEC2F_EQ(ggo::uniform_bspline_linear(points, 1.f),       ggo::pos2_f(2.0f, 0.0f));
}

////////////////////////////////////////////////////////////////////
GGO_TEST(bspline, uniform_bspline_quadratic)
{
  const std::vector<ggo::pos2_f> points({ { 0.f, 1.f }, { 1.f, 1.f }, { 1.f, 0.f }, { 2.f, 0.f } });

  GGO_CHECK_VEC2F_EQ(ggo::uniform_bspline_quadratic(points, 0.f),       ggo::pos2_f(0.5f, 1.0f));
  GGO_CHECK_VEC2F_EQ(ggo::uniform_bspline_quadratic(points, 1.f / 4.f), ggo::pos2_f(0.875f, 0.875f));
  GGO_CHECK_VEC2F_EQ(ggo::uniform_bspline_quadratic(points, 1.f / 2.f), ggo::pos2_f(1.0f, 0.5f));
  GGO_CHECK_VEC2F_EQ(ggo::uniform_bspline_quadratic(points, 3.f / 4.f), ggo::pos2_f(1.125f, 0.125f));
  GGO_CHECK_VEC2F_EQ(ggo::uniform_bspline_quadratic(points, 1.f),       ggo::pos2_f(1.5f, 0.0f));
}

////////////////////////////////////////////////////////////////////
GGO_TEST(bspline, uniform_bspline_cubic)
{
  const std::vector<ggo::pos2_f> points({ { 0.f, 1.f }, { 1.f, 1.f }, { 1.f, 0.f }, { 2.f, 0.f }, { 2.f, 2.f } });

  GGO_CHECK_VEC2F_EQ(ggo::uniform_bspline_cubic(points, 0.f),       ggo::pos2_f(5.0f / 6.0f, 5.0f / 6.0f));
  GGO_CHECK_VEC2F_EQ(ggo::uniform_bspline_cubic(points, 1.f / 4.f), ggo::pos2_f(1.0f, 0.5f));
  GGO_CHECK_VEC2F_EQ(ggo::uniform_bspline_cubic(points, 1.f / 2.f), ggo::pos2_f(7.0f / 6.0f, 1.0f / 6.0f));
  GGO_CHECK_VEC2F_EQ(ggo::uniform_bspline_cubic(points, 3.f / 4.f), ggo::pos2_f(1.5f, 0.0625f));
  GGO_CHECK_VEC2F_EQ(ggo::uniform_bspline_cubic(points, 1.f),       ggo::pos2_f(11.0f / 6.0f, 1.0f / 3.0f));
}

////////////////////////////////////////////////////////////////////
GGO_TEST(bspline, base_linear)
{
  const std::vector<ggo::pos2_f> points({ { 0.f, 1.f }, { 1.f, 1.f } });
  const std::vector<float> knots({ 1.f, 2.f });

  GGO_CHECK_VEC2F_EQ(ggo::bspline_base(points, knots, 1.0f), ggo::pos2_f(0.0f, 1.0f));
  GGO_CHECK_VEC2F_EQ(ggo::bspline_base(points, knots, 1.5f), ggo::pos2_f(0.5f, 1.0f));
  GGO_CHECK_VEC2F_EQ(ggo::bspline_base(points, knots, 2.0f), ggo::pos2_f(1.0f, 1.0f));
}

////////////////////////////////////////////////////////////////////
GGO_TEST(bspline, linear)
{
  const std::vector<ggo::pos2_f> points({ { 0.f, 1.f }, { 1.f, 1.f }, { 1.f, 0.f }, { 2.f, 0.f } });
  const std::vector<float> knots({ 0.f, 1.f, 2.f, 3.f });

  GGO_CHECK_VEC2F_EQ(ggo::bspline(1, points, knots, 0.0f), ggo::pos2_f(0.0f, 1.0f));
  GGO_CHECK_VEC2F_EQ(ggo::bspline(1, points, knots, 0.5f), ggo::pos2_f(0.5f, 1.0f));
  GGO_CHECK_VEC2F_EQ(ggo::bspline(1, points, knots, 1.0f), ggo::pos2_f(1.0f, 1.0f));
  GGO_CHECK_VEC2F_EQ(ggo::bspline(1, points, knots, 1.5f), ggo::pos2_f(1.0f, 0.5f));
  GGO_CHECK_VEC2F_EQ(ggo::bspline(1, points, knots, 2.0f), ggo::pos2_f(1.0f, 0.0f));
  GGO_CHECK_VEC2F_EQ(ggo::bspline(1, points, knots, 2.5f), ggo::pos2_f(1.5f, 0.0f));
  GGO_CHECK_VEC2F_EQ(ggo::bspline(1, points, knots, 3.0f), ggo::pos2_f(2.0f, 0.0f));
}

////////////////////////////////////////////////////////////////////
GGO_TEST(bspline, base_quadratic_uniform)
{
  const std::vector<ggo::pos2_f> points({ { 0.f, 1.f }, { 1.f, 1.f }, { 1.f, 0.f } });
  const std::vector<float> knots({ 0.f, 1.f, 2.f, 3.f });

  GGO_CHECK_VEC2F_EQ(ggo::bspline_base(points, knots, 1.0f), ggo::pos2_f(0.5f, 1.0f));
  GGO_CHECK_VEC2F_EQ(ggo::bspline_base(points, knots, 1.5f), ggo::pos2_f(0.875f, 0.875f));
  GGO_CHECK_VEC2F_EQ(ggo::bspline_base(points, knots, 2.0f), ggo::pos2_f(1.0f, 0.5f));
}

////////////////////////////////////////////////////////////////////
GGO_TEST(bspline, base_quadratic_clamped)
{
  const std::vector<ggo::pos2_f> points({ { 0.f, 1.f }, { 1.f, 1.f }, { 1.f, 0.f } });
  const std::vector<float> knots({ 0.f, 0.f, 1.f, 1.f });

  GGO_CHECK_VEC2F_EQ(ggo::bspline_base(points, knots, 0.0f), ggo::pos2_f(0.0f, 1.0f));
  GGO_CHECK_VEC2F_EQ(ggo::bspline_base(points, knots, 0.5f), ggo::pos2_f(0.75f, 0.75f));
  GGO_CHECK_VEC2F_EQ(ggo::bspline_base(points, knots, 1.0f), ggo::pos2_f(1.0f, 0.0f));
}

////////////////////////////////////////////////////////////////////
GGO_TEST(bspline, quadratic)
{
  const std::vector<ggo::pos2_f> points({ { 0.f, 1.f }, { 1.f, 1.f }, { 1.f, 0.f }, { 2.f, 0.f } });
  const std::vector<float> knots({ 0.f, 1.f, 2.f, 3.f, 4.f });

  GGO_CHECK_VEC2F_EQ(ggo::bspline(2, points, knots, 1.0f), ggo::pos2_f(0.5f, 1.0f));
  GGO_CHECK_VEC2F_EQ(ggo::bspline(2, points, knots, 1.5f), ggo::pos2_f(0.875f, 0.875f));
  GGO_CHECK_VEC2F_EQ(ggo::bspline(2, points, knots, 2.0f), ggo::pos2_f(1.0f, 0.5f));
  GGO_CHECK_VEC2F_EQ(ggo::bspline(2, points, knots, 2.5f), ggo::pos2_f(1.125f, 0.125f));
  GGO_CHECK_VEC2F_EQ(ggo::bspline(2, points, knots, 3.0f), ggo::pos2_f(1.5f, 0.0f));
}

////////////////////////////////////////////////////////////////////
GGO_TEST(bspline, base_cubic_clamped)
{
  const std::vector<ggo::pos2_f> points({ { 0.f, 1.f }, { 1.f, 1.f }, { 1.f, 0.f }, { 2.f, 0.f } });
  const std::vector<float> knots({ 0.f, 0.f, 0.f, 1.f, 1.f, 1.f });

  GGO_CHECK_VEC2F_EQ(ggo::bspline_base(points, knots, 0.0f), ggo::pos2_f(0.0f, 1.0f));
  GGO_CHECK_VEC2F_EQ(ggo::bspline_base(points, knots, 0.5f), ggo::pos2_f(1.0f, 0.5f));
  GGO_CHECK_VEC2F_EQ(ggo::bspline_base(points, knots, 1.0f), ggo::pos2_f(2.0f, 0.0f));
}

////////////////////////////////////////////////////////////////////
GGO_TEST(bspline, cubic)
{
  const std::vector<ggo::pos2_f> points({ { 0.f, 1.f }, { 1.f, 1.f }, { 1.f, 0.f }, { 2.f, 0.f }, { 2.f, 1.f } });
  const std::vector<float> knots({ 0.f, 1.f, 2.f, 3.f, 4.f, 5.f, 6.f });

  GGO_CHECK_VEC2F_EQ(ggo::bspline(3, points, knots, 2.0f), ggo::pos2_f(0.833333f, 0.833333f));
  GGO_CHECK_VEC2F_EQ(ggo::bspline(3, points, knots, 2.5f), ggo::pos2_f(1.0f, 0.5f));
  GGO_CHECK_VEC2F_EQ(ggo::bspline(3, points, knots, 3.0f), ggo::pos2_f(1.166666f, 0.166666f));
  GGO_CHECK_VEC2F_EQ(ggo::bspline(3, points, knots, 3.5f), ggo::pos2_f(1.5f, 0.041666f));
  GGO_CHECK_VEC2F_EQ(ggo::bspline(3, points, knots, 4.0f), ggo::pos2_f(1.833333f, 0.166666f));
}




