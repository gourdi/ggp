#include "../ggo_kernel_nonreg.h"
#include <ggo_shapes3d.h>

/////////////////////////////////////////////////////////////////////
GGO_TEST(basis3d, build)
{
  {
    ggo::vec3f b1(1.f, 1.f, 0.f);
    b1.normalize();
    auto basis = build_basis(b1);

    GGO_CHECK(ggo::is_basis(b1, basis.first, basis.second));
    GGO_CHECK_FLOAT_EQ(basis.first.x(), 1 / std::sqrt(2));
    GGO_CHECK_FLOAT_EQ(basis.first.y(), -1 / std::sqrt(2));
    GGO_CHECK_FLOAT_EQ(basis.first.z(), 0);
    GGO_CHECK_FLOAT_EQ(basis.second.x(), 0);
    GGO_CHECK_FLOAT_EQ(basis.second.y(), 0);
    GGO_CHECK_FLOAT_EQ(basis.second.z(), -1);
  }

  {
    ggo::vec3f b1(1.f, 0.f, 1.f);
    b1.normalize();
    auto basis = build_basis(b1);

    GGO_CHECK(ggo::is_basis(b1, basis.first, basis.second));
    GGO_CHECK_FLOAT_EQ(basis.first.x(), 1 / std::sqrt(2));
    GGO_CHECK_FLOAT_EQ(basis.first.y(), 0);
    GGO_CHECK_FLOAT_EQ(basis.first.z(), -1 / std::sqrt(2));
    GGO_CHECK_FLOAT_EQ(basis.second.x(), 0);
    GGO_CHECK_FLOAT_EQ(basis.second.y(), 1);
    GGO_CHECK_FLOAT_EQ(basis.second.z(), 0);
  }

  {
    ggo::vec3f b1(0.f, 1.f, 1.f);
    b1.normalize();
    auto basis = build_basis(b1);

    GGO_CHECK(ggo::is_basis(b1, basis.first, basis.second));
    GGO_CHECK_FLOAT_EQ(basis.first.x(), 0);
    GGO_CHECK_FLOAT_EQ(basis.first.y(), 1 / std::sqrt(2));
    GGO_CHECK_FLOAT_EQ(basis.first.z(), -1 / std::sqrt(2));
    GGO_CHECK_FLOAT_EQ(basis.second.x(), -1);
    GGO_CHECK_FLOAT_EQ(basis.second.y(), 0);
    GGO_CHECK_FLOAT_EQ(basis.second.z(), 0);
  }
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(basis3d, local_world)
{
  ggo::basis3d_float basis;

  basis.move(1, 2, 1);

  ggo::vec3f v1 = basis.vector_from_local_to_world(ggo::vec3f(1.f, 2.f, 3.f));
  GGO_CHECK_FLOAT_EQ(v1.x(), 1);
  GGO_CHECK_FLOAT_EQ(v1.y(), 2);
  GGO_CHECK_FLOAT_EQ(v1.z(), 3);

  ggo::vec3f v2 = basis.vector_from_world_to_local(ggo::vec3f(1.f, 2.f, 3.f));
  GGO_CHECK_FLOAT_EQ(v2.x(), 1);
  GGO_CHECK_FLOAT_EQ(v2.y(), 2);
  GGO_CHECK_FLOAT_EQ(v2.z(), 3);

  ggo::pos3f p1 = basis.point_from_world_to_local(ggo::pos3f(3.f, 2.f, 0.f));
  GGO_CHECK_FLOAT_EQ(p1.x(), 2);
  GGO_CHECK_FLOAT_EQ(p1.y(), 0);
  GGO_CHECK_FLOAT_EQ(p1.z(), -1);

  ggo::pos3f p2 = basis.point_from_world_to_local(ggo::pos3f(0.f, 2.f, 0.f));
  GGO_CHECK_FLOAT_EQ(p2.x(), -1);
  GGO_CHECK_FLOAT_EQ(p2.y(), 0);
  GGO_CHECK_FLOAT_EQ(p2.z(), -1);

  basis.rotate_z(ggo::pi<float>() / 4);
  GGO_CHECK_FLOAT_EQ(basis.x().x(), 1 / std::sqrt(2));
  GGO_CHECK_FLOAT_EQ(basis.x().y(), 1 / std::sqrt(2));
  GGO_CHECK_FLOAT_EQ(basis.x().z(), 0);
  GGO_CHECK_FLOAT_EQ(basis.y().x(), -1 / std::sqrt(2));
  GGO_CHECK_FLOAT_EQ(basis.y().y(), 1 / std::sqrt(2));
  GGO_CHECK_FLOAT_EQ(basis.y().z(), 0);
  GGO_CHECK_FLOAT_EQ(basis.z().x(), 0);
  GGO_CHECK_FLOAT_EQ(basis.z().y(), 0);
  GGO_CHECK_FLOAT_EQ(basis.z().z(), 1);

  ggo::pos3f p3 = basis.point_from_world_to_local(ggo::pos3f(2.f, 4.f, 0.f));
  GGO_CHECK_FLOAT_EQ(p3.x(), 1.5 * std::sqrt(2));
  GGO_CHECK_FLOAT_EQ(p3.y(), 0.5 * std::sqrt(2));
  GGO_CHECK_FLOAT_EQ(p3.z(), -1);

  ggo::pos3f p4 = basis.point_from_local_to_world(ggo::pos3f(1.f, 2.f, 0.f));
  GGO_CHECK_FLOAT_EQ(p4.x(), 1 - 1 / std::sqrt(2));
  GGO_CHECK_FLOAT_EQ(p4.y(), 2 + 3 / std::sqrt(2));
  GGO_CHECK_FLOAT_EQ(p4.z(), 1);
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(basis3d, look_at)
{
  ggo::basis3d_float basis;

  basis.pos() = { 0.f, 0.f, 1.f };
  basis.look_at({ 0.f, 0.f, 0.f });

  GGO_CHECK_FLOAT_EQ(basis.z().x(), 0);
  GGO_CHECK_FLOAT_EQ(basis.z().y(), 0);
  GGO_CHECK_FLOAT_EQ(basis.z().z(), 1);
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(basis3d, project)
{
  ggo::basis3d_float basis;

  ggo::pos2f proj1 = basis.project({ 0.f, 0.f, -2.f }, 0.5f, 200, 100);
  GGO_CHECK_FLOAT_EQ(proj1.x(), 100.f);
  GGO_CHECK_FLOAT_EQ(proj1.y(), 50.f);

  ggo::pos2f proj2 = basis.project({ 2.f, 2.f, -2.f }, 1.0f, 200, 100);
  GGO_CHECK_FLOAT_EQ(proj2.x(), 150.f);
  GGO_CHECK_FLOAT_EQ(proj2.y(), 100.f);
}