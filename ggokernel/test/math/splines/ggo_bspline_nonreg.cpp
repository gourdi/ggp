#pragma once

#include <stdexcept>
#include <kernel/ggo_kernel.h>

namespace ggo
{
  template <typename data_t, typename scalar_t>
  data_t bspline(std::vector<data_t> control_points, std::vector<scalar_t> knots, scalar_t t)
  {
    GGO_ASSERT_EQ(2 * (control_points.size() - 1), knots.size());

    while (control_points.size() > 1)
    {
      std::vector<data_t> new_control_points;

      for (int i = 0; i < control_points.size() - 1; ++i)
      {
        size_t index1 = i + knots.size() / 2;
        size_t index2 = i;
        GGO_ASSERT_GT(index1, index2);

        scalar_t t1 = knots[index1];
        scalar_t t2 = knots[index2];
        GGO_ASSERT_GT(t1, t2);

        scalar_t w1 = t1 - t;
        scalar_t w2 = t - t2;
        GGO_ASSERT_GT(w1 + w2, scalar_t(0));

        data_t control_point = (w1 * control_points[i] + w2 * control_points[i + 1]) / (w1 + w2);

        new_control_points.push_back(control_point);
      }

      control_points = new_control_points;

      knots.erase(knots.begin());
      knots.pop_back();
    }

    return control_points.front();
  }

  template <typename data_t, typename scalar_t>
  data_t bspline(int degree, std::vector<data_t> control_points, std::vector<scalar_t> knots, scalar_t t)
  {
    GGO_ASSERT_EQ(knots.size(), control_points.size() + degree - 1);

    int begin = degree - 1;
    int end   = knots.size() - degree + 1;
    for (int i = begin; i < end; ++i)
    {
      if ()
    }
  }
}




#include <kernel/nonreg/ggo_nonreg.h>
#include <kernel/ggo_vec2.h>

////////////////////////////////////////////////////////////////////
GGO_TEST(bspline, linear)
{
  const std::vector<ggo::pos2_f> points({ { 0.f, 1.f }, { 1.f, 1.f } });
  const std::vector<float> knots({ 0.f, 1.f });

  const ggo::pos2_f p0 = ggo::bspline<ggo::pos2_f>(points, knots, 0.0f);
  GGO_CHECK_FLOAT_EQ(p0.x(), 0.0f);
  GGO_CHECK_FLOAT_EQ(p0.y(), 1.0f);

  const ggo::pos2_f p1 = ggo::bspline<ggo::pos2_f>(points, knots, 0.5f);
  GGO_CHECK_FLOAT_EQ(p1.x(), 0.5f);
  GGO_CHECK_FLOAT_EQ(p1.y(), 1.0f);

  const ggo::pos2_f p2 = ggo::bspline<ggo::pos2_f>(points, knots, 1.0f);
  GGO_CHECK_FLOAT_EQ(p2.x(), 1.0f);
  GGO_CHECK_FLOAT_EQ(p2.y(), 1.0f);
}

////////////////////////////////////////////////////////////////////
GGO_TEST(bspline, quadratic_uniform)
{
  const std::vector<ggo::pos2_f> points({ { 0.f, 1.f }, { 1.f, 1.f }, { 1.f, 0.f } });
  const std::vector<float> knots({ 0.f, 1.f, 2.f, 3.f });

  const ggo::pos2_f p0 = ggo::bspline<ggo::pos2_f>(points, knots, 1.0f);
  GGO_CHECK_FLOAT_EQ(p0.x(), 0.5f);
  GGO_CHECK_FLOAT_EQ(p0.y(), 1.0f);

  const ggo::pos2_f p1 = ggo::bspline<ggo::pos2_f>(points, knots, 1.5f);
  GGO_CHECK_FLOAT_EQ(p1.x(), 0.875f);
  GGO_CHECK_FLOAT_EQ(p1.y(), 0.875f);

  const ggo::pos2_f p2 = ggo::bspline<ggo::pos2_f>(points, knots, 2.0f);
  GGO_CHECK_FLOAT_EQ(p2.x(), 1.0f);
  GGO_CHECK_FLOAT_EQ(p2.y(), 0.5f);
}

////////////////////////////////////////////////////////////////////
GGO_TEST(bspline, quadratic_clamped)
{
  const std::vector<ggo::pos2_f> points({ { 0.f, 1.f }, { 1.f, 1.f }, { 1.f, 0.f } });
  const std::vector<float> knots({ 0.f, 0.f, 1.f, 1.f });

  const ggo::pos2_f p0 = ggo::bspline<ggo::pos2_f>(points, knots, 0.0f);
  GGO_CHECK_FLOAT_EQ(p0.x(), 0.0f);
  GGO_CHECK_FLOAT_EQ(p0.y(), 1.0f);

  const ggo::pos2_f p1 = ggo::bspline<ggo::pos2_f>(points, knots, 0.5f);
  GGO_CHECK_FLOAT_EQ(p1.x(), 0.75f);
  GGO_CHECK_FLOAT_EQ(p1.y(), 0.75f);

  const ggo::pos2_f p2 = ggo::bspline<ggo::pos2_f>(points, knots, 1.0f);
  GGO_CHECK_FLOAT_EQ(p2.x(), 1.0f);
  GGO_CHECK_FLOAT_EQ(p2.y(), 0.0f);
}

////////////////////////////////////////////////////////////////////
GGO_TEST(bspline, cubic_clamped)
{
  const std::vector<ggo::pos2_f> points({ { 0.f, 1.f }, { 1.f, 1.f }, { 1.f, 0.f }, { 2.f, 0.f } });
  const std::vector<float> knots({ 0.f, 0.f, 0.f, 1.f, 1.f, 1.f });

  const ggo::pos2_f p0 = ggo::bspline<ggo::pos2_f>(points, knots, 0.0f);
  GGO_CHECK_FLOAT_EQ(p0.x(), 0.0f);
  GGO_CHECK_FLOAT_EQ(p0.y(), 1.0f);

  const ggo::pos2_f p1 = ggo::bspline<ggo::pos2_f>(points, knots, 0.5f);
  GGO_CHECK_FLOAT_EQ(p1.x(), 1.0f);
  GGO_CHECK_FLOAT_EQ(p1.y(), 0.5f);

  const ggo::pos2_f p2 = ggo::bspline<ggo::pos2_f>(points, knots, 1.0f);
  GGO_CHECK_FLOAT_EQ(p2.x(), 2.0f);
  GGO_CHECK_FLOAT_EQ(p2.y(), 0.0f);
}



