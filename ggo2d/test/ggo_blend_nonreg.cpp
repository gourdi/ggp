#include <ggo_nonreg.h>
#include <ggo_blend.h>

/////////////////////////////////////////////////////////////////////
GGO_TEST(blend, alpha_y8u)
{
  GGO_CHECK_EQ(int(ggo::alpha_blend(0x00, 0xff, 0xff)), int(0xff));
  GGO_CHECK_EQ(int(ggo::alpha_blend(0x00, 0xff, 0x00)), int(0x00));
  GGO_CHECK_EQ(int(ggo::alpha_blend(0xff, 0x00, 0xff)), int(0x00));
  GGO_CHECK_EQ(int(ggo::alpha_blend(0xff, 0x00, 0x00)), int(0xff));
  GGO_CHECK_EQ(int(ggo::alpha_blend(0x00, 0xff, 0x80)), int(0x80));
  GGO_CHECK_EQ(int(ggo::alpha_blend(0xff, 0x00, 0x80)), int(0x7f));
  GGO_CHECK_EQ(int(ggo::alpha_blend(0x00, 0x01, 0x80)), int(0x01));
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(blend, alpha_rgb8u)
{
  auto c1 = ggo::alpha_blend({ uint8_t(0x00), uint8_t(0x00), uint8_t(0x00) }, { uint8_t(0x00), uint8_t(0x80), uint8_t(0xff) }, 0xff);
  GGO_CHECK_EQ(c1.r(), 0x00);
  GGO_CHECK_EQ(c1.g(), 0x80);
  GGO_CHECK_EQ(c1.b(), 0xff);

  auto c2 = ggo::alpha_blend({ uint8_t(0x00), uint8_t(0x80), uint8_t(0xff) }, { uint8_t(0xff), uint8_t(0xff), uint8_t(0xff) }, 0x00);
  GGO_CHECK_EQ(c2.r(), 0x00);
  GGO_CHECK_EQ(c2.g(), 0x80);
  GGO_CHECK_EQ(c2.b(), 0xff);

  auto c3 = ggo::alpha_blend({ uint8_t(0x00), uint8_t(0x80), uint8_t(0xff) }, { uint8_t(0xff), uint8_t(0xff), uint8_t(0xff) }, 0x80);
  GGO_CHECK_EQ(c3.r(), 0x80);
  GGO_CHECK_EQ(c3.g(), 0xc0);
  GGO_CHECK_EQ(c3.b(), 0xff);
}

////////////////////////////////////////////////////////////////////
GGO_TEST(blend, alpha_float_rgb8u)
{
  ggo::alpha_blender<ggo::color_8u> blender_opaque(1.f);
  ggo::alpha_blender<ggo::color_8u> blender_transparent(0.f);
  ggo::alpha_blender<ggo::color_8u> blender_half(0.5f);

  ggo::color_8u c1 = blender_opaque(0, 0, ggo::color_8u(uint8_t(0x00), uint8_t(0x00), uint8_t(0xff)), ggo::color_8u(uint8_t(0x00), uint8_t(0xff), uint8_t(0xff)));
  GGO_CHECK_EQ(c1.r(), 0x00);
  GGO_CHECK_EQ(c1.g(), 0xff);
  GGO_CHECK_EQ(c1.b(), 0xff);

  ggo::color_8u c2 = blender_transparent(0, 0, ggo::color_8u(uint8_t(0x00), uint8_t(0x00), uint8_t(0xff)), ggo::color_8u(uint8_t(0x00), uint8_t(0xff), uint8_t(0xff)));
  GGO_CHECK_EQ(c2.r(), 0x00);
  GGO_CHECK_EQ(c2.g(), 0x00);
  GGO_CHECK_EQ(c2.b(), 0xff);

  ggo::color_8u c3 = blender_half(0, 0, ggo::color_8u(uint8_t(0x00), uint8_t(0x00), uint8_t(0xff)), ggo::color_8u(uint8_t(0x00), uint8_t(0xff), uint8_t(0xff)));
  GGO_CHECK_EQ(c3.r(), 0x00);
  GGO_CHECK_EQ(c3.g(), 0x80);
  GGO_CHECK_EQ(c3.b(), 0xff);
}
