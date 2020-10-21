#include <kernel/nonreg/ggo_nonreg.h>
#include <kernel/math/ggo_crop.h>

////////////////////////////////////////////////////////////////////
GGO_TEST(crop, map_fit_length)
{
  {
    auto l = ggo::fit<float>(1, ggo::rect<float>::from_left_right_bottom_top(2, 5, 1, 3), { 8, 4 });
    GGO_CHECK_FLOAT_EQ(l, 2);
  }

  {
    auto l = ggo::fit<float>(1, ggo::rect<float>::from_left_right_bottom_top(2, 5, 1, 3), { 6, 8 });
    GGO_CHECK_FLOAT_EQ(l, 2);
  }
}

////////////////////////////////////////////////////////////////////
GGO_TEST(crop, map_fit_point)
{
  {
    auto p = ggo::fit<float>({ 3, 2 }, ggo::rect<float>::from_left_right_bottom_top(2, 5, 1, 3), { 8, 4 });
    GGO_CHECK_FLOAT_EQ(p.x(), 3);
    GGO_CHECK_FLOAT_EQ(p.y(), 2);
  }

  {
    auto p = ggo::fit<float>({ 3, 2 }, ggo::rect<float>::from_left_right_bottom_top(2, 5, 1, 3), { 6, 8 });
    GGO_CHECK_FLOAT_EQ(p.x(), 2);
    GGO_CHECK_FLOAT_EQ(p.y(), 4);
  }
}

