#include "../../ggo_kernel_nonreg.h"
#include <kernel/math/shapes_3d/ggo_shapes3d.h>

/////////////////////////////////////////////////////////////////////
GGO_TEST(disc3d, ray_intersection)
{
  ggo::disc3d_float disc(ggo::ray3d_float({ 5.f, 2.f, 0.f }, { 0.f, 1.f, 0.f }, false), 3);

  ggo::ray3d_float normal;
  float dist = 0;
  GGO_CHECK(disc.intersect_ray(ggo::ray3d_float({ 3.f, 5.f, 0.f }, { 0.f, -1.f, 0.f }, false), dist, normal) == true);
  GGO_CHECK_FLOAT_EQ(dist, 3);
  GGO_CHECK_FLOAT_EQ(normal.pos().x(), 3);
  GGO_CHECK_FLOAT_EQ(normal.pos().y(), 2);
  GGO_CHECK_FLOAT_EQ(normal.pos().z(), 0);
  GGO_CHECK_FLOAT_EQ(normal.dir().x(), 0);
  GGO_CHECK_FLOAT_EQ(normal.dir().y(), 1);
  GGO_CHECK_FLOAT_EQ(normal.dir().z(), 0);

  GGO_CHECK(disc.intersect_ray(ggo::ray3d_float({ 1.f, 5.f, 0.f }, { 0.f, -1.f, 0.f }, false), dist, normal) == false);
  GGO_CHECK(disc.intersect_ray(ggo::ray3d_float({ 4.f, 1.f, 0.f }, { 0.f, -1.f, 0.f }, false), dist, normal) == false);
  GGO_CHECK(disc.intersect_ray(ggo::ray3d_float({ 4.f, 1.f, 0.f }, { 0.f, 1.f, 0.f }, false), dist, normal) == false);
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(disc3d, bounding_box)
{
  ggo::disc3d_float disc({ { 5.f, 2.f, 0.f }, { 1.f, 1.f, 1.f } }, 1);

  auto aabb = disc.get_bounding_box(ggo::basis3d_float({ 1.f, 2.f, 3.f }));
  GGO_CHECK(aabb.has_value() == true);

  GGO_CHECK_FLOAT_EQ(aabb->_x_min, 5.0f);
  GGO_CHECK_FLOAT_EQ(aabb->_x_max, 7.0f);
  GGO_CHECK_FLOAT_EQ(aabb->_y_min, 3.0f);
  GGO_CHECK_FLOAT_EQ(aabb->_y_max, 5.0f);
  GGO_CHECK_FLOAT_EQ(aabb->_z_min, 2.0f);
  GGO_CHECK_FLOAT_EQ(aabb->_z_max, 4.0f);
}

