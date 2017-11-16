#include "../ggo_kernel_nonreg.h"
#include <ggo_shapes3d.h>

/////////////////////////////////////////////////////////////////////  
GGO_TEST(parallelogram3d, ray_intersection)
{
  ggo::parallelogram3d_float parallelogram({ 4.f, 3.f, 1.f }, { 3.f, 0.f, 0.f }, { 2.f, 2.f, 0.f });

  ggo::ray3d_float normal;
  float dist = 0;

  GGO_CHECK(parallelogram.intersect_ray(ggo::ray3d_float({ 5.f, 2.f, 2.f }, { 0.f, 0.f, -1.f }, false), dist, normal) == true);
  GGO_CHECK_FLOAT_EQ(dist, 1);
  GGO_CHECK_FLOAT_EQ(normal.pos().x(), 5);
  GGO_CHECK_FLOAT_EQ(normal.pos().y(), 2);
  GGO_CHECK_FLOAT_EQ(normal.pos().z(), 1);
  GGO_CHECK_FLOAT_EQ(normal.dir().x(), 0);
  GGO_CHECK_FLOAT_EQ(normal.dir().y(), 0);
  GGO_CHECK_FLOAT_EQ(normal.dir().z(), 1);

  GGO_CHECK(parallelogram.intersect_ray(ggo::ray3d_float({ 1.f, 2.f, 3.f }, { 0.f, 0.f, -1.f }, false), dist, normal) == true);
  GGO_CHECK_FLOAT_EQ(dist, 2);
  GGO_CHECK_FLOAT_EQ(normal.pos().x(), 1);
  GGO_CHECK_FLOAT_EQ(normal.pos().y(), 2);
  GGO_CHECK_FLOAT_EQ(normal.pos().z(), 1);
  GGO_CHECK_FLOAT_EQ(normal.dir().x(), 0);
  GGO_CHECK_FLOAT_EQ(normal.dir().y(), 0);
  GGO_CHECK_FLOAT_EQ(normal.dir().z(), 1);

  GGO_CHECK(parallelogram.intersect_ray(ggo::ray3d_float({ 5.f, 2.f, 0.f }, { 0.f, 0.f, -1.f }, false), dist, normal) == false);
  GGO_CHECK(parallelogram.intersect_ray(ggo::ray3d_float({ 5.f, 2.f, 2.f }, { 0.f, 0.f, 1.f }, false), dist, normal) == false);
  GGO_CHECK(parallelogram.intersect_ray(ggo::ray3d_float({ 7.f, 2.f, 2.f }, { 0.f, 0.f, -1.f }, false), dist, normal) == false);
  GGO_CHECK(parallelogram.intersect_ray(ggo::ray3d_float({ 1.f, 4.f, 2.f }, { 0.f, 0.f, -1.f }, false), dist, normal) == false);
}

/////////////////////////////////////////////////////////////////////  
GGO_TEST(parallelogram3d, bounding_box)
{
  ggo::parallelogram3d_float parallelogram({ 4.f, 3.f, 1.f }, { 3.f, 0.f, 0.f }, { 2.f, 2.f, 0.f });

}
