#include "../../ggo_kernel_nonreg.h"

/////////////////////////////////////////////////////////////////////
GGO_TEST(oriented_box, construction)
{
  ggo::oriented_box<float> box({ 3.f, 2.f }, { 2.f, 0.f }, 2.f, 1.f);

  // Check second direction.
  GGO_CHECK(ggo::is_normalized(box.dir()));

  // Check second direction.
  GGO_CHECK(ggo::is_normalized(box.dir2()));
  GGO_CHECK_FLOAT_EQ(ggo::dot(box.dir(), box.dir2()), 0.f);
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(oriented_box, vertices)
{
  ggo::oriented_box<float> box({ 3.f, 2.f }, { 2.f, 0.f }, 2.f, 1.f);

  auto points = box.get_points();
  GGO_CHECK(find_point(points, 1.0f, 1.0f));
  GGO_CHECK(find_point(points, 1.0f, 3.0f));
  GGO_CHECK(find_point(points, 5.0f, 1.0f));
  GGO_CHECK(find_point(points, 5.0f, 3.0f));
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(oriented_box, is_point_inside)
{
  ggo::oriented_box<float> box({ 3.f, 2.f }, { 2.f, 0.f }, 2.f, 1.f);

  GGO_CHECK(box.is_point_inside({ 4.f, 1.5f }) == true);
  GGO_CHECK(box.is_point_inside({ 4.f, 2.5f }) == true);
  GGO_CHECK(box.is_point_inside({ 4.5f, 1.5f }) == true);
  GGO_CHECK(box.is_point_inside({ 2.5f, 2.5f }) == true);
  GGO_CHECK(box.is_point_inside({ 3.f, 3.5f }) == false);
  GGO_CHECK(box.is_point_inside({ 3.f, 0.5f }) == false);
  GGO_CHECK(box.is_point_inside({ 0.5f, 2.f }) == false);
  GGO_CHECK(box.is_point_inside({ 5.5f, 2.f }) == false);
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(oriented_box, rotation)
{
  ggo::oriented_box<float> box({ 3.f, 2.f }, { 2.f, 0.f }, 2.f, 1.f);

  box.rotate(ggo::pi<float>() / 4.f, { 1.f, 1.f });
  auto points2 = box.get_points();
  GGO_CHECK(find_point(points2, 1.0f, 1.0f));
  GGO_CHECK(find_point(points2, 1.0f - std::sqrt(2.f), 1.0f + std::sqrt(2.f)));
  GGO_CHECK(find_point(points2, 1.0f + 2.f * std::sqrt(2.f), 1.0f + 2.f * std::sqrt(2.f)));
  GGO_CHECK(find_point(points2, 1.0f + std::sqrt(2.f), 1.0f + 3.0f * std::sqrt(2.f)));
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(oriented_box, bounding_rect)
{
  ggo::oriented_box<float> box({ 1.f, 2.f }, { 2.f, 1.f }, std::sqrt(5.f), sqrt(1.25f));

  auto bounding_rect = box.get_bounding_rect();

  GGO_CHECK_FLOAT_EQ(bounding_rect._pos.x(), -1.5f);
  GGO_CHECK_FLOAT_EQ(bounding_rect._pos.y(), 0.f);
  GGO_CHECK_FLOAT_EQ(bounding_rect._width, 5.f);
  GGO_CHECK_FLOAT_EQ(bounding_rect._height, 4.f);
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(oriented_box, rect_intersection)
{
  ggo::oriented_box<float> box({ 1.f, 2.f }, { 2.f, 1.f }, std::sqrt(5.f), sqrt(1.25f));

  GGO_CHECK(box.get_rect_intersection(ggo::rect_data_from_left_right_bottom_top(0.f, 1.f, 1.f, 2.f)) == ggo::rect_intersection::rect_in_shape);
  GGO_CHECK(box.get_rect_intersection(ggo::rect_data_from_left_right_bottom_top(-2.f, 4.f, -1.f, 5.f)) == ggo::rect_intersection::shape_in_rect);
  GGO_CHECK(box.get_rect_intersection(ggo::rect_data_from_left_right_bottom_top(-3.f, -2.f, 0.f, 1.f)) == ggo::rect_intersection::disjoints);
  GGO_CHECK(box.get_rect_intersection(ggo::rect_data_from_left_right_bottom_top(4.f, 5.f, -1.f, 5.f)) == ggo::rect_intersection::disjoints);
  GGO_CHECK(box.get_rect_intersection(ggo::rect_data_from_left_right_bottom_top(0.f, 1.f, -2.f, -1.f)) == ggo::rect_intersection::disjoints);
  GGO_CHECK(box.get_rect_intersection(ggo::rect_data_from_left_right_bottom_top(0.f, 1.f, 4.f, 5.f)) == ggo::rect_intersection::disjoints);
  GGO_CHECK(box.get_rect_intersection(ggo::rect_data_from_left_right_bottom_top(-2.f, -1.5f, 0.f, 1.f)) == ggo::rect_intersection::disjoints);
  GGO_CHECK(box.get_rect_intersection(ggo::rect_data_from_left_right_bottom_top(3.f, 4.f, 3.5f, 4.f)) == ggo::rect_intersection::disjoints);
  GGO_CHECK(box.get_rect_intersection(ggo::rect_data_from_left_right_bottom_top(-1.f, 0.f, 3.f, 4.f)) == ggo::rect_intersection::disjoints);
  GGO_CHECK(box.get_rect_intersection(ggo::rect_data_from_left_right_bottom_top(2.f, 3.f, 0.f, 1.f)) == ggo::rect_intersection::disjoints);
  GGO_CHECK(box.get_rect_intersection(ggo::rect_data_from_left_right_bottom_top(-2.f, -1.f, 1.f, 3.f)) == ggo::rect_intersection::partial_overlap);
  GGO_CHECK(box.get_rect_intersection(ggo::rect_data_from_left_right_bottom_top(1.f, 2.f, 3.f, 4.f)) == ggo::rect_intersection::partial_overlap);
  GGO_CHECK(box.get_rect_intersection(ggo::rect_data_from_left_right_bottom_top(1.f, 2.f, 0.f, 5.f)) == ggo::rect_intersection::partial_overlap);
}

