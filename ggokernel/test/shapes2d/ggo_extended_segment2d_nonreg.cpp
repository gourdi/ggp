#include "../ggo_kernel_nonreg.h"

/////////////////////////////////////////////////////////////////////
GGO_TEST(extended_segment, get_bounding_rect)
{
  {
    ggo::extended_segment_float extended_segment(ggo::pos2f(2.f, 1.f), ggo::pos2f(6.f, 2.f), 1.f);

    ggo::rect_float rect = extended_segment.get_bounding_rect();
    GGO_CHECK_FLOAT_EQ(rect.left(), 1);
    GGO_CHECK_FLOAT_EQ(rect.right(), 7);
    GGO_CHECK_FLOAT_EQ(rect.top(), 3);
    GGO_CHECK_FLOAT_EQ(rect.bottom(), 0);
  }

  {
    ggo::extended_segment_float extended_segment(ggo::pos2f(6.f, 5.f), ggo::pos2f(3.f, 2.f), std::sqrt(2.f));

    ggo::rect_float rect = extended_segment.get_bounding_rect();
    GGO_CHECK_FLOAT_EQ(rect.left(), 3 - std::sqrt(2));
    GGO_CHECK_FLOAT_EQ(rect.right(), 6 + std::sqrt(2));
    GGO_CHECK_FLOAT_EQ(rect.top(), 5 + std::sqrt(2));
    GGO_CHECK_FLOAT_EQ(rect.bottom(), 2 - std::sqrt(2));
  }
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(extended_segment, is_point_inside)
{
  {
    ggo::extended_segment_float extended_segment(ggo::pos2f(2.f, 1.f), ggo::pos2f(6.f, 2.f), 1.f);

    GGO_CHECK(extended_segment.is_point_inside({ 1.2f, 1 }) == true);
    GGO_CHECK(extended_segment.is_point_inside({ 4, 2 }) == true);
    GGO_CHECK(extended_segment.is_point_inside({ 6.5, 2 }) == true);
    GGO_CHECK(extended_segment.is_point_inside({ 1, 0 }) == false);
    GGO_CHECK(extended_segment.is_point_inside({ 1, 2 }) == false);
    GGO_CHECK(extended_segment.is_point_inside({ 7, 1 }) == false);
    GGO_CHECK(extended_segment.is_point_inside({ 7, 3 }) == false);
    GGO_CHECK(extended_segment.is_point_inside({ 4, 3 }) == false);
  }

  {

    // Point inside.
    ggo::extended_segment_float extended_segment(ggo::pos2f(6.f, 5.f), ggo::pos2f(3.f, 2.f), std::sqrt(2.f));

    GGO_CHECK(extended_segment.is_point_inside({ 2, 2 }) == true);
    GGO_CHECK(extended_segment.is_point_inside({ 5, 3 }) == true);
    GGO_CHECK(extended_segment.is_point_inside({ 7, 5 }) == true);
    GGO_CHECK(extended_segment.is_point_inside({ 1, 1 }) == false);
    GGO_CHECK(extended_segment.is_point_inside({ 8, 5 }) == false);
    GGO_CHECK(extended_segment.is_point_inside({ 3, 5 }) == false);
  }
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(extended_segment, dist_to_point)
{
  {
    ggo::extended_segment_float extended_segment(ggo::pos2f(2.f, 1.f), ggo::pos2f(6.f, 2.f), 1.f);

    GGO_CHECK_FLOAT_EQ(extended_segment.dist_to_point({ 1.2f, 1 }), 0);
    GGO_CHECK_FLOAT_EQ(extended_segment.dist_to_point({ 4, 2 }), 0);
    GGO_CHECK_FLOAT_EQ(extended_segment.dist_to_point({ 6.5, 2 }), 0);
    GGO_CHECK_FLOAT_EQ(extended_segment.dist_to_point({ -1, 1 }), 2);
    GGO_CHECK_FLOAT_EQ(extended_segment.dist_to_point({ 2, -2 }), 2);
    GGO_CHECK_FLOAT_EQ(extended_segment.dist_to_point({ 1, 2 }), std::sqrt(2) - 1);
  }

  {
    ggo::extended_segment_float extended_segment(ggo::pos2f(6.f, 5.f), ggo::pos2f(3.f, 2.f), std::sqrt(2.f));

    GGO_CHECK_FLOAT_EQ(extended_segment.dist_to_point({ 2, 2 }), 0);
    GGO_CHECK_FLOAT_EQ(extended_segment.dist_to_point({ 5, 3 }), 0);
    GGO_CHECK_FLOAT_EQ(extended_segment.dist_to_point({ 7, 5 }), 0);
    GGO_CHECK_FLOAT_EQ(extended_segment.dist_to_point({ 1, 0 }), std::sqrt(2));
    GGO_CHECK_FLOAT_EQ(extended_segment.dist_to_point({ 6, 1 }), std::sqrt(2));
    GGO_CHECK_FLOAT_EQ(extended_segment.dist_to_point({ 3, 5 }), std::sqrt(2) / 2);
    GGO_CHECK_FLOAT_EQ(extended_segment.dist_to_point({ 8, 5 }), 2 - std::sqrt(2));
  }
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(extended_segment, rect_intersection)
{
  ggo::extended_segment_float extended_segment(ggo::pos2f(6.f, 5.f), ggo::pos2f(3.f, 2.f), std::sqrt(2.f));

  GGO_CHECK_RECT_INTERSECTION(extended_segment, 3, 4, 2, 3, ggo::rect_intersection::rect_in_shape);
  GGO_CHECK_RECT_INTERSECTION(extended_segment, 0, 10, 0, 10, ggo::rect_intersection::shape_in_rect);
  GGO_CHECK_RECT_INTERSECTION(extended_segment, 0, 3, 0, 2, ggo::rect_intersection::partial_overlap);
  GGO_CHECK_RECT_INTERSECTION(extended_segment, 3, 6, 0, 7, ggo::rect_intersection::partial_overlap);
  GGO_CHECK_RECT_INTERSECTION(extended_segment, 7, 8, 5, 6, ggo::rect_intersection::partial_overlap);
  GGO_CHECK_RECT_INTERSECTION(extended_segment, 0, 10, 1, 10, ggo::rect_intersection::partial_overlap);
  GGO_CHECK_RECT_INTERSECTION(extended_segment, 0, 7, 0, 10, ggo::rect_intersection::partial_overlap);
  GGO_CHECK_RECT_INTERSECTION(extended_segment, 0, 10, 5.1f, 5.2f, ggo::rect_intersection::partial_overlap);
  GGO_CHECK_RECT_INTERSECTION(extended_segment, 5.5f, 6, 5.5f, 6.5f, ggo::rect_intersection::partial_overlap);
  GGO_CHECK_RECT_INTERSECTION(extended_segment, 0, 2, 4, 5, ggo::rect_intersection::disjoints);
  GGO_CHECK_RECT_INTERSECTION(extended_segment, 0, 1, 0, 1, ggo::rect_intersection::disjoints);
  GGO_CHECK_RECT_INTERSECTION(extended_segment, 6, 8, 0, 2, ggo::rect_intersection::disjoints);
  GGO_CHECK_RECT_INTERSECTION(extended_segment, 8, 9, 0, 10, ggo::rect_intersection::disjoints);
  GGO_CHECK_RECT_INTERSECTION(extended_segment, 4, 10, 7, 8, ggo::rect_intersection::disjoints);
}
