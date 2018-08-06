#include <kernel/nonreg/ggo_nonreg.h>
#include <2d/paint/ggo_blend.h>

/////////////////////////////////////////////////////////////////////
GGO_TEST(blend, alpha_y8u)
{
  {
    const ggo::alpha_blender_y8u alpha_blend(1.f);

    GGO_CHECK_EQ(alpha_blend(0xff, 0x00), 0x00);
    GGO_CHECK_EQ(alpha_blend(0xff, 0x80), 0x80);
    GGO_CHECK_EQ(alpha_blend(0xff, 0xff), 0xff);
  }

  {
    const ggo::alpha_blender_y8u alpha_blend(0.5f);

    GGO_CHECK_EQ(alpha_blend(0x00, 0x00), 0x00);
    GGO_CHECK_EQ(alpha_blend(0x00, 0x80), 0x40);
    GGO_CHECK_EQ(alpha_blend(0x00, 0xff), 0x80);
  }
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(blend, alpha_rgb8u)
{
  {
    const ggo::alpha_blender_rgb8u alpha_blend(1.f);

    auto c = alpha_blend({ 0xff_u8, 0xff_u8, 0xff_u8 }, { 0x00_u8, 0x80_u8, 0xff_u8 });
    GGO_CHECK_EQ(int(c._r), 0x00);
    GGO_CHECK_EQ(int(c._g), 0x80);
    GGO_CHECK_EQ(int(c._b), 0xff);
  }

  {
    const ggo::alpha_blender_rgb8u alpha_blend(0.5f);

    auto c = alpha_blend({ 0x00_u8, 0x00_u8, 0x00_u8 }, { 0x00_u8, 0x80_u8, 0xff_u8 });
    GGO_CHECK_EQ(int(c._r), 0x00);
    GGO_CHECK_EQ(int(c._g), 0x40);
    GGO_CHECK_EQ(int(c._b), 0x80);
  }
}

////////////////////////////////////////////////////////////////////
GGO_TEST(blend, additive_rgb8u)
{
  ggo::rgb_8u c = ggo::add_blend(ggo::rgb_8u(0x00, 0x80, 0xff), ggo::rgb_8u(0xff, 0xff, 0xff));
  GGO_CHECK_EQ(int(c._r), 0xff);
  GGO_CHECK_EQ(int(c._g), 0xff);
  GGO_CHECK_EQ(int(c._b), 0xff);
}

