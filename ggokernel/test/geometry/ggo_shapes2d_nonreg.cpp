#include "../ggo_kernel_nonreg.h"
#include <ggo_shapes2d.h>

/////////////////////////////////////////////////////////////////////
#define GGO_CHECK_RECT_INTERSECTION(shape, left, right, bottom, top, res) \
  { ggo::rect_data<float> rect_data {{static_cast<float>(left), static_cast<float>(bottom)}, \
                                      static_cast<float>(right - left), \
                                      static_cast<float>(top - bottom)}; \
  GGO_CHECK(shape.get_rect_intersection(rect_data) == res); }

/////////////////////////////////////////////////////////////////////
GGO_TEST(shapes2d, segment)
{
  ggo::segment_float segment(1, 1, 5, 3);

  GGO_CHECK(segment.intersect_horizontal_segment(2, 4, 2) == true);
  GGO_CHECK(segment.intersect_horizontal_segment(0, 2, 2) == false);
  GGO_CHECK(segment.intersect_horizontal_segment(4, 6, 2) == false);
  GGO_CHECK(segment.intersect_horizontal_segment(0, 6, 0) == false);
  GGO_CHECK(segment.intersect_horizontal_segment(0, 6, 4) == false);

  GGO_CHECK(segment.intersect_vertical_segment(3, 1, 3) == true);
  GGO_CHECK(segment.intersect_vertical_segment(3, 0, 1) == false);
  GGO_CHECK(segment.intersect_vertical_segment(3, 3, 4) == false);
  GGO_CHECK(segment.intersect_vertical_segment(0, 0, 3) == false);
  GGO_CHECK(segment.intersect_vertical_segment(6, 0, 4) == false);

  segment = ggo::segment_float(5, 3, 1, 1);

  GGO_CHECK(segment.intersect_horizontal_segment(2, 4, 2) == true);
  GGO_CHECK(segment.intersect_horizontal_segment(0, 2, 2) == false);
  GGO_CHECK(segment.intersect_horizontal_segment(4, 6, 2) == false);
  GGO_CHECK(segment.intersect_horizontal_segment(0, 6, 0) == false);
  GGO_CHECK(segment.intersect_horizontal_segment(0, 6, 4) == false);

  GGO_CHECK(segment.intersect_vertical_segment(3, 1, 3) == true);
  GGO_CHECK(segment.intersect_vertical_segment(3, 0, 1) == false);
  GGO_CHECK(segment.intersect_vertical_segment(3, 3, 4) == false);
  GGO_CHECK(segment.intersect_vertical_segment(0, 0, 3) == false);
  GGO_CHECK(segment.intersect_vertical_segment(6, 0, 4) == false);
  
  segment = ggo::segment_float(3, 1, 6, 4);

  ggo::point2d_float intersect;
  GGO_CHECK(segment.intersect_segment(ggo::segment_float(3, 2, 4, 3)) == false);
  GGO_CHECK(segment.intersect_segment(ggo::segment_float(0, -1, 2, -1)) == false);
  GGO_CHECK(segment.intersect_segment(ggo::segment_float(7, 3, 6, 7)) == false);
  GGO_CHECK(segment.intersect_segment(ggo::segment_float(3, 2, 5, 2), intersect) == true);
  GGO_CHECK_FABS(intersect.x(), 4);
  GGO_CHECK_FABS(intersect.y(), 2);
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(shapes2d, triangle_polygon)
{
  ggo::polygon2d_float triangle;
  triangle.add_point(0, 0);
  triangle.add_point(0, 1);
  triangle.add_point(1, 0);

  // Point inside.
  GGO_CHECK(triangle.is_point_inside(0.1f, 0.1f) == true);
  GGO_CHECK(triangle.is_point_inside(0.8f, 0.1f) == true);
  GGO_CHECK(triangle.is_point_inside(0.1f, 0.8f) == true);
  GGO_CHECK(triangle.is_point_inside(-0.1f, 0) == false);
  GGO_CHECK(triangle.is_point_inside(0, -0.1f) == false);
  GGO_CHECK(triangle.is_point_inside(0.6f, 0.6f) == false);

  // Distance to point.
  GGO_CHECK_FABS(triangle.dist_to_point(0.1f, 0.1f), 0);
  GGO_CHECK_FABS(triangle.dist_to_point(-1, 0), 1);
  GGO_CHECK_FABS(triangle.dist_to_point(-1, 1), 1);
  GGO_CHECK_FABS(triangle.dist_to_point(0, -1), 1);
  GGO_CHECK_FABS(triangle.dist_to_point(1, -1), 1);
  GGO_CHECK_FABS(triangle.dist_to_point(0, 2), 1);
  GGO_CHECK_FABS(triangle.dist_to_point(2, 0), 1);
  GGO_CHECK_FABS(triangle.dist_to_point(1, 1), std::sqrt(2) / 2);

  // Distance to segment.
  GGO_CHECK_FABS(triangle.dist_to_segment(-1, 0.5f, 1, 0.5f), 0);
  GGO_CHECK_FABS(triangle.dist_to_segment(0.5f, 1, 0.1f, 0.1f), 0);
  GGO_CHECK_FABS(triangle.dist_to_segment(0.5, -1, 0.1f, 0.1f), 0);
  GGO_CHECK_FABS(triangle.dist_to_segment(-1, 0.5f, 0.1f, 0.1f), 0);
  GGO_CHECK_FABS(triangle.dist_to_segment(-1, 0, -1, 1), 1);
  GGO_CHECK_FABS(triangle.dist_to_segment(-1, -5, -1, 5), 1);
  GGO_CHECK_FABS(triangle.dist_to_segment(-1, 0.2f, -1, 0.3f), 1);
  GGO_CHECK_FABS(triangle.dist_to_segment(1, 1, 2, 2), std::sqrt(2.f) / 2);
  GGO_CHECK_FABS(triangle.dist_to_segment(0.8f, 1.2f, 1.2f, 0.8f), std::sqrt(2.f) / 2);

  // Rectangle intersection.
  GGO_CHECK_RECT_INTERSECTION(triangle, 0.1f, 0.2f, 0.1f, 0.2f, ggo::rect_intersection::RECT_IN_SHAPE);
  GGO_CHECK_RECT_INTERSECTION(triangle, -1.f, 2, -1.f, 2, ggo::rect_intersection::SHAPE_IN_RECT);
  GGO_CHECK_RECT_INTERSECTION(triangle, 0.5f, 0.6f, 0, 3, ggo::rect_intersection::PARTIAL_OVERLAP);
  GGO_CHECK_RECT_INTERSECTION(triangle, 0.1f, 0.2f, 0.1f, 3, ggo::rect_intersection::PARTIAL_OVERLAP);
  GGO_CHECK_RECT_INTERSECTION(triangle, 0.1f, 0.2f, -1.f, 3, ggo::rect_intersection::PARTIAL_OVERLAP);
  GGO_CHECK_RECT_INTERSECTION(triangle, 2, 3, -1.f, 2, ggo::rect_intersection::DISJOINTS);
  GGO_CHECK_RECT_INTERSECTION(triangle, 2, 3, -2, -1.f, ggo::rect_intersection::DISJOINTS);
  GGO_CHECK_RECT_INTERSECTION(triangle, -2, -1.f, 0, 1.f, ggo::rect_intersection::DISJOINTS);
  GGO_CHECK_RECT_INTERSECTION(triangle, -2, -1.f, 0.5, 10, ggo::rect_intersection::DISJOINTS);
  GGO_CHECK_RECT_INTERSECTION(triangle, -2, -1, -1, 0.5, ggo::rect_intersection::DISJOINTS);
  GGO_CHECK_RECT_INTERSECTION(triangle, 0, 1, -2, -1, ggo::rect_intersection::DISJOINTS);
  GGO_CHECK_RECT_INTERSECTION(triangle, 0.2f, 0.3f, -2, -1, ggo::rect_intersection::DISJOINTS);
  GGO_CHECK_RECT_INTERSECTION(triangle, -1, 0.3f, -2, -1, ggo::rect_intersection::DISJOINTS);
  GGO_CHECK_RECT_INTERSECTION(triangle, 0.2f, 3, -2, -1, ggo::rect_intersection::DISJOINTS);
  GGO_CHECK_RECT_INTERSECTION(triangle, -1, 3, -2, -1, ggo::rect_intersection::DISJOINTS);
  GGO_CHECK_RECT_INTERSECTION(triangle, -1, 3, 2, 3, ggo::rect_intersection::DISJOINTS);
  GGO_CHECK_RECT_INTERSECTION(triangle, 2, 3, 0, 3, ggo::rect_intersection::DISJOINTS);
  GGO_CHECK_RECT_INTERSECTION(triangle, 2, 3, -1, 3, ggo::rect_intersection::DISJOINTS);
  GGO_CHECK_RECT_INTERSECTION(triangle, 1, 2, 1, 2, ggo::rect_intersection::DISJOINTS);
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(shapes2d, triangle)
{
  ggo::triangle2d_float triangle({2, 1}, {7, 2}, {3, 8});
  
  ggo::rect_float bounding_rect(triangle.get_bounding_rect());
  GGO_CHECK_FABS(bounding_rect.left(), 2);
  GGO_CHECK_FABS(bounding_rect.right(), 7);
  GGO_CHECK_FABS(bounding_rect.bottom(), 1);
  GGO_CHECK_FABS(bounding_rect.top(), 8);

  // Point inside.
  GGO_CHECK(triangle.is_point_inside(3, 2) == true);
  GGO_CHECK(triangle.is_point_inside(5, 2) == true);
  GGO_CHECK(triangle.is_point_inside(4, 5) == true);
  GGO_CHECK(triangle.is_point_inside(2, 2) == false);
  GGO_CHECK(triangle.is_point_inside(6, 6) == false);
  GGO_CHECK(triangle.is_point_inside(6, 1.5) == false);
  
  // Rectangle intersection.
  GGO_CHECK_RECT_INTERSECTION(triangle, 3, 4, 2, 5, ggo::rect_intersection::RECT_IN_SHAPE);
  GGO_CHECK_RECT_INTERSECTION(triangle, 1, 9, -1, 9, ggo::rect_intersection::SHAPE_IN_RECT);
  GGO_CHECK_RECT_INTERSECTION(triangle, -1, 1, 1, 9, ggo::rect_intersection::DISJOINTS);
  GGO_CHECK_RECT_INTERSECTION(triangle, 1, 9, -1, 0, ggo::rect_intersection::DISJOINTS);
  GGO_CHECK_RECT_INTERSECTION(triangle, 8, 9, 1, 9, ggo::rect_intersection::DISJOINTS);
  GGO_CHECK_RECT_INTERSECTION(triangle, 1, 9, 9, 10, ggo::rect_intersection::DISJOINTS);
  GGO_CHECK_RECT_INTERSECTION(triangle, 6, 8, 1, 1.5, ggo::rect_intersection::DISJOINTS);
  GGO_CHECK_RECT_INTERSECTION(triangle, 5, 8, 6, 8, ggo::rect_intersection::DISJOINTS);
  GGO_CHECK_RECT_INTERSECTION(triangle, 1, 1.5f, 6, 8, ggo::rect_intersection::DISJOINTS);
  GGO_CHECK_RECT_INTERSECTION(triangle, 2, 4, 7, 8, ggo::rect_intersection::PARTIAL_OVERLAP);
  GGO_CHECK_RECT_INTERSECTION(triangle, 2, 4, 2, 4, ggo::rect_intersection::PARTIAL_OVERLAP);
  GGO_CHECK_RECT_INTERSECTION(triangle, 6, 8, 1, 5, ggo::rect_intersection::PARTIAL_OVERLAP);
  GGO_CHECK_RECT_INTERSECTION(triangle, 4, 5, 5, 8, ggo::rect_intersection::PARTIAL_OVERLAP);
  GGO_CHECK_RECT_INTERSECTION(triangle, 1, 9, 4, 5, ggo::rect_intersection::PARTIAL_OVERLAP);
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(shapes2d, polygon)
{
  {
    ggo::polygon2d_float polygon;
    ggo::point2d_float p1(1, 2);
    ggo::point2d_float p2(3, 4);
    ggo::point2d_float p3(5, 6);
    polygon.add_points(p1, p2, p3);

    GGO_CHECK(polygon.get_points_count() == 3); 
    GGO_CHECK_FABS(polygon.get_point(0).x(), 1);
    GGO_CHECK_FABS(polygon.get_point(0).y(), 2);
    GGO_CHECK_FABS(polygon.get_point(1).x(), 3);
    GGO_CHECK_FABS(polygon.get_point(1).y(), 4);
    GGO_CHECK_FABS(polygon.get_point(2).x(), 5);
    GGO_CHECK_FABS(polygon.get_point(2).y(), 6);
  }

  {
    ggo::polygon2d_float polygon;
    polygon.add_point(0, 0);
    polygon.add_point(5, 0);
    polygon.add_point(1, 1);
    polygon.add_point(5, 5);
    polygon.add_point(0, 5);

    // Point inside.
    GGO_CHECK(polygon.is_point_inside(2, 0.1f) == true);
    GGO_CHECK(polygon.is_point_inside(0.1f, 0.1f) == true);
    GGO_CHECK(polygon.is_point_inside(4, 0.1f) == true);
    GGO_CHECK(polygon.is_point_inside(1, 4) == true);
    GGO_CHECK(polygon.is_point_inside(3, 4) == true);
    GGO_CHECK(polygon.is_point_inside(2, 1) == false);
    GGO_CHECK(polygon.is_point_inside(4, 3) == false);
    GGO_CHECK(polygon.is_point_inside(4, 1) == false);
    GGO_CHECK(polygon.is_point_inside(4, 6) == false);
    GGO_CHECK(polygon.is_point_inside(6, 6) == false);

    // Distance to point.
    GGO_CHECK_FABS(polygon.dist_to_point(1, 4), 0);
    GGO_CHECK_FABS(polygon.dist_to_point(0.1f, 2), 0);
    GGO_CHECK_FABS(polygon.dist_to_point(4, 3), std::sqrt(2.f) / 2);
    GGO_CHECK_FABS(polygon.dist_to_point(0, 6), 1);

    // Distance to segment.
    GGO_CHECK_FABS(polygon.dist_to_segment(0.5f, 0.5f, 0.5f, 4.5f), 0);
    GGO_CHECK_FABS(polygon.dist_to_segment(0.5f, 4.5f, 4, 4.5f), 0);
    GGO_CHECK_FABS(polygon.dist_to_segment(3, 0.1f, 3, 4), 0);
    GGO_CHECK_FABS(polygon.dist_to_segment(3, 0.1f, 3, 4), 0);
    GGO_CHECK_FABS(polygon.dist_to_segment(3, 0.1f, 3, 1), 0);
    GGO_CHECK_FABS(polygon.dist_to_segment(3, 1, 3, 4), 0);
    GGO_CHECK_FABS(polygon.dist_to_segment(3, 4, 6, 4), 0);
    GGO_CHECK_FABS(polygon.dist_to_segment(4, 3, 6, 3), std::sqrt(2.f) / 2);
    GGO_CHECK_FABS(polygon.dist_to_segment(-1, 6, -1, 7), std::sqrt(2.f));
    GGO_CHECK_FABS(polygon.dist_to_segment(-1, 4, -2, 7), 1);
    GGO_CHECK_FABS(polygon.dist_to_segment(1, -1, 1, -2), 1);
    GGO_CHECK_FABS(polygon.dist_to_segment(6, 0, 7, 0), 1);

    // Rectangle intersection.
    GGO_CHECK_RECT_INTERSECTION(polygon, 0.1f, 0.2f, 0.1f, 0.2f, ggo::rect_intersection::RECT_IN_SHAPE);
    GGO_CHECK_RECT_INTERSECTION(polygon, 1, 2, 3, 4, ggo::rect_intersection::RECT_IN_SHAPE);
    GGO_CHECK_RECT_INTERSECTION(polygon, 3, 3.5f, 4, 4.5, ggo::rect_intersection::RECT_IN_SHAPE);
    GGO_CHECK_RECT_INTERSECTION(polygon, -1, 6, -1, 6, ggo::rect_intersection::SHAPE_IN_RECT);
    GGO_CHECK_RECT_INTERSECTION(polygon, 3, 4, 1, 2, ggo::rect_intersection::DISJOINTS);
    GGO_CHECK_RECT_INTERSECTION(polygon, 1, 2, 6, 10, ggo::rect_intersection::DISJOINTS);
    GGO_CHECK_RECT_INTERSECTION(polygon, 6, 8, -2, 10, ggo::rect_intersection::DISJOINTS);
    GGO_CHECK_RECT_INTERSECTION(polygon, -1, 1, 4, 6, ggo::rect_intersection::PARTIAL_OVERLAP);
    GGO_CHECK_RECT_INTERSECTION(polygon, -1, 6, 4, 6, ggo::rect_intersection::PARTIAL_OVERLAP);
    GGO_CHECK_RECT_INTERSECTION(polygon, 4, 6, -1, 1, ggo::rect_intersection::PARTIAL_OVERLAP);
    GGO_CHECK_RECT_INTERSECTION(polygon, 0.5f, 2, 0.5f, 1, ggo::rect_intersection::PARTIAL_OVERLAP);
    GGO_CHECK_RECT_INTERSECTION(polygon, 2, 3, -1, 7, ggo::rect_intersection::PARTIAL_OVERLAP);
    GGO_CHECK_RECT_INTERSECTION(polygon, -1, 1, 3, 4, ggo::rect_intersection::PARTIAL_OVERLAP);
    GGO_CHECK_RECT_INTERSECTION(polygon, 2, 7, 3, 4, ggo::rect_intersection::PARTIAL_OVERLAP);
    GGO_CHECK_RECT_INTERSECTION(polygon, 1, 2, -1, 0.1, ggo::rect_intersection::PARTIAL_OVERLAP);
    GGO_CHECK_RECT_INTERSECTION(polygon, 1, 2, 6, 7, ggo::rect_intersection::DISJOINTS);
    GGO_CHECK_RECT_INTERSECTION(polygon, -10, -8, 3, 4, ggo::rect_intersection::DISJOINTS);
    GGO_CHECK_RECT_INTERSECTION(polygon, 4, 6, 2, 3, ggo::rect_intersection::DISJOINTS);
    GGO_CHECK_RECT_INTERSECTION(polygon, 3, 8, 1.5f, 2, ggo::rect_intersection::DISJOINTS);
    GGO_CHECK_RECT_INTERSECTION(polygon, 6, 8, -1, 1, ggo::rect_intersection::DISJOINTS);
  }
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(shapes2d, circle)
{
  {
    ggo::circle_float circle(ggo::point2d_float(3, 2), 1);

    {
      auto points = circle.intersect_line(ggo::point2d_float(0, 0), ggo::point2d_float(1, 1), false);
      GGO_CHECK(points.size() == 2);
      GGO_CHECK(find_point(points, ggo::point2d_float(2, 2)) == true);
      GGO_CHECK(find_point(points, ggo::point2d_float(3, 3)) == true);
    }

    {
      auto points = circle.intersect_segment(ggo::point2d_float(0, 0), ggo::point2d_float(2.5, 2.5));
      GGO_CHECK(points.size() == 1);
      GGO_CHECK(find_point(points, ggo::point2d_float(2, 2)) == true);
    }

    {
      auto points = circle.intersect_segment(ggo::point2d_float(4, 4), ggo::point2d_float(2.5, 2.5));
      GGO_CHECK(points.size() == 1);
      GGO_CHECK(find_point(points, ggo::point2d_float(3, 3)) == true);
    }

    {
      auto points = circle.intersect_segment(ggo::point2d_float(0, 0), ggo::point2d_float(1, 1));
      GGO_CHECK(points.empty() == true);
    }

    {
      auto points = circle.intersect_segment(ggo::point2d_float(0, 0), ggo::point2d_float(10, 10));
      GGO_CHECK(points.size() == 2);
      GGO_CHECK(find_point(points, ggo::point2d_float(2, 2)) == true);
      GGO_CHECK(find_point(points, ggo::point2d_float(3, 3)) == true);
    }
  }

  {
    ggo::circle_float circle(ggo::point2d_float(3, 1), 1);

    {
      auto points = circle.intersect_line(ggo::point2d_float(0, 0), ggo::point2d_float(2, 1), false);
      GGO_CHECK(points.size() == 2);
      GGO_CHECK(find_point(points, ggo::point2d_float(2, 1)) == true);
    }

    {
      auto points = circle.intersect_segment(ggo::point2d_float(0, 0), ggo::point2d_float(1, 0.5)); 
      GGO_CHECK(points.empty() == true);
    }
  }
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(shapes2d, disc)
{
  ggo::disc_float disc(ggo::point2d_float(2, 12), 10);

  // Bounding rectangle.
  ggo::rect_float rect = disc.get_bounding_rect();
  GGO_CHECK_FABS(rect.left(), -8);
  GGO_CHECK_FABS(rect.right(), 12);
  GGO_CHECK_FABS(rect.top(), 22);
  GGO_CHECK_FABS(rect.bottom(), 2);

  // Point inside.
  GGO_CHECK(disc.is_point_inside(disc.center()) == true);
  GGO_CHECK(disc.is_point_inside(0, 10) == true);
  GGO_CHECK(disc.is_point_inside(2, 1) == false);
  GGO_CHECK(disc.is_point_inside(3, 2) == false);
  GGO_CHECK(disc.is_point_inside(8, 21) == false);
  GGO_CHECK(disc.is_point_inside(8, 21) == false);
  GGO_CHECK(disc.is_point_inside(-6, 21) == false);
  GGO_CHECK(disc.is_point_inside(-4, -3) == false);

  // Distance to point.
  GGO_CHECK_FABS(disc.dist_to_point(disc.center()), 0);
  GGO_CHECK_FABS(disc.dist_to_point(0, 10), 0);
  GGO_CHECK_FABS(disc.dist_to_point(14, 12), 2);
  GGO_CHECK_FABS(disc.dist_to_point(-10, 12), 2);
  GGO_CHECK_FABS(disc.dist_to_point(2, 0), 2);
  GGO_CHECK_FABS(disc.dist_to_point(2, 24), 2);
  GGO_CHECK_FABS(disc.dist_to_point(2, 24), 2);
  GGO_CHECK_FABS(disc.dist_to_point(22, 36), ggo::distance(disc.center(), ggo::point2d_float(22, 36)) - disc.radius());
  GGO_CHECK_FABS(disc.dist_to_point(-10, 100), ggo::distance(disc.center(), ggo::point2d_float(-10, 100)) - disc.radius());
  GGO_CHECK_FABS(disc.dist_to_point(0, -50), ggo::distance(disc.center(), ggo::point2d_float(0, -50)) - disc.radius());

  // Rectangle intersection.
  GGO_CHECK_RECT_INTERSECTION(disc, 0, 1, 11, 12, ggo::rect_intersection::RECT_IN_SHAPE);
  GGO_CHECK_RECT_INTERSECTION(disc, -20, 20, 0, 40, ggo::rect_intersection::SHAPE_IN_RECT);
  GGO_CHECK_RECT_INTERSECTION(disc, -20, -18, 0, 40, ggo::rect_intersection::DISJOINTS);
  GGO_CHECK_RECT_INTERSECTION(disc, 13, 14, 0, 40, ggo::rect_intersection::DISJOINTS);
  GGO_CHECK_RECT_INTERSECTION(disc, 0, 20, 23, 24, ggo::rect_intersection::DISJOINTS);
  GGO_CHECK_RECT_INTERSECTION(disc, 13, 14, 0, 20, ggo::rect_intersection::DISJOINTS);
  GGO_CHECK_RECT_INTERSECTION(disc, 10, 11, 3, 4, ggo::rect_intersection::DISJOINTS);
  GGO_CHECK_RECT_INTERSECTION(disc, 10, 11, 20, 21, ggo::rect_intersection::DISJOINTS);
  GGO_CHECK_RECT_INTERSECTION(disc, -7, -6, 20, 21, ggo::rect_intersection::DISJOINTS);
  GGO_CHECK_RECT_INTERSECTION(disc, -7, -6, 3, 4, ggo::rect_intersection::DISJOINTS);
  GGO_CHECK_RECT_INTERSECTION(disc, -10, -6, 0, 20, ggo::rect_intersection::PARTIAL_OVERLAP);
  GGO_CHECK_RECT_INTERSECTION(disc, 0, 10, 4, 12, ggo::rect_intersection::PARTIAL_OVERLAP);
  GGO_CHECK_RECT_INTERSECTION(disc, 0, 10, 18, 20, ggo::rect_intersection::PARTIAL_OVERLAP);
  GGO_CHECK_RECT_INTERSECTION(disc, -6, -4, 18, 20, ggo::rect_intersection::PARTIAL_OVERLAP);
  GGO_CHECK_RECT_INTERSECTION(disc, -6, -4, 4, 12, ggo::rect_intersection::PARTIAL_OVERLAP);
  GGO_CHECK_RECT_INTERSECTION(disc, -6, 10, 4, 20, ggo::rect_intersection::PARTIAL_OVERLAP);
  GGO_CHECK_RECT_INTERSECTION(disc, 2, 12, 4, 20, ggo::rect_intersection::PARTIAL_OVERLAP);
  GGO_CHECK_RECT_INTERSECTION(disc, 0, 2, 0, 40, ggo::rect_intersection::PARTIAL_OVERLAP);
  GGO_CHECK_RECT_INTERSECTION(disc, -10, 20, 10, 24, ggo::rect_intersection::PARTIAL_OVERLAP);
  GGO_CHECK_RECT_INTERSECTION(disc, -10, 20, 0, 10, ggo::rect_intersection::PARTIAL_OVERLAP);
  GGO_CHECK_RECT_INTERSECTION(disc, 0, 20, 10, 12, ggo::rect_intersection::PARTIAL_OVERLAP);
  GGO_CHECK_RECT_INTERSECTION(disc, -20, 0, 10, 12, ggo::rect_intersection::PARTIAL_OVERLAP);
  GGO_CHECK_RECT_INTERSECTION(disc, -10, 10, -2, 0, ggo::rect_intersection::DISJOINTS);
  GGO_CHECK_RECT_INTERSECTION(disc, 14, 16, 0, 12, ggo::rect_intersection::DISJOINTS);
  GGO_CHECK_RECT_INTERSECTION(disc, 0, 4, 23, 24, ggo::rect_intersection::DISJOINTS);
  GGO_CHECK_RECT_INTERSECTION(disc, -20, -10, 0, 12, ggo::rect_intersection::DISJOINTS);
  GGO_CHECK_RECT_INTERSECTION(disc, 10, 12, 0, 4, ggo::rect_intersection::DISJOINTS);
  GGO_CHECK_RECT_INTERSECTION(disc, 10, 12, 21, 22, ggo::rect_intersection::DISJOINTS);
  GGO_CHECK_RECT_INTERSECTION(disc, -10, -6, 21, 22, ggo::rect_intersection::DISJOINTS);
  GGO_CHECK_RECT_INTERSECTION(disc, -10, -8, -4, -2, ggo::rect_intersection::DISJOINTS);
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(shapes2d, rect_data)
{
  {
    auto rect = ggo::rect_data_union(ggo::rect_data<float>{ {1.f, -1.f}, 2.f, 3.f },
                                     ggo::rect_data<float>{ {2.f, 1.f}, 2.f, 2.f });

    GGO_CHECK_FABS(rect._pos.x(), 1);
    GGO_CHECK_FABS(rect._pos.y(), -1);
    GGO_CHECK_FABS(rect._width, 3);
    GGO_CHECK_FABS(rect._height, 4);
  }
  
  {
    ggo::rect_data<float> result;
    ggo::rect_data<float> rect_data1{ { 1.f, 2.f }, 2.f, 1.f };
    ggo::rect_data<float> rect_data2{ { 4.f, 1.f }, 2.f, 3.f };

    GGO_CHECK(ggo::rect_data_intersection(rect_data1, rect_data2, result) == false);
    GGO_CHECK(ggo::rect_data_intersection(rect_data2, rect_data1, result) == false);
  }

  {
    ggo::rect_data<float> result;
    ggo::rect_data<float> rect_data1{ { 2.f, 1.f }, 2.f, 1.f };
    ggo::rect_data<float> rect_data2{ { 1.f, 3.f }, 5.f, 1.f };

    GGO_CHECK(ggo::rect_data_intersection(rect_data1, rect_data2, result) == false);
    GGO_CHECK(ggo::rect_data_intersection(rect_data2, rect_data1, result) == false);
  }

  {
    ggo::rect_data<float> result;
    ggo::rect_data<float> rect_data1{ { 1.f, -1.f }, 2.f, 6.f };
    ggo::rect_data<float> rect_data2{ { 2.f, 1.f }, 3.f, 2.f };

    GGO_CHECK(ggo::rect_data_intersection(rect_data1, rect_data2, result) == true);
    GGO_CHECK_FABS(result._pos.x(), 2);
    GGO_CHECK_FABS(result._pos.y(), 1);
    GGO_CHECK_FABS(result._width, 1);
    GGO_CHECK_FABS(result._height, 2);

    GGO_CHECK(ggo::rect_data_intersection(rect_data2, rect_data1, result) == true);
    GGO_CHECK_FABS(result._pos.x(), 2);
    GGO_CHECK_FABS(result._pos.y(), 1);
    GGO_CHECK_FABS(result._width, 1);
    GGO_CHECK_FABS(result._height, 2);
  }

  {
    ggo::rect_data<float> result;
    ggo::rect_data<float> rect_data1{ { 2.f, 2.f }, 5.f, 3.f };
    ggo::rect_data<float> rect_data2{ { 3.f, 1.f }, 2.f, 2.f };

    GGO_CHECK(ggo::rect_data_intersection(rect_data1, rect_data2, result) == true);
    GGO_CHECK_FABS(result._pos.x(), 3);
    GGO_CHECK_FABS(result._pos.y(), 2);
    GGO_CHECK_FABS(result._width, 2);
    GGO_CHECK_FABS(result._height, 1);

    GGO_CHECK(ggo::rect_data_intersection(rect_data2, rect_data1, result) == true);
    GGO_CHECK_FABS(result._pos.x(), 3);
    GGO_CHECK_FABS(result._pos.y(), 2);
    GGO_CHECK_FABS(result._width, 2);
    GGO_CHECK_FABS(result._height, 1);
  }
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(shapes2d, rectangle)
{
  // Construction
  {
    ggo::rect_float rect(ggo::rect_data<float>{ {1, 2}, 3.f, 4.f });
    GGO_CHECK_FABS(rect.left(), 1);
    GGO_CHECK_FABS(rect.bottom(), 2);
    GGO_CHECK_FABS(rect.width(), 3);
    GGO_CHECK_FABS(rect.height(), 4);
  }
  
  ggo::rect_float rect(ggo::rect_data<float>{ {3, 1}, 3, 2 });

  // Point inside.
  GGO_CHECK(rect.is_point_inside(4, 2) == true);
  GGO_CHECK(rect.is_point_inside(2, 2) == false);
  GGO_CHECK(rect.is_point_inside(2, 4) == false);
  GGO_CHECK(rect.is_point_inside(4, 4) == false);
  GGO_CHECK(rect.is_point_inside(10, 10) == false);
  GGO_CHECK(rect.is_point_inside(10, 2) == false);
  GGO_CHECK(rect.is_point_inside(10, -1) == false);
  GGO_CHECK(rect.is_point_inside(4, 0) == false);
  GGO_CHECK(rect.is_point_inside(-1, -1) == false);

  // Distance to point.
  GGO_CHECK_FABS(rect.dist_to_point(4, 2), 0);
  GGO_CHECK_FABS(rect.dist_to_point(0, 2), 3);
  GGO_CHECK_FABS(rect.dist_to_point(2, 4), std::sqrt(2));
  GGO_CHECK_FABS(rect.dist_to_point(4, 4), 1);
  GGO_CHECK_FABS(rect.dist_to_point(7, 4), std::sqrt(2));
  GGO_CHECK_FABS(rect.dist_to_point(7, 2), 1);
  GGO_CHECK_FABS(rect.dist_to_point(7, 0), std::sqrt(2));
  GGO_CHECK_FABS(rect.dist_to_point(4, -1), 2);
  GGO_CHECK_FABS(rect.dist_to_point(2, 0), std::sqrt(2));

  // Rectangle intersection.
  GGO_CHECK_RECT_INTERSECTION(rect, 4, 5, 1.5, 2.5, ggo::rect_intersection::RECT_IN_SHAPE);
  GGO_CHECK_RECT_INTERSECTION(rect, 0, 10, 0, 4, ggo::rect_intersection::SHAPE_IN_RECT);
  GGO_CHECK_RECT_INTERSECTION(rect, 0, 4, 0, 10, ggo::rect_intersection::PARTIAL_OVERLAP);
  GGO_CHECK_RECT_INTERSECTION(rect, 0, 4, 2, 10, ggo::rect_intersection::PARTIAL_OVERLAP);
  GGO_CHECK_RECT_INTERSECTION(rect, 0, 4, 0, 2, ggo::rect_intersection::PARTIAL_OVERLAP);
  GGO_CHECK_RECT_INTERSECTION(rect, 4, 5, 0, 4, ggo::rect_intersection::PARTIAL_OVERLAP);
  GGO_CHECK_RECT_INTERSECTION(rect, 4, 5, 2, 4, ggo::rect_intersection::PARTIAL_OVERLAP);
  GGO_CHECK_RECT_INTERSECTION(rect, 4, 5, 0, 2, ggo::rect_intersection::PARTIAL_OVERLAP);
  GGO_CHECK_RECT_INTERSECTION(rect, 4, 7, 0, 4, ggo::rect_intersection::PARTIAL_OVERLAP);
  GGO_CHECK_RECT_INTERSECTION(rect, 4, 7, 2, 4, ggo::rect_intersection::PARTIAL_OVERLAP);
  GGO_CHECK_RECT_INTERSECTION(rect, 4, 7, 0, 2, ggo::rect_intersection::PARTIAL_OVERLAP);
  GGO_CHECK_RECT_INTERSECTION(rect, 0, 10, 0, 2, ggo::rect_intersection::PARTIAL_OVERLAP);
  GGO_CHECK_RECT_INTERSECTION(rect, 0, 5, 0, 2, ggo::rect_intersection::PARTIAL_OVERLAP);
  GGO_CHECK_RECT_INTERSECTION(rect, 5, 10, 0, 2, ggo::rect_intersection::PARTIAL_OVERLAP);
  GGO_CHECK_RECT_INTERSECTION(rect, 0, 10, 1.5, 2, ggo::rect_intersection::PARTIAL_OVERLAP);
  GGO_CHECK_RECT_INTERSECTION(rect, 0, 5, 1.5, 2, ggo::rect_intersection::PARTIAL_OVERLAP);
  GGO_CHECK_RECT_INTERSECTION(rect, 5, 10, 1.5, 2, ggo::rect_intersection::PARTIAL_OVERLAP);
  GGO_CHECK_RECT_INTERSECTION(rect, 0, 10, 2, 4, ggo::rect_intersection::PARTIAL_OVERLAP);
  GGO_CHECK_RECT_INTERSECTION(rect, 0, 5, 2, 4, ggo::rect_intersection::PARTIAL_OVERLAP);
  GGO_CHECK_RECT_INTERSECTION(rect, 5, 10, 2, 4, ggo::rect_intersection::PARTIAL_OVERLAP);
  GGO_CHECK_RECT_INTERSECTION(rect, 0, 1, 0, 10, ggo::rect_intersection::DISJOINTS);
  GGO_CHECK_RECT_INTERSECTION(rect, 0, 1, 6, 10, ggo::rect_intersection::DISJOINTS);
  GGO_CHECK_RECT_INTERSECTION(rect, 0, 1, -2, -1, ggo::rect_intersection::DISJOINTS);
  GGO_CHECK_RECT_INTERSECTION(rect, 0, 10, 4, 5, ggo::rect_intersection::DISJOINTS);
  GGO_CHECK_RECT_INTERSECTION(rect, 0, 10, -1, 0, ggo::rect_intersection::DISJOINTS);
  GGO_CHECK_RECT_INTERSECTION(rect, 7, 10, 1, 3, ggo::rect_intersection::DISJOINTS);
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(shapes2d, extended_segment)
{
  {
    ggo::extended_segment_float extended_segment(ggo::point2d_float(2, 1), ggo::point2d_float(6, 2), 1);

    // Bounding rectangle.
    ggo::rect_float rect = extended_segment.get_bounding_rect();
    GGO_CHECK_FABS(rect.left(), 1);
    GGO_CHECK_FABS(rect.right(), 7);
    GGO_CHECK_FABS(rect.top(), 3);
    GGO_CHECK_FABS(rect.bottom(), 0);

    // Point inside.
    GGO_CHECK(extended_segment.is_point_inside(1.2f, 1) == true);
    GGO_CHECK(extended_segment.is_point_inside(4, 2) == true);
    GGO_CHECK(extended_segment.is_point_inside(6.5, 2) == true);
    GGO_CHECK(extended_segment.is_point_inside(1, 0) == false);
    GGO_CHECK(extended_segment.is_point_inside(1, 2) == false);
    GGO_CHECK(extended_segment.is_point_inside(7, 1) == false);
    GGO_CHECK(extended_segment.is_point_inside(7, 3) == false);
    GGO_CHECK(extended_segment.is_point_inside(4, 3) == false);

    // Distance to point.
    GGO_CHECK_FABS(extended_segment.dist_to_point(1.2f, 1), 0);
    GGO_CHECK_FABS(extended_segment.dist_to_point(4, 2), 0);
    GGO_CHECK_FABS(extended_segment.dist_to_point(6.5, 2), 0);
    GGO_CHECK_FABS(extended_segment.dist_to_point(-1, 1), 2);
    GGO_CHECK_FABS(extended_segment.dist_to_point(2, -2), 2);
    GGO_CHECK_FABS(extended_segment.dist_to_point(1, 2), std::sqrt(2) - 1);
  }

  {
    ggo::extended_segment_float extended_segment(ggo::point2d_float(6, 5), ggo::point2d_float(3, 2), std::sqrt(2.f));

    // Bounding rectangle.
    ggo::rect_float rect = extended_segment.get_bounding_rect();
    GGO_CHECK_FABS(rect.left(), 3 - std::sqrt(2));
    GGO_CHECK_FABS(rect.right(), 6 + std::sqrt(2));
    GGO_CHECK_FABS(rect.top(), 5 + std::sqrt(2));
    GGO_CHECK_FABS(rect.bottom(), 2 - std::sqrt(2));

    // Point inside.
    GGO_CHECK(extended_segment.is_point_inside(2, 2) == true);
    GGO_CHECK(extended_segment.is_point_inside(5, 3) == true);
    GGO_CHECK(extended_segment.is_point_inside(7, 5) == true);
    GGO_CHECK(extended_segment.is_point_inside(1, 1) == false);
    GGO_CHECK(extended_segment.is_point_inside(8, 5) == false);
    GGO_CHECK(extended_segment.is_point_inside(3, 5) == false);

    // Distance to point.
    GGO_CHECK_FABS(extended_segment.dist_to_point(2, 2), 0);
    GGO_CHECK_FABS(extended_segment.dist_to_point(5, 3), 0);
    GGO_CHECK_FABS(extended_segment.dist_to_point(7, 5), 0);
    GGO_CHECK_FABS(extended_segment.dist_to_point(1, 0), std::sqrt(2));
    GGO_CHECK_FABS(extended_segment.dist_to_point(6, 1), std::sqrt(2));
    GGO_CHECK_FABS(extended_segment.dist_to_point(3, 5), std::sqrt(2) / 2);
    GGO_CHECK_FABS(extended_segment.dist_to_point(8, 5), 2 - std::sqrt(2));

    // Rectangle intersection.
    GGO_CHECK_RECT_INTERSECTION(extended_segment, 3, 4, 2, 3, ggo::rect_intersection::RECT_IN_SHAPE);
    GGO_CHECK_RECT_INTERSECTION(extended_segment, 0, 10, 0, 10, ggo::rect_intersection::SHAPE_IN_RECT);
    GGO_CHECK_RECT_INTERSECTION(extended_segment, 0, 3, 0, 2, ggo::rect_intersection::PARTIAL_OVERLAP);
    GGO_CHECK_RECT_INTERSECTION(extended_segment, 3, 6, 0, 7, ggo::rect_intersection::PARTIAL_OVERLAP);
    GGO_CHECK_RECT_INTERSECTION(extended_segment, 7, 8, 5, 6, ggo::rect_intersection::PARTIAL_OVERLAP);
    GGO_CHECK_RECT_INTERSECTION(extended_segment, 0, 10, 1, 10, ggo::rect_intersection::PARTIAL_OVERLAP);
    GGO_CHECK_RECT_INTERSECTION(extended_segment, 0, 7, 0, 10, ggo::rect_intersection::PARTIAL_OVERLAP);
    GGO_CHECK_RECT_INTERSECTION(extended_segment, 0, 10, 5.1f, 5.2f, ggo::rect_intersection::PARTIAL_OVERLAP);
    GGO_CHECK_RECT_INTERSECTION(extended_segment, 5.5f, 6, 5.5f, 6.5f, ggo::rect_intersection::PARTIAL_OVERLAP);
    GGO_CHECK_RECT_INTERSECTION(extended_segment, 0, 2, 4, 5, ggo::rect_intersection::DISJOINTS);
    GGO_CHECK_RECT_INTERSECTION(extended_segment, 0, 1, 0, 1, ggo::rect_intersection::DISJOINTS);
    GGO_CHECK_RECT_INTERSECTION(extended_segment, 6, 8, 0, 2, ggo::rect_intersection::DISJOINTS);
    GGO_CHECK_RECT_INTERSECTION(extended_segment, 8, 9, 0, 10, ggo::rect_intersection::DISJOINTS);
    GGO_CHECK_RECT_INTERSECTION(extended_segment, 4, 10, 7, 8, ggo::rect_intersection::DISJOINTS);
  }
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(shapes2d, multi_shape)
{
  // Union.
  {
    ggo::multi_shape_float multi_shape;
    multi_shape.add_shapes(std::make_shared<ggo::disc_float>(2.f, 1.f, 3.f), std::make_shared<ggo::disc_float>(5.f, 3.f, 2.f));

    // Bounding rectangle.
    ggo::rect_float rect = multi_shape.get_bounding_rect();
    GGO_CHECK_FABS(rect.left(), -1);
    GGO_CHECK_FABS(rect.right(), 7);
    GGO_CHECK_FABS(rect.bottom(), -2);
    GGO_CHECK_FABS(rect.top(), 5);

    // Point inside.
    GGO_CHECK(multi_shape.is_point_inside(0, 0) == true);
    GGO_CHECK(multi_shape.is_point_inside(6, 4) == true);
    GGO_CHECK(multi_shape.is_point_inside(3, 5) == false);
    GGO_CHECK(multi_shape.is_point_inside(5, 0) == false);

    // Rectangle intersection.
    GGO_CHECK_RECT_INTERSECTION(multi_shape, 1, 2, 5, 6, ggo::rect_intersection::DISJOINTS);
    GGO_CHECK_RECT_INTERSECTION(multi_shape, 1, 2, 1, 3, ggo::rect_intersection::RECT_IN_SHAPE);
    GGO_CHECK_RECT_INTERSECTION(multi_shape, 4, 6, 2, 4, ggo::rect_intersection::RECT_IN_SHAPE);
    GGO_CHECK_RECT_INTERSECTION(multi_shape, -2, 8, -3, 8, ggo::rect_intersection::SHAPE_IN_RECT);
    GGO_CHECK_RECT_INTERSECTION(multi_shape, 2, 5, 2, 3, ggo::rect_intersection::UNKNOWN);
  }

  // Intersection.
  {
    ggo::multi_shape<float, ggo::boolean_mode::INTERSECTION> multi_shape;
    multi_shape.add_shapes(std::make_shared<ggo::disc_float>(2.f, 1.f, 3.f), std::make_shared<ggo::disc_float>(3.f, 3.f, 2.f));

    // Bounding rectangle.
    ggo::rect_float rect = multi_shape.get_bounding_rect();
    GGO_CHECK_FABS(rect.left(), 1);
    GGO_CHECK_FABS(rect.right(), 5);
    GGO_CHECK_FABS(rect.bottom(), 1);
    GGO_CHECK_FABS(rect.top(), 4);

    // Point inside.
    GGO_CHECK(multi_shape.is_point_inside(0, 0) == false);
    GGO_CHECK(multi_shape.is_point_inside(1, 1) == false);
    GGO_CHECK(multi_shape.is_point_inside(5, 1) == false);
    GGO_CHECK(multi_shape.is_point_inside(4, 4) == false);
    GGO_CHECK(multi_shape.is_point_inside(2, 2) == true);
    GGO_CHECK(multi_shape.is_point_inside(4, 2) == true);

    // Rectangle intersection.
    GGO_CHECK_RECT_INTERSECTION(multi_shape, 0, 2, 0, 1, ggo::rect_intersection::DISJOINTS);
    GGO_CHECK_RECT_INTERSECTION(multi_shape, 4, 8, 4, 8, ggo::rect_intersection::DISJOINTS);
    GGO_CHECK_RECT_INTERSECTION(multi_shape, 2, 3, 2, 3, ggo::rect_intersection::RECT_IN_SHAPE);
    GGO_CHECK_RECT_INTERSECTION(multi_shape, 2, 4, 2, 4, ggo::rect_intersection::UNKNOWN);
    GGO_CHECK_RECT_INTERSECTION(multi_shape, -2, 8, -3, 8, ggo::rect_intersection::SHAPE_IN_RECT);
  }

  // Difference.
  {
    ggo::multi_shape<float, ggo::boolean_mode::DIFFERENCE> multi_shape;
    auto rect1 = std::make_shared<ggo::rect_float>(ggo::rect_data<float>{ {2.f, 1.f}, 7.f, 7.f });
    auto rect2 = std::make_shared<ggo::rect_float>(ggo::rect_data<float>{ {3.f, 4.f}, 3.f, 3.f });
    auto rect3 = std::make_shared<ggo::rect_float>(ggo::rect_data<float>{ {7.f, 2.f}, 5.f, 5.f });
    multi_shape.add_shapes(rect1, rect2, rect3);

    // Bounding rectangle.
    ggo::rect_float rect = multi_shape.get_bounding_rect();
    GGO_CHECK_FABS(rect.left(), 2);
    GGO_CHECK_FABS(rect.right(), 9);
    GGO_CHECK_FABS(rect.bottom(), 1);
    GGO_CHECK_FABS(rect.top(), 8);

    // Point inside.
    GGO_CHECK(multi_shape.is_point_inside(0, 0) == false);
    GGO_CHECK(multi_shape.is_point_inside(1, 1) == false);
    GGO_CHECK(multi_shape.is_point_inside(3, 3) == true);
    GGO_CHECK(multi_shape.is_point_inside(4, 2) == true);
    GGO_CHECK(multi_shape.is_point_inside(4, 6) == false);
    GGO_CHECK(multi_shape.is_point_inside(8, 4) == false);

    // Rectangle intersection.
    GGO_CHECK_RECT_INTERSECTION(multi_shape, 0, 1, 0, 1, ggo::rect_intersection::DISJOINTS);
    GGO_CHECK_RECT_INTERSECTION(multi_shape, 1, 10, 0, 10, ggo::rect_intersection::SHAPE_IN_RECT);
    GGO_CHECK_RECT_INTERSECTION(multi_shape, 3, 4, 2, 3, ggo::rect_intersection::RECT_IN_SHAPE);
    GGO_CHECK_RECT_INTERSECTION(multi_shape, 4, 5, 3, 5, ggo::rect_intersection::UNKNOWN);
    GGO_CHECK_RECT_INTERSECTION(multi_shape, 4, 5, 5, 6, ggo::rect_intersection::DISJOINTS);
  }
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(shapes2d, half_plane)
{
  ggo::half_plane<float> half_plane({ 1.f, 1.f }, 1.f);

  GGO_CHECK(half_plane.is_point_inside(0.f, 0.f) == true);
  GGO_CHECK(half_plane.is_point_inside(1.f, 1.f) == false);
  GGO_CHECK(half_plane.is_point_inside(2.f, 0.f) == false);
  GGO_CHECK(half_plane.is_point_inside(0.f, 2.f) == false);
  GGO_CHECK(half_plane.is_point_inside(-1.f, 0.f) == true);
  GGO_CHECK(half_plane.is_point_inside(0.f, -1.f) == true);

  GGO_CHECK_FABS(half_plane.dist_to_point(0, 0), 0);
  GGO_CHECK_FABS(half_plane.dist_to_point(0, 1), 0);
  GGO_CHECK_FABS(half_plane.dist_to_point(1, 0), 0);
  GGO_CHECK_FABS(half_plane.dist_to_point(1, 1), std::sqrt(2.f) - 1.f);
  GGO_CHECK_FABS(half_plane.dist_to_point(2, 0), std::sqrt(2.f) - 1.f);
  GGO_CHECK_FABS(half_plane.dist_to_point(0, 2), std::sqrt(2.f) - 1.f);
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(shapes2d, oriented_box)
{
  ggo::oriented_box<float> box({ 3.f, 2.f }, { 2.f, 0.f }, 2.f, 1.f);

  // Check second direction.
  GGO_CHECK(box.dir().is_normalized());

  // Check second direction.
  GGO_CHECK(box.dir2().is_normalized());
  GGO_CHECK_FABS(ggo::dot(box.dir(), box.dir2()), 0.f);

  // Vertices.
  auto points = box.get_points();
  GGO_CHECK(find_point(points, 1.0f, 1.0f));
  GGO_CHECK(find_point(points, 1.0f, 3.0f));
  GGO_CHECK(find_point(points, 5.0f, 1.0f));
  GGO_CHECK(find_point(points, 5.0f, 3.0f));

  // Rotation.
  box.rotate(ggo::PI<float>() / 4.f, { 1.f, 1.f });
  auto points2 = box.get_points();
  GGO_CHECK(find_point(points2, 1.0f, 1.0f));
  GGO_CHECK(find_point(points2, 1.0f - std::sqrt(2.f), 1.0f + std::sqrt(2.f)));
  GGO_CHECK(find_point(points2, 1.0f + 2.f * std::sqrt(2.f), 1.0f + 2.f * std::sqrt(2.f)));
  GGO_CHECK(find_point(points2, 1.0f + std::sqrt(2.f), 1.0f + 3.0f * std::sqrt(2.f)));
}
