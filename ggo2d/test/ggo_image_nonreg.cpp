#include "ggo_2d_nonreg.h"
#include <2d/ggo_image.h>

/////////////////////////////////////////////////////////////////////
GGO_TEST(static_image, move)
{
  auto img1 = make_image_t<ggo::pixel_type::rgb_8u>({ 20, 30 }, ggo::black_8u());
  auto img2 = make_image_t<ggo::pixel_type::rgb_8u>({ 10, 20 }, ggo::black_8u());

  void * ptr = img2.data();

  img1 = std::move(img2);

  GGO_CHECK_EQ(img1.data(), ptr);
  GGO_CHECK_EQ(img1.size(), ggo::size(10, 20));
  GGO_CHECK_EQ(img1.pixel_type(), ggo::pixel_type::rgb_8u);
  GGO_CHECK(img2.data() == nullptr);
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(dynamic_image, move)
{
  auto img1 = make_image<ggo::pixel_type::rgb_8u>({ 20, 30 }, ggo::black_8u());
  auto img2 = make_image_rows_down<ggo::pixel_type::y_32f>({ 10, 20 }, 0.f);

  void * ptr = img2.data();

  img1 = std::move(img2);

  GGO_CHECK_EQ(img1.data(), ptr);
  GGO_CHECK_EQ(img1.size(), ggo::size(10, 20));
  GGO_CHECK_EQ(img1.pixel_type(), ggo::pixel_type::y_32f);
  GGO_CHECK(img2.data() == nullptr);
}

#if 0
/////////////////////////////////////////////////////////////////////
GGO_TEST(image, image_view_yd)
{
  auto img = make_image_t<ggo::pixel_type::y_8u, ggo::lines_order::down>({
    { 00, 01, 02, 03, 04, 05 },
    { 10, 11, 12, 13, 14, 15 },
    { 20, 21, 22, 23, 24, 25 },
    { 30, 31, 32, 33, 34, 35 },
    { 40, 41, 42, 43, 44, 45 } });

  auto view = ggo::make_image_view(img, ggo::rect_int::from_left_right_bottom_top(2, 4, 1, 2));

  GGO_CHECK_EQ(view->read_pixel(0, 0), 32);
  GGO_CHECK_EQ(view->read_pixel(1, 0), 33);
  GGO_CHECK_EQ(view->read_pixel(2, 0), 34);
  GGO_CHECK_EQ(view->read_pixel(0, 1), 22);
  GGO_CHECK_EQ(view->read_pixel(1, 1), 23);
  GGO_CHECK_EQ(view->read_pixel(2, 1), 24);
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(image, image_view_yu)
{
  auto img = make_image_t<ggo::pixel_type::y_8u, ggo::lines_order::up>({
    { 00, 01, 02, 03, 04, 05 },
    { 10, 11, 12, 13, 14, 15 },
    { 20, 21, 22, 23, 24, 25 },
    { 30, 31, 32, 33, 34, 35 },
    { 40, 41, 42, 43, 44, 45 } });

  auto view = ggo::make_image_view(img, ggo::rect_int::from_left_right_bottom_top(2, 4, 1, 2));

  GGO_CHECK_EQ(view->read_pixel(0, 0), 12);
  GGO_CHECK_EQ(view->read_pixel(1, 0), 13);
  GGO_CHECK_EQ(view->read_pixel(2, 0), 14);
  GGO_CHECK_EQ(view->read_pixel(0, 1), 22);
  GGO_CHECK_EQ(view->read_pixel(1, 1), 23);
  GGO_CHECK_EQ(view->read_pixel(2, 1), 24);
}
#endif