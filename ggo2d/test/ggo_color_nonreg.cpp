#include <ggo_nonreg.h>
#include <ggo_curve.h>
#include <ggo_color.h>

////////////////////////////////////////////////////////////////////
GGO_TEST(color, operators)
{
  ggo::color_32f c1(0.25f, 0.5f, 0.75f);
  ggo::color_32f c2(0.01f, 0.1f, 0.01f);
  
  auto c3 = c1 + c2;
  GGO_CHECK_FABS(c3._r, 0.26f);
  GGO_CHECK_FABS(c3._g, 0.6f);
  GGO_CHECK_FABS(c3._b, 0.76f);
}

////////////////////////////////////////////////////////////////////
GGO_TEST(color, alpha_blending)
{
  ggo::color_8u c1(0x00, 0x00, 0xff);
  ggo::color_8u c2(0x00, 0xff, 0xff);

  auto c3 = ggo::alpha_blend<8>(c1, 0, c2);
  GGO_CHECK_EQ(c3._r, 0x00);
  GGO_CHECK_EQ(c3._g, 0xff);
  GGO_CHECK_EQ(c3._b, 0xff);

  auto c4 = ggo::alpha_blend<8>(c1, 0x100, c2);
  GGO_CHECK_EQ(c4._r, 0x00);
  GGO_CHECK_EQ(c4._g, 0x00);
  GGO_CHECK_EQ(c4._b, 0xff);

  auto c5 = ggo::alpha_blend<8>(c1, 64, c2);
  GGO_CHECK_EQ(c5._r, 0x00);
  GGO_CHECK_EQ(c5._g, 0x00);
  GGO_CHECK_EQ(c5._b, 0xff);

  auto c6 = ggo::alpha_blend<24>(c1, 0, c2);
  GGO_CHECK_EQ(c6._r, 0x00);
  GGO_CHECK_EQ(c6._g, 0xff);
  GGO_CHECK_EQ(c6._b, 0xff);

  auto c7 = ggo::alpha_blend<24>(c1, 0x1000000, c2);
  GGO_CHECK_EQ(c7._r, 0x00);
  GGO_CHECK_EQ(c7._g, 0x00);
  GGO_CHECK_EQ(c7._b, 0xff);
}

////////////////////////////////////////////////////////////////////
GGO_TEST(color, linear_curve)
{
  ggo::linear_curve<float, ggo::color_32f> curve;
  curve.push_point(0, ggo::color_32f(1.f, 0.f, 0.f));
  curve.push_point(1, ggo::color_32f(0.f, 1.f, 1.f));
  ggo::color_32f c = curve.evaluate(0.25f);
  
  GGO_CHECK_FABS(c._r, 0.75f);
  GGO_CHECK_FABS(c._g, 0.25f);
  GGO_CHECK_FABS(c._b, 0.25f);
}
