#include <ggo_nonreg.h>
#include <ggo_pixel_rect.h>

/////////////////////////////////////////////////////////////////////
GGO_TEST(pixel_rect, clip_rect)
{
  {
    ggo::pixel_rect r = ggo::pixel_rect::from_left_right_bottom_top(3, 5, 2, 3);
    GGO_CHECK(r.clip(ggo::pixel_rect::from_left_right_bottom_top(-5, -2, 0, 10)) == false);
    GGO_CHECK(r.clip(ggo::pixel_rect::from_left_right_bottom_top(10, 15, 0, 10)) == false);
    GGO_CHECK(r.clip(ggo::pixel_rect::from_left_right_bottom_top(0, 10, -10, 0)) == false);
    GGO_CHECK(r.clip(ggo::pixel_rect::from_left_right_bottom_top(0, 10, 40, 50)) == false);
  }

  {
    ggo::pixel_rect r = ggo::pixel_rect::from_left_right_bottom_top(3, 5, 2, 3);
    GGO_CHECK(r.clip(ggo::pixel_rect::from_left_right_bottom_top(0, 10, 0, 10)) == true);
    GGO_CHECK_EQ(r.left(), 3);
    GGO_CHECK_EQ(r.right(), 5);
    GGO_CHECK_EQ(r.bottom(), 2);
    GGO_CHECK_EQ(r.top(), 3);
  }

  {
    ggo::pixel_rect r = ggo::pixel_rect::from_left_right_bottom_top(3, 5, 2, 3);
    GGO_CHECK(r.clip(ggo::pixel_rect::from_left_right_bottom_top(1, 4, 3, 5)) == true);
    GGO_CHECK_EQ(r.left(), 3);
    GGO_CHECK_EQ(r.right(), 4);
    GGO_CHECK_EQ(r.bottom(), 3);
    GGO_CHECK_EQ(r.top(), 3);
  }
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(pixel_rect, clip_width_height)
{
  {
    ggo::pixel_rect r = ggo::pixel_rect::from_left_right_bottom_top(3, 5, 2, 3);
    GGO_CHECK(r.clip(2, 5) == false);
  }

  {
    ggo::pixel_rect r = ggo::pixel_rect::from_left_right_bottom_top(3, 5, 2, 3);
    GGO_CHECK(r.clip(5, 7) == true);
    GGO_CHECK_EQ(r.left(), 3);
    GGO_CHECK_EQ(r.right(), 4);
    GGO_CHECK_EQ(r.bottom(), 2);
    GGO_CHECK_EQ(r.top(), 3);
  }

  {
    ggo::pixel_rect r = ggo::pixel_rect::from_left_right_bottom_top(3, 5, 2, 3);
    GGO_CHECK(r.clip(10, 10) == true);
    GGO_CHECK_EQ(r.left(), 3);
    GGO_CHECK_EQ(r.right(), 5);
    GGO_CHECK_EQ(r.bottom(), 2);
    GGO_CHECK_EQ(r.top(), 3);
  }

  {
    ggo::pixel_rect r = ggo::pixel_rect::from_left_right_bottom_top(3, 5, 2, 3);
    GGO_CHECK(r.clip(4, 3) == true);
    GGO_CHECK_EQ(r.left(), 3);
    GGO_CHECK_EQ(r.right(), 3);
    GGO_CHECK_EQ(r.bottom(), 2);
    GGO_CHECK_EQ(r.top(), 2);
  }
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(pixel_rect, vertical_flip)
{
  ggo::pixel_rect r = ggo::pixel_rect::from_left_right_bottom_top(3, 5, 1, 3);
  r.vertical_flip(6);

  GGO_CHECK_EQ(r.left(), 3);
  GGO_CHECK_EQ(r.right(), 5);
  GGO_CHECK_EQ(r.bottom(), 2);
  GGO_CHECK_EQ(r.top(), 4);
}


