#include "../../ggo_kernel_nonreg.h"
#include <kernel/math/shapes_3d/ggo_shapes3d.h>

/////////////////////////////////////////////////////////////////////  
GGO_TEST(rectangle3d, ray_intersection)
{
  {
    ggo::rectangle3d<0, 0, 1, float> rect({ 4.f, 2.f, 1.f }, 2.f, 1.f);
    float dist = 0.f;
    ggo::ray3d_f normal;

    GGO_CHECK(rect.intersect_ray({ { 3.f, 2.f, 2.f },{ 0.f, 0.f, -1.f } }, dist, normal) == true);
    GGO_CHECK_FLOAT_EQ(dist, 1.f);
    GGO_CHECK_FLOAT_EQ(normal.pos().x(), 3.f);
    GGO_CHECK_FLOAT_EQ(normal.pos().y(), 2.f);
    GGO_CHECK_FLOAT_EQ(normal.pos().z(), 1.f);
    GGO_CHECK_FLOAT_EQ(normal.dir().x(), 0.f);
    GGO_CHECK_FLOAT_EQ(normal.dir().y(), 0.f);
    GGO_CHECK_FLOAT_EQ(normal.dir().z(), 1.f);

    GGO_CHECK(rect.intersect_ray({ { 3.f, 2.f, 2.f },{ 1.f, 0.f, -1.f } }, dist, normal) == true);
    GGO_CHECK_FLOAT_EQ(dist, ggo::sqrt2<float>());
    GGO_CHECK_FLOAT_EQ(normal.pos().x(), 4.f);
    GGO_CHECK_FLOAT_EQ(normal.pos().y(), 2.f);
    GGO_CHECK_FLOAT_EQ(normal.pos().z(), 1.f);
    GGO_CHECK_FLOAT_EQ(normal.dir().x(), 0.f);
    GGO_CHECK_FLOAT_EQ(normal.dir().y(), 0.f);
    GGO_CHECK_FLOAT_EQ(normal.dir().z(), 1.f);

    GGO_CHECK(rect.intersect_ray({ { 3.f, 2.f, 0.f },{ 0.f, 0.f, -1.f } }, dist, normal) == false);
    GGO_CHECK(rect.intersect_ray({ { 1.f, 2.f, 2.f },{ 0.f, 0.f, -1.f } }, dist, normal) == false);
    GGO_CHECK(rect.intersect_ray({ { 3.f, 2.f, 0.f },{ 0.f, 0.f, 1.f } }, dist, normal) == false);
  }

  {
    ggo::rectangle3d<0, 0, -1, float> rect({ 4.f, 2.f, 1.f }, 2.f, 1.f);
    float dist = 0.f;
    ggo::ray3d_f normal;

    GGO_CHECK(rect.intersect_ray({ { 3.f, 2.f, 0.f },{ 0.f, 0.f, 1.f } }, dist, normal) == true);
    GGO_CHECK_FLOAT_EQ(dist, 1.f);
    GGO_CHECK_FLOAT_EQ(normal.pos().x(), 3.f);
    GGO_CHECK_FLOAT_EQ(normal.pos().y(), 2.f);
    GGO_CHECK_FLOAT_EQ(normal.pos().z(), 1.f);
    GGO_CHECK_FLOAT_EQ(normal.dir().x(), 0.f);
    GGO_CHECK_FLOAT_EQ(normal.dir().y(), 0.f);
    GGO_CHECK_FLOAT_EQ(normal.dir().z(), -1.f);

    GGO_CHECK(rect.intersect_ray({ { 3.f, 2.f, 0.f },{ 1.f, 0.f, 1.f } }, dist, normal) == true);
    GGO_CHECK_FLOAT_EQ(dist, ggo::sqrt2<float>());
    GGO_CHECK_FLOAT_EQ(normal.pos().x(), 4.f);
    GGO_CHECK_FLOAT_EQ(normal.pos().y(), 2.f);
    GGO_CHECK_FLOAT_EQ(normal.pos().z(), 1.f);
    GGO_CHECK_FLOAT_EQ(normal.dir().x(), 0.f);
    GGO_CHECK_FLOAT_EQ(normal.dir().y(), 0.f);
    GGO_CHECK_FLOAT_EQ(normal.dir().z(), -1.f);

    GGO_CHECK(rect.intersect_ray({ { 3.f, 2.f, 2.f },{ 0.f, 0.f, 1.f } }, dist, normal) == false);
    GGO_CHECK(rect.intersect_ray({ { 1.f, 2.f, 0.f },{ 0.f, 0.f, 1.f } }, dist, normal) == false);
    GGO_CHECK(rect.intersect_ray({ { 3.f, 2.f, 2.f },{ 0.f, 0.f, -1.f } }, dist, normal) == false);
  }
}

/////////////////////////////////////////////////////////////////////  
GGO_TEST(rectangle3d, bounding_box)
{
  ggo::rectangle3d<0, 0, 1, float> rect({ 4.f, 4.f, 0.f }, 2.f, 1.f);
  auto aabb = rect.get_bounding_box(ggo::basis3d_f({ 0.f, 0.f, 1.f }));
  GGO_CHECK(aabb.has_value() == true);

  GGO_CHECK_FLOAT_EQ(aabb->_x_min, 2.0f);
  GGO_CHECK_FLOAT_EQ(aabb->_x_max, 6.0f);
  GGO_CHECK_FLOAT_EQ(aabb->_y_min, 3.0f);
  GGO_CHECK_FLOAT_EQ(aabb->_y_max, 5.0f);
  GGO_CHECK_FLOAT_EQ(aabb->_z_min, 1.0f);
  GGO_CHECK_FLOAT_EQ(aabb->_z_max, 1.0f);
}
