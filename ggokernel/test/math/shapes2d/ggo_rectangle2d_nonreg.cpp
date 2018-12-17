#include "../../ggo_kernel_nonreg.h"

/////////////////////////////////////////////////////////////////////
GGO_TEST(rectangle, construction)
{
  ggo::rect_f rect(ggo::rect_data<float>{ {1.f, 2.f}, 3.f, 4.f });
  GGO_CHECK_FLOAT_EQ(rect.left(), 1);
  GGO_CHECK_FLOAT_EQ(rect.bottom(), 2);
  GGO_CHECK_FLOAT_EQ(rect.width(), 3);
  GGO_CHECK_FLOAT_EQ(rect.height(), 4);
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(rectangle, is_point_inside)
{
  ggo::rect_f rect(ggo::rect_data<float>{ {3.f, 1.f}, 3.f, 2.f });

  GGO_CHECK(rect.is_point_inside({ 4, 2 }) == true);
  GGO_CHECK(rect.is_point_inside({ 2, 2 }) == false);
  GGO_CHECK(rect.is_point_inside({ 2, 4 }) == false);
  GGO_CHECK(rect.is_point_inside({ 4, 4 }) == false);
  GGO_CHECK(rect.is_point_inside({ 10, 10 }) == false);
  GGO_CHECK(rect.is_point_inside({ 10, 2 }) == false);
  GGO_CHECK(rect.is_point_inside({ 10, -1 }) == false);
  GGO_CHECK(rect.is_point_inside({ 4, 0 }) == false);
  GGO_CHECK(rect.is_point_inside({ -1, -1 }) == false);
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(rectangle, dist_to_point)
{
  ggo::rect_f rect(ggo::rect_data<float>{ {3.f, 1.f}, 3.f, 2.f });

  GGO_CHECK_FLOAT_EQ(rect.dist_to_point({ 4, 2 }), 0);
  GGO_CHECK_FLOAT_EQ(rect.dist_to_point({ 0, 2 }), 3);
  GGO_CHECK_FLOAT_EQ(rect.dist_to_point({ 2, 4 }), std::sqrt(2));
  GGO_CHECK_FLOAT_EQ(rect.dist_to_point({ 4, 4 }), 1);
  GGO_CHECK_FLOAT_EQ(rect.dist_to_point({ 7, 4 }), std::sqrt(2));
  GGO_CHECK_FLOAT_EQ(rect.dist_to_point({ 7, 2 }), 1);
  GGO_CHECK_FLOAT_EQ(rect.dist_to_point({ 7, 0 }), std::sqrt(2));
  GGO_CHECK_FLOAT_EQ(rect.dist_to_point({ 4, -1 }), 2);
  GGO_CHECK_FLOAT_EQ(rect.dist_to_point({ 2, 0 }), std::sqrt(2));
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(rectangle, rect_intersection)
{
  ggo::rect_f rect(ggo::rect_data<float>{ {3.f, 1.f}, 3.f, 2.f });

  GGO_CHECK_RECT_INTERSECTION(rect, 4, 5, 1.5, 2.5, ggo::rect_intersection::rect_in_shape);
  GGO_CHECK_RECT_INTERSECTION(rect, 0, 10, 0, 4, ggo::rect_intersection::shape_in_rect);
  GGO_CHECK_RECT_INTERSECTION(rect, 0, 4, 0, 10, ggo::rect_intersection::partial_overlap);
  GGO_CHECK_RECT_INTERSECTION(rect, 0, 4, 2, 10, ggo::rect_intersection::partial_overlap);
  GGO_CHECK_RECT_INTERSECTION(rect, 0, 4, 0, 2, ggo::rect_intersection::partial_overlap);
  GGO_CHECK_RECT_INTERSECTION(rect, 4, 5, 0, 4, ggo::rect_intersection::partial_overlap);
  GGO_CHECK_RECT_INTERSECTION(rect, 4, 5, 2, 4, ggo::rect_intersection::partial_overlap);
  GGO_CHECK_RECT_INTERSECTION(rect, 4, 5, 0, 2, ggo::rect_intersection::partial_overlap);
  GGO_CHECK_RECT_INTERSECTION(rect, 4, 7, 0, 4, ggo::rect_intersection::partial_overlap);
  GGO_CHECK_RECT_INTERSECTION(rect, 4, 7, 2, 4, ggo::rect_intersection::partial_overlap);
  GGO_CHECK_RECT_INTERSECTION(rect, 4, 7, 0, 2, ggo::rect_intersection::partial_overlap);
  GGO_CHECK_RECT_INTERSECTION(rect, 0, 10, 0, 2, ggo::rect_intersection::partial_overlap);
  GGO_CHECK_RECT_INTERSECTION(rect, 0, 5, 0, 2, ggo::rect_intersection::partial_overlap);
  GGO_CHECK_RECT_INTERSECTION(rect, 5, 10, 0, 2, ggo::rect_intersection::partial_overlap);
  GGO_CHECK_RECT_INTERSECTION(rect, 0, 10, 1.5, 2, ggo::rect_intersection::partial_overlap);
  GGO_CHECK_RECT_INTERSECTION(rect, 0, 5, 1.5, 2, ggo::rect_intersection::partial_overlap);
  GGO_CHECK_RECT_INTERSECTION(rect, 5, 10, 1.5, 2, ggo::rect_intersection::partial_overlap);
  GGO_CHECK_RECT_INTERSECTION(rect, 0, 10, 2, 4, ggo::rect_intersection::partial_overlap);
  GGO_CHECK_RECT_INTERSECTION(rect, 0, 5, 2, 4, ggo::rect_intersection::partial_overlap);
  GGO_CHECK_RECT_INTERSECTION(rect, 5, 10, 2, 4, ggo::rect_intersection::partial_overlap);
  GGO_CHECK_RECT_INTERSECTION(rect, 0, 1, 0, 10, ggo::rect_intersection::disjoints);
  GGO_CHECK_RECT_INTERSECTION(rect, 0, 1, 6, 10, ggo::rect_intersection::disjoints);
  GGO_CHECK_RECT_INTERSECTION(rect, 0, 1, -2, -1, ggo::rect_intersection::disjoints);
  GGO_CHECK_RECT_INTERSECTION(rect, 0, 10, 4, 5, ggo::rect_intersection::disjoints);
  GGO_CHECK_RECT_INTERSECTION(rect, 0, 10, -1, 0, ggo::rect_intersection::disjoints);
  GGO_CHECK_RECT_INTERSECTION(rect, 7, 10, 1, 3, ggo::rect_intersection::disjoints);
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(rectangle, extend)
{
  ggo::rect_f rect(ggo::rect_data<float>{ {3.f, 1.f}, 3.f, 2.f });

  auto check_extend = [&](const ggo::rect_f & rect, const ggo::pos2_f & p, const ggo::rect_f & expected_rect)
  {
    ggo::rect_f extended_rect(rect);
    extended_rect.extend(p);

    GGO_CHECK_FLOAT_EQ(extended_rect.left(), expected_rect.left());
    GGO_CHECK_FLOAT_EQ(extended_rect.bottom(), expected_rect.bottom());
    GGO_CHECK_FLOAT_EQ(extended_rect.width(), expected_rect.width());
    GGO_CHECK_FLOAT_EQ(extended_rect.height(), expected_rect.height());
  };
  check_extend(rect, { 4.f, 2.f }, rect);
  check_extend(rect, { 1.f, 4.f }, ggo::rect_f::from_left_right_bottom_top(1.f, 6.f, 1.f, 4.f));
  check_extend(rect, { 7.f, -1.f }, ggo::rect_f::from_left_right_bottom_top(3.f, 7.f, -1.f, 3.f));
}