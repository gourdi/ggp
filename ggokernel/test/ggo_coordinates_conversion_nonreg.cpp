#include <ggo_nonreg.h>
#include <ggo_coordinates_conversions.h>

////////////////////////////////////////////////////////////////////
GGO_TEST(coordinates_conversions, from_pixel_to_math_point)
{
  ggo::pos2f p = ggo::from_pixel_to_math<float>({ -1, 2 });

  GGO_CHECK_FABS(p.x(), -1.f);
  GGO_CHECK_FABS(p.y(), 2.f);
}

////////////////////////////////////////////////////////////////////
GGO_TEST(coordinates_conversions, from_math_to_pixel_point)
{
  ggo::pos2i p1 = ggo::from_math_to_pixel(ggo::pos2f(-0.4f, 1.4f));
  GGO_CHECK_EQ(p1.x(), 0);
  GGO_CHECK_EQ(p1.y(), 1);

  ggo::pos2i p2 = ggo::from_math_to_pixel(ggo::pos2f(-0.6f, 1.6f));
  GGO_CHECK_EQ(p2.x(), -1);
  GGO_CHECK_EQ(p2.y(), 2);
}

////////////////////////////////////////////////////////////////////
GGO_TEST(coordinates_conversions, from_pixel_to_math_rect)
{
  ggo::rect_data<float> r = ggo::from_pixel_to_math<float>(ggo::rect_int::from_left_right_bottom_top(-1, 2, 1, 6));

  GGO_CHECK_FABS(r._pos.x(), -1.5f);
  GGO_CHECK_FABS(r._pos.y(), 0.5f);
  GGO_CHECK_FABS(r._width, 4.0f);
  GGO_CHECK_FABS(r._height, 6.0f);
}

////////////////////////////////////////////////////////////////////
GGO_TEST(coordinates_conversions, from_math_to_pixel_rect)
{
  ggo::rect_int r1 = ggo::from_math_to_pixel(ggo::rect_data<float>({ -1.4f, -0.4f }, 3.8f, 2.8f));
  GGO_CHECK_EQ(r1.left(), -1);
  GGO_CHECK_EQ(r1.right(), 2);
  GGO_CHECK_EQ(r1.bottom(), 0);
  GGO_CHECK_EQ(r1.top(), 2);

  ggo::rect_int r2 = ggo::from_math_to_pixel(ggo::rect_data<float>({ -1.6f, -0.6f }, 4.2f, 3.2f));
  GGO_CHECK_EQ(r2.left(), -2);
  GGO_CHECK_EQ(r2.right(), 3);
  GGO_CHECK_EQ(r2.bottom(), -1);
  GGO_CHECK_EQ(r2.top(), 3);
}
