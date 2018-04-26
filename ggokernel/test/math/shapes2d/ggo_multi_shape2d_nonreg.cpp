#include "../../ggo_kernel_nonreg.h"

/////////////////////////////////////////////////////////////////////
GGO_TEST(multi_shape, shape_union)
{
  ggo::multi_shape_float multi_shape;
  multi_shape.add_shapes(std::make_shared<ggo::disc_float>(ggo::pos2f(2.f, 1.f), 3.f), std::make_shared<ggo::disc_float>(ggo::pos2f(5.f, 3.f), 2.f));

  // Bounding rectangle.
  ggo::rect_float rect = multi_shape.get_bounding_rect();
  GGO_CHECK_FLOAT_EQ(rect.left(), -1);
  GGO_CHECK_FLOAT_EQ(rect.right(), 7);
  GGO_CHECK_FLOAT_EQ(rect.bottom(), -2);
  GGO_CHECK_FLOAT_EQ(rect.top(), 5);

  // Point inside.
  GGO_CHECK(multi_shape.is_point_inside({ 0, 0 }) == true);
  GGO_CHECK(multi_shape.is_point_inside({ 6, 4 }) == true);
  GGO_CHECK(multi_shape.is_point_inside({ 3, 5 }) == false);
  GGO_CHECK(multi_shape.is_point_inside({ 5, 0 }) == false);

  // Rectangle intersection.
  GGO_CHECK_RECT_INTERSECTION(multi_shape, 1, 2, 5, 6, ggo::rect_intersection::disjoints);
  GGO_CHECK_RECT_INTERSECTION(multi_shape, 1, 2, 1, 3, ggo::rect_intersection::rect_in_shape);
  GGO_CHECK_RECT_INTERSECTION(multi_shape, 4, 6, 2, 4, ggo::rect_intersection::rect_in_shape);
  GGO_CHECK_RECT_INTERSECTION(multi_shape, -2, 8, -3, 8, ggo::rect_intersection::shape_in_rect);
  GGO_CHECK_RECT_INTERSECTION(multi_shape, 2, 5, 2, 3, ggo::rect_intersection::unknown);
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(multi_shape, shape_intersection)
{
  ggo::multi_shape<float, ggo::boolean_mode::INTERSECTION> multi_shape;
  multi_shape.add_shapes(std::make_shared<ggo::disc_float>(ggo::pos2f(2.f, 1.f), 3.f), std::make_shared<ggo::disc_float>(ggo::pos2f(3.f, 3.f), 2.f));

  // Bounding rectangle.
  ggo::rect_float rect = multi_shape.get_bounding_rect();
  GGO_CHECK_FLOAT_EQ(rect.left(), 1);
  GGO_CHECK_FLOAT_EQ(rect.right(), 5);
  GGO_CHECK_FLOAT_EQ(rect.bottom(), 1);
  GGO_CHECK_FLOAT_EQ(rect.top(), 4);

  // Point inside.
  GGO_CHECK(multi_shape.is_point_inside({ 0, 0 }) == false);
  GGO_CHECK(multi_shape.is_point_inside({ 1, 1 }) == false);
  GGO_CHECK(multi_shape.is_point_inside({ 5, 1 }) == false);
  GGO_CHECK(multi_shape.is_point_inside({ 4, 4 }) == false);
  GGO_CHECK(multi_shape.is_point_inside({ 2, 2 }) == true);
  GGO_CHECK(multi_shape.is_point_inside({ 4, 2 }) == true);

  // Rectangle intersection.
  GGO_CHECK_RECT_INTERSECTION(multi_shape, 0, 2, 0, 1, ggo::rect_intersection::disjoints);
  GGO_CHECK_RECT_INTERSECTION(multi_shape, 4, 8, 4, 8, ggo::rect_intersection::disjoints);
  GGO_CHECK_RECT_INTERSECTION(multi_shape, 2, 3, 2, 3, ggo::rect_intersection::rect_in_shape);
  GGO_CHECK_RECT_INTERSECTION(multi_shape, 2, 4, 2, 4, ggo::rect_intersection::unknown);
  GGO_CHECK_RECT_INTERSECTION(multi_shape, -2, 8, -3, 8, ggo::rect_intersection::shape_in_rect);
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(multi_shape, shape_difference)
{
  ggo::multi_shape<float, ggo::boolean_mode::DIFFERENCE> multi_shape;
  auto rect1 = std::make_shared<ggo::rect_float>(ggo::rect_data<float>{ {2.f, 1.f}, 7.f, 7.f });
  auto rect2 = std::make_shared<ggo::rect_float>(ggo::rect_data<float>{ {3.f, 4.f}, 3.f, 3.f });
  auto rect3 = std::make_shared<ggo::rect_float>(ggo::rect_data<float>{ {7.f, 2.f}, 5.f, 5.f });
  multi_shape.add_shapes(rect1, rect2, rect3);

  // Bounding rectangle.
  ggo::rect_float rect = multi_shape.get_bounding_rect();
  GGO_CHECK_FLOAT_EQ(rect.left(), 2);
  GGO_CHECK_FLOAT_EQ(rect.right(), 9);
  GGO_CHECK_FLOAT_EQ(rect.bottom(), 1);
  GGO_CHECK_FLOAT_EQ(rect.top(), 8);

  // Point inside.
  GGO_CHECK(multi_shape.is_point_inside({ 0, 0 }) == false);
  GGO_CHECK(multi_shape.is_point_inside({ 1, 1 }) == false);
  GGO_CHECK(multi_shape.is_point_inside({ 3, 3 }) == true);
  GGO_CHECK(multi_shape.is_point_inside({ 4, 2 }) == true);
  GGO_CHECK(multi_shape.is_point_inside({ 4, 6 }) == false);
  GGO_CHECK(multi_shape.is_point_inside({ 8, 4 }) == false);

  // Rectangle intersection.
  GGO_CHECK_RECT_INTERSECTION(multi_shape, 0, 1, 0, 1, ggo::rect_intersection::disjoints);
  GGO_CHECK_RECT_INTERSECTION(multi_shape, 1, 10, 0, 10, ggo::rect_intersection::shape_in_rect);
  GGO_CHECK_RECT_INTERSECTION(multi_shape, 3, 4, 2, 3, ggo::rect_intersection::rect_in_shape);
  GGO_CHECK_RECT_INTERSECTION(multi_shape, 4, 5, 3, 5, ggo::rect_intersection::unknown);
  GGO_CHECK_RECT_INTERSECTION(multi_shape, 4, 5, 5, 6, ggo::rect_intersection::disjoints);
}
