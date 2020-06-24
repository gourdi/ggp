#include <kernel/nonreg/ggo_nonreg.h>
#include <kernel/ggo_vec2.h>
#include <kernel/math/splines/ggo_bspline.h>

////////////////////////////////////////////////////////////////////
GGO_TEST(bspline, uniform_bspline_linear)
{
  const std::vector<ggo::pos2_f> points({ { 0.f, 1.f }, { 1.f, 1.f }, { 1.f, 0.f }, { 2.f, 0.f } });

  const ggo::pos2_f p0 = ggo::uniform_bspline_linear<ggo::pos2_f>(points, 0.f);
  GGO_CHECK_FLOAT_EQ(p0.x(), 0.0f);
  GGO_CHECK_FLOAT_EQ(p0.y(), 1.0f);
  
  const ggo::pos2_f p1 = ggo::uniform_bspline_linear<ggo::pos2_f>(points, 1.f / 6.f);
  GGO_CHECK_FLOAT_EQ(p1.x(), 0.5f);
  GGO_CHECK_FLOAT_EQ(p1.y(), 1.0f);
  
  const ggo::pos2_f p2 = ggo::uniform_bspline_linear<ggo::pos2_f>(points, 1.f / 3.f);
  GGO_CHECK_FLOAT_EQ(p2.x(), 1.0f);
  GGO_CHECK_FLOAT_EQ(p2.y(), 1.0f);
  
  const ggo::pos2_f p3 = ggo::uniform_bspline_linear<ggo::pos2_f>(points, 1.f / 2.f);
  GGO_CHECK_FLOAT_EQ(p3.x(), 1.0f);
  GGO_CHECK_FLOAT_EQ(p3.y(), 0.5f);
  
  const ggo::pos2_f p4 = ggo::uniform_bspline_linear<ggo::pos2_f>(points, 2.f / 3.f);
  GGO_CHECK_FLOAT_EQ(p4.x(), 1.0f);
  GGO_CHECK_FLOAT_EQ(p4.y(), 0.0f);
  
  const ggo::pos2_f p5 = ggo::uniform_bspline_linear<ggo::pos2_f>(points, 5.f / 6.f);
  GGO_CHECK_FLOAT_EQ(p5.x(), 1.5f);
  GGO_CHECK_FLOAT_EQ(p5.y(), 0.0f);
  
  const ggo::pos2_f p6 = ggo::uniform_bspline_linear<ggo::pos2_f>(points, 1.f);
  GGO_CHECK_FLOAT_EQ(p6.x(), 2.0f);
  GGO_CHECK_FLOAT_EQ(p6.y(), 0.0f);
}

////////////////////////////////////////////////////////////////////
GGO_TEST(bspline, uniform_bspline_quadratic)
{
  const std::vector<ggo::pos2_f> points({ { 0.f, 1.f }, { 1.f, 1.f }, { 1.f, 0.f }, { 2.f, 0.f } });

  const ggo::pos2_f p0 = ggo::uniform_bspline_quadratic<ggo::pos2_f>(points, 0.f);
  GGO_CHECK_FLOAT_EQ(p0.x(), 0.5f);
  GGO_CHECK_FLOAT_EQ(p0.y(), 1.0f);
  
  const ggo::pos2_f p1 = ggo::uniform_bspline_quadratic<ggo::pos2_f>(points, 1.f / 4.f);
  GGO_CHECK_FLOAT_EQ(p1.x(), 0.875f);
  GGO_CHECK_FLOAT_EQ(p1.y(), 0.875f);
  
  const ggo::pos2_f p2 = ggo::uniform_bspline_quadratic<ggo::pos2_f>(points, 1.f / 2.f);
  GGO_CHECK_FLOAT_EQ(p2.x(), 1.0f);
  GGO_CHECK_FLOAT_EQ(p2.y(), 0.5f);
  
  const ggo::pos2_f p3 = ggo::uniform_bspline_quadratic<ggo::pos2_f>(points, 3.f / 4.f);
  GGO_CHECK_FLOAT_EQ(p3.x(), 1.125f);
  GGO_CHECK_FLOAT_EQ(p3.y(), 0.125f);
  
  const ggo::pos2_f p4 = ggo::uniform_bspline_quadratic<ggo::pos2_f>(points, 1.f);
  GGO_CHECK_FLOAT_EQ(p4.x(), 1.5f);
  GGO_CHECK_FLOAT_EQ(p4.y(), 0.0f);
}

////////////////////////////////////////////////////////////////////
GGO_TEST(bspline, uniform_bspline_cubic)
{
  const std::vector<ggo::pos2_f> points({ { 0.f, 1.f }, { 1.f, 1.f }, { 1.f, 0.f }, { 2.f, 0.f }, { 2.f, 2.f } });

  const ggo::pos2_f p0 = ggo::uniform_bspline_cubic<ggo::pos2_f>(points, 0.f);
  GGO_CHECK_FLOAT_EQ(p0.x(), 5.0f / 6.0f);
  GGO_CHECK_FLOAT_EQ(p0.y(), 5.0f / 6.0f);
  
  const ggo::pos2_f p1 = ggo::uniform_bspline_cubic<ggo::pos2_f>(points, 1.f / 4.f);
  GGO_CHECK_FLOAT_EQ(p1.x(), 1.0f);
  GGO_CHECK_FLOAT_EQ(p1.y(), 0.5f);
  
  const ggo::pos2_f p2 = ggo::uniform_bspline_cubic<ggo::pos2_f>(points, 1.f / 2.f);
  GGO_CHECK_FLOAT_EQ(p2.x(), 7.0f / 6.0f);
  GGO_CHECK_FLOAT_EQ(p2.y(), 1.0f / 6.0f);
  
  const ggo::pos2_f p3 = ggo::uniform_bspline_cubic<ggo::pos2_f>(points, 3.f / 4.f);
  GGO_CHECK_FLOAT_EQ(p3.x(), 1.5f);
  GGO_CHECK_FLOAT_EQ(p3.y(), 0.0625f);
  
  const ggo::pos2_f p4 = ggo::uniform_bspline_cubic<ggo::pos2_f>(points, 1.f);
  GGO_CHECK_FLOAT_EQ(p4.x(), 11.0f / 6.0f);
  GGO_CHECK_FLOAT_EQ(p4.y(), 1.0f / 3.0f);
}

////////////////////////////////////////////////////////////////////
GGO_TEST(bspline, base_linear)
{
  const std::vector<ggo::pos2_f> points({ { 0.f, 1.f }, { 1.f, 1.f } });
  const std::vector<float> knots({ 1.f, 2.f });

  const ggo::pos2_f p0 = ggo::bspline_base<ggo::pos2_f>(points, knots, 1.0f);
  GGO_CHECK_FLOAT_EQ(p0.x(), 0.0f);
  GGO_CHECK_FLOAT_EQ(p0.y(), 1.0f);

  const ggo::pos2_f p1 = ggo::bspline_base<ggo::pos2_f>(points, knots, 1.5f);
  GGO_CHECK_FLOAT_EQ(p1.x(), 0.5f);
  GGO_CHECK_FLOAT_EQ(p1.y(), 1.0f);

  const ggo::pos2_f p2 = ggo::bspline_base<ggo::pos2_f>(points, knots, 2.0f);
  GGO_CHECK_FLOAT_EQ(p2.x(), 1.0f);
  GGO_CHECK_FLOAT_EQ(p2.y(), 1.0f);
}

////////////////////////////////////////////////////////////////////
GGO_TEST(bspline, linear)
{
  const std::vector<ggo::pos2_f> points({ { 0.f, 1.f }, { 1.f, 1.f }, { 1.f, 0.f }, { 2.f, 0.f } });
  const std::vector<float> knots({ 0.f, 1.f, 2.f, 3.f });

  struct test_case
  {
    float _t;
    ggo::pos2_f _expected_pos;
  };
  const std::vector<test_case> test_cases({
    { 0.0f, { 0.0f, 1.0f } },
    { 0.5f, { 0.5f, 1.0f } },
    { 1.0f, { 1.0f, 1.0f } },
    { 1.5f, { 1.0f, 0.5f } },
    { 2.0f, { 1.0f, 0.0f } },
    { 2.5f, { 1.5f, 0.0f } },
    { 3.0f, { 2.0f, 0.0f } }
    });

  for (const auto & test_case : test_cases)
  {
    const ggo::pos2_f p = ggo::bspline<ggo::pos2_f>(1, points, knots, test_case._t);
    GGO_CHECK_FLOAT_EQ(p.x(), test_case._expected_pos.x());
    GGO_CHECK_FLOAT_EQ(p.y(), test_case._expected_pos.y());
  }
}

////////////////////////////////////////////////////////////////////
GGO_TEST(bspline, base_quadratic_uniform)
{
  const std::vector<ggo::pos2_f> points({ { 0.f, 1.f }, { 1.f, 1.f }, { 1.f, 0.f } });
  const std::vector<float> knots({ 0.f, 1.f, 2.f, 3.f });

  const ggo::pos2_f p0 = ggo::bspline_base<ggo::pos2_f>(points, knots, 1.0f);
  GGO_CHECK_FLOAT_EQ(p0.x(), 0.5f);
  GGO_CHECK_FLOAT_EQ(p0.y(), 1.0f);

  const ggo::pos2_f p1 = ggo::bspline_base<ggo::pos2_f>(points, knots, 1.5f);
  GGO_CHECK_FLOAT_EQ(p1.x(), 0.875f);
  GGO_CHECK_FLOAT_EQ(p1.y(), 0.875f);

  const ggo::pos2_f p2 = ggo::bspline_base<ggo::pos2_f>(points, knots, 2.0f);
  GGO_CHECK_FLOAT_EQ(p2.x(), 1.0f);
  GGO_CHECK_FLOAT_EQ(p2.y(), 0.5f);
}

////////////////////////////////////////////////////////////////////
GGO_TEST(bspline, base_quadratic_clamped)
{
  const std::vector<ggo::pos2_f> points({ { 0.f, 1.f }, { 1.f, 1.f }, { 1.f, 0.f } });
  const std::vector<float> knots({ 0.f, 0.f, 1.f, 1.f });

  const ggo::pos2_f p0 = ggo::bspline_base<ggo::pos2_f>(points, knots, 0.0f);
  GGO_CHECK_FLOAT_EQ(p0.x(), 0.0f);
  GGO_CHECK_FLOAT_EQ(p0.y(), 1.0f);

  const ggo::pos2_f p1 = ggo::bspline_base<ggo::pos2_f>(points, knots, 0.5f);
  GGO_CHECK_FLOAT_EQ(p1.x(), 0.75f);
  GGO_CHECK_FLOAT_EQ(p1.y(), 0.75f);

  const ggo::pos2_f p2 = ggo::bspline_base<ggo::pos2_f>(points, knots, 1.0f);
  GGO_CHECK_FLOAT_EQ(p2.x(), 1.0f);
  GGO_CHECK_FLOAT_EQ(p2.y(), 0.0f);
}

////////////////////////////////////////////////////////////////////
GGO_TEST(bspline, quadratic)
{
  const std::vector<ggo::pos2_f> points({ { 0.f, 1.f }, { 1.f, 1.f }, { 1.f, 0.f }, { 2.f, 0.f } });
  const std::vector<float> knots({ 0.f, 1.f, 2.f, 3.f, 4.f });

  struct test_case
  {
    float _t;
    ggo::pos2_f _expected_pos;
  };
  const std::vector<test_case> test_cases({
    { 1.0f, { 0.5f, 1.0f } },
    { 1.5f, { 0.875f, 0.875f } },
    { 2.0f, { 1.0f, 0.5f } },
    { 2.5f, { 1.125f, 0.125f } },
    { 3.0f, { 1.5f, 0.0f } }
    });

  for (const auto& test_case : test_cases)
  {
    const ggo::pos2_f p = ggo::bspline<ggo::pos2_f>(2, points, knots, test_case._t);
    GGO_CHECK_FLOAT_EQ(p.x(), test_case._expected_pos.x());
    GGO_CHECK_FLOAT_EQ(p.y(), test_case._expected_pos.y());
  }
}

////////////////////////////////////////////////////////////////////
GGO_TEST(bspline, base_cubic_clamped)
{
  const std::vector<ggo::pos2_f> points({ { 0.f, 1.f }, { 1.f, 1.f }, { 1.f, 0.f }, { 2.f, 0.f } });
  const std::vector<float> knots({ 0.f, 0.f, 0.f, 1.f, 1.f, 1.f });

  const ggo::pos2_f p0 = ggo::bspline_base<ggo::pos2_f>(points, knots, 0.0f);
  GGO_CHECK_FLOAT_EQ(p0.x(), 0.0f);
  GGO_CHECK_FLOAT_EQ(p0.y(), 1.0f);

  const ggo::pos2_f p1 = ggo::bspline_base<ggo::pos2_f>(points, knots, 0.5f);
  GGO_CHECK_FLOAT_EQ(p1.x(), 1.0f);
  GGO_CHECK_FLOAT_EQ(p1.y(), 0.5f);

  const ggo::pos2_f p2 = ggo::bspline_base<ggo::pos2_f>(points, knots, 1.0f);
  GGO_CHECK_FLOAT_EQ(p2.x(), 2.0f);
  GGO_CHECK_FLOAT_EQ(p2.y(), 0.0f);
}

////////////////////////////////////////////////////////////////////
GGO_TEST(bspline, cubic)
{
  const std::vector<ggo::pos2_f> points({ { 0.f, 1.f }, { 1.f, 1.f }, { 1.f, 0.f }, { 2.f, 0.f }, { 2.f, 1.f } });
  const std::vector<float> knots({ 0.f, 1.f, 2.f, 3.f, 4.f, 5.f, 6.f });

  struct test_case
  {
    float _t;
    ggo::pos2_f _expected_pos;
  };
  const std::vector<test_case> test_cases({
    { 2.0f, { 0.833333f, 0.833333f } },
    { 2.5f, { 1.0f, 0.5f } },
    { 3.0f, { 1.166666f, 0.166666f } },
    { 3.5f, { 1.5f, 0.041666f } },
    { 4.0f, { 1.833333f, 0.166666f } }
    });

  for (const auto& test_case : test_cases)
  {
    const ggo::pos2_f p = ggo::bspline<ggo::pos2_f>(3, points, knots, test_case._t);
    GGO_CHECK_FLOAT_EQ(p.x(), test_case._expected_pos.x());
    GGO_CHECK_FLOAT_EQ(p.y(), test_case._expected_pos.y());
  }
}




