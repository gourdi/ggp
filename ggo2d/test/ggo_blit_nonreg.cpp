#include "ggo_2d_nonreg.h"
#include <kernel/memory/ggo_array.h>
#include <2d/processing/ggo_blit.h>

/////////////////////////////////////////////////////////////////////
GGO_TEST(blit, clipping)
{
  auto dst = make_image_t<ggo::pixel_type::y_8u, ggo::lines_order::up>(6, 4, 0);
  auto src = make_image_t<ggo::pixel_type::y_8u, ggo::lines_order::up>({
    { 1, 2, 3 },
    { 4, 5, 6 } });

  const ggo::array2d_8u expected(dst.width(), dst.height(), 0);

  ggo::blit(src, dst, -3, 0);
  GGO_CHECK_IMG(dst, expected);

  ggo::blit(src, dst, 6, 0);
  GGO_CHECK_IMG(dst, expected);

  ggo::blit(src, dst, 0, -2);
  GGO_CHECK_IMG(dst, expected);

  ggo::blit(src, dst, 0, 5);
  GGO_CHECK_IMG(dst, expected);
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(blit, y_up_fit)
{
  auto dst = make_image_t<ggo::pixel_type::y_8u, ggo::lines_order::up>(6, 5, 0);
  auto src = make_image_t<ggo::pixel_type::y_8u, ggo::lines_order::up>({
    { 1, 2, 3 },
    { 4, 5, 6 } });

  ggo::blit(src, dst, 2, 1);

  const ggo::array2d_8u expected({
    { 0, 0, 0, 0, 0, 0 },
    { 0, 0, 1, 2, 3, 0 },
    { 0, 0, 4, 5, 6, 0 },
    { 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0 } });

  GGO_CHECK_IMG(dst, expected);
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(blit, y_down_fit)
{
  auto dst = make_image_t<ggo::pixel_type::y_8u, ggo::lines_order::down>(6, 5, 0);
  auto src = make_image_t<ggo::pixel_type::y_8u, ggo::lines_order::down>({
    { 1, 2, 3 },
    { 4, 5, 6 } });

  ggo::blit(src, dst, 2, 1);

  const ggo::array2d_8u expected({
    { 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0 },
    { 0, 0, 1, 2, 3, 0 },
    { 0, 0, 4, 5, 6, 0 },
    { 0, 0, 0, 0, 0, 0 } });

  GGO_CHECK_IMG(dst, expected);
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(blit, y_up_bottom_left)
{
  auto dst = make_image_t<ggo::pixel_type::y_8u, ggo::lines_order::up>(6, 5, 0);
  auto src = make_image_t<ggo::pixel_type::y_8u, ggo::lines_order::up>({
    { 1, 2, 3 },
    { 4, 5, 6 } });

  ggo::blit(src, dst, -2, -1);

  const ggo::array2d_8u expected({
    { 6, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0 } });

  GGO_CHECK_IMG(dst, expected);
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(blit, y_down_bottom_left)
{
  auto dst = make_image_t<ggo::pixel_type::y_8u, ggo::lines_order::down>(6, 5, 0);
  auto src = make_image_t<ggo::pixel_type::y_8u, ggo::lines_order::down>({
    { 1, 2, 3 },
    { 4, 5, 6 } });

  ggo::blit(src, dst, -2, -1);

  const ggo::array2d_8u expected({
    { 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0 },
    { 3, 0, 0, 0, 0, 0 } });

  GGO_CHECK_IMG(dst, expected);
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(blit, y_up_top_right)
{
  auto dst = make_image_t<ggo::pixel_type::y_8u, ggo::lines_order::up>(6, 5, 0);
  auto src = make_image_t<ggo::pixel_type::y_8u, ggo::lines_order::up>({
    { 1, 2, 3 },
    { 4, 5, 6 } });

  ggo::blit(src, dst, 4, 4);

  const ggo::array2d_8u expected({
    { 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 1, 2 } });

  GGO_CHECK_IMG(dst, expected);
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(blit, y_down_top_right)
{
  auto dst = make_image_t<ggo::pixel_type::y_8u, ggo::lines_order::down>(6, 5, 0);
  auto src = make_image_t<ggo::pixel_type::y_8u, ggo::lines_order::down>({
    { 1, 2, 3 },
    { 4, 5, 6 } });

  ggo::blit(src, dst, 4, 4);

  const ggo::array2d_8u expected({
    { 0, 0, 0, 0, 4, 5 },
    { 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0 } });

  GGO_CHECK_IMG(dst, expected);
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(blit, y_up_overlap)
{
  auto src = make_image_t<ggo::pixel_type::y_8u, ggo::lines_order::up>({
    { 11, 12, 13, 14, 15, 16 },
    { 21, 22, 23, 24, 25, 26 },
    { 31, 32, 33, 34, 35, 36 },
    { 41, 42, 43, 44, 45, 46 } });

  {
    auto dst = make_image_t<ggo::pixel_type::y_8u, ggo::lines_order::up>(3, 2, 0);

    ggo::blit(src, dst, -2, -1);

    const ggo::array2d_8u expected({
      { 23, 24, 25 },
      { 33, 34, 35 } });

    GGO_CHECK_IMG(dst, expected);
  }

  {
    auto dst = make_image_t<ggo::pixel_type::y_8u, ggo::lines_order::up>(3, 2, 0);

    ggo::blit(src, dst, -5, -1);

    const ggo::array2d_8u expected({
      { 26, 0, 0 },
      { 36, 0, 0 } });

    GGO_CHECK_IMG(dst, expected);
  }
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(blit, y_down_overlap)
{
  ggo::array2d_8u dst(3, 2);

  auto src = make_image_t<ggo::pixel_type::y_8u, ggo::lines_order::down>({
    { 11, 12, 13, 14, 15, 16 },
    { 21, 22, 23, 24, 25, 26 },
    { 31, 32, 33, 34, 35, 36 },
    { 41, 42, 43, 44, 45, 46 } });

  {
    auto dst = make_image_t<ggo::pixel_type::y_8u, ggo::lines_order::down>(3, 2, 0);

    ggo::blit(src, dst, -2, -1);

    const ggo::array2d_8u expected({
      { 23, 24, 25 },
      { 33, 34, 35 } });

    GGO_CHECK_IMG(dst, expected);
  }

  {
    auto dst = make_image_t<ggo::pixel_type::y_8u, ggo::lines_order::down>(3, 2, 0);

    ggo::blit(src, dst, -5, -1);

    const ggo::array2d_8u expected({
      { 26, 0, 0 },
      { 36, 0, 0 } });

    GGO_CHECK_IMG(dst, expected);
  }
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(blit, dst_down_src_up_fit)
{
  auto dst = make_image_t<ggo::pixel_type::y_8u, ggo::lines_order::up>(6, 5, 0);
  auto src = make_image_t<ggo::pixel_type::y_8u, ggo::lines_order::up>({
    { 1, 2, 3 },
    { 4, 5, 6 } });

  ggo::blit(src, dst, 2, 1);

  const ggo::array2d_8u expected({
    { 0, 0, 0, 0, 0, 0 },
    { 0, 0, 1, 2, 3, 0 },
    { 0, 0, 4, 5, 6, 0 },
    { 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0 } });

  GGO_CHECK_IMG(dst, expected);
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(blit, dst_up_src_down_fit)
{
  auto dst = make_image_t<ggo::pixel_type::y_8u, ggo::lines_order::down>(6, 5 , 0);
  auto src = make_image_t<ggo::pixel_type::y_8u, ggo::lines_order::up>({
     { 1, 2, 3 },
     { 4, 5, 6 } });

  ggo::blit(src, dst, 2, 1);

  const ggo::array2d_8u expected({
    { 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0 },
    { 0, 0, 4, 5, 6, 0 },
    { 0, 0, 1, 2, 3, 0 },
    { 0, 0, 0, 0, 0, 0 } });

  GGO_CHECK_IMG(dst, expected);
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(blit, brush_rgba8_background_rgb8)
{
  auto dst = make_image_t<ggo::pixel_type::rgb_8u, ggo::lines_order::down>(3, 5, 0);
  auto src = make_image_t<ggo::pixel_type::rgba_8u, ggo::lines_order::down>({
    { { 0xff, 0x80, 0x40, 0xff }, { 0xff, 0x80, 0x40, 0x80 } },
    { { 0xff, 0x80, 0x40, 0x40 }, { 0xff, 0x80, 0x40, 0x00 } } });

  ggo::blit(src, dst, 1, 2);

  const ggo::array<ggo::rgb_8u, 2> expected({
    { { 0x00, 0x00, 0x00 }, { 0x00, 0x00, 0x00 }, { 0x00, 0x00, 0x00 } },
    { { 0x00, 0x00, 0x00 }, { 0xff, 0x80, 0x40 }, { 0x80, 0x40, 0x20 } },
    { { 0x00, 0x00, 0x00 }, { 0x40, 0x20, 0x10 }, { 0x00, 0x00, 0x00 } },
    { { 0x00, 0x00, 0x00 }, { 0x00, 0x00, 0x00 }, { 0x00, 0x00, 0x00 } },
    { { 0x00, 0x00, 0x00 }, { 0x00, 0x00, 0x00 }, { 0x00, 0x00, 0x00 } } });

  GGO_CHECK_IMG(dst, expected);
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(blit, dynamic_image)
{
  auto dst = make_image<ggo::pixel_type::y_8u, ggo::lines_order::down>(6, 5, 0);
  auto src = make_image<ggo::pixel_type::y_8u, ggo::lines_order::down>({
    { 1, 2, 3 },
    { 4, 5, 6 } });

  ggo::blit(src, dst, 2, 1);

  const ggo::array2d_8u expected({
    { 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0 },
    { 0, 0, 1, 2, 3, 0 },
    { 0, 0, 4, 5, 6, 0 },
    { 0, 0, 0, 0, 0, 0 } });

  GGO_CHECK_IMG(dst, expected);
}
