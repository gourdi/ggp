#include "../ggo_kernel_nonreg.h"
#include <ggo_shapes2d.h>

/////////////////////////////////////////////////////////////////////
#define GGO_CHECK_RECT_INTERSECTION(shape, left, right, bottom, top, res) \
  { ggo::rect_data<float> rect_data {{static_cast<float>(left), static_cast<float>(bottom)}, \
                                      static_cast<float>(right - left), \
                                      static_cast<float>(top - bottom)}; \
  GGO_CHECK(shape.get_rect_intersection(rect_data) == res); }

/////////////////////////////////////////////////////////////////////
GGO_TEST(shapes2d, basis2d)
{
  // Rotation.
  {
    ggo::basis2d<float> basis({ 2.f, 0.f }, { 1.f, 0.f }, { 0.f, 1.f });
    basis.rotate({ 1.f, 0.f }, ggo::pi<float>() / 2.f);

    GGO_CHECK_FABS(basis.pos().x(), 1.f);
    GGO_CHECK_FABS(basis.pos().y(), 1.f);
    GGO_CHECK_FABS(basis.x().x(), 0.f);
    GGO_CHECK_FABS(basis.x().y(), 1.f);
    GGO_CHECK_FABS(basis.y().x(), -1.f);
    GGO_CHECK_FABS(basis.y().y(), 0.f);
  }

  // From local to world.
  {
    ggo::basis2d<float> basis({ 2.f, 1.f }, { 3.f, 1.f }, { -1.f, 2.f });

    ggo::pos2f p1 = basis.point_from_local_to_world({ 1.f, 2.f });
    GGO_CHECK_FABS(p1.x(), 3.f);
    GGO_CHECK_FABS(p1.y(), 6.f);

    ggo::pos2f p2 = basis.point_from_world_to_local({ 3.f, 6.f });
    GGO_CHECK_FABS(p2.x(), 1.f);
    GGO_CHECK_FABS(p2.y(), 2.f);
  }
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(shapes2d, orthogonal_basis2d)
{
  // Rotation.
  {
    ggo::orthogonal_basis2d<float> basis({ 2.f, 0.f }, { 2.f, 0.f });
    basis.rotate({ 1.f, 0.f }, ggo::pi<float>() / 2.f);

    GGO_CHECK_FABS(basis.pos().x(), 1.f);
    GGO_CHECK_FABS(basis.pos().y(), 1.f);
    GGO_CHECK_FABS(basis.x().x(), 0.f);
    GGO_CHECK_FABS(basis.x().y(), 2.f);
    GGO_CHECK_FABS(basis.y().x(), -2.f);
    GGO_CHECK_FABS(basis.y().y(), 0.f);
  }

  // From local to world.
  {
    ggo::orthogonal_basis2d<float> basis({ 2.f, 1.f }, { 3.f, 1.f });

    ggo::pos2f p1 = basis.point_from_local_to_world({ 1.f, 2.f });
    GGO_CHECK_FABS(p1.x(), 3.f);
    GGO_CHECK_FABS(p1.y(), 8.f);

    ggo::pos2f p2 = basis.point_from_world_to_local({ 7.f, 6.f });
    GGO_CHECK_FABS(p2.x(), 2.f);
    GGO_CHECK_FABS(p2.y(), 1.f);
  }
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(shapes2d, orthonormal_basis2d)
{
  // Rotation.
  {
    ggo::orthonormal_basis2d<float> basis({ 2.f, 0.f }, { 2.f, 0.f });
    basis.rotate({ 1.f, 0.f }, ggo::pi<float>() / 2.f);

    GGO_CHECK_FABS(basis.pos().x(), 1.f);
    GGO_CHECK_FABS(basis.pos().y(), 1.f);
    GGO_CHECK_FABS(basis.x().x(), 0.f);
    GGO_CHECK_FABS(basis.x().y(), 1.f);
    GGO_CHECK_FABS(basis.y().x(), -1.f);
    GGO_CHECK_FABS(basis.y().y(), 0.f);
  }

  // From local to world.
  {
    ggo::orthonormal_basis2d<float> basis({ 2.f, 1.f }, { 0.f, 1.f });

    ggo::pos2f p1 = basis.point_from_local_to_world({ 3.f, 1.f });
    GGO_CHECK_FABS(p1.x(), 1.f);
    GGO_CHECK_FABS(p1.y(), 4.f);

    ggo::pos2f p2 = basis.point_from_world_to_local({ 1.f, 4.f });
    GGO_CHECK_FABS(p2.x(), 3.0f);
    GGO_CHECK_FABS(p2.y(), 1.0f);
  }
}

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

  ggo::pos2f intersect;
  GGO_CHECK(segment.intersect_segment(ggo::segment_float(3, 2, 4, 3)) == false);
  GGO_CHECK(segment.intersect_segment(ggo::segment_float(0, -1, 2, -1)) == false);
  GGO_CHECK(segment.intersect_segment(ggo::segment_float(7, 3, 6, 7)) == false);
  GGO_CHECK(segment.intersect_segment(ggo::segment_float(3, 2, 5, 2), intersect) == true);
  GGO_CHECK_FABS(intersect.x(), 4);
  GGO_CHECK_FABS(intersect.y(), 2);

  segment = ggo::segment_float({ 2.f, 1.f }, { 4.f, 3.f });
  GGO_CHECK_FABS(segment.dist_to_point(1.f, 1.f), 1.f);
  GGO_CHECK_FABS(segment.dist_to_point(4.f, 5.f), 2.f);
  GGO_CHECK_FABS(segment.hypot_to_point(1.f, 4.f), 8.f);
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

/////////////////////////////////////////////////////////////////////
GGO_TEST(shapes2d, triangle)
{
  ggo::triangle2d_float triangle({2.f, 1.f }, {7.f, 2.f }, {3.f, 8.f });
  
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
  GGO_CHECK_RECT_INTERSECTION(triangle, 3, 4, 2, 5, ggo::rect_intersection::rect_in_shape);
  GGO_CHECK_RECT_INTERSECTION(triangle, 1, 9, -1, 9, ggo::rect_intersection::shape_in_rect);
  GGO_CHECK_RECT_INTERSECTION(triangle, -1, 1, 1, 9, ggo::rect_intersection::disjoints);
  GGO_CHECK_RECT_INTERSECTION(triangle, 1, 9, -1, 0, ggo::rect_intersection::disjoints);
  GGO_CHECK_RECT_INTERSECTION(triangle, 8, 9, 1, 9, ggo::rect_intersection::disjoints);
  GGO_CHECK_RECT_INTERSECTION(triangle, 1, 9, 9, 10, ggo::rect_intersection::disjoints);
  GGO_CHECK_RECT_INTERSECTION(triangle, 6, 8, 1, 1.5, ggo::rect_intersection::disjoints);
  GGO_CHECK_RECT_INTERSECTION(triangle, 5, 8, 6, 8, ggo::rect_intersection::disjoints);
  GGO_CHECK_RECT_INTERSECTION(triangle, 1, 1.5f, 6, 8, ggo::rect_intersection::disjoints);
  GGO_CHECK_RECT_INTERSECTION(triangle, 2, 4, 7, 8, ggo::rect_intersection::partial_overlap);
  GGO_CHECK_RECT_INTERSECTION(triangle, 2, 4, 2, 4, ggo::rect_intersection::partial_overlap);
  GGO_CHECK_RECT_INTERSECTION(triangle, 6, 8, 1, 5, ggo::rect_intersection::partial_overlap);
  GGO_CHECK_RECT_INTERSECTION(triangle, 4, 5, 5, 8, ggo::rect_intersection::partial_overlap);
  GGO_CHECK_RECT_INTERSECTION(triangle, 1, 9, 4, 5, ggo::rect_intersection::partial_overlap);

  // Triangle intersection.
  GGO_CHECK(ggo::get_triangle_intersection<float>({ { 1.f, 1.f },{ 7.f, 1.f },{ 1.f, 5.f } }, { { 2.f, 2.f },{ 3.f, 3.f },{ 2.f, 3.f } }) == ggo::triangle_intersection::triangle2_in_triangle1);
  GGO_CHECK(ggo::get_triangle_intersection<float>({ { 2.f, 2.f },{ 3.f, 3.f },{ 2.f, 3.f } }, { { 1.f, 1.f },{ 7.f, 1.f },{ 1.f, 5.f } }) == ggo::triangle_intersection::triangle1_in_triangle2);
  GGO_CHECK(ggo::get_triangle_intersection<float>({ { 1.f, 1.f },{ 7.f, 1.f },{ 1.f, 5.f } }, { { 6.f, 3.f },{ 7.f, 3.f },{ 6.f, 4.f } }) == ggo::triangle_intersection::disjoints);
  GGO_CHECK(ggo::get_triangle_intersection<float>({ { 1.f, 1.f },{ 7.f, 1.f },{ 1.f, 5.f } }, { { 4.f, 2.f },{ 5.f, 4.f },{ 4.f, 4.f } }) == ggo::triangle_intersection::partial_overlap);
  GGO_CHECK(ggo::get_triangle_intersection<float>({ { 2.f, 1.f },{ 5.f, 3.f },{ 2.f, 5.f } }, { { 4.f, 5.f },{ 5.f, 6.f },{ 4.f, 7.f } }) == ggo::triangle_intersection::disjoints);
  GGO_CHECK(ggo::get_triangle_intersection<float>({ { 145.f, 145.f },{ 176.f, 70.f },{ 245.f, 126.f } }, { { 175.f, 93.f },{ 245.f, 126.f },{ 253.f, 51.f } }) == ggo::triangle_intersection::partial_overlap);


  // Triangle area.
  auto check_area = [&](float x1, float y1, float x2, float y2, float x3, float y3, float expected_area)
  {
    ggo::triangle2d_float triangle({ x1, y1 }, { x2, y2 }, { x3, y3 });
    GGO_CHECK_FABS(triangle.area(), expected_area);
  };
  check_area(2.0f, 2.0f, 4.0f, 2.0f, 5.0f, 3.0f, 1.0f);
  check_area(4.0f, 2.0f, 2.0f, 2.0f, 5.0f, 3.0f, 1.0f);
  check_area(4.0f, 2.0f, 5.0f, 3.0f, 2.0f, 2.0f, 1.0f);
  check_area(2.0f, 1.0f, 5.0f, 2.0f, 3.0f, 6.0f, 7.0f);
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(shapes2d, polygon)
{
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
    std::vector<ggo::pos2f> points3{ { 0.f, 1.f },{ 2.f, 3.f },{4.f, 5.f} };
    polygon.set_points(points3);
    GGO_CHECK(points3.size() == 3);
    GGO_CHECK(polygon.get_points_count() == 3);
  }

  {
    ggo::polygon2d_float polygon;
    ggo::pos2f p1(1.f, 2.f);
    ggo::pos2f p2(3.f, 4.f);
    ggo::pos2f p3(5.f, 6.f);
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

/////////////////////////////////////////////////////////////////////
GGO_TEST(shapes2d, circle)
{
  {
    ggo::circle_float circle(ggo::pos2f(3.f, 2.f), 1);

    {
      auto points = circle.intersect_line(ggo::pos2f(0.f, 0.f), ggo::pos2f(1.f, 1.f), false);
      GGO_CHECK(points.size() == 2);
      GGO_CHECK(find_point(points, ggo::pos2f(2.f, 2.f)) == true);
      GGO_CHECK(find_point(points, ggo::pos2f(3.f, 3.f)) == true);
    }

    {
      auto points = circle.intersect_segment(ggo::pos2f(0.f, 0.f), ggo::pos2f(2.5f, 2.5f));
      GGO_CHECK(points.size() == 1);
      GGO_CHECK(find_point(points, ggo::pos2f(2.f, 2.f)) == true);
    }

    {
      auto points = circle.intersect_segment(ggo::pos2f(4.f, 4.f), ggo::pos2f(2.5f, 2.5f));
      GGO_CHECK(points.size() == 1);
      GGO_CHECK(find_point(points, ggo::pos2f(3.f, 3.f)) == true);
    }

    {
      auto points = circle.intersect_segment(ggo::pos2f(0.f, 0.f), ggo::pos2f(1.f, 1.f));
      GGO_CHECK(points.empty() == true);
    }

    {
      auto points = circle.intersect_segment(ggo::pos2f(0.f, 0.f), ggo::pos2f(10.f, 10.f));
      GGO_CHECK(points.size() == 2);
      GGO_CHECK(find_point(points, ggo::pos2f(2.f, 2.f)) == true);
      GGO_CHECK(find_point(points, ggo::pos2f(3.f, 3.f)) == true);
    }
  }

  {
    ggo::circle_float circle(ggo::pos2f(3.f, 1.f), 1);

    {
      auto points = circle.intersect_line(ggo::pos2f(0.f, 0.f), ggo::pos2f(2.f, 1.f), false);
      GGO_CHECK(points.size() == 2);
      GGO_CHECK(find_point(points, ggo::pos2f(2.f, 1.f)) == true);
    }

    {
      auto points = circle.intersect_segment(ggo::pos2f(0.f, 0.f), ggo::pos2f(1.f, 0.5f));
      GGO_CHECK(points.empty() == true);
    }
  }
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(shapes2d, disc)
{
  ggo::disc_float disc(ggo::pos2f(2.f, 12.f), 10.f);

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
  GGO_CHECK_FABS(disc.dist_to_point(22, 36), ggo::distance(disc.center(), ggo::pos2f(22.f, 36.f)) - disc.radius());
  GGO_CHECK_FABS(disc.dist_to_point(-10, 100), ggo::distance(disc.center(), ggo::pos2f(-10.f, 100.f)) - disc.radius());
  GGO_CHECK_FABS(disc.dist_to_point(0, -50), ggo::distance(disc.center(), ggo::pos2f(0.f, -50.f)) - disc.radius());

  // Rectangle intersection.
  GGO_CHECK_RECT_INTERSECTION(disc, 0, 1, 11, 12, ggo::rect_intersection::rect_in_shape);
  GGO_CHECK_RECT_INTERSECTION(disc, -20, 20, 0, 40, ggo::rect_intersection::shape_in_rect);
  GGO_CHECK_RECT_INTERSECTION(disc, -20, -18, 0, 40, ggo::rect_intersection::disjoints);
  GGO_CHECK_RECT_INTERSECTION(disc, 13, 14, 0, 40, ggo::rect_intersection::disjoints);
  GGO_CHECK_RECT_INTERSECTION(disc, 0, 20, 23, 24, ggo::rect_intersection::disjoints);
  GGO_CHECK_RECT_INTERSECTION(disc, 13, 14, 0, 20, ggo::rect_intersection::disjoints);
  GGO_CHECK_RECT_INTERSECTION(disc, 10, 11, 3, 4, ggo::rect_intersection::disjoints);
  GGO_CHECK_RECT_INTERSECTION(disc, 10, 11, 20, 21, ggo::rect_intersection::disjoints);
  GGO_CHECK_RECT_INTERSECTION(disc, -7, -6, 20, 21, ggo::rect_intersection::disjoints);
  GGO_CHECK_RECT_INTERSECTION(disc, -7, -6, 3, 4, ggo::rect_intersection::disjoints);
  GGO_CHECK_RECT_INTERSECTION(disc, -10, -6, 0, 20, ggo::rect_intersection::partial_overlap);
  GGO_CHECK_RECT_INTERSECTION(disc, 0, 10, 4, 12, ggo::rect_intersection::partial_overlap);
  GGO_CHECK_RECT_INTERSECTION(disc, 0, 10, 18, 20, ggo::rect_intersection::partial_overlap);
  GGO_CHECK_RECT_INTERSECTION(disc, -6, -4, 18, 20, ggo::rect_intersection::partial_overlap);
  GGO_CHECK_RECT_INTERSECTION(disc, -6, -4, 4, 12, ggo::rect_intersection::partial_overlap);
  GGO_CHECK_RECT_INTERSECTION(disc, -6, 10, 4, 20, ggo::rect_intersection::partial_overlap);
  GGO_CHECK_RECT_INTERSECTION(disc, 2, 12, 4, 20, ggo::rect_intersection::partial_overlap);
  GGO_CHECK_RECT_INTERSECTION(disc, 0, 2, 0, 40, ggo::rect_intersection::partial_overlap);
  GGO_CHECK_RECT_INTERSECTION(disc, -10, 20, 10, 24, ggo::rect_intersection::partial_overlap);
  GGO_CHECK_RECT_INTERSECTION(disc, -10, 20, 0, 10, ggo::rect_intersection::partial_overlap);
  GGO_CHECK_RECT_INTERSECTION(disc, 0, 20, 10, 12, ggo::rect_intersection::partial_overlap);
  GGO_CHECK_RECT_INTERSECTION(disc, -20, 0, 10, 12, ggo::rect_intersection::partial_overlap);
  GGO_CHECK_RECT_INTERSECTION(disc, -10, 10, -2, 0, ggo::rect_intersection::disjoints);
  GGO_CHECK_RECT_INTERSECTION(disc, 14, 16, 0, 12, ggo::rect_intersection::disjoints);
  GGO_CHECK_RECT_INTERSECTION(disc, 0, 4, 23, 24, ggo::rect_intersection::disjoints);
  GGO_CHECK_RECT_INTERSECTION(disc, -20, -10, 0, 12, ggo::rect_intersection::disjoints);
  GGO_CHECK_RECT_INTERSECTION(disc, 10, 12, 0, 4, ggo::rect_intersection::disjoints);
  GGO_CHECK_RECT_INTERSECTION(disc, 10, 12, 21, 22, ggo::rect_intersection::disjoints);
  GGO_CHECK_RECT_INTERSECTION(disc, -10, -6, 21, 22, ggo::rect_intersection::disjoints);
  GGO_CHECK_RECT_INTERSECTION(disc, -10, -8, -4, -2, ggo::rect_intersection::disjoints);
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
    ggo::rect_float rect(ggo::rect_data<float>{ {1.f, 2.f}, 3.f, 4.f });
    GGO_CHECK_FABS(rect.left(), 1);
    GGO_CHECK_FABS(rect.bottom(), 2);
    GGO_CHECK_FABS(rect.width(), 3);
    GGO_CHECK_FABS(rect.height(), 4);
  }
  
  ggo::rect_float rect(ggo::rect_data<float>{ {3.f, 1.f}, 3.f, 2.f });

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

  // Extend.
  auto check_extend = [&](const ggo::rect_float & rect, const ggo::pos2f & p, const ggo::rect_float & expected_rect)
  {
    ggo::rect_float extended_rect(rect);
    extended_rect.extend(p);

    GGO_CHECK_FABS(extended_rect.left(),   expected_rect.left());
    GGO_CHECK_FABS(extended_rect.bottom(), expected_rect.bottom());
    GGO_CHECK_FABS(extended_rect.width(),  expected_rect.width());
    GGO_CHECK_FABS(extended_rect.height(), expected_rect.height());
  };
  check_extend(rect, { 4.f, 2.f }, rect);
  check_extend(rect, { 1.f, 4.f }, ggo::rect_float::from_left_right_bottom_top(1.f, 6.f, 1.f, 4.f));
  check_extend(rect, { 7.f, -1.f }, ggo::rect_float::from_left_right_bottom_top(3.f, 7.f, -1.f, 3.f));
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(shapes2d, extended_segment)
{
  {
    ggo::extended_segment_float extended_segment(ggo::pos2f(2.f, 1.f), ggo::pos2f(6.f, 2.f), 1.f);

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
    ggo::extended_segment_float extended_segment(ggo::pos2f(6.f, 5.f), ggo::pos2f(3.f, 2.f), std::sqrt(2.f));

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
    GGO_CHECK_RECT_INTERSECTION(multi_shape, 1, 2, 5, 6, ggo::rect_intersection::disjoints);
    GGO_CHECK_RECT_INTERSECTION(multi_shape, 1, 2, 1, 3, ggo::rect_intersection::rect_in_shape);
    GGO_CHECK_RECT_INTERSECTION(multi_shape, 4, 6, 2, 4, ggo::rect_intersection::rect_in_shape);
    GGO_CHECK_RECT_INTERSECTION(multi_shape, -2, 8, -3, 8, ggo::rect_intersection::shape_in_rect);
    GGO_CHECK_RECT_INTERSECTION(multi_shape, 2, 5, 2, 3, ggo::rect_intersection::unknown);
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
    GGO_CHECK_RECT_INTERSECTION(multi_shape, 0, 2, 0, 1, ggo::rect_intersection::disjoints);
    GGO_CHECK_RECT_INTERSECTION(multi_shape, 4, 8, 4, 8, ggo::rect_intersection::disjoints);
    GGO_CHECK_RECT_INTERSECTION(multi_shape, 2, 3, 2, 3, ggo::rect_intersection::rect_in_shape);
    GGO_CHECK_RECT_INTERSECTION(multi_shape, 2, 4, 2, 4, ggo::rect_intersection::unknown);
    GGO_CHECK_RECT_INTERSECTION(multi_shape, -2, 8, -3, 8, ggo::rect_intersection::shape_in_rect);
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
    GGO_CHECK_RECT_INTERSECTION(multi_shape, 0, 1, 0, 1, ggo::rect_intersection::disjoints);
    GGO_CHECK_RECT_INTERSECTION(multi_shape, 1, 10, 0, 10, ggo::rect_intersection::shape_in_rect);
    GGO_CHECK_RECT_INTERSECTION(multi_shape, 3, 4, 2, 3, ggo::rect_intersection::rect_in_shape);
    GGO_CHECK_RECT_INTERSECTION(multi_shape, 4, 5, 3, 5, ggo::rect_intersection::unknown);
    GGO_CHECK_RECT_INTERSECTION(multi_shape, 4, 5, 5, 6, ggo::rect_intersection::disjoints);
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

  //Point inside.
  GGO_CHECK(box.is_point_inside(4.f, 1.5f) == true);
  GGO_CHECK(box.is_point_inside(4.f, 2.5f) == true);
  GGO_CHECK(box.is_point_inside(4.5f, 1.5f) == true);
  GGO_CHECK(box.is_point_inside(2.5f, 2.5f) == true);
  GGO_CHECK(box.is_point_inside(3.f, 3.5f) == false);
  GGO_CHECK(box.is_point_inside(3.f, 0.5f) == false);
  GGO_CHECK(box.is_point_inside(0.5f, 2.f) == false);
  GGO_CHECK(box.is_point_inside(5.5f, 2.f) == false);

  // Rotation.
  box.rotate(ggo::pi<float>() / 4.f, { 1.f, 1.f });
  auto points2 = box.get_points();
  GGO_CHECK(find_point(points2, 1.0f, 1.0f));
  GGO_CHECK(find_point(points2, 1.0f - std::sqrt(2.f), 1.0f + std::sqrt(2.f)));
  GGO_CHECK(find_point(points2, 1.0f + 2.f * std::sqrt(2.f), 1.0f + 2.f * std::sqrt(2.f)));
  GGO_CHECK(find_point(points2, 1.0f + std::sqrt(2.f), 1.0f + 3.0f * std::sqrt(2.f)));
}
