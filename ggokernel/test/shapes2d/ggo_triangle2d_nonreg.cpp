#include "../ggo_kernel_nonreg.h"

/////////////////////////////////////////////////////////////////////
GGO_TEST(triangle, bounding_rect)
{
  ggo::triangle2d_float triangle({ 2.f, 1.f }, { 7.f, 2.f }, { 3.f, 8.f });

  ggo::rect_float bounding_rect(triangle.get_bounding_rect());
  GGO_CHECK_FLOAT_EQ(bounding_rect.left(), 2);
  GGO_CHECK_FLOAT_EQ(bounding_rect.right(), 7);
  GGO_CHECK_FLOAT_EQ(bounding_rect.bottom(), 1);
  GGO_CHECK_FLOAT_EQ(bounding_rect.top(), 8);
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(triangle, is_point_inside)
{
  ggo::triangle2d_float triangle({ 2.f, 1.f }, { 7.f, 2.f }, { 3.f, 8.f });

  GGO_CHECK(triangle.is_point_inside({ 3, 2 }) == true);
  GGO_CHECK(triangle.is_point_inside({ 5, 2 }) == true);
  GGO_CHECK(triangle.is_point_inside({ 4, 5 }) == true);
  GGO_CHECK(triangle.is_point_inside({ 2, 2 }) == false);
  GGO_CHECK(triangle.is_point_inside({ 6, 6 }) == false);
  GGO_CHECK(triangle.is_point_inside({ 6, 1.5 }) == false);
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(triangle, rect_intersection)
{
  ggo::triangle2d_float triangle({ 2.f, 1.f }, { 7.f, 2.f }, { 3.f, 8.f });

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
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(triangle, triangle_intersection)
{
  ggo::triangle2d_float triangle({ 2.f, 1.f }, { 7.f, 2.f }, { 3.f, 8.f });

  GGO_CHECK(ggo::get_triangle_intersection<float>({ { 1.f, 1.f },{ 7.f, 1.f },{ 1.f, 5.f } }, { { 2.f, 2.f },{ 3.f, 3.f },{ 2.f, 3.f } }) == ggo::triangle_intersection::triangle2_in_triangle1);
  GGO_CHECK(ggo::get_triangle_intersection<float>({ { 2.f, 2.f },{ 3.f, 3.f },{ 2.f, 3.f } }, { { 1.f, 1.f },{ 7.f, 1.f },{ 1.f, 5.f } }) == ggo::triangle_intersection::triangle1_in_triangle2);
  GGO_CHECK(ggo::get_triangle_intersection<float>({ { 1.f, 1.f },{ 7.f, 1.f },{ 1.f, 5.f } }, { { 6.f, 3.f },{ 7.f, 3.f },{ 6.f, 4.f } }) == ggo::triangle_intersection::disjoints);
  GGO_CHECK(ggo::get_triangle_intersection<float>({ { 1.f, 1.f },{ 7.f, 1.f },{ 1.f, 5.f } }, { { 4.f, 2.f },{ 5.f, 4.f },{ 4.f, 4.f } }) == ggo::triangle_intersection::partial_overlap);
  GGO_CHECK(ggo::get_triangle_intersection<float>({ { 2.f, 1.f },{ 5.f, 3.f },{ 2.f, 5.f } }, { { 4.f, 5.f },{ 5.f, 6.f },{ 4.f, 7.f } }) == ggo::triangle_intersection::disjoints);
  GGO_CHECK(ggo::get_triangle_intersection<float>({ { 145.f, 145.f },{ 176.f, 70.f },{ 245.f, 126.f } }, { { 175.f, 93.f },{ 245.f, 126.f },{ 253.f, 51.f } }) == ggo::triangle_intersection::partial_overlap);
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(triangle, area)
{
  ggo::triangle2d_float triangle({ 2.f, 1.f }, { 7.f, 2.f }, { 3.f, 8.f });

  auto check_area = [&](float x1, float y1, float x2, float y2, float x3, float y3, float expected_area)
  {
    ggo::triangle2d_float triangle({ x1, y1 }, { x2, y2 }, { x3, y3 });
    GGO_CHECK_FLOAT_EQ(triangle.area(), expected_area);
  };
  check_area(2.0f, 2.0f, 4.0f, 2.0f, 5.0f, 3.0f, 1.0f);
  check_area(4.0f, 2.0f, 2.0f, 2.0f, 5.0f, 3.0f, 1.0f);
  check_area(4.0f, 2.0f, 5.0f, 3.0f, 2.0f, 2.0f, 1.0f);
  check_area(2.0f, 1.0f, 5.0f, 2.0f, 3.0f, 6.0f, 7.0f);
}
