#include <kernel/nonreg/ggo_nonreg.h>
#include <2d/blend/ggo_alpha_blend.h>
#include <2d/blend/ggo_additive_blend.h>
#include <2d/blend/ggo_opacity_blend.h>


// Tests y8u.
namespace ggo
{
  static_assert(alpha_blend(0xff_u8, ggo::ya_8u(0x00, 0xff)) == 0x00);
  static_assert(alpha_blend(0xff_u8, ggo::ya_8u(0x00, 0x00)) == 0xff);
  static_assert(alpha_blend(0xff_u8, ggo::ya_8u(0x00, 0x80)) == 0x7f);

  static_assert(alpha_blend(0xff_u8, ggo::ya_16u(0, 0xffff)) == 0x00);
  static_assert(alpha_blend(0xff_u8, ggo::ya_16u(0, 0x0000)) == 0xff);
  static_assert(alpha_blend(0xff_u8, ggo::ya_16u(0, 0x8000)) == 0x7f);

  static_assert(alpha_blend(0xff_u8, ggo::ya_32u(0, 0xffffffff)) == 0x00);
  static_assert(alpha_blend(0xff_u8, ggo::ya_32u(0, 0x00000000)) == 0xff);
  static_assert(alpha_blend(0xff_u8, ggo::ya_32u(0, 0x80000000)) == 0x7f);

  static_assert(alpha_blend(0xff_u8, ggo::ya_32f(0, 1.0f)) == 0x00);
  static_assert(alpha_blend(0xff_u8, ggo::ya_32f(0, 0.0f)) == 0xff);
  static_assert(alpha_blend(0xff_u8, ggo::ya_32f(0, 0.5f)) == 0x7f);
}

// Tests y16u.
namespace ggo
{
  static_assert(alpha_blend(0xffff_u16, ggo::ya_8u(0x00, 0xff)) == 0x0000);
  static_assert(alpha_blend(0xffff_u16, ggo::ya_8u(0x00, 0x00)) == 0xffff);
  static_assert(alpha_blend(0xffff_u16, ggo::ya_8u(0x00, 0x80)) == 0x7f7f);

  static_assert(alpha_blend(0xffff_u16, ggo::ya_16u(0, 0xffff)) == 0x0000);
  static_assert(alpha_blend(0xffff_u16, ggo::ya_16u(0, 0x0000)) == 0xffff);
  static_assert(alpha_blend(0xffff_u16, ggo::ya_16u(0, 0x8000)) == 0x7fff);

  static_assert(alpha_blend(0xffff_u16, ggo::ya_32u(0, 0xffffffff)) == 0x0000);
  static_assert(alpha_blend(0xffff_u16, ggo::ya_32u(0, 0x00000000)) == 0xffff);
  static_assert(alpha_blend(0xffff_u16, ggo::ya_32u(0, 0x80000000)) == 0x7fff);

  static_assert(alpha_blend(0xffff_u16, ggo::ya_32f(0, 1.0f)) == 0x0000);
  static_assert(alpha_blend(0xffff_u16, ggo::ya_32f(0, 0.0f)) == 0xffff);
  static_assert(alpha_blend(0xffff_u16, ggo::ya_32f(0, 0.5f)) == 0x7fff);
}

// Tests y32u.
namespace ggo
{
  static_assert(alpha_blend(0xffffffff_u32, ggo::ya_8u(0x00, 0xff)) == 0x00000000);
  static_assert(alpha_blend(0xffffffff_u32, ggo::ya_8u(0x00, 0x00)) == 0xffffffff);
  static_assert(alpha_blend(0xffffffff_u32, ggo::ya_8u(0x00, 0x80)) == 0x7f7f7f7f);

  static_assert(alpha_blend(0xffffffff_u32, ggo::ya_16u(0, 0xffff)) == 0x00000000);
  static_assert(alpha_blend(0xffffffff_u32, ggo::ya_16u(0, 0x0000)) == 0xffffffff);
  static_assert(alpha_blend(0xffffffff_u32, ggo::ya_16u(0, 0x8000)) == 0x7fff7fff);

  static_assert(alpha_blend(0xffffffff_u32, ggo::ya_32u(0, 0xffffffff)) == 0x00000000);
  static_assert(alpha_blend(0xffffffff_u32, ggo::ya_32u(0, 0x00000000)) == 0xffffffff);
  static_assert(alpha_blend(0xffffffff_u32, ggo::ya_32u(0, 0x80000000)) == 0x7fffffff);

  static_assert(alpha_blend(0xffffffff_u32, ggo::ya_32f(0, 1.0f)) == 0x00000000);
  static_assert(alpha_blend(0xffffffff_u32, ggo::ya_32f(0, 0.0f)) == 0xffffffff);
  static_assert(alpha_blend(0xffffffff_u32, ggo::ya_32f(0, 0.5f)) == 0x7fffffff);
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(blend, alpha_y32f)
{
  GGO_CHECK_FLOAT_EQ(alpha_blend(1.f, ggo::ya_8u(0x00, 0xff)), 0.0f);
  GGO_CHECK_FLOAT_EQ(alpha_blend(1.f, ggo::ya_8u(0x00, 0x00)), 1.0f);
  GGO_CHECK_FLOAT_EQ(alpha_blend(1.f, ggo::ya_8u(0x00, 0x80)), 127.f / 255.f);

  GGO_CHECK_FLOAT_EQ(alpha_blend(1.f, ggo::ya_16u(0, 0xffff)), 0.0f);
  GGO_CHECK_FLOAT_EQ(alpha_blend(1.f, ggo::ya_16u(0, 0x0000)), 1.0f);
  GGO_CHECK_FLOAT_EQ(alpha_blend(1.f, ggo::ya_16u(0, 0x8000)), 32767.f / 65535.f);

  GGO_CHECK_FLOAT_EQ(alpha_blend(1.f, ggo::ya_32u(0, 0xffffffff)), 0.0f);
  GGO_CHECK_FLOAT_EQ(alpha_blend(1.f, ggo::ya_32u(0, 0x00000000)), 1.0f);
  GGO_CHECK_FLOAT_EQ(alpha_blend(1.f, ggo::ya_32u(0, 0x80000000)), 2147483647.f / 4294967295.f);

  GGO_CHECK_FLOAT_EQ(alpha_blend(1.f, ggo::ya_32f(0, 1.0f)), 0.0f);
  GGO_CHECK_FLOAT_EQ(alpha_blend(1.f, ggo::ya_32f(0, 0.0f)), 1.0f);
  GGO_CHECK_FLOAT_EQ(alpha_blend(1.f, ggo::ya_32f(0, 0.5f)), 0.5f);
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(blend, alpha_rgb8u)
{
  GGO_CHECK_EQ(alpha_blend(ggo::rgb_8u(0xff, 0xff, 0xff), ggo::rgba_8u(0xff, 0x80, 0x00, 0xff)), ggo::rgb_8u(0xff, 0x80, 0x00));
  GGO_CHECK_EQ(alpha_blend(ggo::rgb_8u(0xff, 0xff, 0xff), ggo::rgba_8u(0xff, 0x80, 0x00, 0x80)), ggo::rgb_8u(0xff, 0xbf, 0x7f));
  GGO_CHECK_EQ(alpha_blend(ggo::rgb_8u(0xff, 0xff, 0xff), ggo::rgba_8u(0xff, 0x80, 0x00, 0x00)), ggo::rgb_8u(0xff, 0xff, 0xff));
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(blend, alpha_rgb16u)
{
  GGO_CHECK_EQ(alpha_blend(ggo::rgb_16u(0xffff, 0xffff, 0xffff), ggo::rgba_16u(0xffff, 0x8000, 0x0000, 0xffff)), ggo::rgb_16u(0xffff, 0x8000, 0x0000));
  GGO_CHECK_EQ(alpha_blend(ggo::rgb_16u(0xffff, 0xffff, 0xffff), ggo::rgba_16u(0xffff, 0x8000, 0x0000, 0x8000)), ggo::rgb_16u(0xffff, 0xbfff, 0x7fff));
  GGO_CHECK_EQ(alpha_blend(ggo::rgb_16u(0xffff, 0xffff, 0xffff), ggo::rgba_16u(0xffff, 0x8000, 0x0000, 0x0000)), ggo::rgb_16u(0xffff, 0xffff, 0xffff));
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(blend, alpha_rgb32u)
{
  GGO_CHECK_EQ(alpha_blend(ggo::rgb_32u(0xffffffff, 0xffffffff, 0xffffffff), ggo::rgba_32u(0xffffffff, 0x80000000, 0x00000000, 0xffffffff)), ggo::rgb_32u(0xffffffff, 0x80000000, 0x00000000));
  GGO_CHECK_EQ(alpha_blend(ggo::rgb_32u(0xffffffff, 0xffffffff, 0xffffffff), ggo::rgba_32u(0xffffffff, 0x80000000, 0x00000000, 0x80000000)), ggo::rgb_32u(0xffffffff, 0xbfffffff, 0x7fffffff));
  GGO_CHECK_EQ(alpha_blend(ggo::rgb_32u(0xffffffff, 0xffffffff, 0xffffffff), ggo::rgba_32u(0xffffffff, 0x80000000, 0x00000000, 0x00000000)), ggo::rgb_32u(0xffffffff, 0xffffffff, 0xffffffff));
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(blend, alpha_rgb32f)
{
  GGO_CHECK_EQ(alpha_blend(ggo::rgb_32f(1.0f, 1.0f, 1.0f), ggo::rgba_32f(1.0f, 0.5f, 0.0f, 1.0f)), ggo::rgb_32f(1.0f, 0.5f, 0.0f));
  GGO_CHECK_EQ(alpha_blend(ggo::rgb_32f(1.0f, 1.0f, 1.0f), ggo::rgba_32f(1.0f, 0.5f, 0.0f, 0.5f)), ggo::rgb_32f(1.0f, 0.75f, 0.5f));
  GGO_CHECK_EQ(alpha_blend(ggo::rgb_32f(1.0f, 1.0f, 1.0f), ggo::rgba_32f(1.0f, 0.5f, 0.0f, 0.0f)), ggo::rgb_32f(1.0f, 1.0f, 1.0f));
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(blend, alpha_ya8u)
{
  // Brush alpha is 0.5.
  GGO_CHECK_EQ(alpha_blend(ggo::ya_8u(0x00, 0x00), ggo::ya_8u(0x00, 0x80)), ggo::ya_8u(0x00, 0x80));
  GGO_CHECK_EQ(alpha_blend(ggo::ya_8u(0xff, 0xff), ggo::ya_8u(0x00, 0x80)), ggo::ya_8u(0x7f, 0xff));
  GGO_CHECK_EQ(alpha_blend(ggo::ya_8u(0xff, 0x20), ggo::ya_8u(0x00, 0x80)), ggo::ya_8u(0x1c, 0x90));
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(blend, alpha_ya32f)
{
  // Brush alpha is 1.
  GGO_CHECK_EQ(alpha_blend(ggo::ya_32f(1.0f, 0.0f), ggo::ya_32f(0.0f, 1.0f)), ggo::ya_32f(0.0f, 1.0f));
  GGO_CHECK_EQ(alpha_blend(ggo::ya_32f(1.0f, 0.0f), ggo::ya_32f(0.5f, 1.0f)), ggo::ya_32f(0.5f, 1.0f));
  GGO_CHECK_EQ(alpha_blend(ggo::ya_32f(1.0f, 0.0f), ggo::ya_32f(1.0f, 1.0f)), ggo::ya_32f(1.0f, 1.0f));
  GGO_CHECK_EQ(alpha_blend(ggo::ya_32f(0.5f, 0.5f), ggo::ya_32f(0.0f, 1.0f)), ggo::ya_32f(0.0f, 1.0f));
  GGO_CHECK_EQ(alpha_blend(ggo::ya_32f(0.5f, 0.5f), ggo::ya_32f(0.5f, 1.0f)), ggo::ya_32f(0.5f, 1.0f));
  GGO_CHECK_EQ(alpha_blend(ggo::ya_32f(0.5f, 0.5f), ggo::ya_32f(1.0f, 1.0f)), ggo::ya_32f(1.0f, 1.0f));
  GGO_CHECK_EQ(alpha_blend(ggo::ya_32f(1.0f, 1.0f), ggo::ya_32f(0.0f, 1.0f)), ggo::ya_32f(0.0f, 1.0f));
  GGO_CHECK_EQ(alpha_blend(ggo::ya_32f(1.0f, 1.0f), ggo::ya_32f(0.5f, 1.0f)), ggo::ya_32f(0.5f, 1.0f));
  GGO_CHECK_EQ(alpha_blend(ggo::ya_32f(1.0f, 1.0f), ggo::ya_32f(1.0f, 1.0f)), ggo::ya_32f(1.0f, 1.0f));

  // Brush alpha is 0.
  GGO_CHECK_EQ(alpha_blend(ggo::ya_32f(1.0f, 0.0f), ggo::ya_32f(0.0f, 0.0f)), ggo::ya_32f(0.0f, 0.0f));
  GGO_CHECK_EQ(alpha_blend(ggo::ya_32f(1.0f, 0.0f), ggo::ya_32f(0.5f, 0.0f)), ggo::ya_32f(0.0f, 0.0f));
  GGO_CHECK_EQ(alpha_blend(ggo::ya_32f(1.0f, 0.0f), ggo::ya_32f(1.0f, 0.0f)), ggo::ya_32f(0.0f, 0.0f));
  GGO_CHECK_EQ(alpha_blend(ggo::ya_32f(0.5f, 0.5f), ggo::ya_32f(0.0f, 0.0f)), ggo::ya_32f(0.5f, 0.5f));
  GGO_CHECK_EQ(alpha_blend(ggo::ya_32f(0.5f, 0.5f), ggo::ya_32f(0.5f, 0.0f)), ggo::ya_32f(0.5f, 0.5f));
  GGO_CHECK_EQ(alpha_blend(ggo::ya_32f(0.5f, 0.5f), ggo::ya_32f(1.0f, 0.0f)), ggo::ya_32f(0.5f, 0.5f));
  GGO_CHECK_EQ(alpha_blend(ggo::ya_32f(1.0f, 1.0f), ggo::ya_32f(0.0f, 0.0f)), ggo::ya_32f(1.0f, 1.0f));
  GGO_CHECK_EQ(alpha_blend(ggo::ya_32f(1.0f, 1.0f), ggo::ya_32f(0.5f, 0.0f)), ggo::ya_32f(1.0f, 1.0f));
  GGO_CHECK_EQ(alpha_blend(ggo::ya_32f(1.0f, 1.0f), ggo::ya_32f(1.0f, 0.0f)), ggo::ya_32f(1.0f, 1.0f));

  // Brush alpha is 0.5.
  GGO_CHECK_EQ(alpha_blend(ggo::ya_32f(0.0f, 0.0f), ggo::ya_32f(0.0f, 0.5f)), ggo::ya_32f(0.0f, 0.5f));
  GGO_CHECK_EQ(alpha_blend(ggo::ya_32f(1.0f, 1.0f), ggo::ya_32f(0.0f, 0.5f)), ggo::ya_32f(0.5f, 1.0f));
  GGO_CHECK_EQ(alpha_blend(ggo::ya_32f(1.0f, 0.1f), ggo::ya_32f(0.0f, 0.5f)), ggo::ya_32f(0.05f / 0.55f, 0.55f));
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(blend, alpha_rgba8u)
{
  GGO_CHECK_EQ(ggo::alpha_blend(ggo::rgba_8u{ 0x00, 0x80, 0xff, 0x00 }, ggo::rgba_8u{ 0x00, 0x80, 0xff, 0x00 }), ggo::rgba_8u(0x00, 0x00, 0x00, 0x00));
  GGO_CHECK_EQ(ggo::alpha_blend(ggo::rgba_8u{ 0x00, 0x80, 0xff, 0x00 }, ggo::rgba_8u{ 0x00, 0x80, 0xff, 0xff }), ggo::rgba_8u(0x00, 0x80, 0xff, 0xff));
  GGO_CHECK_EQ(ggo::alpha_blend(ggo::rgba_8u{ 0x80, 0xff, 0x00, 0x20 }, ggo::rgba_8u{ 0x00, 0x80, 0xff, 0xee }), ggo::rgba_8u(0x01, 0x81, 0xfd, 0xf0));
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(blend, alpha_rgba32f)
{
  GGO_CHECK_EQ(ggo::alpha_blend(ggo::rgba_32f{ 0.0f, 0.5f, 1.0f, 0.0f }, ggo::rgba_32f{ 0.0f, 0.5f, 1.0f, 0.0f }), ggo::rgba_32f(0.0f, 0.0f, 0.0f, 0.0f));
  GGO_CHECK_EQ(ggo::alpha_blend(ggo::rgba_32f{ 0.0f, 0.5f, 1.0f, 0.0f }, ggo::rgba_32f{ 0.0f, 0.5f, 1.0f, 1.0f }), ggo::rgba_32f(0.0f, 0.5f, 1.0f, 1.0f));

  const ggo::rgba_32f out = ggo::alpha_blend(ggo::rgba_32f{ 0.5f, 1.0f, 0.0f, 0.1f }, ggo::rgba_32f{ 0.0f, 0.5f, 1.0f, 0.9f });
  GGO_CHECK_FLOAT_EQ(out.r(), (0.9f * 0.0f + 0.01f * 0.5f) / 0.91f);
  GGO_CHECK_FLOAT_EQ(out.g(), (0.9f * 0.5f + 0.01f * 1.0f) / 0.91f);
  GGO_CHECK_FLOAT_EQ(out.b(), (0.9f * 1.0f + 0.01f * 0.0f) / 0.91f);
  GGO_CHECK_FLOAT_EQ(out.a(), 0.91f);
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(blend, opacity_y8u)
{
  {
    const ggo::opacity_blender<uint8_t, ggo::alpha_blender<uint8_t, uint8_t>> blend(1.f);

    GGO_CHECK_EQ(blend(0xff, 0x00), 0x00);
    GGO_CHECK_EQ(blend(0xff, 0x80), 0x80);
    GGO_CHECK_EQ(blend(0xff, 0xff), 0xff);
  }

  {
    const ggo::opacity_blender<uint8_t, ggo::alpha_blender<uint8_t, uint8_t>> blend(0.5f);

    GGO_CHECK_EQ(blend(0x00, 0x00), 0x00);
    GGO_CHECK_EQ(blend(0x00, 0x80), 0x40);
    GGO_CHECK_EQ(blend(0x00, 0xff), 0x80);
  }
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(blend, opacity_rgb8u)
{
  {
    const ggo::opacity_blender<ggo::rgb_8u, ggo::alpha_blender<ggo::rgb_8u, ggo::rgb_8u>> blend(1.f);

    auto c = blend({ 0xff_u8, 0xff_u8, 0xff_u8 }, ggo::rgb_8u{ 0x00_u8, 0x80_u8, 0xff_u8 });
    GGO_CHECK_EQ(int(c.r()), 0x00);
    GGO_CHECK_EQ(int(c.g()), 0x80);
    GGO_CHECK_EQ(int(c.b()), 0xff);
  }

  {
    const ggo::opacity_blender<ggo::rgb_8u, ggo::alpha_blender<ggo::rgb_8u, ggo::rgb_8u>> blend(0.5f);

    auto c = blend({ 0x00_u8, 0x00_u8, 0x00_u8 }, ggo::rgb_8u{ 0x00_u8, 0x80_u8, 0xff_u8 });
    GGO_CHECK_EQ(int(c.r()), 0x00);
    GGO_CHECK_EQ(int(c.g()), 0x40);
    GGO_CHECK_EQ(int(c.b()), 0x80);
  }
}

