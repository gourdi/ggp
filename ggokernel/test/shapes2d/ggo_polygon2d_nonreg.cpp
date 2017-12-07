#include "../ggo_kernel_nonreg.h"

/////////////////////////////////////////////////////////////////////
GGO_TEST(polygon, construction)
{
  // Move on contruction.
  std::vector<ggo::pos2f> points1{ { 0.f, 1.f },{ 2.f, 3.f },{ 4.f, 5.f } };
  ggo::polygon2d_float polygon(std::move(points1));
  GGO_CHECK(points1.empty() == true);
  GGO_CHECK(polygon.get_points_count() == 3);

  // Move on setting points.
  std::vector<ggo::pos2f> points2{ { 0.f, 1.f },{ 2.f, 3.f } };
  polygon.set_points(std::move(points2));
  GGO_CHECK(points2.empty() == true);
  GGO_CHECK(polygon.get_points_count() == 2);

  // Copy points.
  std::vector<ggo::pos2f> points3{ { 0.f, 1.f },{ 2.f, 3.f },{ 4.f, 5.f } };
  polygon.set_points(points3);
  GGO_CHECK(points3.size() == 3);
  GGO_CHECK(polygon.get_points_count() == 3);
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(polygon, add_get)
{
  ggo::polygon2d_float polygon;
  ggo::pos2f p1(1.f, 2.f);
  ggo::pos2f p2(3.f, 4.f);
  ggo::pos2f p3(5.f, 6.f);
  polygon.add_points(p1, p2, p3);

  GGO_CHECK(polygon.get_points_count() == 3);
  GGO_CHECK_FLOAT_EQ(polygon.get_point(0).x(), 1);
  GGO_CHECK_FLOAT_EQ(polygon.get_point(0).y(), 2);
  GGO_CHECK_FLOAT_EQ(polygon.get_point(1).x(), 3);
  GGO_CHECK_FLOAT_EQ(polygon.get_point(1).y(), 4);
  GGO_CHECK_FLOAT_EQ(polygon.get_point(2).x(), 5);
  GGO_CHECK_FLOAT_EQ(polygon.get_point(2).y(), 6);
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(polygon, is_point_inside)
{
  {
    ggo::polygon2d_float triangle({ { 0, 0 },{ 0, 1 },{ 1, 0 } });

    GGO_CHECK(triangle.is_point_inside({ 0.1f, 0.1f }) == true);
    GGO_CHECK(triangle.is_point_inside({ 0.8f, 0.1f }) == true);
    GGO_CHECK(triangle.is_point_inside({ 0.1f, 0.8f }) == true);
    GGO_CHECK(triangle.is_point_inside({ -0.1f, 0.f }) == false);
    GGO_CHECK(triangle.is_point_inside({ 0.f, -0.1f }) == false);
    GGO_CHECK(triangle.is_point_inside({ 0.6f , 0.6f }) == false);
  }

  {
    ggo::polygon2d_float polygon({ {0, 0}, {5, 0}, {1, 1}, {5, 5},{0, 5} });

    GGO_CHECK(polygon.is_point_inside({ 2, 0.1f }) == true);
    GGO_CHECK(polygon.is_point_inside({ 0.1f, 0.1f }) == true);
    GGO_CHECK(polygon.is_point_inside({ 4, 0.1f }) == true);
    GGO_CHECK(polygon.is_point_inside({ 1, 4 }) == true);
    GGO_CHECK(polygon.is_point_inside({ 3, 4 }) == true);
    GGO_CHECK(polygon.is_point_inside({ 2, 1 }) == false);
    GGO_CHECK(polygon.is_point_inside({ 4, 3 }) == false);
    GGO_CHECK(polygon.is_point_inside({ 4, 1 }) == false);
    GGO_CHECK(polygon.is_point_inside({ 4, 6 }) == false);
    GGO_CHECK(polygon.is_point_inside({ 6, 6 }) == false);
  }
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(polygon, dist_to_point)
{
  {
    ggo::polygon2d_float triangle({ { 0, 0 },{ 0, 1 },{ 1, 0 } });

    GGO_CHECK_FLOAT_EQ(triangle.dist_to_point({ 0.1f, 0.1f }), 0);
    GGO_CHECK_FLOAT_EQ(triangle.dist_to_point({ -1, 0 }), 1);
    GGO_CHECK_FLOAT_EQ(triangle.dist_to_point({ -1, 1 }), 1);
    GGO_CHECK_FLOAT_EQ(triangle.dist_to_point({ 0, -1 }), 1);
    GGO_CHECK_FLOAT_EQ(triangle.dist_to_point({ 1, -1 }), 1);
    GGO_CHECK_FLOAT_EQ(triangle.dist_to_point({ 0, 2 }), 1);
    GGO_CHECK_FLOAT_EQ(triangle.dist_to_point({ 2, 0 }), 1);
    GGO_CHECK_FLOAT_EQ(triangle.dist_to_point({ 1, 1 }), std::sqrt(2) / 2);
  }

  {
    ggo::polygon2d_float polygon({ { 0, 0 },{ 5, 0 },{ 1, 1 },{ 5, 5 },{ 0, 5 } });

    GGO_CHECK_FLOAT_EQ(polygon.dist_to_point({ 1, 4 }), 0);
    GGO_CHECK_FLOAT_EQ(polygon.dist_to_point({ 0.1f, 2 }), 0);
    GGO_CHECK_FLOAT_EQ(polygon.dist_to_point({ 4, 3 }), std::sqrt(2.f) / 2);
    GGO_CHECK_FLOAT_EQ(polygon.dist_to_point({ 0, 6 }), 1);
  }
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(polygon, dist_to_segment)
{
  {
    ggo::polygon2d_float triangle({ { 0, 0 },{ 0, 1 },{ 1, 0 } });

    GGO_CHECK_FLOAT_EQ(triangle.dist_to_segment({ -1.f, 0.5f }, { 1.f, 0.5f }), 0);
    GGO_CHECK_FLOAT_EQ(triangle.dist_to_segment({ 0.5f, 1.f }, { 0.1f, 0.1f }), 0);
    GGO_CHECK_FLOAT_EQ(triangle.dist_to_segment({ 0.5f, -1.f }, { 0.1f, 0.1f }), 0);
    GGO_CHECK_FLOAT_EQ(triangle.dist_to_segment({ -1.f, 0.5f }, { 0.1f, 0.1f }), 0);
    GGO_CHECK_FLOAT_EQ(triangle.dist_to_segment({ -1.f, 0.f }, { -1.f, 1.f }), 1);
    GGO_CHECK_FLOAT_EQ(triangle.dist_to_segment({ -1.f, -5.f }, { -1.f, 5.f }), 1);
    GGO_CHECK_FLOAT_EQ(triangle.dist_to_segment({ -1.f, 0.2f }, { -1.f, 0.3f }), 1);
    GGO_CHECK_FLOAT_EQ(triangle.dist_to_segment({ 1.f, 1.f }, { 2.f, 2.f }), std::sqrt(2.f) / 2);
    GGO_CHECK_FLOAT_EQ(triangle.dist_to_segment({ 0.8f, 1.2f }, { 1.2f, 0.8f }), std::sqrt(2.f) / 2);
  }

  {
    ggo::polygon2d_float polygon({ { 0, 0 },{ 5, 0 },{ 1, 1 },{ 5, 5 },{ 0, 5 } });

    GGO_CHECK_FLOAT_EQ(polygon.dist_to_segment({ 0.5f, 0.5f }, { 0.5f, 4.5f }), 0);
    GGO_CHECK_FLOAT_EQ(polygon.dist_to_segment({ 0.5f, 4.5f }, { 4.f, 4.5f }), 0);
    GGO_CHECK_FLOAT_EQ(polygon.dist_to_segment({ 3.f, 0.1f }, { 3.f, 4.f }), 0);
    GGO_CHECK_FLOAT_EQ(polygon.dist_to_segment({ 3.f, 0.1f }, { 3.f, 4.f }), 0);
    GGO_CHECK_FLOAT_EQ(polygon.dist_to_segment({ 3.f, 0.1f }, { 3.f, 1.f }), 0);
    GGO_CHECK_FLOAT_EQ(polygon.dist_to_segment({ 3.f, 1.f }, { 3.f, 4.f }), 0);
    GGO_CHECK_FLOAT_EQ(polygon.dist_to_segment({ 3.f, 4.f }, { 6.f, 4.f }), 0);
    GGO_CHECK_FLOAT_EQ(polygon.dist_to_segment({ 4.f, 3.f }, { 6.f, 3.f }), std::sqrt(2.f) / 2);
    GGO_CHECK_FLOAT_EQ(polygon.dist_to_segment({ -1.f, 6.f }, { -1.f, 7.f }), std::sqrt(2.f));
    GGO_CHECK_FLOAT_EQ(polygon.dist_to_segment({ -1.f, 4.f }, { -2.f, 7.f }), 1);
    GGO_CHECK_FLOAT_EQ(polygon.dist_to_segment({ 1.f, -1.f }, { 1.f, -2.f }), 1);
    GGO_CHECK_FLOAT_EQ(polygon.dist_to_segment({ 6.f, 0.f }, { 7.f, 0.f }), 1);
  }
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(polygon, rect_intersection)
{
  {
    ggo::polygon2d_float triangle({ { 0, 0 },{ 0, 1 },{ 1, 0 } });

    GGO_CHECK_RECT_INTERSECTION(triangle, 0.1f, 0.2f, 0.1f, 0.2f, ggo::rect_intersection::rect_in_shape);
    GGO_CHECK_RECT_INTERSECTION(triangle, -1.f, 2, -1.f, 2, ggo::rect_intersection::shape_in_rect);
    GGO_CHECK_RECT_INTERSECTION(triangle, 0.5f, 0.6f, 0, 3, ggo::rect_intersection::partial_overlap);
    GGO_CHECK_RECT_INTERSECTION(triangle, 0.1f, 0.2f, 0.1f, 3, ggo::rect_intersection::partial_overlap);
    GGO_CHECK_RECT_INTERSECTION(triangle, 0.1f, 0.2f, -1.f, 3, ggo::rect_intersection::partial_overlap);
    GGO_CHECK_RECT_INTERSECTION(triangle, 2, 3, -1.f, 2, ggo::rect_intersection::disjoints);
    GGO_CHECK_RECT_INTERSECTION(triangle, 2, 3, -2, -1.f, ggo::rect_intersection::disjoints);
    GGO_CHECK_RECT_INTERSECTION(triangle, -2, -1.f, 0, 1.f, ggo::rect_intersection::disjoints);
    GGO_CHECK_RECT_INTERSECTION(triangle, -2, -1.f, 0.5, 10, ggo::rect_intersection::disjoints);
    GGO_CHECK_RECT_INTERSECTION(triangle, -2, -1, -1, 0.5, ggo::rect_intersection::disjoints);
    GGO_CHECK_RECT_INTERSECTION(triangle, 0, 1, -2, -1, ggo::rect_intersection::disjoints);
    GGO_CHECK_RECT_INTERSECTION(triangle, 0.2f, 0.3f, -2, -1, ggo::rect_intersection::disjoints);
    GGO_CHECK_RECT_INTERSECTION(triangle, -1, 0.3f, -2, -1, ggo::rect_intersection::disjoints);
    GGO_CHECK_RECT_INTERSECTION(triangle, 0.2f, 3, -2, -1, ggo::rect_intersection::disjoints);
    GGO_CHECK_RECT_INTERSECTION(triangle, -1, 3, -2, -1, ggo::rect_intersection::disjoints);
    GGO_CHECK_RECT_INTERSECTION(triangle, -1, 3, 2, 3, ggo::rect_intersection::disjoints);
    GGO_CHECK_RECT_INTERSECTION(triangle, 2, 3, 0, 3, ggo::rect_intersection::disjoints);
    GGO_CHECK_RECT_INTERSECTION(triangle, 2, 3, -1, 3, ggo::rect_intersection::disjoints);
    GGO_CHECK_RECT_INTERSECTION(triangle, 1, 2, 1, 2, ggo::rect_intersection::disjoints);
  }

  {
    ggo::polygon2d_float polygon({ { 0, 0 },{ 5, 0 },{ 1, 1 },{ 5, 5 },{ 0, 5 } });

    GGO_CHECK_RECT_INTERSECTION(polygon, 0.1f, 0.2f, 0.1f, 0.2f, ggo::rect_intersection::rect_in_shape);
    GGO_CHECK_RECT_INTERSECTION(polygon, 1, 2, 3, 4, ggo::rect_intersection::rect_in_shape);
    GGO_CHECK_RECT_INTERSECTION(polygon, 3, 3.5f, 4, 4.5, ggo::rect_intersection::rect_in_shape);
    GGO_CHECK_RECT_INTERSECTION(polygon, -1, 6, -1, 6, ggo::rect_intersection::shape_in_rect);
    GGO_CHECK_RECT_INTERSECTION(polygon, 3, 4, 1, 2, ggo::rect_intersection::disjoints);
    GGO_CHECK_RECT_INTERSECTION(polygon, 1, 2, 6, 10, ggo::rect_intersection::disjoints);
    GGO_CHECK_RECT_INTERSECTION(polygon, 6, 8, -2, 10, ggo::rect_intersection::disjoints);
    GGO_CHECK_RECT_INTERSECTION(polygon, -1, 1, 4, 6, ggo::rect_intersection::partial_overlap);
    GGO_CHECK_RECT_INTERSECTION(polygon, -1, 6, 4, 6, ggo::rect_intersection::partial_overlap);
    GGO_CHECK_RECT_INTERSECTION(polygon, 4, 6, -1, 1, ggo::rect_intersection::partial_overlap);
    GGO_CHECK_RECT_INTERSECTION(polygon, 0.5f, 2, 0.5f, 1, ggo::rect_intersection::partial_overlap);
    GGO_CHECK_RECT_INTERSECTION(polygon, 2, 3, -1, 7, ggo::rect_intersection::partial_overlap);
    GGO_CHECK_RECT_INTERSECTION(polygon, -1, 1, 3, 4, ggo::rect_intersection::partial_overlap);
    GGO_CHECK_RECT_INTERSECTION(polygon, 2, 7, 3, 4, ggo::rect_intersection::partial_overlap);
    GGO_CHECK_RECT_INTERSECTION(polygon, 1, 2, -1, 0.1, ggo::rect_intersection::partial_overlap);
    GGO_CHECK_RECT_INTERSECTION(polygon, 1, 2, 6, 7, ggo::rect_intersection::disjoints);
    GGO_CHECK_RECT_INTERSECTION(polygon, -10, -8, 3, 4, ggo::rect_intersection::disjoints);
    GGO_CHECK_RECT_INTERSECTION(polygon, 4, 6, 2, 3, ggo::rect_intersection::disjoints);
    GGO_CHECK_RECT_INTERSECTION(polygon, 3, 8, 1.5f, 2, ggo::rect_intersection::disjoints);
    GGO_CHECK_RECT_INTERSECTION(polygon, 6, 8, -1, 1, ggo::rect_intersection::disjoints);
  }
}

