#include <kernel/nonreg/ggo_nonreg.h>
#include <kernel/ggo_vec2.h>
#include <kernel/math/splines/ggo_bezier.h>

////////////////////////////////////////////////////////////////////
GGO_TEST(bezier, quadratic)
{
  ggo::pos2_f p0 = ggo::quadratic_bezier(ggo::pos2_f(0, 0), ggo::pos2_f(0, 1), ggo::pos2_f(1, 1), 0.f);
  GGO_CHECK_FLOAT_EQ(p0.x(), 0.f);
  GGO_CHECK_FLOAT_EQ(p0.y(), 0.f);

  ggo::pos2_f p1 = ggo::quadratic_bezier(ggo::pos2_f(0, 0), ggo::pos2_f(0, 1), ggo::pos2_f(1, 1), 1.f);
  GGO_CHECK_FLOAT_EQ(p1.x(), 1.f);
  GGO_CHECK_FLOAT_EQ(p1.y(), 1.f);

  ggo::pos2_f p2 = ggo::quadratic_bezier(ggo::pos2_f(0, 0), ggo::pos2_f(0, 1), ggo::pos2_f(1, 1), 0.5f);
  GGO_CHECK_FLOAT_EQ(p2.x(), 0.25f);
  GGO_CHECK_FLOAT_EQ(p2.y(), 0.75f);
}

////////////////////////////////////////////////////////////////////
GGO_TEST(bezier, cubic)
{
  ggo::pos2_f p0 = ggo::cubic_bezier(ggo::pos2_f(0, 0), ggo::pos2_f(0, 1), ggo::pos2_f(1, 1), ggo::pos2_f(1, 0), 0.f);
  GGO_CHECK_FLOAT_EQ(p0.x(), 0.f);
  GGO_CHECK_FLOAT_EQ(p0.y(), 0.f);

  ggo::pos2_f p1 = ggo::cubic_bezier(ggo::pos2_f(0, 0), ggo::pos2_f(0, 1), ggo::pos2_f(1, 1), ggo::pos2_f(1, 0), 1.f);
  GGO_CHECK_FLOAT_EQ(p1.x(), 1.f);
  GGO_CHECK_FLOAT_EQ(p1.y(), 0.f);

  ggo::pos2_f p2 = ggo::cubic_bezier(ggo::pos2_f(0, 0), ggo::pos2_f(0, 1), ggo::pos2_f(1, 1), ggo::pos2_f(1, 0), 0.5f);
  GGO_CHECK_FLOAT_EQ(p2.x(), 0.5f);
  GGO_CHECK_FLOAT_EQ(p2.y(), 0.75f);
}


