#include <kernel/nonreg/ggo_nonreg.h>
#include <kernel/math/ggo_coordinates_conversions.h>

////////////////////////////////////////////////////////////////////
GGO_TEST(coordinates_conversions, from_pixel_to_continuous_point)
{
  ggo::pos2_f p = ggo::from_pixel_to_continuous<float>({ -1, 2 });

  GGO_CHECK_FLOAT_EQ(p.x(), -0.5f);
  GGO_CHECK_FLOAT_EQ(p.y(), 2.5f);
}

////////////////////////////////////////////////////////////////////
GGO_TEST(coordinates_conversions, from_continuous_to_pixel_point)
{
  ggo::pos2_i p1 = ggo::from_continuous_to_pixel(ggo::pos2_f(-1.1f, 2.9f));
  GGO_CHECK_EQ(p1.x(), -2);
  GGO_CHECK_EQ(p1.y(), 2);

  ggo::pos2_i p2 = ggo::from_continuous_to_pixel(ggo::pos2_f(-0.4f, 3.1f));
  GGO_CHECK_EQ(p2.x(), -1);
  GGO_CHECK_EQ(p2.y(), 3);

  ggo::pos2_i p3 = ggo::from_continuous_to_pixel(ggo::pos2_f(-2.9f, 3.9f));
  GGO_CHECK_EQ(p3.x(), -3);
  GGO_CHECK_EQ(p3.y(), 3);
}

////////////////////////////////////////////////////////////////////
GGO_TEST(coordinates_conversions, from_pixel_to_continuous_rect)
{
  ggo::rect_data<float> r = ggo::from_pixel_to_continuous<float>(ggo::rect_int::from_left_right_bottom_top(-1, 2, 1, 6));

  GGO_CHECK_FLOAT_EQ(r._pos.x(), -1.0f);
  GGO_CHECK_FLOAT_EQ(r._pos.y(), 1.0f);
  GGO_CHECK_FLOAT_EQ(r._width, 4.0f);
  GGO_CHECK_FLOAT_EQ(r._height, 6.0f);
}

////////////////////////////////////////////////////////////////////
GGO_TEST(coordinates_conversions, from_continuous_to_pixel_rect_exclusive)
{
  const std::vector<std::tuple<ggo::rect_data<float>, ggo::rect_int>> test_set{
    { ggo::rect_data_from_left_right_bottom_top(1.0f, 4.0f, 2.0f, 6.0f), ggo::rect_int::from_left_right_bottom_top(1, 3, 2, 5) },
    { ggo::rect_data_from_left_right_bottom_top(1.1f, 3.9f, 2.1f, 6.0f), ggo::rect_int::from_left_right_bottom_top(1, 3, 2, 5) },
    { ggo::rect_data_from_left_right_bottom_top(1.1f, 4.0f, 2.1f, 6.0f), ggo::rect_int::from_left_right_bottom_top(1, 3, 2, 5) },
    { ggo::rect_data_from_left_right_bottom_top(1.0f, 3.9f, 2.0f, 5.9f), ggo::rect_int::from_left_right_bottom_top(1, 3, 2, 5) },
    { ggo::rect_data_from_left_right_bottom_top(0.9f, 4.1f, 1.9f, 6.1f), ggo::rect_int::from_left_right_bottom_top(0, 4, 1, 6) },
    { ggo::rect_data_from_left_right_bottom_top(0.0f, 1.0f, 0.0f, 1.0f), ggo::rect_int::from_left_right_bottom_top(0, 0, 0, 0) },
    { ggo::rect_data_from_left_right_bottom_top(0.4f, 0.6f, 0.4f, 0.6f), ggo::rect_int::from_left_right_bottom_top(0, 0, 0, 0) },
    { ggo::rect_data_from_left_right_bottom_top(-1.0f, 2.0f, -2.0f, 2.0f), ggo::rect_int::from_left_right_bottom_top(-1, 1, -2, 1) },
    { ggo::rect_data_from_left_right_bottom_top(-0.9f, 1.9f, -1.9f, 1.9f), ggo::rect_int::from_left_right_bottom_top(-1, 1, -2, 1) },
  };

  for (const auto & test : test_set)
  {
    ggo::rect_int pixel_rect = ggo::from_continuous_to_pixel_exclusive(std::get<0>(test));
    GGO_CHECK_EQ(pixel_rect, std::get<1>(test));
  }
}

////////////////////////////////////////////////////////////////////
GGO_TEST(coordinates_conversions, from_continuous_to_pixel_rect_inclusive)
{
  const std::vector<ggo::rect_data<float>> empty_test_set{
    ggo::rect_data_from_left_right_bottom_top(0.1f, 0.2f, 0.1f, 0.2f),
    ggo::rect_data_from_left_right_bottom_top(0.1f, 1.9f, 1.0f, 5.0f),
    ggo::rect_data_from_left_right_bottom_top(1.0f, 5.0f, 0.1f, 1.9f),
  };

  for (const auto & test_rect : empty_test_set)
  {
    auto r = ggo::from_continuous_to_pixel_inclusive(test_rect);
    GGO_CHECK(r.has_value() == false);
  }

  const std::vector<std::tuple<ggo::rect_data<float>, ggo::rect_int>> test_set{
    { ggo::rect_data_from_left_right_bottom_top(1.0f, 4.0f, 2.0f, 6.0f), ggo::rect_int::from_left_right_bottom_top(1, 3, 2, 5) },
    { ggo::rect_data_from_left_right_bottom_top(0.9f, 4.1f, 1.9f, 6.1f), ggo::rect_int::from_left_right_bottom_top(1, 3, 2, 5) },
    { ggo::rect_data_from_left_right_bottom_top(1.0f, 4.1f, 2.0f, 6.1f), ggo::rect_int::from_left_right_bottom_top(1, 3, 2, 5) },
    { ggo::rect_data_from_left_right_bottom_top(0.9f, 4.0f, 1.9f, 6.0f), ggo::rect_int::from_left_right_bottom_top(1, 3, 2, 5) },
    { ggo::rect_data_from_left_right_bottom_top(1.1f, 3.9f, 2.1f, 5.9f), ggo::rect_int::from_left_right_bottom_top(2, 2, 3, 4) },
  };

  for (const auto & test : test_set)
  {
    auto pixel_rect = ggo::from_continuous_to_pixel_inclusive(std::get<0>(test));
    GGO_CHECK(pixel_rect.has_value());
    GGO_CHECK_EQ(*pixel_rect, std::get<1>(test));
  }
}

////////////////////////////////////////////////////////////////////
GGO_TEST(coordinates_conversions, map_fit_length)
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
GGO_TEST(coordinates_conversions, map_fit_point)
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

