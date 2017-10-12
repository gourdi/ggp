#include "../ggo_kernel_nonreg.h"
#include <ggo_shapes3d.h>

/////////////////////////////////////////////////////////////////////
GGO_TEST(axis_aligned_box3d, ray_intersection)
{
  ggo::box3d_float box(-1, 1, -1, 2, 2, 5);

  ggo::ray3d_float normal;
  float dist = 0;

  ggo::ray3d_float ray;

  // Top -> down rays.
  ray.set_pos(2, 0, 4);
  ray.set_normalized_dir(0, 0, -1);
  GGO_CHECK(box.intersect_ray(ray) == false);
  GGO_CHECK(box.intersect_ray(ray, dist, normal) == false);

  ray.set_pos(0, 0, 1);
  ray.set_normalized_dir(0, 0, -1);
  GGO_CHECK(box.intersect_ray(ray) == false);
  GGO_CHECK(box.intersect_ray(ray, dist, normal) == false);

  ray.set_pos(0, 0, 8);
  ray.set_normalized_dir(0, 0, -1);
  GGO_CHECK(box.intersect_ray(ray) == true);
  GGO_CHECK(box.intersect_ray(ray, dist, normal) == true);
  GGO_CHECK_FABS(dist, 3);
  GGO_CHECK_FABS(normal.pos().x(), 0);
  GGO_CHECK_FABS(normal.pos().y(), 0);
  GGO_CHECK_FABS(normal.pos().z(), 5);
  GGO_CHECK_FABS(normal.dir().x(), 0);
  GGO_CHECK_FABS(normal.dir().y(), 0);
  GGO_CHECK_FABS(normal.dir().z(), 1);

  // Bottom -> up rays.
  ray.set_pos(2, 0, 1);
  ray.set_normalized_dir(0, 0, 1);
  GGO_CHECK(box.intersect_ray(ray) == false);
  GGO_CHECK(box.intersect_ray(ray, dist, normal) == false);

  ray.set_pos(0, 0, 6);
  ray.set_normalized_dir(0, 0, 1);
  GGO_CHECK(box.intersect_ray(ray) == false);
  GGO_CHECK(box.intersect_ray(ray, dist, normal) == false);

  ray.set_pos(0, 0, 0);
  ray.set_normalized_dir(0, 0, 1);
  GGO_CHECK(box.intersect_ray(ray) == true);
  GGO_CHECK(box.intersect_ray(ray, dist, normal) == true);
  GGO_CHECK_FABS(dist, 2);
  GGO_CHECK_FABS(normal.pos().x(), 0);
  GGO_CHECK_FABS(normal.pos().y(), 0);
  GGO_CHECK_FABS(normal.pos().z(), 2);
  GGO_CHECK_FABS(normal.dir().x(), 0);
  GGO_CHECK_FABS(normal.dir().y(), 0);
  GGO_CHECK_FABS(normal.dir().z(), -1);

  // Left -> right rays.
  ray.set_pos(0, 3, 4);
  ray.set_normalized_dir(0, 1, 0);
  GGO_CHECK(box.intersect_ray(ray) == false);
  GGO_CHECK(box.intersect_ray(ray, dist, normal) == false);

  ray.set_pos(2, 0, 1);
  ray.set_normalized_dir(0, 1, 0);
  GGO_CHECK(box.intersect_ray(ray) == false);
  GGO_CHECK(box.intersect_ray(ray, dist, normal) == false);

  ray.set_pos(0, -5, 4);
  ray.set_normalized_dir(0, 1, 0);
  GGO_CHECK(box.intersect_ray(ray) == true);
  GGO_CHECK(box.intersect_ray(ray, dist, normal) == true);
  GGO_CHECK_FABS(dist, 4);
  GGO_CHECK_FABS(normal.pos().x(), 0);
  GGO_CHECK_FABS(normal.pos().y(), -1);
  GGO_CHECK_FABS(normal.pos().z(), 4);
  GGO_CHECK_FABS(normal.dir().x(), 0);
  GGO_CHECK_FABS(normal.dir().y(), -1);
  GGO_CHECK_FABS(normal.dir().z(), 0);

  // Right -> left rays.
  ray.set_pos(0, -2, 4);
  ray.set_normalized_dir(0, -1, 0);
  GGO_CHECK(box.intersect_ray(ray) == false);
  GGO_CHECK(box.intersect_ray(ray, dist, normal) == false);

  ray.set_pos(2, 4, 1);
  ray.set_normalized_dir(0, -1, 0);
  GGO_CHECK(box.intersect_ray(ray) == false);
  GGO_CHECK(box.intersect_ray(ray, dist, normal) == false);

  ray.set_pos(0, 10, 4);
  ray.set_normalized_dir(0, -1, 0);
  GGO_CHECK(box.intersect_ray(ray) == true);
  GGO_CHECK(box.intersect_ray(ray, dist, normal) == true);
  GGO_CHECK_FABS(dist, 8);
  GGO_CHECK_FABS(normal.pos().x(), 0);
  GGO_CHECK_FABS(normal.pos().y(), 2);
  GGO_CHECK_FABS(normal.pos().z(), 4);
  GGO_CHECK_FABS(normal.dir().x(), 0);
  GGO_CHECK_FABS(normal.dir().y(), 1);
  GGO_CHECK_FABS(normal.dir().z(), 0);

  // Far -> near rays.
  ray.set_pos(-2, 10, 10);
  ray.set_normalized_dir(1, 0, 0);
  GGO_CHECK(box.intersect_ray(ray) == false);
  GGO_CHECK(box.intersect_ray(ray, dist, normal) == false);

  ray.set_pos(2, 0, 3);
  ray.set_normalized_dir(1, 0, 0);
  GGO_CHECK(box.intersect_ray(ray) == false);
  GGO_CHECK(box.intersect_ray(ray, dist, normal) == false);

  ray.set_pos(-2, 1, 3);
  ray.set_normalized_dir(1, 0, 0);
  GGO_CHECK(box.intersect_ray(ray) == true);
  GGO_CHECK(box.intersect_ray(ray, dist, normal) == true);
  GGO_CHECK_FABS(dist, 1);
  GGO_CHECK_FABS(normal.pos().x(), -1);
  GGO_CHECK_FABS(normal.pos().y(), 1);
  GGO_CHECK_FABS(normal.pos().z(), 3);
  GGO_CHECK_FABS(normal.dir().x(), -1);
  GGO_CHECK_FABS(normal.dir().y(), 0);
  GGO_CHECK_FABS(normal.dir().z(), 0);

  // Near -> far rays.
  ray.set_pos(2, 10, 10);
  ray.set_normalized_dir(-1, 0, 0);
  GGO_CHECK(box.intersect_ray(ray) == false);
  GGO_CHECK(box.intersect_ray(ray, dist, normal) == false);

  ray.set_pos(-2, 0, 3);
  ray.set_normalized_dir(-1, 0, 0);
  GGO_CHECK(box.intersect_ray(ray) == false);
  GGO_CHECK(box.intersect_ray(ray, dist, normal) == false);

  ray.set_pos(8, 1, 3);
  ray.set_normalized_dir(-1, 0, 0);
  GGO_CHECK(box.intersect_ray(ray) == true);
  GGO_CHECK(box.intersect_ray(ray, dist, normal) == true);
  GGO_CHECK_FABS(dist, 7);
  GGO_CHECK_FABS(normal.pos().x(), 1);
  GGO_CHECK_FABS(normal.pos().y(), 1);
  GGO_CHECK_FABS(normal.pos().z(), 3);
  GGO_CHECK_FABS(normal.dir().x(), 1);
  GGO_CHECK_FABS(normal.dir().y(), 0);
  GGO_CHECK_FABS(normal.dir().z(), 0);
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(axis_aligned_box3d, get_union)
{
  auto box = ggo::get_union(ggo::box3d_float(-1, 1, -1, 2, 2, 5), ggo::box3d_float(-3, -2, -10, 20, 3, 4));

  GGO_CHECK_FABS(box.x_min(), -3);
  GGO_CHECK_FABS(box.x_max(), 1);
  GGO_CHECK_FABS(box.y_min(), -10);
  GGO_CHECK_FABS(box.y_max(), 20);
  GGO_CHECK_FABS(box.z_min(), 2);
  GGO_CHECK_FABS(box.z_max(), 5);
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(axis_aligned_box3d, intersection)
{
  const std::vector<std::tuple<ggo::box3d_float, ggo::box3d_float, bool>> intersect_tests{
    { { 0, 1, 1, 2, 2, 3 },{ 2, 3, 3, 4, 4, 5 }, false },
    { { 2, 3, 3, 4, 4, 5 },{ 0, 1, 1, 2, 2, 3 }, false },
    { { 0, 1, 0, 1, 0, 1 },{ 0, 1, 0, 1, 2, 3 }, false },
    { { 0, 1, 0, 1, 0, 1 },{ 0, 1, 2, 3, 0, 1 }, false },
    { { 0, 1, 0, 1, 0, 1 },{ 2, 3, 0, 1, 0, 1 }, false },
    { { 0, 1, 0, 1, 2, 3 },{ 0, 1, 0, 1, 0, 1 }, false },
    { { 0, 1, 2, 3, 0, 1 },{ 0, 1, 0, 1, 0, 1 }, false },
    { { 2, 3, 0, 1, 0, 1 },{ 0, 1, 0, 1, 0, 1 }, false },
    { { 0, 2, 0, 2, 0, 2 },{ 1, 3, 1, 3, 1, 3 }, true },
    { { 0, 2, 0, 2, 0, 2 },{ 1, 3, 1, 3, 1, 3 }, true },
    { { 1, 2, 1, 2, 1, 2 },{ 0, 3, 0, 3, 0, 3 }, true },
    { { 0, 3, 0, 3, 0, 3 },{ 1, 2, 1, 2, 1, 2 }, true },
  };
  for (const auto & test : intersect_tests)
  {
    GGO_CHECK(ggo::test_intersection(std::get<0>(test), std::get<1>(test)) == std::get<2>(test));
  }
}