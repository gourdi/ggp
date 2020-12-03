#include "ggo_2d_nonreg.h"
#include <2d/processing/ggo_crop.h>

////////////////////////////////////////////////////////////////////
GGO_TEST(crop, y_8u_rows_up)
{
  const auto image = make_image_t<ggo::pixel_type::y_8u>({ 6, 5 }, {
    00, 01, 02, 03, 04, 05,
    10, 11, 12, 13, 14, 15,
    20, 21, 22, 23, 24, 25,
    30, 31, 32, 33, 34, 35,
    40, 41, 42, 43, 44, 45 });

  {
    const auto cropped = ggo::crop(image, ggo::rect_int::from_left_right_bottom_top(2, 4, 1, 1));
    const auto expected = make_image_t<ggo::pixel_type::y_8u>({ 3, 1 }, { 12, 13, 14 });
    GGO_CHECK_IMG(cropped, expected);
  }

  {
    const auto cropped = ggo::crop(image, ggo::rect_int::from_left_right_bottom_top(-1, 4, 1, 1));
    const auto expected = make_image_t<ggo::pixel_type::y_8u>({ 5, 1 }, { 10, 11, 12, 13, 14 });
    GGO_CHECK_IMG(cropped, expected);
  }

  {
    const auto cropped = ggo::crop(image, ggo::rect_int::from_left_right_bottom_top(-1, 1, 1, 1));
    const auto expected = make_image_t<ggo::pixel_type::y_8u>({ 2, 1 }, { 10, 11 });
    GGO_CHECK_IMG(cropped, expected);
  }

  {
    const auto cropped = ggo::crop(image, ggo::rect_int::from_left_right_bottom_top(4, 10, 1, 2));
    const auto expected = make_image_t<ggo::pixel_type::y_8u>({ 2, 2 }, { 14, 15, 24, 25 });
    GGO_CHECK_IMG(cropped, expected);
  }

  {
    GGO_CHECK_THROW(ggo::crop(image, ggo::rect_int::from_left_right_bottom_top(-2, -1, 1, 2)));
    GGO_CHECK_THROW(ggo::crop(image, ggo::rect_int::from_left_right_bottom_top(6, 10, 1, 2)));
    GGO_CHECK_THROW(ggo::crop(image, ggo::rect_int::from_left_right_bottom_top(1, 2, -2, -1)));
    GGO_CHECK_THROW(ggo::crop(image, ggo::rect_int::from_left_right_bottom_top(1, 2, 5, 7)));
  }
}

////////////////////////////////////////////////////////////////////
GGO_TEST(crop, y_8u_rows_down)
{
  const auto image = make_image_rows_down_t<ggo::pixel_type::y_8u>({ 6, 5 }, { 
    00, 01, 02, 03, 04, 05,
    10, 11, 12, 13, 14, 15,
    20, 21, 22, 23, 24, 25,
    30, 31, 32, 33, 34, 35,
    40, 41, 42, 43, 44, 45 });

  {
    const auto cropped = ggo::crop(image, ggo::rect_int::from_left_right_bottom_top(2, 4, 1, 1));
    const auto expected = make_image_rows_down_t<ggo::pixel_type::y_8u>({ 3, 1 }, { 32, 33, 34 });
    GGO_CHECK_IMG(cropped, expected);
  }

  {
    const auto cropped = ggo::crop(image, ggo::rect_int::from_left_right_bottom_top(-1, 4, 1, 1));
    const auto expected = make_image_rows_down_t<ggo::pixel_type::y_8u>({ 5, 1 }, { 30, 31, 32, 33, 34 });
    GGO_CHECK_IMG(cropped, expected);
  }

  {
    const auto cropped = ggo::crop(image, ggo::rect_int::from_left_right_bottom_top(-1, 1, 1, 1));
    const auto expected = make_image_rows_down_t<ggo::pixel_type::y_8u>({ 2, 1 }, { 30, 31 });
    GGO_CHECK_IMG(cropped, expected);
  }

  {
    const auto cropped = ggo::crop(image, ggo::rect_int::from_left_right_bottom_top(4, 10, 1, 2));
    const auto expected = make_image_rows_down_t<ggo::pixel_type::y_8u>({ 2, 2 }, { 24, 25, 34, 35 });
    GGO_CHECK_EQ(cropped.height(), 2);
  }
}
