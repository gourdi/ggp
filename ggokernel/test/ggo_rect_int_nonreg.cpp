#include <kernel/ggo_rect_int.h>
#include <kernel/nonreg/ggo_nonreg.h>

/////////////////////////////////////////////////////////////////////
GGO_TEST(rect_int, clip_rect)
{
  {
    ggo::rect_int r = ggo::rect_int::from_left_right_bottom_top(3, 5, 2, 3);
    GGO_CHECK(r.clip(ggo::rect_int::from_left_right_bottom_top(-5, -2, 0, 10)) == false);
    GGO_CHECK(r.clip(ggo::rect_int::from_left_right_bottom_top(10, 15, 0, 10)) == false);
    GGO_CHECK(r.clip(ggo::rect_int::from_left_right_bottom_top(0, 10, -10, 0)) == false);
    GGO_CHECK(r.clip(ggo::rect_int::from_left_right_bottom_top(0, 10, 40, 50)) == false);
  }

  {
    ggo::rect_int r = ggo::rect_int::from_left_right_bottom_top(3, 5, 2, 3);
    GGO_CHECK(r.clip(ggo::rect_int::from_left_right_bottom_top(0, 10, 0, 10)) == true);
    GGO_CHECK_EQ(r.left(), 3);
    GGO_CHECK_EQ(r.right(), 5);
    GGO_CHECK_EQ(r.bottom(), 2);
    GGO_CHECK_EQ(r.top(), 3);
  }

  {
    ggo::rect_int r = ggo::rect_int::from_left_right_bottom_top(3, 5, 2, 3);
    GGO_CHECK(r.clip(ggo::rect_int::from_left_right_bottom_top(1, 4, 3, 5)) == true);
    GGO_CHECK_EQ(r.left(), 3);
    GGO_CHECK_EQ(r.right(), 4);
    GGO_CHECK_EQ(r.bottom(), 3);
    GGO_CHECK_EQ(r.top(), 3);
  }
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(rect_int, clip_width_height)
{
  {
    ggo::rect_int r = ggo::rect_int::from_left_right_bottom_top(3, 5, 2, 3);
    GGO_CHECK(r.clip(2, 5) == false);
  }

  {
    ggo::rect_int r = ggo::rect_int::from_left_right_bottom_top(3, 5, 2, 3);
    GGO_CHECK(r.clip(5, 7) == true);
    GGO_CHECK_EQ(r.left(), 3);
    GGO_CHECK_EQ(r.right(), 4);
    GGO_CHECK_EQ(r.bottom(), 2);
    GGO_CHECK_EQ(r.top(), 3);
  }

  {
    ggo::rect_int r = ggo::rect_int::from_left_right_bottom_top(3, 5, 2, 3);
    GGO_CHECK(r.clip(10, 10) == true);
    GGO_CHECK_EQ(r.left(), 3);
    GGO_CHECK_EQ(r.right(), 5);
    GGO_CHECK_EQ(r.bottom(), 2);
    GGO_CHECK_EQ(r.top(), 3);
  }

  {
    ggo::rect_int r = ggo::rect_int::from_left_right_bottom_top(3, 5, 2, 3);
    GGO_CHECK(r.clip(4, 3) == true);
    GGO_CHECK_EQ(r.left(), 3);
    GGO_CHECK_EQ(r.right(), 3);
    GGO_CHECK_EQ(r.bottom(), 2);
    GGO_CHECK_EQ(r.top(), 2);
  }
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(rect_int, vertical_flip)
{
  ggo::rect_int r = ggo::rect_int::from_left_right_bottom_top(3, 5, 1, 3);
  r.vertical_flip(6);

  GGO_CHECK_EQ(r.left(), 3);
  GGO_CHECK_EQ(r.right(), 5);
  GGO_CHECK_EQ(r.bottom(), 2);
  GGO_CHECK_EQ(r.top(), 4);
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(rect_int, is_inside)
{
  ggo::rect_int r = ggo::rect_int::from_left_right_bottom_top(3, 5, 1, 3);
  GGO_CHECK_EQ(r.is_inside({ 3, 1 }), true);
  GGO_CHECK_EQ(r.is_inside({ 5, 3 }), true);
  GGO_CHECK_EQ(r.is_inside({ 2, 0 }), false);
  GGO_CHECK_EQ(r.is_inside({ 6, 4 }), false);
}


