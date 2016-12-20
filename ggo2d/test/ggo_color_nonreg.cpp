#include <ggo_nonreg.h>
#include <ggo_curve.h>
#include <ggo_color.h>

////////////////////////////////////////////////////////////////////
GGO_TEST(color, operators)
{
  ggo::color_32f c1(0.25f, 0.5f, 0.75f);
  ggo::color_32f c2(0.01f, 0.1f, 0.01f);
  
  auto c3 = c1 + c2;
  GGO_CHECK_FABS(c3.r(), 0.26f);
  GGO_CHECK_FABS(c3.g(), 0.6f);
  GGO_CHECK_FABS(c3.b(), 0.76f);
}

////////////////////////////////////////////////////////////////////
GGO_TEST(color, linear_curve)
{
  ggo::linear_curve<float, ggo::color_32f> curve;
  curve.push_point(0, ggo::color_32f(1.f, 0.f, 0.f));
  curve.push_point(1, ggo::color_32f(0.f, 1.f, 1.f));
  ggo::color_32f c = curve.evaluate(0.25f);

  GGO_CHECK_FABS(c.r(), 0.75f);
  GGO_CHECK_FABS(c.g(), 0.25f);
  GGO_CHECK_FABS(c.b(), 0.25f);
}
