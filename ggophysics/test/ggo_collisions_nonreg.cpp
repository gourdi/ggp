#include <kernel/nonreg/ggo_nonreg.h>
#include <physics/ggo_collisions.h>

//////////////////////////////////////////////////////////////////
GGO_TEST(collisions, separation_axis_no_intersection)
{
  const ggo::oriented_box_f box1{ { 0.f, 0.f }, 0.f, 2.f, 1.f };
  const ggo::oriented_box_f box2{ { 0.f, 4.f }, 0.f, 2.f, 1.f };

  auto separation = ggo::find_minimum_penetration_separating_axis(box1, box2);
  GGO_CHECK(separation.has_value() == false);
}

//////////////////////////////////////////////////////////////////
GGO_TEST(collisions, separation_axis_case1)
{
  const ggo::oriented_box_f box1{ { 0.f, 0.f }, 0.f, 2.f, 1.f };
  const ggo::oriented_box_f box2{ { 0.f, 2.f }, ggo::pi_f / 4, 1.f, 1.f };

  auto separation = ggo::find_minimum_penetration_separating_axis(box1, box2);

  GGO_CHECK(separation.has_value() == true);
  GGO_CHECK_FLOAT_EQ(separation->_penetration, std::sqrt(2.f) - 1.f);
  GGO_CHECK_FLOAT_EQ(separation->_reference_box_size_normal, 1.f);
  GGO_CHECK_FLOAT_EQ(separation->_reference_box_size_side, 2.f);
  GGO_CHECK_EQ(separation->_reference_box_id, ggo::box_id::box1);
  GGO_CHECK_EQ(separation->_reference_box, &box1);
  GGO_CHECK_EQ(separation->_incident_box, &box2);

  GGO_CHECK_EQ(separation->_reference_edge._id, ggo::oriented_box_edge_id::top);
  GGO_CHECK_FLOAT_EQ(separation->_reference_edge._normal.x(), 0.f);
  GGO_CHECK_FLOAT_EQ(separation->_reference_edge._normal.y(), 1.f);

  GGO_CHECK(separation->_reference_edge._vertices[0]._id == ggo::oriented_box_vertex_id::left_top);
  GGO_CHECK_FLOAT_EQ(separation->_reference_edge._vertices[0]._pos.x(), -2.f);
  GGO_CHECK_FLOAT_EQ(separation->_reference_edge._vertices[0]._pos.y(), 1.f);

  GGO_CHECK(separation->_reference_edge._vertices[1]._id == ggo::oriented_box_vertex_id::right_top);
  GGO_CHECK_FLOAT_EQ(separation->_reference_edge._vertices[1]._pos.x(), 2.f);
  GGO_CHECK_FLOAT_EQ(separation->_reference_edge._vertices[1]._pos.y(), 1.f);
}

//////////////////////////////////////////////////////////////////
GGO_TEST(collisions, separation_axis_case2)
{
  const ggo::oriented_box_f box1{ { 0.f, 0.f }, ggo::pi_f / 2, 1.f, 2.f };
  const ggo::oriented_box_f box2{ { 0.f, 2.f }, ggo::pi_f / 4, 1.f, 1.f };

  auto separation = ggo::find_minimum_penetration_separating_axis(box1, box2);

  GGO_CHECK(separation.has_value() == true);
  GGO_CHECK_FLOAT_EQ(separation->_penetration, std::sqrt(2.f) - 1.f);
  GGO_CHECK_FLOAT_EQ(separation->_reference_box_size_normal, 1.f);
  GGO_CHECK_FLOAT_EQ(separation->_reference_box_size_side, 2.f);
  GGO_CHECK_EQ(separation->_reference_box_id, ggo::box_id::box1); 
  GGO_CHECK_EQ(separation->_reference_box, &box1);
  GGO_CHECK_EQ(separation->_incident_box, &box2);

  GGO_CHECK_EQ(separation->_reference_edge._id, ggo::oriented_box_edge_id::right);
  GGO_CHECK_FLOAT_EQ(separation->_reference_edge._normal.x(), 0.f);
  GGO_CHECK_FLOAT_EQ(separation->_reference_edge._normal.y(), 1.f);

  GGO_CHECK(separation->_reference_edge._vertices[0]._id == ggo::oriented_box_vertex_id::right_bottom);
  GGO_CHECK_FLOAT_EQ(separation->_reference_edge._vertices[0]._pos.x(), 2.f);
  GGO_CHECK_FLOAT_EQ(separation->_reference_edge._vertices[0]._pos.y(), 1.f);

  GGO_CHECK(separation->_reference_edge._vertices[1]._id == ggo::oriented_box_vertex_id::right_top);
  GGO_CHECK_FLOAT_EQ(separation->_reference_edge._vertices[1]._pos.x(), -2.f);
  GGO_CHECK_FLOAT_EQ(separation->_reference_edge._vertices[1]._pos.y(), 1.f);
}

//////////////////////////////////////////////////////////////////
GGO_TEST(collisions, separation_axis_case3)
{
  const ggo::oriented_box_f box1{ { 0.f, 2.f }, ggo::pi_f / 4, 1.f, 1.f };
  const ggo::oriented_box_f box2{ { 0.f, 0.f }, 0.f, 2.f, 1.f };

  auto separation = ggo::find_minimum_penetration_separating_axis(box1, box2);

  GGO_CHECK(separation.has_value() == true);
  GGO_CHECK_FLOAT_EQ(separation->_penetration, std::sqrt(2.f) - 1.f);
  GGO_CHECK_FLOAT_EQ(separation->_reference_box_size_normal, 1.f);
  GGO_CHECK_FLOAT_EQ(separation->_reference_box_size_side, 2.f);
  GGO_CHECK_EQ(separation->_reference_box_id, ggo::box_id::box2); 
  GGO_CHECK_EQ(separation->_reference_box, &box2);
  GGO_CHECK_EQ(separation->_incident_box, &box1);

  GGO_CHECK_EQ(separation->_reference_edge._id, ggo::oriented_box_edge_id::top);
  GGO_CHECK_FLOAT_EQ(separation->_reference_edge._normal.x(), 0.f);
  GGO_CHECK_FLOAT_EQ(separation->_reference_edge._normal.y(), 1.f);

  GGO_CHECK(separation->_reference_edge._vertices[0]._id == ggo::oriented_box_vertex_id::left_top);
  GGO_CHECK_FLOAT_EQ(separation->_reference_edge._vertices[0]._pos.x(), -2.f);
  GGO_CHECK_FLOAT_EQ(separation->_reference_edge._vertices[0]._pos.y(), 1.f);

  GGO_CHECK(separation->_reference_edge._vertices[1]._id == ggo::oriented_box_vertex_id::right_top);
  GGO_CHECK_FLOAT_EQ(separation->_reference_edge._vertices[1]._pos.x(), 2.f);
  GGO_CHECK_FLOAT_EQ(separation->_reference_edge._vertices[1]._pos.y(), 1.f);
}

//////////////////////////////////////////////////////////////////
GGO_TEST(collisions, separation_axis_case4)
{
  const ggo::oriented_box_f box1{ { 4.f, 4.f }, ggo::pi_f / 4, 1.f, 1.f };
  const ggo::oriented_box_f box2{ { 4.f, 2.f }, ggo::pi_f / 2, 1.f, 2.f };

  auto separation = ggo::find_minimum_penetration_separating_axis(box1, box2);

  GGO_CHECK(separation.has_value() == true);
  GGO_CHECK_FLOAT_EQ(separation->_penetration, std::sqrt(2.f) - 1.f);
  GGO_CHECK_FLOAT_EQ(separation->_reference_box_size_normal, 1.f);
  GGO_CHECK_FLOAT_EQ(separation->_reference_box_size_side, 2.f);
  GGO_CHECK_EQ(separation->_reference_box_id, ggo::box_id::box2);
  GGO_CHECK_EQ(separation->_reference_box, &box2);
  GGO_CHECK_EQ(separation->_incident_box, &box1);

  GGO_CHECK_EQ(separation->_reference_edge._id, ggo::oriented_box_edge_id::right);
  GGO_CHECK_FLOAT_EQ(separation->_reference_edge._normal.x(), 0.f);
  GGO_CHECK_FLOAT_EQ(separation->_reference_edge._normal.y(), 1.f);

  GGO_CHECK(separation->_reference_edge._vertices[0]._id == ggo::oriented_box_vertex_id::right_bottom);
  GGO_CHECK_FLOAT_EQ(separation->_reference_edge._vertices[0]._pos.x(), 6.f);
  GGO_CHECK_FLOAT_EQ(separation->_reference_edge._vertices[0]._pos.y(), 3.f);

  GGO_CHECK(separation->_reference_edge._vertices[1]._id == ggo::oriented_box_vertex_id::right_top);
  GGO_CHECK_FLOAT_EQ(separation->_reference_edge._vertices[1]._pos.x(), 2.f);
  GGO_CHECK_FLOAT_EQ(separation->_reference_edge._vertices[1]._pos.y(), 3.f);
}

//////////////////////////////////////////////////////////////////
GGO_TEST(collisions, separation_axis_case5)
{
  const ggo::oriented_box_f box1{ { 0.f, 0.f }, ggo::pi_f, 2.f, 1.f };
  const ggo::oriented_box_f box2{ { 0.f, 2.f }, -3 * ggo::pi_f / 4, 1.f, 1.f };

  auto separation = ggo::find_minimum_penetration_separating_axis(box1, box2);

  GGO_CHECK(separation.has_value() == true);
  GGO_CHECK_FLOAT_EQ(separation->_penetration, std::sqrt(2.f) - 1.f);
  GGO_CHECK_FLOAT_EQ(separation->_reference_box_size_normal, 1.f);
  GGO_CHECK_FLOAT_EQ(separation->_reference_box_size_side, 2.f);
  GGO_CHECK_EQ(separation->_reference_box_id, ggo::box_id::box1);
  GGO_CHECK_EQ(separation->_reference_box, &box1);
  GGO_CHECK_EQ(separation->_incident_box, &box2);

  GGO_CHECK_EQ(separation->_reference_edge._id, ggo::oriented_box_edge_id::bottom);
  GGO_CHECK_FLOAT_EQ(separation->_reference_edge._normal.x(), 0.f);
  GGO_CHECK_FLOAT_EQ(separation->_reference_edge._normal.y(), 1.f);

  GGO_CHECK(separation->_reference_edge._vertices[0]._id == ggo::oriented_box_vertex_id::left_bottom);
  GGO_CHECK_FLOAT_EQ(separation->_reference_edge._vertices[0]._pos.x(), 2.f);
  GGO_CHECK_FLOAT_EQ(separation->_reference_edge._vertices[0]._pos.y(), 1.f);

  GGO_CHECK(separation->_reference_edge._vertices[1]._id == ggo::oriented_box_vertex_id::right_bottom);
  GGO_CHECK_FLOAT_EQ(separation->_reference_edge._vertices[1]._pos.x(), -2.f);
  GGO_CHECK_FLOAT_EQ(separation->_reference_edge._vertices[1]._pos.y(), 1.f);
}

//////////////////////////////////////////////////////////////////
GGO_TEST(collisions, incident_edge_case1)
{
  ggo::oriented_box_f box{ { 0.f, 0.f }, 0.f, 1.f, 2.f };
  auto edge = find_incident_edge({ 0.f, 1.f }, box);

  GGO_CHECK_EQ(edge[0]._id, ggo::oriented_box_vertex_id::left_bottom);
  GGO_CHECK_FLOAT_EQ(edge[0]._pos.x(), -1.f);
  GGO_CHECK_FLOAT_EQ(edge[0]._pos.y(), -2.f);

  GGO_CHECK_EQ(edge[1]._id, ggo::oriented_box_vertex_id::right_bottom);
  GGO_CHECK_FLOAT_EQ(edge[1]._pos.x(), 1.f);
  GGO_CHECK_FLOAT_EQ(edge[1]._pos.y(), -2.f);
}

//////////////////////////////////////////////////////////////////
GGO_TEST(collisions, incident_edge_case2)
{
  ggo::oriented_box_f box{ { 0.f, 0.f }, ggo::pi_f / 2, 1.f, 2.f };
  auto edge = find_incident_edge({ 0.f, 1.f }, box);

  GGO_CHECK_EQ(edge[0]._id, ggo::oriented_box_vertex_id::left_bottom);
  GGO_CHECK_FLOAT_EQ(edge[0]._pos.x(), 2.f);
  GGO_CHECK_FLOAT_EQ(edge[0]._pos.y(), -1.f);

  GGO_CHECK_EQ(edge[1]._id, ggo::oriented_box_vertex_id::left_top);
  GGO_CHECK_FLOAT_EQ(edge[1]._pos.x(), -2.f);
  GGO_CHECK_FLOAT_EQ(edge[1]._pos.y(), -1.f);
}

//////////////////////////////////////////////////////////////////
GGO_TEST(collisions, incident_edge_case3)
{
  ggo::oriented_box_f box{ { 0.f, 0.f }, 0.f, 1.f, 2.f };
  auto edge = find_incident_edge({ 0.f, -1.f }, box);

  GGO_CHECK_EQ(edge[0]._id, ggo::oriented_box_vertex_id::left_top);
  GGO_CHECK_FLOAT_EQ(edge[0]._pos.x(), -1.f);
  GGO_CHECK_FLOAT_EQ(edge[0]._pos.y(), 2.f);

  GGO_CHECK_EQ(edge[1]._id, ggo::oriented_box_vertex_id::right_top);
  GGO_CHECK_FLOAT_EQ(edge[1]._pos.x(), 1.f);
  GGO_CHECK_FLOAT_EQ(edge[1]._pos.y(), 2.f);
}

//////////////////////////////////////////////////////////////////
GGO_TEST(collisions, incident_edge_case4)
{
  ggo::oriented_box_f box{ { 0.f, 0.f }, ggo::pi_f / 2, 1.f, 2.f };
  auto edge = find_incident_edge({ 0.f, -1.f }, box);

  GGO_CHECK_EQ(edge[0]._id, ggo::oriented_box_vertex_id::right_bottom);
  GGO_CHECK_FLOAT_EQ(edge[0]._pos.x(), 2.f);
  GGO_CHECK_FLOAT_EQ(edge[0]._pos.y(), 1.f);

  GGO_CHECK_EQ(edge[1]._id, ggo::oriented_box_vertex_id::right_top);
  GGO_CHECK_FLOAT_EQ(edge[1]._pos.x(), -2.f);
  GGO_CHECK_FLOAT_EQ(edge[1]._pos.y(), 1.f);
}

//////////////////////////////////////////////////////////////////
GGO_TEST(collisions, clip_incident_edge_case1)
{
  const std::array<ggo::oriented_box_f::vertex, 2> incident_edge{ {
    { { -2.f, 0.f }, ggo::oriented_box_vertex_id::left_bottom },
    { { 2.f, 0.f }, ggo::oriented_box_vertex_id::left_top } } };

  auto edge = ggo::clip_incident_edge({ 0.f, 1.f }, { 0.f, 0.f }, 1.f, incident_edge);

  GGO_CHECK_EQ(edge[0]._id, ggo::oriented_box_vertex_id::left_bottom);
  GGO_CHECK_FLOAT_EQ(edge[0]._pos.x(), -1.f);
  GGO_CHECK_FLOAT_EQ(edge[0]._pos.y(), 0.f);

  GGO_CHECK_EQ(edge[1]._id, ggo::oriented_box_vertex_id::left_top);
  GGO_CHECK_FLOAT_EQ(edge[1]._pos.x(), 1.f);
  GGO_CHECK_FLOAT_EQ(edge[1]._pos.y(), 0.f);
}

//////////////////////////////////////////////////////////////////
GGO_TEST(collisions, clip_incident_edge_case2)
{
  const std::array<ggo::oriented_box_f::vertex, 2> incident_edge{ {
    { { 5.f, 2.f }, ggo::oriented_box_vertex_id::left_bottom },
    { { 5.f, 6.f }, ggo::oriented_box_vertex_id::left_top } } };

  auto edge = ggo::clip_incident_edge({ 1.f, 0.f }, { 2.f, 4.f }, 1.f, incident_edge);

  GGO_CHECK_EQ(edge[0]._id, ggo::oriented_box_vertex_id::left_bottom);
  GGO_CHECK_FLOAT_EQ(edge[0]._pos.x(), 5.f);
  GGO_CHECK_FLOAT_EQ(edge[0]._pos.y(), 3.f);

  GGO_CHECK_EQ(edge[1]._id, ggo::oriented_box_vertex_id::left_top);
  GGO_CHECK_FLOAT_EQ(edge[1]._pos.x(), 5.f);
  GGO_CHECK_FLOAT_EQ(edge[1]._pos.y(), 5.f);
}

//////////////////////////////////////////////////////////////////
GGO_TEST(collisions, clip_incident_edge_case3)
{
  const std::array<ggo::oriented_box_f::vertex, 2> incident_edge{ {
    { { 3.f, 2.f }, ggo::oriented_box_vertex_id::left_bottom },
    { { 7.f, 6.f }, ggo::oriented_box_vertex_id::left_top } } };

  auto edge = ggo::clip_incident_edge({ 1.f, 0.f }, { 2.f, 4.f }, 1.f, incident_edge);

  GGO_CHECK_EQ(edge[0]._id, ggo::oriented_box_vertex_id::left_bottom);
  GGO_CHECK_FLOAT_EQ(edge[0]._pos.x(), 4.f);
  GGO_CHECK_FLOAT_EQ(edge[0]._pos.y(), 3.f);

  GGO_CHECK_EQ(edge[1]._id, ggo::oriented_box_vertex_id::left_top);
  GGO_CHECK_FLOAT_EQ(edge[1]._pos.x(), 6.f);
  GGO_CHECK_FLOAT_EQ(edge[1]._pos.y(), 5.f);
}

//////////////////////////////////////////////////////////////////
GGO_TEST(collisions, clip_incident_edge_case4)
{
  const std::array<ggo::oriented_box_f::vertex, 2> incident_edge{ {
    { { 3.f, 6.f }, ggo::oriented_box_vertex_id::left_bottom },
    { { 7.f, 2.f }, ggo::oriented_box_vertex_id::left_top } } };

  auto edge = ggo::clip_incident_edge({ 1.f, 0.f }, { 2.f, 4.f }, 1.f, incident_edge);

  GGO_CHECK_EQ(edge[0]._id, ggo::oriented_box_vertex_id::left_bottom);
  GGO_CHECK_FLOAT_EQ(edge[0]._pos.x(), 4.f);
  GGO_CHECK_FLOAT_EQ(edge[0]._pos.y(), 5.f);

  GGO_CHECK_EQ(edge[1]._id, ggo::oriented_box_vertex_id::left_top);
  GGO_CHECK_FLOAT_EQ(edge[1]._pos.x(), 6.f);
  GGO_CHECK_FLOAT_EQ(edge[1]._pos.y(), 3.f);
}

//////////////////////////////////////////////////////////////////
GGO_TEST(collisions, clip_incident_edge_case5)
{
  const std::array<ggo::oriented_box_f::vertex, 2> incident_edge{ {
    { { 5.f, 3.f }, ggo::oriented_box_vertex_id::left_bottom },
    { { 4.f, 6.f }, ggo::oriented_box_vertex_id::left_top } } };

  auto edge = ggo::clip_incident_edge({ 1.f / std::sqrt(2.f), 1.f / std::sqrt(2.f) }, { 3.f, 3.f }, 2.f, incident_edge);

  GGO_CHECK_EQ(edge[0]._id, ggo::oriented_box_vertex_id::left_bottom);
  GGO_CHECK_FLOAT_EQ(edge[0]._pos.x(), 5.f);
  GGO_CHECK_FLOAT_EQ(edge[0]._pos.y(), 3.f);

  GGO_CHECK_EQ(edge[1]._id, ggo::oriented_box_vertex_id::left_top);
  GGO_CHECK_FLOAT_EQ(edge[1]._pos.x(), 4.f);
  GGO_CHECK_FLOAT_EQ(edge[1]._pos.y(), 6.f);
}

//////////////////////////////////////////////////////////////////
GGO_TEST(collisions, discard_outside_points_case1)
{
  const std::array<ggo::oriented_box_f::vertex, 2> incident_edge{ {
    { { 1.f, 4.f }, ggo::oriented_box_vertex_id::left_bottom },
    { { 4.f, 4.f }, ggo::oriented_box_vertex_id::left_top } } };

  auto contacts = ggo::discard_outside_contacts({ 3.f, 1.f }, { 0.f, 1.f }, 2.f, incident_edge);

  GGO_CHECK_EQ(contacts._count, 0);
}

//////////////////////////////////////////////////////////////////
GGO_TEST(collisions, discard_outside_points_case2)
{
  const std::array<ggo::oriented_box_f::vertex, 2> incident_edge{ {
    { { 1.f, 2.f }, ggo::oriented_box_vertex_id::left_bottom },
    { { 4.f, 4.f }, ggo::oriented_box_vertex_id::left_top } } };

  auto contacts = ggo::discard_outside_contacts({ 3.f, 1.f }, { 0.f, 1.f }, 2.f, incident_edge);

  GGO_CHECK_EQ(contacts._count, 1);

  GGO_CHECK_EQ(contacts._vertices[0]._vertex._id, ggo::oriented_box_vertex_id::left_bottom);
  GGO_CHECK_FLOAT_EQ(contacts._vertices[0]._vertex._pos.x(), 1.f);
  GGO_CHECK_FLOAT_EQ(contacts._vertices[0]._vertex._pos.y(), 2.f);
  GGO_CHECK_FLOAT_EQ(contacts._vertices[0]._penetration, 1.f);
}

//////////////////////////////////////////////////////////////////
GGO_TEST(collisions, discard_outside_points_case3)
{
  const std::array<ggo::oriented_box_f::vertex, 2> incident_edge{ {
    { { 1.f, 2.f }, ggo::oriented_box_vertex_id::left_bottom },
    { { 4.f, 1.f }, ggo::oriented_box_vertex_id::left_top } } };

  auto contacts = ggo::discard_outside_contacts({ 3.f, 1.f }, { 0.f, 1.f }, 2.f, incident_edge);

  GGO_CHECK_EQ(contacts._count, 2);

  GGO_CHECK_EQ(contacts._vertices[0]._vertex._id, ggo::oriented_box_vertex_id::left_bottom);
  GGO_CHECK_FLOAT_EQ(contacts._vertices[0]._vertex._pos.x(), 1.f);
  GGO_CHECK_FLOAT_EQ(contacts._vertices[0]._vertex._pos.y(), 2.f);
  GGO_CHECK_FLOAT_EQ(contacts._vertices[0]._penetration, 1.f);

  GGO_CHECK_EQ(contacts._vertices[1]._vertex._id, ggo::oriented_box_vertex_id::left_top);
  GGO_CHECK_FLOAT_EQ(contacts._vertices[1]._vertex._pos.x(), 4.f);
  GGO_CHECK_FLOAT_EQ(contacts._vertices[1]._vertex._pos.y(), 1.f);
  GGO_CHECK_FLOAT_EQ(contacts._vertices[1]._penetration, 2.f);
}
