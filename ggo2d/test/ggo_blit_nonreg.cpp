#include <ggo_nonreg.h>
#include <ggo_array.h>
#include <ggo_blit.h>

/////////////////////////////////////////////////////////////////////
GGO_TEST(blit, clipping)
{
  ggo::array<uint8_t, 2> dst(6, 4, 0);

  const ggo::array<uint8_t, 2> src({
    { 1, 2, 3 },
    { 4, 5, 6 } });

  const ggo::array<uint8_t, 2> expected(dst.width(), dst.height(), 0);

  ggo::blit<ggo::y_8u_yu, ggo::y_8u_yu>(
    src.data(), src.width(), src.height(), src.width(),
    dst.data(), dst.width(), dst.height(), dst.width(), -3, 0);
  GGO_CHECK(dst == expected);

  ggo::blit<ggo::y_8u_yu, ggo::y_8u_yu>(
    src.data(), src.width(), src.height(), src.width(),
    dst.data(), dst.width(), dst.height(), dst.width(), 6, 0);
  GGO_CHECK(dst == expected);

  ggo::blit<ggo::y_8u_yu, ggo::y_8u_yu>(
    src.data(), src.width(), src.height(), src.width(),
    dst.data(), dst.width(), dst.height(), dst.width(), 0, -2);
  GGO_CHECK(dst == expected);

  ggo::blit<ggo::y_8u_yu, ggo::y_8u_yu>(
    src.data(), src.width(), src.height(), src.width(),
    dst.data(), dst.width(), dst.height(), dst.width(), 0, 5);
  GGO_CHECK(dst == expected);
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(blit, y_up_fit)
{
  ggo::array<uint8_t, 2> dst(6, 5, 0);

  const ggo::array<uint8_t, 2> src({
    { 1, 2, 3 },
    { 4, 5, 6 } });

  ggo::blit<ggo::y_8u_yu, ggo::y_8u_yu>(
    src.data(), src.width(), src.height(), src.width(),
    dst.data(), dst.width(), dst.height(), dst.width(), 2, 1);

  const ggo::array<uint8_t, 2> expected({
    { 0, 0, 0, 0, 0, 0 },
    { 0, 0, 1, 2, 3, 0 },
    { 0, 0, 4, 5, 6, 0 },
    { 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0 } });

  GGO_CHECK(dst == expected);
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(blit, y_up_bottom_left)
{
  ggo::array<uint8_t, 2> dst(6, 5, 0);

  const ggo::array<uint8_t, 2> src({
    { 1, 2, 3 },
    { 4, 5, 6 } });

  ggo::blit<ggo::y_8u_yu, ggo::y_8u_yu>(
    src.data(), src.width(), src.height(), src.width(),
    dst.data(), dst.width(), dst.height(), dst.width(), -2, -1);

  const ggo::array<uint8_t, 2> expected({
    { 6, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0 } });

  GGO_CHECK(dst == expected);
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(blit, y_up_top_right)
{
  ggo::array<uint8_t, 2> dst(6, 5, 0);

  const ggo::array<uint8_t, 2> src({
    { 1, 2, 3 },
    { 4, 5, 6 } });

  ggo::blit<ggo::y_8u_yu, ggo::y_8u_yu>(
    src.data(), src.width(), src.height(), src.width(),
    dst.data(), dst.width(), dst.height(), dst.width(), 4, 4);

  const ggo::array<uint8_t, 2> expected({
    { 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 1, 2 } });

  GGO_CHECK(dst == expected);
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(blit, y_up_overlap)
{
  ggo::array<uint8_t, 2> dst(3, 2);

  const ggo::array<uint8_t, 2> src({
    { 11, 12, 13, 14, 15, 16 },
    { 21, 22, 23, 24, 25, 26 },
    { 31, 32, 33, 34, 35, 36 },
    { 41, 42, 43, 44, 45, 46 } });

  {
    dst.fill(0);

    ggo::blit<ggo::y_8u_yu, ggo::y_8u_yu>(
      src.data(), src.width(), src.height(), src.width(),
      dst.data(), dst.width(), dst.height(), dst.width(), -2, -1);

    const ggo::array<uint8_t, 2> expected({
      { 23, 24, 25 },
      { 33, 34, 35 } });

    GGO_CHECK(dst == expected);
  }

  {
    dst.fill(0);

    ggo::blit<ggo::y_8u_yu, ggo::y_8u_yu>(
      src.data(), src.width(), src.height(), src.width(),
      dst.data(), dst.width(), dst.height(), dst.width(), -5, -1);

    const ggo::array<uint8_t, 2> expected({
      { 26, 0, 0 },
      { 36, 0, 0 } });

    GGO_CHECK(dst == expected);
  }
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(blit, y_down_fit)
{
  ggo::array<uint8_t, 2> dst(6, 5, 0);

  const ggo::array<uint8_t, 2> src({
    { 1, 2, 3 },
    { 4, 5, 6 } });

  ggo::blit<ggo::y_8u_yd, ggo::y_8u_yd>(
    src.data(), src.width(), src.height(), src.width(),
    dst.data(), dst.width(), dst.height(), dst.width(), 2, 1);

  const ggo::array<uint8_t, 2> expected({
    { 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0 },
    { 0, 0, 1, 2, 3, 0 },
    { 0, 0, 4, 5, 6, 0 },
    { 0, 0, 0, 0, 0, 0 } });

  GGO_CHECK(dst == expected);
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(blit, y_down_bottom_left)
{
  ggo::array<uint8_t, 2> dst(6, 5, 0);

  const ggo::array<uint8_t, 2> src({
    { 1, 2, 3 },
    { 4, 5, 6 } });

  ggo::blit<ggo::y_8u_yd, ggo::y_8u_yd>(
    src.data(), src.width(), src.height(), src.width(),
    dst.data(), dst.width(), dst.height(), dst.width(), -2, -1);

  const ggo::array<uint8_t, 2> expected({
    { 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0 },
    { 3, 0, 0, 0, 0, 0 } });

  GGO_CHECK(dst == expected);
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(blit, y_down_top_right)
{
  ggo::array<uint8_t, 2> dst(6, 5, 0);

  const ggo::array<uint8_t, 2> src({
    { 1, 2, 3 },
    { 4, 5, 6 } });

  ggo::blit<ggo::y_8u_yd, ggo::y_8u_yd>(
    src.data(), src.width(), src.height(), src.width(),
    dst.data(), dst.width(), dst.height(), dst.width(), 4, 4);

  const ggo::array<uint8_t, 2> expected({
    { 0, 0, 0, 0, 4, 5 },
    { 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0 } });

  GGO_CHECK(dst == expected);
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(blit, y_down_overlap)
{
  ggo::array<uint8_t, 2> dst(3, 2);

  const ggo::array<uint8_t, 2> src({
    { 11, 12, 13, 14, 15, 16 },
    { 21, 22, 23, 24, 25, 26 },
    { 31, 32, 33, 34, 35, 36 },
    { 41, 42, 43, 44, 45, 46 } });

  {
    dst.fill(0);

    ggo::blit<ggo::y_8u_yd, ggo::y_8u_yd>(
      src.data(), src.width(), src.height(), src.width(),
      dst.data(), dst.width(), dst.height(), dst.width(), -2, -1);

    const ggo::array<uint8_t, 2> expected({
      { 23, 24, 25 },
      { 33, 34, 35 } });

    GGO_CHECK(dst == expected);
  }

  {
    dst.fill(0);

    ggo::blit<ggo::y_8u_yu, ggo::y_8u_yu>(
      src.data(), src.width(), src.height(), src.width(),
      dst.data(), dst.width(), dst.height(), dst.width(), -5, -1);

    const ggo::array<uint8_t, 2> expected({
      { 26, 0, 0 },
      { 36, 0, 0 } });

    GGO_CHECK(dst == expected);
  }
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(blit, dst_down_src_up_fit)
{
  ggo::array<uint8_t, 2> dst(6, 5, 0);

  const ggo::array<uint8_t, 2> src({
    { 1, 2, 3 },
    { 4, 5, 6 } });

  ggo::blit<ggo::y_8u_yu, ggo::y_8u_yd>(
    src.data(), src.width(), src.height(), src.width(),
    dst.data(), dst.width(), dst.height(), dst.width(), 2, 1);

  const ggo::array<uint8_t, 2> expected({
    { 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0 },
    { 0, 0, 4, 5, 6, 0 },
    { 0, 0, 1, 2, 3, 0 },
    { 0, 0, 0, 0, 0, 0 } });

  GGO_CHECK(dst == expected);
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(blit, dst_up_src_down_fit)
{
  ggo::array<uint8_t, 2> dst(6, 5 , 0);

  const ggo::array<uint8_t, 2> src({
    { 1, 2, 3 },
    { 4, 5, 6 } });

  ggo::blit<ggo::y_8u_yd, ggo::y_8u_yu>(
    src.data(), src.width(), src.height(), src.width(),
    dst.data(), dst.width(), dst.height(), dst.width(), 2, 1);

  const ggo::array<uint8_t, 2> expected({
    { 0, 0, 0, 0, 0, 0 },
    { 0, 0, 4, 5, 6, 0 },
    { 0, 0, 1, 2, 3, 0 },
    { 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0 } });

  GGO_CHECK(dst == expected);
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(blit, brush_rgba8_background_rgb8)
{
  ggo::array<uint8_t, 2> dst(9, 5, 0);

  const ggo::array<uint8_t, 2> src({
    { 0xff, 0x80, 0x40, 0xff, 0xff, 0x80, 0x40, 0x80 },
    { 0xff, 0x80, 0x40, 0x40, 0xff, 0x80, 0x40, 0x00 } });

  ggo::blit<ggo::rgba_8u_yd, ggo::rgb_8u_yd>(
    src.data(), 2, 2, 8,
    dst.data(), 3, 5, 9, 1, 2);

  const ggo::array<uint8_t, 2> expected({
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
    { 0x00, 0x00, 0x00, 0xff, 0x80, 0x40, 0x80, 0x40, 0x20 },
    { 0x00, 0x00, 0x00, 0x40, 0x20, 0x10, 0x00, 0x00, 0x00 },
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 } });

  GGO_CHECK(dst == expected);
}



