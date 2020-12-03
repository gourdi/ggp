#include "ggo_2d_nonreg.h"
#include <kernel/memory/ggo_array.h>
#include <2d/processing/ggo_blit.h>

/////////////////////////////////////////////////////////////////////
GGO_TEST(blit, clipping)
{
  auto dst = make_image_t<ggo::pixel_type::y_8u>({ 6, 4 }, 0);
  const auto src = make_image_t<ggo::pixel_type::y_8u>({ 3, 2 }, {
    1, 2, 3,
    4, 5, 6 });

  const auto expected = make_image_t<ggo::pixel_type::y_8u>({ 6, 4 }, 0);

  ggo::blit(dst, src, { -3, 0 });
  GGO_CHECK_IMG(dst, expected);

  ggo::blit(dst, src, { 6, 0 });
  GGO_CHECK_IMG(dst, expected);

  ggo::blit(dst, src, { 0, -2 });
  GGO_CHECK_IMG(dst, expected);

  ggo::blit(dst, src, { 0, 5 });
  GGO_CHECK_IMG(dst, expected);
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(blit, rows_up_fit)
{
  auto dst = make_image_t<ggo::pixel_type::y_8u>({ 6, 5 }, 0);
  const auto src = make_image_t<ggo::pixel_type::y_8u>({ 3, 2 }, {
    1, 2, 3,
    4, 5, 6 });

  ggo::blit(dst, src, { 2, 1 });

  GGO_CHECK_PIXELS(dst, {
    0, 0, 0, 0, 0, 0,
    0, 0, 1, 2, 3, 0,
    0, 0, 4, 5, 6, 0,
    0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0 });
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(blit, rows_down_fit)
{
  auto dst = make_image_rows_down_t<ggo::pixel_type::y_8u>({ 6, 5 }, 0);
  const auto src = make_image_rows_down_t<ggo::pixel_type::y_8u>({ 3, 2 }, {
    1, 2, 3,
    4, 5, 6 });

  ggo::blit(dst, src, { 2, 1 });

  GGO_CHECK_PIXELS(dst, {
    0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0,
    0, 0, 1, 2, 3, 0,
    0, 0, 4, 5, 6, 0,
    0, 0, 0, 0, 0, 0 });
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(blit, rows_up_bottom_left)
{
  auto dst = make_image_t<ggo::pixel_type::y_8u>({ 6, 5 }, 0);
  const auto src = make_image_t<ggo::pixel_type::y_8u>({ 3, 2 }, {
    1, 2, 3,
    4, 5, 6 });

  ggo::blit(dst, src, { -2, -1 });

  GGO_CHECK_PIXELS(dst, {
    6, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0 });
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(blit, y_down_bottom_left)
{
  auto dst = make_image_rows_down_t<ggo::pixel_type::y_8u>({ 6, 5 }, 0);
  const auto src = make_image_rows_down_t<ggo::pixel_type::y_8u>({ 3, 2 }, {
    1, 2, 3,
    4, 5, 6 });

  ggo::blit(dst, src, { -2, -1 });

  GGO_CHECK_PIXELS(dst, {
    0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0,
    3, 0, 0, 0, 0, 0 });
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(blit, rows_up_top_right)
{
  auto dst = make_image_t<ggo::pixel_type::y_8u>({ 6, 5 }, 0);
  const auto src = make_image_t<ggo::pixel_type::y_8u>({ 3, 2 }, {
    1, 2, 3,
    4, 5, 6 });

  ggo::blit(dst, src, { 4, 4 });

  GGO_CHECK_PIXELS(dst, {
    0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 1, 2 });
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(blit, rows_down_top_right)
{
  auto dst = make_image_rows_down_t<ggo::pixel_type::y_8u>({ 6, 5 }, 0);
  const auto src = make_image_rows_down_t<ggo::pixel_type::y_8u>({ 3, 2 }, {
    1, 2, 3,
    4, 5, 6 });

  ggo::blit(dst, src, { 4, 4 });

  GGO_CHECK_PIXELS(dst, {
    0, 0, 0, 0, 4, 5,
    0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0 });
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(blit, rows_up_overlap)
{
  const auto src = make_image_t<ggo::pixel_type::y_8u>({ 6, 4 }, {
    11, 12, 13, 14, 15, 16,
    21, 22, 23, 24, 25, 26,
    31, 32, 33, 34, 35, 36,
    41, 42, 43, 44, 45, 46 });

  {
    auto dst = make_image_t<ggo::pixel_type::y_8u>({ 3, 2 }, 0);

    ggo::blit(dst, src, { -2, -1 });

    GGO_CHECK_PIXELS(dst, {
      23, 24, 25,
      33, 34, 35 });
  }

  {
    auto dst = make_image_t<ggo::pixel_type::y_8u>({ 3, 2 }, 0);

    ggo::blit(dst, src, { -5, -1 });

    GGO_CHECK_PIXELS(dst, {
      26, 0, 0,
      36, 0, 0 });
  }
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(blit, rows_down_overlap)
{
  const auto src = make_image_rows_down_t<ggo::pixel_type::y_8u>({ 6, 4 }, {
    11, 12, 13, 14, 15, 16,
    21, 22, 23, 24, 25, 26,
    31, 32, 33, 34, 35, 36,
    41, 42, 43, 44, 45, 46 });

  {
    auto dst = make_image_rows_down_t<ggo::pixel_type::y_8u>({ 3, 2 }, 0);

    ggo::blit(dst, src, { -2, -1 });

    GGO_CHECK_PIXELS(dst, {
      23, 24, 25,
      33, 34, 35 });
  }

  {
    auto dst = make_image_rows_down_t<ggo::pixel_type::y_8u>({ 3, 2 }, 0);

    ggo::blit(dst, src, { -5, -1 });

    GGO_CHECK_PIXELS(dst, {
      26, 0, 0,
      36, 0, 0 });
  }
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(blit, dst_down_src_up_fit)
{
  auto dst = make_image_rows_down_t<ggo::pixel_type::y_8u>({ 6, 5 }, 0);
  const auto src = make_image_t<ggo::pixel_type::y_8u>({ 3, 2 }, {
    1, 2, 3,
    4, 5, 6 });

  ggo::blit(dst, src, { 2, 1 });

  GGO_CHECK_PIXELS(dst, {
    0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0,
    0, 0, 4, 5, 6, 0,
    0, 0, 1, 2, 3, 0,
    0, 0, 0, 0, 0, 0 });
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(blit, dst_up_src_down_fit)
{
  auto dst = make_image_t<ggo::pixel_type::y_8u>({ 6, 5 }, 0);
  const auto src = make_image_rows_down_t<ggo::pixel_type::y_8u>({ 3, 2 }, {
     1, 2, 3,
     4, 5, 6 });

  ggo::blit(dst, src, { 2, 1 });

  GGO_CHECK_PIXELS(dst, {
    0, 0, 0, 0, 0, 0,
    0, 0, 4, 5, 6, 0,
    0, 0, 1, 2, 3, 0,
    0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0 });
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(blit, brush_rgba8_background_rgb8)
{
  auto dst = make_image_rows_down_t<ggo::pixel_type::rgb_8u>({ 3, 5 }, 0);
  auto src = make_image_rows_down_t<ggo::pixel_type::rgba_8u>({ 2, 2 }, {
    { 0xff, 0x80, 0x40, 0xff }, { 0xff, 0x80, 0x40, 0x80 },
    { 0xff, 0x80, 0x40, 0x40 }, { 0xff, 0x80, 0x40, 0x00 } });

  ggo::blit(dst, src, { 1, 2 });

  GGO_CHECK_PIXELS(dst, {
    { 0x00, 0x00, 0x00 }, { 0x00, 0x00, 0x00 }, { 0x00, 0x00, 0x00 },
    { 0x00, 0x00, 0x00 }, { 0xff, 0x80, 0x40 }, { 0x80, 0x40, 0x20 },
    { 0x00, 0x00, 0x00 }, { 0x40, 0x20, 0x10 }, { 0x00, 0x00, 0x00 },
    { 0x00, 0x00, 0x00 }, { 0x00, 0x00, 0x00 }, { 0x00, 0x00, 0x00 },
    { 0x00, 0x00, 0x00 }, { 0x00, 0x00, 0x00 }, { 0x00, 0x00, 0x00 } });
}
#if 0
/////////////////////////////////////////////////////////////////////
GGO_TEST(blit, dynamic_image)
{
  auto dst = make_image_rows_down<ggo::pixel_type::rgb_8u>({ 3, 5 }, 0);
  auto src = make_image_rows_down<ggo::pixel_type::rgba_8u>({ 2, 2 }, {
    { 0xff, 0x80, 0x40, 0xff }, { 0xff, 0x80, 0x40, 0x80 },
    { 0xff, 0x80, 0x40, 0x40 }, { 0xff, 0x80, 0x40, 0x00 } });

  ggo::blit(dst, src, { 1, 2 });

  GGO_CHECK_EQ(ggo::read_pixel<ggo::pixel_type::rgb_8u>(dst, 0, 0), ggo::rgb_8u(0x00, 0x00, 0x00));
  GGO_CHECK_EQ(ggo::read_pixel<ggo::pixel_type::rgb_8u>(dst, 1, 0), ggo::rgb_8u(0x00, 0x00, 0x00));
  GGO_CHECK_EQ(ggo::read_pixel<ggo::pixel_type::rgb_8u>(dst, 2, 0), ggo::rgb_8u(0x00, 0x00, 0x00));

  GGO_CHECK_EQ(ggo::read_pixel<ggo::pixel_type::rgb_8u>(dst, 0, 1), ggo::rgb_8u(0x00, 0x00, 0x00));
  GGO_CHECK_EQ(ggo::read_pixel<ggo::pixel_type::rgb_8u>(dst, 1, 1), ggo::rgb_8u(0x00, 0x00, 0x00));
  GGO_CHECK_EQ(ggo::read_pixel<ggo::pixel_type::rgb_8u>(dst, 2, 1), ggo::rgb_8u(0x00, 0x00, 0x00));

  GGO_CHECK_EQ(ggo::read_pixel<ggo::pixel_type::rgb_8u>(dst, 0, 2), ggo::rgb_8u(0x00, 0x00, 0x00));
  GGO_CHECK_EQ(ggo::read_pixel<ggo::pixel_type::rgb_8u>(dst, 1, 2), ggo::rgb_8u(0x40, 0x20, 0x10));
  GGO_CHECK_EQ(ggo::read_pixel<ggo::pixel_type::rgb_8u>(dst, 2, 2), ggo::rgb_8u(0x00, 0x00, 0x00));

  GGO_CHECK_EQ(ggo::read_pixel<ggo::pixel_type::rgb_8u>(dst, 0, 3), ggo::rgb_8u(0x00, 0x00, 0x00));
  GGO_CHECK_EQ(ggo::read_pixel<ggo::pixel_type::rgb_8u>(dst, 1, 3), ggo::rgb_8u(0xff, 0x80, 0x40));
  GGO_CHECK_EQ(ggo::read_pixel<ggo::pixel_type::rgb_8u>(dst, 2, 3), ggo::rgb_8u(0x80, 0x40, 0x20));

  GGO_CHECK_EQ(ggo::read_pixel<ggo::pixel_type::rgb_8u>(dst, 0, 4), ggo::rgb_8u(0x00, 0x00, 0x00));
  GGO_CHECK_EQ(ggo::read_pixel<ggo::pixel_type::rgb_8u>(dst, 1, 4), ggo::rgb_8u(0x00, 0x00, 0x00));
  GGO_CHECK_EQ(ggo::read_pixel<ggo::pixel_type::rgb_8u>(dst, 2, 4), ggo::rgb_8u(0x00, 0x00, 0x00));
}
#endif