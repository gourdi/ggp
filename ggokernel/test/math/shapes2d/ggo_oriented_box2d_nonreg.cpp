#include "../../ggo_kernel_nonreg.h"

/////////////////////////////////////////////////////////////////////
GGO_TEST(oriented_box, construction)
{
  ggo::oriented_box_f box({ 3.f, 2.f }, 0.f, 2.f, 1.f);

  GGO_CHECK(ggo::is_normalized(box.dir_x()));
  GGO_CHECK(ggo::is_normalized(box.dir_y()));
  GGO_CHECK_FLOAT_EQ(ggo::dot(box.dir_x(), box.dir_y()), 0.f);
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(oriented_box, vertices)
{
  ggo::oriented_box_f box({ 3.f, 2.f }, 0.f, 2.f, 1.f);

  auto points = box.get_points();
  GGO_CHECK(ggo::find(points, { 1.0f, 1.0f }));
  GGO_CHECK(ggo::find(points, { 1.0f, 3.0f }));
  GGO_CHECK(ggo::find(points, { 5.0f, 1.0f }));
  GGO_CHECK(ggo::find(points, { 5.0f, 3.0f }));
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(oriented_box, is_point_inside)
{
  ggo::oriented_box_f box({ 3.f, 2.f }, 0.f, 2.f, 1.f);

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
  constexpr float tolerance = 0.0001f;

  ggo::oriented_box_f box({ 3.f, 2.f }, 0.f, 2.f, 1.f);

  box.rotate(ggo::pi<float>() / 4.f, { 1.f, 1.f });
  auto points = box.get_points();
  GGO_CHECK(ggo::find(points, { 1.0f, 1.0f }, tolerance));
  GGO_CHECK(ggo::find(points, { 1.0f - std::sqrt(2.f), 1.0f + std::sqrt(2.f) }, tolerance));
  GGO_CHECK(ggo::find(points, { 1.0f + 2.f * std::sqrt(2.f), 1.0f + 2.f * std::sqrt(2.f) }, tolerance));
  GGO_CHECK(ggo::find(points, { 1.0f + std::sqrt(2.f), 1.0f + 3.0f * std::sqrt(2.f) }, tolerance));
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(oriented_box, bounding_rect)
{
  ggo::oriented_box_f box({ 1.f, 2.f }, std::atan2(1.f, 2.f), std::sqrt(5.f), sqrt(1.25f));

  auto bounding_rect = box.get_bounding_rect();

  GGO_CHECK_FLOAT_EQ(bounding_rect._pos.x(), -1.5f);
  GGO_CHECK_FLOAT_EQ(bounding_rect._pos.y(), 0.f);
  GGO_CHECK_FLOAT_EQ(bounding_rect._width, 5.f);
  GGO_CHECK_FLOAT_EQ(bounding_rect._height, 4.f);
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(oriented_box, rect_intersection)
{
  ggo::oriented_box_f box({ 1.f, 2.f }, std::atan2(1.f, 2.f), std::sqrt(5.f), sqrt(1.25f));

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

/////////////////////////////////////////////////////////////////////
GGO_TEST(oriented_box, project)
{
  {
    ggo::oriented_box_f box({ 2.f, 4.f }, 0.f, 1, 2);

    auto proj1 = ggo::project(box, { 1.f, 0.f });
    GGO_CHECK_FLOAT_EQ(proj1._inf, 1.f);
    GGO_CHECK_FLOAT_EQ(proj1._sup, 3.f);

    auto proj2 = ggo::project(box, { 0.f, 1.f });
    GGO_CHECK_FLOAT_EQ(proj2._inf, 2.f);
    GGO_CHECK_FLOAT_EQ(proj2._sup, 6.f);

    auto proj3 = ggo::project(box, ggo::normalize<ggo::vec2_f>({ 1.f, 1.f }));
    GGO_CHECK_FLOAT_EQ(proj3._inf, 3.f / std::sqrt(2.f));
    GGO_CHECK_FLOAT_EQ(proj3._sup, 9.f / std::sqrt(2.f));
  }

  {
    ggo::oriented_box_f box({ 3.f, 4.f }, std::atan2(2.f, 1.f), 2 * std::sqrt(5.f), std::sqrt(5.f));

    auto proj1 = ggo::project(box, { 1.f, 0.f });
    GGO_CHECK_FLOAT_EQ(proj1._inf, -1.f);
    GGO_CHECK_FLOAT_EQ(proj1._sup, 7.f);

    auto proj2 = ggo::project(box, { 0.f, 1.f });
    GGO_CHECK_FLOAT_EQ(proj2._inf, -1.f);
    GGO_CHECK_FLOAT_EQ(proj2._sup, 9.f);

    auto proj3 = ggo::project(box, ggo::normalize<ggo::vec2_f>({ 1.f, 1.f }));
    GGO_CHECK_FLOAT_EQ(proj3._inf, 0.f);
    GGO_CHECK_FLOAT_EQ(proj3._sup, 14.f / std::sqrt(2.f));
  }
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(oriented_box, get_edges)
{
  ggo::oriented_box_f box({ 3, 2 }, std::atan2(1.f, 2.f), std::sqrt(5.f), 2.f * std::sqrt(5.f));

  auto edges = box.get_edges();

  GGO_CHECK_EQ(edges[0]._id, ggo::oriented_box_edge_id::left);
  GGO_CHECK(ggo::compare(edges[0]._normal, ggo::normalize<ggo::vec2_f>({ -2, -1 })));

  GGO_CHECK_EQ(edges[0]._vertices[0]._id, ggo::oriented_box_vertex_id::left_bottom);
  GGO_CHECK(ggo::compare(edges[0]._vertices[0]._pos, { 3.f, -3.f }));

  GGO_CHECK_EQ(edges[0]._vertices[1]._id, ggo::oriented_box_vertex_id::left_top);
  GGO_CHECK(ggo::compare(edges[0]._vertices[1]._pos, { -1.f, 5.f }));


  GGO_CHECK_EQ(edges[1]._id, ggo::oriented_box_edge_id::right);
  GGO_CHECK(ggo::compare(edges[1]._normal, ggo::normalize<ggo::vec2_f>({ 2, 1 })));

  GGO_CHECK_EQ(edges[1]._vertices[0]._id, ggo::oriented_box_vertex_id::right_bottom);
  GGO_CHECK(ggo::compare(edges[1]._vertices[0]._pos, { 7.f, -1.f }));

  GGO_CHECK_EQ(edges[1]._vertices[1]._id,ggo::oriented_box_vertex_id::right_top);
  GGO_CHECK(ggo::compare(edges[1]._vertices[1]._pos, { 3.f, 7.f }));


  GGO_CHECK_EQ(edges[2]._id, ggo::oriented_box_edge_id::bottom);
  GGO_CHECK(ggo::compare(edges[2]._normal, ggo::normalize<ggo::vec2_f>({ 1, -2 })));

  GGO_CHECK_EQ(edges[2]._vertices[0]._id, ggo::oriented_box_vertex_id::left_bottom);
  GGO_CHECK(ggo::compare(edges[2]._vertices[0]._pos, { 3.f, -3.f }));

  GGO_CHECK_EQ(edges[2]._vertices[1]._id, ggo::oriented_box_vertex_id::right_bottom);
  GGO_CHECK(ggo::compare(edges[2]._vertices[1]._pos, { 7.f, -1.f }));


  GGO_CHECK_EQ(edges[3]._id, ggo::oriented_box_edge_id::top);
  GGO_CHECK(ggo::compare(edges[3]._normal, ggo::normalize<ggo::vec2_f>({ -1, 2 })));

  GGO_CHECK_EQ(edges[3]._vertices[0]._id, ggo::oriented_box_vertex_id::left_top);
  GGO_CHECK(ggo::compare(edges[3]._vertices[0]._pos, { -1.f, 5.f }));

  GGO_CHECK_EQ(edges[3]._vertices[1]._id, ggo::oriented_box_vertex_id::right_top);
  GGO_CHECK(ggo::compare(edges[3]._vertices[1]._pos, { 3.f, 7.f }));
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(oriented_box, test_intersection)
{
  const std::vector<ggo::oriented_box_f> boxes1{
    { { 3, 2 }, ggo::pi_f / 4, 2 * std::sqrt(2.f), std::sqrt(2.f) },
    { { 3, 2 }, -ggo::pi_f / 4, std::sqrt(2.f), 2 * std::sqrt(2.f) } };

  const std::vector<ggo::oriented_box_f> boxes2{
  { { 5, 7 }, 0.f, 10.f, 1.f },
  { { 5, 7 }, ggo::pi_f / 2, 1.f, 10.f } };

  for (const auto & box1 : boxes1)
  {
    for (const auto & box2 : boxes2)
    {
      GGO_CHECK(ggo::test_intersection(box1, box2) == false);
      GGO_CHECK(ggo::test_intersection(box2, box1) == false);
    }
  }

  const std::vector<ggo::oriented_box_f> boxes3{
    { { 4, 5 }, 0.f, 10.f, 1.f },
    { { 4, 5 }, ggo::pi_f / 2, 1.f, 10.f } };

  for (const auto & box1 : boxes1)
  {
    for (const auto & box3 : boxes3)
    {
      GGO_CHECK(ggo::test_intersection(box1, box3) == true);
      GGO_CHECK(ggo::test_intersection(box3, box1) == true);
    }
  }
}




