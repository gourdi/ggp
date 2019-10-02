#include "ggo_2d_nonreg.h"
#include <2d/processing/ggo_image_crop.h>

////////////////////////////////////////////////////////////////////
GGO_TEST(crop, y_8u_yu)
{
  auto image = make_image<ggo::pixel_type::y_8u>(6, 5, {
    00, 01, 02, 03, 04, 05,
    10, 11, 12, 13, 14, 15,
    20, 21, 22, 23, 24, 25,
    30, 31, 32, 33, 34, 35,
    40, 41, 42, 43, 44, 45 });

  {
    auto cropped = ggo::crop(image, ggo::rect_int::from_left_right_bottom_top(2, 4, 1, 1));
    GGO_CHECK_EQ(cropped.width(), 3);
    GGO_CHECK_EQ(cropped.height(), 1);

    const uint8_t * ptr = static_cast<const uint8_t *>(cropped.data());
    GGO_CHECK_EQ(ptr[0], 12);
    GGO_CHECK_EQ(ptr[1], 13);
    GGO_CHECK_EQ(ptr[2], 14);
  }

  {
    auto cropped = ggo::crop(image, ggo::rect_int::from_left_right_bottom_top(-1, 4, 1, 1));
    GGO_CHECK_EQ(cropped.width(), 5);
    GGO_CHECK_EQ(cropped.height(), 1);

    const uint8_t * ptr = static_cast<const uint8_t *>(cropped.data());
    GGO_CHECK_EQ(ptr[0], 10);
    GGO_CHECK_EQ(ptr[1], 11);
    GGO_CHECK_EQ(ptr[2], 12);
    GGO_CHECK_EQ(ptr[3], 13);
    GGO_CHECK_EQ(ptr[4], 14);
  }

  {
    auto cropped = ggo::crop(image, ggo::rect_int::from_left_right_bottom_top(-1, 0, 1, 1));
    GGO_CHECK_EQ(cropped.width(), 1);
    GGO_CHECK_EQ(cropped.height(), 1);

    const uint8_t * ptr = static_cast<const uint8_t *>(cropped.data());
    GGO_CHECK_EQ(ptr[0], 10);
  }

  {
    auto cropped = ggo::crop(image, ggo::rect_int::from_left_right_bottom_top(4, 10, 1, 2));
    GGO_CHECK_EQ(cropped.width(), 2);
    GGO_CHECK_EQ(cropped.height(), 2);

    const uint8_t * ptr = static_cast<const uint8_t *>(cropped.data());
    GGO_CHECK_EQ(ptr[0], 14);
    GGO_CHECK_EQ(ptr[1], 15);
    GGO_CHECK_EQ(ptr[2], 24);
    GGO_CHECK_EQ(ptr[3], 25);
  }

  {
    GGO_CHECK_THROW(ggo::crop(image, ggo::rect_int::from_left_right_bottom_top(-2, -1, 1, 2)));
    GGO_CHECK_THROW(ggo::crop(image, ggo::rect_int::from_left_right_bottom_top(6, 10, 1, 2)));
    GGO_CHECK_THROW(ggo::crop(image, ggo::rect_int::from_left_right_bottom_top(1, 2, -2, -1)));
    GGO_CHECK_THROW(ggo::crop(image, ggo::rect_int::from_left_right_bottom_top(1, 2, 5, 7)));
  }
}

////////////////////////////////////////////////////////////////////
GGO_TEST(crop, y_8u_yd)
{
  auto image = make_image<ggo::pixel_type::y_8u, ggo::lines_order::down>(6, 5, {
    00, 01, 02, 03, 04, 05,
    10, 11, 12, 13, 14, 15,
    20, 21, 22, 23, 24, 25,
    30, 31, 32, 33, 34, 35,
    40, 41, 42, 43, 44, 45 });

  {
    auto cropped = ggo::crop(image, ggo::rect_int::from_left_right_bottom_top(2, 4, 1, 1));
    GGO_CHECK_EQ(cropped.width(), 3);
    GGO_CHECK_EQ(cropped.height(), 1);

    const uint8_t * ptr = static_cast<const uint8_t *>(cropped.data());
    GGO_CHECK_EQ(ptr[0], 32);
    GGO_CHECK_EQ(ptr[1], 33);
    GGO_CHECK_EQ(ptr[2], 34);
  }

  {
    auto cropped = ggo::crop(image, ggo::rect_int::from_left_right_bottom_top(-1, 4, 1, 1));
    GGO_CHECK_EQ(cropped.width(), 5);
    GGO_CHECK_EQ(cropped.height(), 1);

    const uint8_t * ptr = static_cast<const uint8_t *>(cropped.data());
    GGO_CHECK_EQ(ptr[0], 30);
    GGO_CHECK_EQ(ptr[1], 31);
    GGO_CHECK_EQ(ptr[2], 32);
    GGO_CHECK_EQ(ptr[3], 33);
    GGO_CHECK_EQ(ptr[4], 34);
  }

  {
    auto cropped = ggo::crop(image, ggo::rect_int::from_left_right_bottom_top(-1, 0, 1, 1));
    GGO_CHECK_EQ(cropped.width(), 1);
    GGO_CHECK_EQ(cropped.height(), 1);

    const uint8_t * ptr = static_cast<const uint8_t *>(cropped.data());
    GGO_CHECK_EQ(ptr[0], 30);
  }

  {
    auto cropped = ggo::crop(image, ggo::rect_int::from_left_right_bottom_top(4, 10, 1, 2));
    GGO_CHECK_EQ(cropped.width(), 2);
    GGO_CHECK_EQ(cropped.height(), 2);

    const uint8_t * ptr = static_cast<const uint8_t *>(cropped.data());
    GGO_CHECK_EQ(ptr[0], 24);
    GGO_CHECK_EQ(ptr[1], 25);
    GGO_CHECK_EQ(ptr[2], 34);
    GGO_CHECK_EQ(ptr[3], 35);
  }
}


