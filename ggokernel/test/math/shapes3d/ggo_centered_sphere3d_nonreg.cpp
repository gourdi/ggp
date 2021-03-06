#include "../../ggo_kernel_nonreg.h"
#include <kernel/math/shapes_3d/ggo_shapes3d.h>

/////////////////////////////////////////////////////////////////////
GGO_TEST(centered_sphere3d, ray_intersection)
{
  ggo::centered_sphere3d_f sphere(2);

  float dist = 0;
  ggo::ray3d_f ray(ggo::pos3_f(0.f, 0.f, 10.f), ggo::vec3_f(0.f, 0.f, -1.f)), normal;
  GGO_CHECK(sphere.intersect_ray(ray, dist, normal) == true);

  GGO_CHECK_FLOAT_EQ(dist, 8);
  GGO_CHECK_FLOAT_EQ(normal.pos().x(), 0);
  GGO_CHECK_FLOAT_EQ(normal.pos().y(), 0);
  GGO_CHECK_FLOAT_EQ(normal.pos().z(), 2);
  GGO_CHECK_FLOAT_EQ(normal.dir().x(), 0);
  GGO_CHECK_FLOAT_EQ(normal.dir().y(), 0);
  GGO_CHECK_FLOAT_EQ(normal.dir().z(), 1);

  ray.pos() = ggo::pos3_f(5.f, 0.f, 5.f);
  GGO_CHECK(sphere.intersect_ray(ray, dist, normal) == false);

  ray.pos() = ggo::pos3_f(0.f, 0.f, -5.f);
  GGO_CHECK(sphere.intersect_ray(ray, dist, normal) == false);

  // Ray's origin inside the shpere.
  ggo::ray3d_f ray2(ggo::pos3_f(0.f, 0.f, 0.f), ggo::vec3_f(0.f, 1.f, 0.f));
  GGO_CHECK(sphere.intersect_ray(ray2, dist, normal) == true);
  GGO_CHECK_FLOAT_EQ(dist, 2);
  GGO_CHECK_FLOAT_EQ(normal.pos().x(), 0);
  GGO_CHECK_FLOAT_EQ(normal.pos().y(), 2);
  GGO_CHECK_FLOAT_EQ(normal.pos().z(), 0);
  GGO_CHECK_FLOAT_EQ(normal.dir().x(), 0);
  GGO_CHECK_FLOAT_EQ(normal.dir().y(), -1);
  GGO_CHECK_FLOAT_EQ(normal.dir().z(), 0);
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(centered_sphere3d, bounding_box)
{
  ggo::centered_sphere3d_f sphere(0.5f);

  ggo::basis3d_f basis({ 1.f, 2.f, 3.f });

  auto aabb = sphere.get_bounding_box(basis);
  GGO_CHECK(aabb.has_value() == true);

  GGO_CHECK_FLOAT_EQ(aabb->_x_min, 0.5f);
  GGO_CHECK_FLOAT_EQ(aabb->_x_max, 1.5f);
  GGO_CHECK_FLOAT_EQ(aabb->_y_min, 1.5f);
  GGO_CHECK_FLOAT_EQ(aabb->_y_max, 2.5f);
  GGO_CHECK_FLOAT_EQ(aabb->_z_min, 2.5f);
  GGO_CHECK_FLOAT_EQ(aabb->_z_max, 3.5f);
}

