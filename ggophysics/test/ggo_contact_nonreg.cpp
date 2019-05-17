#include <physics/ggo_contact.h>
#include <physics/ggo_rigid_body.h>
#include <kernel/nonreg/ggo_nonreg.h>

GGO_TEST(contact, update_case1)
{
  ggo::rigid_body body1(0.f, 0.f, 0.f, 0.f);
  ggo::rigid_body body2(0.f, 0.f, 0.f, 0.f);

  ggo::collision_data collision;
  collision._reference_box_id = ggo::box_id::box1;
  collision._reference_edge._id = ggo::oriented_box_edge_id::left;
  collision._reference_edge._normal = { 1.f, 0.f };
  collision._reference_edge._vertices[0] = { { 0.f, 0.f }, ggo::oriented_box_vertex_id::left_bottom };
  collision._reference_edge._vertices[1] = { { 2.f, 0.f }, ggo::oriented_box_vertex_id::left_top };
  collision._incident_vertices._count = 2;
  collision._incident_vertices._vertices[0] = { 10.f, { { 1.f, 2.f }, ggo::oriented_box_vertex_id::right_bottom } };
  collision._incident_vertices._vertices[1] = { 11.f, { { 3.f, 4.f }, ggo::oriented_box_vertex_id::left_bottom } };

  ggo::contact contact;
  contact._reference_body = &body1;
  contact._incident_body = &body2;
  contact._reference_edge._id = ggo::oriented_box_edge_id::left;
  contact._reference_edge._normal = { 1.f, 0.f };
  contact._reference_edge._vertices[0] = { { 0.f, 0.f }, ggo::oriented_box_vertex_id::left_bottom };
  contact._reference_edge._vertices[1] = { { 2.f, 0.f }, ggo::oriented_box_vertex_id::left_top };
  contact._points_count = 1;
  contact._points[0]._normal_impulse = 5.f;
  contact._points[0]._tangent_impulse = 6.f;
  contact._points[0]._penetration = 1.2f;
  contact._points[0]._vertex = { { 7.f, 8.f }, ggo::oriented_box_vertex_id::left_bottom };

  contact = ggo::update_contact(contact, collision, &body1, &body2);

  GGO_CHECK_EQ(contact._reference_body, &body1);
  GGO_CHECK_EQ(contact._incident_body, &body2);
  GGO_CHECK_EQ(contact._reference_edge._id, ggo::oriented_box_edge_id::left);
  GGO_CHECK(ggo::compare(contact._reference_edge._normal, { 1.f, 0.f }));
  GGO_CHECK_EQ(contact._reference_edge._vertices[0]._id, ggo::oriented_box_vertex_id::left_bottom);
  GGO_CHECK(ggo::compare(contact._reference_edge._vertices[0]._pos, { 0.f, 0.f }));
  GGO_CHECK_EQ(contact._reference_edge._vertices[1]._id, ggo::oriented_box_vertex_id::left_top);
  GGO_CHECK(ggo::compare(contact._reference_edge._vertices[1]._pos, { 2.f, 0.f }));
  GGO_CHECK_EQ(contact._points_count, 2);
  GGO_CHECK_FLOAT_EQ(contact._points[0]._penetration, 10.f);
  GGO_CHECK_FLOAT_EQ(contact._points[0]._normal_impulse, 0.f);
  GGO_CHECK_FLOAT_EQ(contact._points[0]._tangent_impulse, 0.f);
  GGO_CHECK_EQ(contact._points[0]._vertex._id, ggo::oriented_box_vertex_id::right_bottom);
  GGO_CHECK(ggo::compare(contact._points[0]._vertex._pos, { 1.f, 2.f }));
  GGO_CHECK_FLOAT_EQ(contact._points[1]._penetration, 11.f);
  GGO_CHECK_FLOAT_EQ(contact._points[1]._normal_impulse, 5.f); // Previous value is not lost.
  GGO_CHECK_FLOAT_EQ(contact._points[1]._tangent_impulse, 6.f); // Previous value is not lost.
  GGO_CHECK_EQ(contact._points[1]._vertex._id, ggo::oriented_box_vertex_id::left_bottom);
  GGO_CHECK(ggo::compare(contact._points[1]._vertex._pos, { 3.f, 4.f }));
}
