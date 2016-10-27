#include <ggo_nonreg.h>
#include <ggo_curve.h>
#include <ggo_color.h>
#include <ggo_blender.h>

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
  ggo::alpha_blender<ggo::color_8u> blender_opaque(1.f);
  ggo::alpha_blender<ggo::color_8u> blender_transparent(0.f);
  ggo::alpha_blender<ggo::color_8u> blender_half(0.5f);

  ggo::color_8u c1 = blender_opaque(ggo::color_8u(0x00, 0x00, 0xff), ggo::color_8u(0x00, 0xff, 0xff));
  GGO_CHECK_EQ(c1._r, 0x00);
  GGO_CHECK_EQ(c1._g, 0xff);
  GGO_CHECK_EQ(c1._b, 0xff);

  ggo::color_8u c2 = blender_transparent(ggo::color_8u(0x00, 0x00, 0xff), ggo::color_8u(0x00, 0xff, 0xff));
  GGO_CHECK_EQ(c2._r, 0x00);
  GGO_CHECK_EQ(c2._g, 0x00);
  GGO_CHECK_EQ(c2._b, 0xff);

  ggo::color_8u c3 = blender_half(ggo::color_8u(0x00, 0x00, 0xff), ggo::color_8u(0x00, 0xff, 0xff));
  GGO_CHECK_EQ(c3._r, 0x00);
  GGO_CHECK_EQ(c3._g, 0x80);
  GGO_CHECK_EQ(c3._b, 0xff);
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
