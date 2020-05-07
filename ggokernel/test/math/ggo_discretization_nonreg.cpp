#include <kernel/nonreg/ggo_nonreg.h>
#include <kernel/math/ggo_discretization.h>

////////////////////////////////////////////////////////////////////
GGO_TEST(discretization, from_discrete_to_continuous_point)
{
  constexpr ggo::pos2_f p = ggo::get_pixel_center<float>({ -1, 2 });

  static_assert(p.x() == -0.5f);
  static_assert(p.y() ==  2.5f);
}

////////////////////////////////////////////////////////////////////
GGO_TEST(discretization, from_continuous_to_discrete_point)
{
  constexpr ggo::pos2_i p1 = ggo::discretize(ggo::pos2_f(-1.1f, 2.9f));
  static_assert(p1.x() == -2);
  static_assert(p1.y() == 2);

  constexpr ggo::pos2_i p2 = ggo::discretize(ggo::pos2_f(-0.4f, 3.1f));
  static_assert(p2.x() == -1);
  static_assert(p2.y() == 3);

  constexpr ggo::pos2_i p3 = ggo::discretize(ggo::pos2_f(-2.9f, 3.9f));
  static_assert(p3.x() == -3);
  static_assert(p3.y() == 3);
}

////////////////////////////////////////////////////////////////////
GGO_TEST(discretization, from_discrete_to_continuous_rect)
{
  ggo::rect_data<float> r = ggo::to_continuous<float>(ggo::rect_int::from_left_right_bottom_top(-1, 2, 1, 6));

  GGO_CHECK_FLOAT_EQ(r._pos.x(), -1.0f);
  GGO_CHECK_FLOAT_EQ(r._pos.y(), 1.0f);
  GGO_CHECK_FLOAT_EQ(r._width, 4.0f);
  GGO_CHECK_FLOAT_EQ(r._height, 6.0f);
}

////////////////////////////////////////////////////////////////////
GGO_TEST(discretization, from_continuous_to_discrete_rect_exclusive)
{
  const std::vector<std::tuple<ggo::rect_data<float>, ggo::rect_int>> test_set{
    { ggo::rect_data<float>::from_left_right_bottom_top(1.0f, 4.0f, 2.0f, 6.0f), ggo::rect_int::from_left_right_bottom_top(1, 3, 2, 5) },
    { ggo::rect_data<float>::from_left_right_bottom_top(1.1f, 3.9f, 2.1f, 6.0f), ggo::rect_int::from_left_right_bottom_top(1, 3, 2, 5) },
    { ggo::rect_data<float>::from_left_right_bottom_top(1.1f, 4.0f, 2.1f, 6.0f), ggo::rect_int::from_left_right_bottom_top(1, 3, 2, 5) },
    { ggo::rect_data<float>::from_left_right_bottom_top(1.0f, 3.9f, 2.0f, 5.9f), ggo::rect_int::from_left_right_bottom_top(1, 3, 2, 5) },
    { ggo::rect_data<float>::from_left_right_bottom_top(0.9f, 4.1f, 1.9f, 6.1f), ggo::rect_int::from_left_right_bottom_top(0, 4, 1, 6) },
    { ggo::rect_data<float>::from_left_right_bottom_top(0.0f, 1.0f, 0.0f, 1.0f), ggo::rect_int::from_left_right_bottom_top(0, 0, 0, 0) },
    { ggo::rect_data<float>::from_left_right_bottom_top(0.4f, 0.6f, 0.4f, 0.6f), ggo::rect_int::from_left_right_bottom_top(0, 0, 0, 0) },
    { ggo::rect_data<float>::from_left_right_bottom_top(-1.0f, 2.0f, -2.0f, 2.0f), ggo::rect_int::from_left_right_bottom_top(-1, 1, -2, 1) },
    { ggo::rect_data<float>::from_left_right_bottom_top(-0.9f, 1.9f, -1.9f, 1.9f), ggo::rect_int::from_left_right_bottom_top(-1, 1, -2, 1) },
  };

  for (const auto & test : test_set)
  {
    ggo::rect_int discrete_rect = ggo::discretize(std::get<0>(test));
    GGO_CHECK_EQ(discrete_rect, std::get<1>(test));
  }
}

////////////////////////////////////////////////////////////////////
GGO_TEST(discretization, map_fit_length)
{
  {
    auto l = ggo::map_fit<float>(1, ggo::rect<float>::from_left_right_bottom_top(2, 5, 1, 3), { 8, 4 });
    GGO_CHECK_FLOAT_EQ(l, 2);
  }

  {
    auto l = ggo::map_fit<float>(1, ggo::rect<float>::from_left_right_bottom_top(2, 5, 1, 3), { 6, 8 });
    GGO_CHECK_FLOAT_EQ(l, 2);
  }
}

////////////////////////////////////////////////////////////////////
GGO_TEST(discretization, map_fit_point)
{
  {
    auto p = ggo::map_fit<float>({ 3, 2 }, ggo::rect<float>::from_left_right_bottom_top(2, 5, 1, 3), { 8, 4 });
    GGO_CHECK_FLOAT_EQ(p.x(), 3);
    GGO_CHECK_FLOAT_EQ(p.y(), 2);
  }

  {
    auto p = ggo::map_fit<float>({ 3, 2 }, ggo::rect<float>::from_left_right_bottom_top(2, 5, 1, 3), { 6, 8 });
    GGO_CHECK_FLOAT_EQ(p.x(), 2);
    GGO_CHECK_FLOAT_EQ(p.y(), 4);
  }
}

