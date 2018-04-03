#include "../../ggo_kernel_nonreg.h"
#include <ggo_shapes3d.h>

/////////////////////////////////////////////////////////////////////  
GGO_TEST(polygon3d, ray_intersection)
{
  std::vector<ggo::vertex<float>> vertices;
  vertices.emplace_back(ggo::pos3f(1.f, 2.f, 5.f), ggo::vec3f(0.f, 0.f, 1.f));
  vertices.emplace_back(ggo::pos3f(2.f, 2.f, 5.f), ggo::vec3f(1.f, 0.f, 0.f));
  vertices.emplace_back(ggo::pos3f(1.f, 3.f, 5.f), ggo::vec3f(0.f, 1.f, 0.f));

  std::vector<ggo::polygon3d_float::face> faces;
  faces.emplace_back(0, 1, 2);

  ggo::polygon3d_float polygon(vertices, faces);

  // Hit vertex 1
  {
    ggo::ray3d_float normal, ray(ggo::pos3f(1.f, 2.f, 6.f), ggo::vec3f(0.f, 0.f, -1.f));
    float dist = 0;
    GGO_CHECK(polygon.intersect_ray(ray, dist, normal));
    GGO_CHECK_FLOAT_EQ(dist, 1);
    GGO_CHECK_FLOAT_EQ(normal.pos().x(), 1);
    GGO_CHECK_FLOAT_EQ(normal.pos().y(), 2);
    GGO_CHECK_FLOAT_EQ(normal.pos().z(), 5);
    GGO_CHECK_FLOAT_EQ(normal.dir().x(), 0);
    GGO_CHECK_FLOAT_EQ(normal.dir().y(), 0);
    GGO_CHECK_FLOAT_EQ(normal.dir().z(), 1);
  }

  // Hit vertex 2
  {
    ggo::ray3d_float normal, ray(ggo::pos3f(2.f, 2.f, 10.f), ggo::vec3f(0.f, 0.f, -1.f));
    float dist = 0;
    GGO_CHECK(polygon.intersect_ray(ray, dist, normal));
    GGO_CHECK_FLOAT_EQ(dist, 5);
    GGO_CHECK_FLOAT_EQ(normal.pos().x(), 2);
    GGO_CHECK_FLOAT_EQ(normal.pos().y(), 2);
    GGO_CHECK_FLOAT_EQ(normal.pos().z(), 5);
    GGO_CHECK_FLOAT_EQ(normal.dir().x(), 1);
    GGO_CHECK_FLOAT_EQ(normal.dir().y(), 0);
    GGO_CHECK_FLOAT_EQ(normal.dir().z(), 0);
  }

  // Hit vertex 3
  {
    ggo::ray3d_float normal, ray(ggo::pos3f(1.f, 3.f, 7.f), ggo::vec3f(0.f, 0.f, -1.f));
    float dist = 0;
    GGO_CHECK(polygon.intersect_ray(ray, dist, normal));
    GGO_CHECK_FLOAT_EQ(dist, 2);
    GGO_CHECK_FLOAT_EQ(normal.pos().x(), 1);
    GGO_CHECK_FLOAT_EQ(normal.pos().y(), 3);
    GGO_CHECK_FLOAT_EQ(normal.pos().z(), 5);
    GGO_CHECK_FLOAT_EQ(normal.dir().x(), 0);
    GGO_CHECK_FLOAT_EQ(normal.dir().y(), 1);
    GGO_CHECK_FLOAT_EQ(normal.dir().z(), 0);
  }

  // Hit between vertex 1 and vertex 2
  {
    ggo::ray3d_float normal, ray(ggo::pos3f(1.5f, 2.f, 10.f), ggo::vec3f(0.f, 0.f, -1.f));
    float dist = 0;
    GGO_CHECK(polygon.intersect_ray(ray, dist, normal));
    GGO_CHECK_FLOAT_EQ(dist, 5);
    GGO_CHECK_FLOAT_EQ(normal.pos().x(), 1.5);
    GGO_CHECK_FLOAT_EQ(normal.pos().y(), 2);
    GGO_CHECK_FLOAT_EQ(normal.pos().z(), 5);
    GGO_CHECK_FLOAT_EQ(normal.dir().x(), 1 / std::sqrt(2));
    GGO_CHECK_FLOAT_EQ(normal.dir().y(), 0);
    GGO_CHECK_FLOAT_EQ(normal.dir().z(), 1 / std::sqrt(2));
  }

  // Hit between vertex 2 and vertex 3
  {
    ggo::ray3d_float normal, ray(ggo::pos3f(1.5f, 2.5f, 10.f), ggo::vec3f(0.f, 0.f, -1.f));
    float dist = 0;
    GGO_CHECK(polygon.intersect_ray(ray, dist, normal));
    GGO_CHECK_FLOAT_EQ(dist, 5);
    GGO_CHECK_FLOAT_EQ(normal.pos().x(), 1.5);
    GGO_CHECK_FLOAT_EQ(normal.pos().y(), 2.5);
    GGO_CHECK_FLOAT_EQ(normal.pos().z(), 5);
    GGO_CHECK_FLOAT_EQ(normal.dir().x(), 1 / std::sqrt(2));
    GGO_CHECK_FLOAT_EQ(normal.dir().y(), 1 / std::sqrt(2));
    GGO_CHECK_FLOAT_EQ(normal.dir().z(), 0);
  }

  // Hit between vertex 3 and vertex 1
  {
    ggo::ray3d_float normal, ray(ggo::pos3f(1.f, 2.5f, 10.f), ggo::vec3f(0.f, 0.f, -1.f));
    float dist = 0;
    GGO_CHECK(polygon.intersect_ray(ray, dist, normal));
    GGO_CHECK_FLOAT_EQ(dist, 5);
    GGO_CHECK_FLOAT_EQ(normal.pos().x(), 1);
    GGO_CHECK_FLOAT_EQ(normal.pos().y(), 2.5);
    GGO_CHECK_FLOAT_EQ(normal.pos().z(), 5);
    GGO_CHECK_FLOAT_EQ(normal.dir().x(), 0);
    GGO_CHECK_FLOAT_EQ(normal.dir().y(), 1 / std::sqrt(2));
    GGO_CHECK_FLOAT_EQ(normal.dir().z(), 1 / std::sqrt(2));
  }

  // Hit inside the triangle
  {
    ggo::ray3d_float normal, ray(ggo::pos3f(1.25f, 2.25f, 10.f), ggo::vec3f(0.f, 0.f, -1.f));
    float dist = 0;
    GGO_CHECK(polygon.intersect_ray(ray, dist, normal));
    GGO_CHECK_FLOAT_EQ(dist, 5);
    GGO_CHECK_FLOAT_EQ(normal.pos().x(), 1.25);
    GGO_CHECK_FLOAT_EQ(normal.pos().y(), 2.25);
    GGO_CHECK_FLOAT_EQ(normal.pos().z(), 5);
    GGO_CHECK_FLOAT_EQ(normal.dir().x(), 0.25 / std::sqrt(0.5 * 0.5 + 0.25 * 0.25 + 0.25 * 0.25));
    GGO_CHECK_FLOAT_EQ(normal.dir().y(), 0.25 / std::sqrt(0.5 * 0.5 + 0.25 * 0.25 + 0.25 * 0.25));
    GGO_CHECK_FLOAT_EQ(normal.dir().z(), 0.5 / std::sqrt(0.5 * 0.5 + 0.25 * 0.25 + 0.25 * 0.25));
  }

  // No hit
  {
    float dist = 0;
    ggo::ray3d_float normal;

    GGO_CHECK(polygon.intersect_ray(ggo::ray3d_float(ggo::pos3f(1.25f, 2.25f, 4.f), ggo::vec3f(0.f, 0.f, -1.f)), dist, normal) == false);
    GGO_CHECK(polygon.intersect_ray(ggo::ray3d_float(ggo::pos3f(0.f, 0.f, 10.f), ggo::vec3f(0.f, 0.f, -1.f)), dist, normal) == false);
  }
}

/////////////////////////////////////////////////////////////////////  
GGO_TEST(shapes3d, bounding_box)
{
  std::vector<ggo::vertex<float>> vertices;
  vertices.emplace_back(ggo::pos3f(1.f, 2.f, 5.f), ggo::vec3f(0.f, 0.f, 1.f));
  vertices.emplace_back(ggo::pos3f(2.f, 2.f, 5.f), ggo::vec3f(1.f, 0.f, 0.f));
  vertices.emplace_back(ggo::pos3f(1.f, 3.f, 6.f), ggo::vec3f(0.f, 1.f, 0.f));

  std::vector<ggo::polygon3d_float::face> faces;
  faces.emplace_back(0, 1, 2);

  ggo::polygon3d_float polygon(vertices, faces);

  ggo::basis3d_float basis({ 0.f, 0.f, 1.f });
  basis.rotate_z(ggo::pi<float>() / 2);
  auto aabb = polygon.get_bounding_box(basis);
  GGO_CHECK(aabb.has_value() == true);

  GGO_CHECK_FLOAT_EQ(aabb->_x_min, -3.0f);
  GGO_CHECK_FLOAT_EQ(aabb->_x_max, -2.0f);
  GGO_CHECK_FLOAT_EQ(aabb->_y_min, 1.0f);
  GGO_CHECK_FLOAT_EQ(aabb->_y_max, 2.0f);
  GGO_CHECK_FLOAT_EQ(aabb->_z_min, 6.0f);
  GGO_CHECK_FLOAT_EQ(aabb->_z_max, 7.0f);
}

