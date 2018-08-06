#include <kernel/nonreg/ggo_nonreg.h>
#include <kernel/ggo_kernel.h>
#include <kernel/math/interpolation/ggo_curve.h>
#include <2d/ggo_color.h>

////////////////////////////////////////////////////////////////////
GGO_TEST(color, operators)
{
  ggo::rgb_32f c1(0.25f, 0.5f, 0.75f);
  ggo::rgb_32f c2(0.01f, 0.1f, 0.01f);
  
  auto c3 = c1 + c2;
  GGO_CHECK_FLOAT_EQ(c3._r, 0.26f);
  GGO_CHECK_FLOAT_EQ(c3._g, 0.6f);
  GGO_CHECK_FLOAT_EQ(c3._b, 0.76f);
}

////////////////////////////////////////////////////////////////////
GGO_TEST(color, conversions)
{
  // y 16u => y 8u
  {
    GGO_CHECK_EQ(ggo::convert_color_to<uint16_t>(uint8_t(0xff)), 0xffff);
    GGO_CHECK_EQ(ggo::convert_color_to<uint16_t>(uint8_t(0x80)), 0x8080);
    GGO_CHECK_EQ(ggo::convert_color_to<uint16_t>(uint8_t(0x00)), 0x0000);
  }

  // rgb 8u => rgb 16u
  {
    ggo::rgb_16u c = ggo::convert_color_to<ggo::rgb_16u>(ggo::rgb_8u(0xff, 0x80, 0x00));
    GGO_CHECK_EQ(c._r, 0xffff);
    GGO_CHECK_EQ(c._g, 0x8080);
    GGO_CHECK_EQ(c._b, 0x0000);
  }
}

////////////////////////////////////////////////////////////////////
GGO_TEST(color, linear_curve)
{
  ggo::linear_curve<float, ggo::rgb_32f> curve;
  curve.push_point(0, ggo::rgb_32f(1.f, 0.f, 0.f));
  curve.push_point(1, ggo::rgb_32f(0.f, 1.f, 1.f));
  ggo::rgb_32f c = curve.evaluate(0.25f);

  GGO_CHECK_FLOAT_EQ(c._r, 0.75f);
  GGO_CHECK_FLOAT_EQ(c._g, 0.25f);
  GGO_CHECK_FLOAT_EQ(c._b, 0.25f);
}

////////////////////////////////////////////////////////////////////
GGO_TEST(color, hex)
{
  ggo::rgb_8u c(0xF0, 0x42, 0x0F);
  const std::string s = to_hex(c);
  GGO_CHECK_EQ(s, "F0420F");
  
  ggo::rgb_8u c2(ggo::from_hex(s));
  GGO_CHECK_EQ(c, c2);
}

////////////////////////////////////////////////////////////////////
GGO_TEST(color, linear_interpolation_y8u)
{
  // 1 sample.
  GGO_CHECK_EQ(int(ggo::linerp(0xff_u8, 0x00_u8, ggo::log2_fract<0>(0))), int(0x00));
  GGO_CHECK_EQ(int(ggo::linerp(0xff_u8, 0x00_u8, ggo::log2_fract<0>(1))), int(0xff));

  // 4 samples
  GGO_CHECK_EQ(int(ggo::linerp(0xff_u8, 0x00_u8, ggo::log2_fract<2>(0))), int(0x00));
  GGO_CHECK_EQ(int(ggo::linerp(0xff_u8, 0x00_u8, ggo::log2_fract<2>(1))), int(0x40));
  GGO_CHECK_EQ(int(ggo::linerp(0xff_u8, 0x00_u8, ggo::log2_fract<2>(2))), int(0x80));
  GGO_CHECK_EQ(int(ggo::linerp(0xff_u8, 0x00_u8, ggo::log2_fract<2>(3))), int(0xbf));
  GGO_CHECK_EQ(int(ggo::linerp(0xff_u8, 0x00_u8, ggo::log2_fract<2>(4))), int(0xff));

  // 16 samples
  GGO_CHECK_EQ(int(ggo::linerp(0xff_u8, 0x00_u8, ggo::log2_fract<4>(0))), int(0x00));
  GGO_CHECK_EQ(int(ggo::linerp(0xff_u8, 0x00_u8, ggo::log2_fract<4>(1))), int(0x10));
  GGO_CHECK_EQ(int(ggo::linerp(0xff_u8, 0x00_u8, ggo::log2_fract<4>(7))), int(0x70));
  GGO_CHECK_EQ(int(ggo::linerp(0xff_u8, 0x00_u8, ggo::log2_fract<4>(8))), int(0x80));
  GGO_CHECK_EQ(int(ggo::linerp(0xff_u8, 0x00_u8, ggo::log2_fract<4>(9))), int(0x8f));
  GGO_CHECK_EQ(int(ggo::linerp(0xff_u8, 0x00_u8, ggo::log2_fract<4>(15))), int(0xef));
  GGO_CHECK_EQ(int(ggo::linerp(0xff_u8, 0x00_u8, ggo::log2_fract<4>(16))), int(0xff));

  // 64 samples
  GGO_CHECK_EQ(int(ggo::linerp(0xff_u8, 0x00_u8, ggo::log2_fract<6>(0))), int(0x00));
  GGO_CHECK_EQ(int(ggo::linerp(0xff_u8, 0x00_u8, ggo::log2_fract<6>(1))), int(0x04));
  GGO_CHECK_EQ(int(ggo::linerp(0xff_u8, 0x00_u8, ggo::log2_fract<6>(31))), int(0x7c));
  GGO_CHECK_EQ(int(ggo::linerp(0xff_u8, 0x00_u8, ggo::log2_fract<6>(32))), int(0x80));
  GGO_CHECK_EQ(int(ggo::linerp(0xff_u8, 0x00_u8, ggo::log2_fract<6>(33))), int(0x83));
  GGO_CHECK_EQ(int(ggo::linerp(0xff_u8, 0x00_u8, ggo::log2_fract<6>(63))), int(0xfb));
  GGO_CHECK_EQ(int(ggo::linerp(0xff_u8, 0x00_u8, ggo::log2_fract<6>(64))), int(0xff));

  // 256 samples
  GGO_CHECK_EQ(int(ggo::linerp(0xff_u8, 0x00_u8, ggo::log2_fract<8>(0))), int(0x00));
  GGO_CHECK_EQ(int(ggo::linerp(0xff_u8, 0x00_u8, ggo::log2_fract<8>(1))), int(0x01));
  GGO_CHECK_EQ(int(ggo::linerp(0xff_u8, 0x00_u8, ggo::log2_fract<8>(127))), int(0x7f));
  GGO_CHECK_EQ(int(ggo::linerp(0xff_u8, 0x00_u8, ggo::log2_fract<8>(128))), int(0x80));
  GGO_CHECK_EQ(int(ggo::linerp(0xff_u8, 0x00_u8, ggo::log2_fract<8>(129))), int(0x80));
  GGO_CHECK_EQ(int(ggo::linerp(0xff_u8, 0x00_u8, ggo::log2_fract<8>(255))), int(0xfe));
  GGO_CHECK_EQ(int(ggo::linerp(0xff_u8, 0x00_u8, ggo::log2_fract<8>(256))), int(0xff));
}

////////////////////////////////////////////////////////////////////
GGO_TEST(color, linear_interpolation_rgba8u)
{
  auto c1 = ggo::linerp(ggo::rgba_8u(0xff, 0x80, 0x40, 0x80), ggo::rgba_8u(0x40, 0xff, 0x80, 0x40), ggo::log2_fract<2>(4));
  GGO_CHECK_EQ(c1._r, 0xcb);
  GGO_CHECK_EQ(c1._g, 0xa3);
  GGO_CHECK_EQ(c1._b, 0x51);
  GGO_CHECK_EQ(c1._a, 0xb0);

  auto c2 = ggo::linerp(ggo::rgba_8u(0x00, 0x80, 0xff, 0xff), ggo::rgba_8u(0x00, 0x00, 0x00, 0x00), ggo::log2_fract<8>(1));
  GGO_CHECK_EQ(c2._r, 0x00);
  GGO_CHECK_EQ(c2._g, 0x80);
  GGO_CHECK_EQ(c2._b, 0xff);
  GGO_CHECK_EQ(c2._a, 0x01);

  auto c3 = ggo::linerp(ggo::rgba_8u(0x00, 0x80, 0xff, 0xff), ggo::rgba_8u(0xff, 0xff, 0xff, 0x00), ggo::log2_fract<8>(1));
  GGO_CHECK_EQ(c3._r, 0x00);
  GGO_CHECK_EQ(c3._g, 0x80);
  GGO_CHECK_EQ(c3._b, 0xff);
  GGO_CHECK_EQ(c3._a, 0x01);

  auto c4 = ggo::linerp(ggo::rgba_8u(0x00, 0x80, 0xff, 0x01), ggo::rgba_8u(0x00, 0x00, 0x00, 0x00), ggo::log2_fract<2>(4));
  GGO_CHECK_EQ(c4._r, 0x00);
  GGO_CHECK_EQ(c4._g, 0x80);
  GGO_CHECK_EQ(c4._b, 0xff);
  GGO_CHECK_EQ(c4._a, 0x01);
}
