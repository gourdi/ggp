#include "../../ggo_kernel_nonreg.h"
#include <kernel/math/shapes_3d/ggo_shapes3d.h>

/////////////////////////////////////////////////////////////////////
GGO_TEST(sphere3d, merge)
{
  // Sphere2 inside sphere1.
  {
    ggo::sphere3d_f sphere1(ggo::pos3_f(1.f, 2.f, 3.f), 10.f);
    ggo::sphere3d_f sphere2(ggo::pos3_f(3.f, 2.f, 1.f), 1.f);

    ggo::sphere3d_f sphere3 = ggo::get_union(sphere1, sphere2);

    GGO_CHECK_FLOAT_EQ(sphere3.center().x(), sphere1.center().x());
    GGO_CHECK_FLOAT_EQ(sphere3.center().y(), sphere1.center().y());
    GGO_CHECK_FLOAT_EQ(sphere3.center().z(), sphere1.center().z());
    GGO_CHECK_FLOAT_EQ(sphere3.radius(), sphere1.radius());
  }

  // Sphere1 inside sphere2.
  {
    ggo::sphere3d_f sphere1(ggo::pos3_f(10.f, 10.f, 10.f), 1.f);
    ggo::sphere3d_f sphere2(ggo::pos3_f(5.f, 6.f, 7.f), 100.f);

    ggo::sphere3d_f sphere3 = ggo::get_union(sphere1, sphere2);

    GGO_CHECK_FLOAT_EQ(sphere3.center().x(), sphere2.center().x());
    GGO_CHECK_FLOAT_EQ(sphere3.center().y(), sphere2.center().y());
    GGO_CHECK_FLOAT_EQ(sphere3.center().z(), sphere2.center().z());
    GGO_CHECK_FLOAT_EQ(sphere3.radius(), sphere2.radius());
  }

  // Disjoints spheres.
  {
    ggo::sphere3d_f sphere1(ggo::pos3_f(-3.f, 0.f, 0.f), 2.f);
    ggo::sphere3d_f sphere2(ggo::pos3_f(3.f, 0.f, 0.f), 1.f);

    ggo::sphere3d_f sphere3 = ggo::get_union(sphere1, sphere2);

    GGO_CHECK_FLOAT_EQ(sphere3.center().x(), -0.5);
    GGO_CHECK_FLOAT_EQ(sphere3.center().y(), 0);
    GGO_CHECK_FLOAT_EQ(sphere3.center().z(), 0);
    GGO_CHECK_FLOAT_EQ(sphere3.radius(), 4.5);
  }

  // Overlapping spheres.
  {
    ggo::sphere3d_f sphere1(ggo::pos3_f(0.f, 2.f, 0.f), 1.f);
    ggo::sphere3d_f sphere2(ggo::pos3_f(0.f, -1.f, 0.f), 3.f);

    ggo::sphere3d_f sphere3 = ggo::get_union(sphere1, sphere2);

    GGO_CHECK_FLOAT_EQ(sphere3.center().x(), 0);
    GGO_CHECK_FLOAT_EQ(sphere3.center().y(), -0.5);
    GGO_CHECK_FLOAT_EQ(sphere3.center().z(), 0);
    GGO_CHECK_FLOAT_EQ(sphere3.radius(), 3.5);
  }
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(sphere3d, ray_intersection)
{
  ggo::sphere3d_f sphere(ggo::pos3_f(10.f, 11.f, 12.f), 2.f);
  float t = 0;
  ggo::ray3d_f normal;

  ggo::ray3d_f ray1(ggo::pos3_f(10.f, 11.f, 3.f), ggo::vec3_f(0.f, 0.f, 1.f));
  GGO_CHECK(sphere.intersect_ray(ray1, t, normal) == true);
  GGO_CHECK_FLOAT_EQ(t, 7);
  GGO_CHECK_FLOAT_EQ(normal.pos().x(), 10);
  GGO_CHECK_FLOAT_EQ(normal.pos().y(), 11);
  GGO_CHECK_FLOAT_EQ(normal.pos().z(), 10);
  GGO_CHECK_FLOAT_EQ(normal.dir().x(), 0);
  GGO_CHECK_FLOAT_EQ(normal.dir().y(), 0);
  GGO_CHECK_FLOAT_EQ(normal.dir().z(), -1);

  ggo::ray3d_f ray2(ggo::pos3_f(10.f, 2.f, 3.f), ggo::vec3_f(1.f, 1.f, 1.f));
  GGO_CHECK(sphere.intersect_ray(ray2, t, normal) == false);

  ggo::ray3d_f ray4(ggo::pos3_f(8.f, 11.f, 8.f), ggo::vec3_f(1.f, 0.f, 1.f));
  GGO_CHECK(sphere.intersect_ray(ray4, t, normal) == true);
  GGO_CHECK_FLOAT_EQ(t, 2 * std::sqrt(2));
  GGO_CHECK_FLOAT_EQ(normal.pos().x(), 10);
  GGO_CHECK_FLOAT_EQ(normal.pos().y(), 11);
  GGO_CHECK_FLOAT_EQ(normal.pos().z(), 10);
  GGO_CHECK_FLOAT_EQ(normal.dir().x(), 0);
  GGO_CHECK_FLOAT_EQ(normal.dir().y(), 0);
  GGO_CHECK_FLOAT_EQ(normal.dir().z(), -1);

  ggo::ray3d_f ray5(ggo::pos3_f(13.f, 16.f, 12.f), ggo::vec3_f(-1.f, -1.f, 0.f));
  GGO_CHECK(sphere.intersect_ray(ray5, t, normal) == true);
  GGO_CHECK_FLOAT_EQ(t, 3 * std::sqrt(2));
  GGO_CHECK_FLOAT_EQ(normal.pos().x(), 10);
  GGO_CHECK_FLOAT_EQ(normal.pos().y(), 13);
  GGO_CHECK_FLOAT_EQ(normal.pos().z(), 12);
  GGO_CHECK_FLOAT_EQ(normal.dir().x(), 0);
  GGO_CHECK_FLOAT_EQ(normal.dir().y(), 1);
  GGO_CHECK_FLOAT_EQ(normal.dir().z(), 0);

  // Ray'sphere origin inside the shpere.
  ggo::ray3d_f ray6(ggo::pos3_f(10.f, 11.f, 12.f), ggo::vec3_f(0.f, 1.f, 0.f));
  GGO_CHECK(sphere.intersect_ray(ray6, t, normal) == true);
  GGO_CHECK_FLOAT_EQ(t, 2);
  GGO_CHECK_FLOAT_EQ(normal.pos().x(), 10);
  GGO_CHECK_FLOAT_EQ(normal.pos().y(), 13);
  GGO_CHECK_FLOAT_EQ(normal.pos().z(), 12);
  GGO_CHECK_FLOAT_EQ(normal.dir().x(), 0);
  GGO_CHECK_FLOAT_EQ(normal.dir().y(), -1);
  GGO_CHECK_FLOAT_EQ(normal.dir().z(), 0);
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(sphere3d, is_point_inside)
{
  ggo::sphere3d_f sphere({ 3.0f, 1.0f, 0.0f }, 2.f);
  GGO_CHECK(sphere.is_point_inside(0.0f, 0.0f, 0.0f) == false);
  GGO_CHECK(sphere.is_point_inside(3.0f, 1.0f, 3.0f) == false);
  GGO_CHECK(sphere.is_point_inside(3.0f, 1.0f, 0.0f) == true);
  GGO_CHECK(sphere.is_point_inside(2.0f, 2.0f, 0.0f) == true);
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(sphere3d, bounding_box)
{
  ggo::sphere3d_f sphere({ 3.0f, 1.0f, 0.0f }, 0.5f);
  auto aabb = sphere.get_bounding_box(ggo::basis3d_f({ 1.f, 2.f, 3.f }));
  GGO_CHECK(aabb.has_value() == true);

  GGO_CHECK_FLOAT_EQ(aabb->_x_min, 3.5f);
  GGO_CHECK_FLOAT_EQ(aabb->_x_max, 4.5f);
  GGO_CHECK_FLOAT_EQ(aabb->_y_min, 2.5f);
  GGO_CHECK_FLOAT_EQ(aabb->_y_max, 3.5f);
  GGO_CHECK_FLOAT_EQ(aabb->_z_min, 2.5f);
  GGO_CHECK_FLOAT_EQ(aabb->_z_max, 3.5f);
}

