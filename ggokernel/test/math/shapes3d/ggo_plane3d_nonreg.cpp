#include "../../ggo_kernel_nonreg.h"
#include <kernel/math/shapes_3d/ggo_shapes3d.h>

/////////////////////////////////////////////////////////////////////
GGO_TEST(plane3d, ray_intersection)
{
  {
    ggo::plane3d_f plane({ 0.f, 0.f, 1.f }, -1.f); // z = -1
    float dist = 0;
    ggo::ray3d_f normal;

    ggo::ray3d_f ray1(ggo::pos3_f(2.f, 3.f, 5.f), ggo::vec3_f(0.f, 0.f, -1.f));
    GGO_CHECK(plane.intersect_ray(ray1, dist, normal) == true);
    GGO_CHECK_FLOAT_EQ(dist, 6);
    GGO_CHECK_FLOAT_EQ(normal.pos().x(), 2);
    GGO_CHECK_FLOAT_EQ(normal.pos().y(), 3);
    GGO_CHECK_FLOAT_EQ(normal.pos().z(), -1);
    GGO_CHECK_FLOAT_EQ(normal.dir().x(), 0);
    GGO_CHECK_FLOAT_EQ(normal.dir().y(), 0);
    GGO_CHECK_FLOAT_EQ(normal.dir().z(), 1);

    ggo::ray3d_f ray2(ggo::pos3_f(2.f, 4.f, -2.f), ggo::vec3_f(-1.f, 2.f, -1.f));
    GGO_CHECK(plane.intersect_ray(ray2, dist, normal) == false);

    ggo::ray3d_f ray3(ggo::pos3_f(1.f, 5.f, -2.f), ggo::vec3_f(0.f, 0.f, 1.f));
    GGO_CHECK(plane.intersect_ray(ray3, dist, normal) == false);
  }

  {
    ggo::plane3d_f plane({ 1.f, 2.f, -1.f }, 1 / std::sqrt(6.f));
    float dist = 0;
    ggo::ray3d_f normal;

    ggo::ray3d_f ray(ggo::pos3_f(1.f, 3.f, 2.f), ggo::vec3_f(1.f, -2.f, 1.f));
    GGO_CHECK(plane.intersect_ray(ray, dist, normal) == true);
    GGO_CHECK_FLOAT_EQ(dist, std::sqrt(6));
    GGO_CHECK_FLOAT_EQ(normal.pos().x(), 2);
    GGO_CHECK_FLOAT_EQ(normal.pos().y(), 1);
    GGO_CHECK_FLOAT_EQ(normal.pos().z(), 3);
    GGO_CHECK_FLOAT_EQ(normal.dir().x(), 1 / std::sqrt(6.f));
    GGO_CHECK_FLOAT_EQ(normal.dir().y(), 2 / std::sqrt(6.f));
    GGO_CHECK_FLOAT_EQ(normal.dir().z(), -1 / std::sqrt(6.f));
  }
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(const_plane3d, ray_intersection)
{
  {
    ggo::const_plane3d<float, 0, 0, 1, -1> plane; // z = -1
    float dist = 0;
    ggo::ray3d_f normal;

    ggo::ray3d_f ray1(ggo::pos3_f(2.f, 3.f, 5.f), ggo::vec3_f(0.f, 0.f, -1.f));
    GGO_CHECK(plane.intersect_ray(ray1, dist, normal) == true);
    GGO_CHECK_FLOAT_EQ(dist, 6);
    GGO_CHECK_FLOAT_EQ(normal.pos().x(), 2);
    GGO_CHECK_FLOAT_EQ(normal.pos().y(), 3);
    GGO_CHECK_FLOAT_EQ(normal.pos().z(), -1);
    GGO_CHECK_FLOAT_EQ(normal.dir().x(), 0);
    GGO_CHECK_FLOAT_EQ(normal.dir().y(), 0);
    GGO_CHECK_FLOAT_EQ(normal.dir().z(), 1);

    ggo::ray3d_f ray2(ggo::pos3_f(2.f, 4.f, -2.f), ggo::vec3_f(-1.f, 2.f, -1.f));
    GGO_CHECK(plane.intersect_ray(ray2, dist, normal) == false);

    ggo::ray3d_f ray3(ggo::pos3_f(1.f, 5.f, -2.f), ggo::vec3_f(0.f, 0.f, 1.f));
    GGO_CHECK(plane.intersect_ray(ray3, dist, normal) == false);
  }
}

/////////////////////////////////////////////////////////////////////  
GGO_TEST(influence_plane3d, ray_intersection)
{
  ggo::influence_plane3d<float> plane({ 0.f, 1.f, 0.f }, 3.f, 1.f);

  auto intersections = plane.intersect_ray({ { 3.f, 5.f, 0.f },{ 0.f, -1.f, 0.f } });
  GGO_CHECK(intersections.size() == 2);
  GGO_CHECK_FLOAT_EQ(intersections[0], 1.f);
  GGO_CHECK_FLOAT_EQ(intersections[1], 3.f);

  intersections = plane.intersect_ray({ { 3.f, -1.f, 0.f },{ 0.f, 1.f, 0.f } });
  GGO_CHECK(intersections.size() == 2);
  GGO_CHECK_FLOAT_EQ(intersections[0], 3.f);
  GGO_CHECK_FLOAT_EQ(intersections[1], 5.f);

  intersections = plane.intersect_ray({ { -2.f, 1.f, 0.f },{ 1.f, 1.f, 0.f } });
  GGO_CHECK(intersections.size() == 2);
  GGO_CHECK_FLOAT_EQ(intersections[0], ggo::sqrt2<float>());
  GGO_CHECK_FLOAT_EQ(intersections[1], 3 * ggo::sqrt2<float>());

  GGO_CHECK_FLOAT_EQ(plane.hypot_to_center({ 2.1f, 2.5f, 3.14f }), 0.25f);
  GGO_CHECK_FLOAT_EQ(plane.hypot_to_center({ 2.1f, 3.0f, 3.14f }), 0.0f);
}
