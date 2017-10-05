#include "../ggo_kernel_nonreg.h"
#include <ggo_shapes3d.h>

/////////////////////////////////////////////////////////////////////
GGO_TEST(shapes3d, vec3)
{
  {
    ggo::vec3f b1(1.f, 1.f, 0.f);
    b1.normalize();
    auto basis = build_basis(b1);

    GGO_CHECK(ggo::is_basis(b1, basis.first, basis.second));
    GGO_CHECK_FABS(basis.first.x(), 1 / std::sqrt(2));
    GGO_CHECK_FABS(basis.first.y(), -1 / std::sqrt(2));
    GGO_CHECK_FABS(basis.first.z(), 0);
    GGO_CHECK_FABS(basis.second.x(), 0);
    GGO_CHECK_FABS(basis.second.y(), 0);
    GGO_CHECK_FABS(basis.second.z(), -1);
  }

  {
    ggo::vec3f b1(1.f, 0.f, 1.f);
    b1.normalize();
    auto basis = build_basis(b1);

    GGO_CHECK(ggo::is_basis(b1, basis.first, basis.second));
    GGO_CHECK_FABS(basis.first.x(), 1 / std::sqrt(2));
    GGO_CHECK_FABS(basis.first.y(), 0);
    GGO_CHECK_FABS(basis.first.z(), -1 / std::sqrt(2));
    GGO_CHECK_FABS(basis.second.x(), 0);
    GGO_CHECK_FABS(basis.second.y(), 1);
    GGO_CHECK_FABS(basis.second.z(), 0);
  }

  {
    ggo::vec3f b1(0.f, 1.f, 1.f);
    b1.normalize();
    auto basis = build_basis(b1);

    GGO_CHECK(ggo::is_basis(b1, basis.first, basis.second));
    GGO_CHECK_FABS(basis.first.x(), 0);
    GGO_CHECK_FABS(basis.first.y(), 1 / std::sqrt(2));
    GGO_CHECK_FABS(basis.first.z(), -1 / std::sqrt(2));
    GGO_CHECK_FABS(basis.second.x(), -1);
    GGO_CHECK_FABS(basis.second.y(), 0);
    GGO_CHECK_FABS(basis.second.z(), 0);
  }
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(shapes3d, centered_spheres)
{
  // Rays intersection tests.
  {
    ggo::centered_sphere3d_float sphere(2);

    float dist = 0;
    ggo::ray3d_float ray(ggo::pos3f(0.f, 0.f, 10.f), ggo::vec3f(0.f, 0.f, -1.f)), normal;
    GGO_CHECK(sphere.intersect_ray(ray, dist, normal) == true);

    GGO_CHECK_FABS(dist, 8);
    GGO_CHECK_FABS(normal.pos().x(), 0);
    GGO_CHECK_FABS(normal.pos().y(), 0);
    GGO_CHECK_FABS(normal.pos().z(), 2);
    GGO_CHECK_FABS(normal.dir().x(), 0);
    GGO_CHECK_FABS(normal.dir().y(), 0);
    GGO_CHECK_FABS(normal.dir().z(), 1);

    ray.pos() = ggo::pos3f(5.f, 0.f, 5.f);
    GGO_CHECK(sphere.intersect_ray(ray, dist, normal) == false);

    ray.pos() = ggo::pos3f(0.f, 0.f, -5.f);
    GGO_CHECK(sphere.intersect_ray(ray, dist, normal) == false);

    // Ray's origin inside the shpere.
    ggo::ray3d_float ray2(ggo::pos3f(0.f, 0.f, 0.f), ggo::vec3f(0.f, 1.f, 0.f));
    GGO_CHECK(sphere.intersect_ray(ray2, dist, normal) == true);
    GGO_CHECK_FABS(dist, 2);
    GGO_CHECK_FABS(normal.pos().x(), 0);
    GGO_CHECK_FABS(normal.pos().y(), 2);
    GGO_CHECK_FABS(normal.pos().z(), 0);
    GGO_CHECK_FABS(normal.dir().x(), 0);
    GGO_CHECK_FABS(normal.dir().y(), -1);
    GGO_CHECK_FABS(normal.dir().z(), 0);
  }
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(shapes3d, spheres)
{
  // Sphere2 inside sphere1.
  {
    ggo::sphere3d_float sphere1(ggo::pos3f(1.f, 2.f, 3.f), 10.f);
    ggo::sphere3d_float sphere2(ggo::pos3f(3.f, 2.f, 1.f), 1.f);

    ggo::sphere3d_float sphere3 = ggo::sphere3d_float::merge(sphere1, sphere2);

    GGO_CHECK_FABS(sphere3.center().x(), sphere1.center().x());
    GGO_CHECK_FABS(sphere3.center().y(), sphere1.center().y());
    GGO_CHECK_FABS(sphere3.center().z(), sphere1.center().z());
    GGO_CHECK_FABS(sphere3.radius(), sphere1.radius());
  }

  // Sphere1 inside sphere2.
  {
    ggo::sphere3d_float sphere1(ggo::pos3f(10.f, 10.f, 10.f), 1.f);
    ggo::sphere3d_float sphere2(ggo::pos3f(5.f, 6.f, 7.f), 100.f);

    ggo::sphere3d_float sphere3 = ggo::sphere3d_float::merge(sphere1, sphere2);

    GGO_CHECK_FABS(sphere3.center().x(), sphere2.center().x());
    GGO_CHECK_FABS(sphere3.center().y(), sphere2.center().y());
    GGO_CHECK_FABS(sphere3.center().z(), sphere2.center().z());
    GGO_CHECK_FABS(sphere3.radius(), sphere2.radius());
  }

  // Disjoints spheres.
  {
    ggo::sphere3d_float sphere1(ggo::pos3f(-3.f, 0.f, 0.f), 2.f);
    ggo::sphere3d_float sphere2(ggo::pos3f(3.f, 0.f, 0.f), 1.f);

    ggo::sphere3d_float sphere3 = ggo::sphere3d_float::merge(sphere1, sphere2);

    GGO_CHECK_FABS(sphere3.center().x(), -0.5);
    GGO_CHECK_FABS(sphere3.center().y(), 0);
    GGO_CHECK_FABS(sphere3.center().z(), 0);
    GGO_CHECK_FABS(sphere3.radius(), 4.5);
  }

  // Overlapping spheres.
  {
    ggo::sphere3d_float sphere1(ggo::pos3f(0.f, 2.f, 0.f), 1.f);
    ggo::sphere3d_float sphere2(ggo::pos3f(0.f, -1.f, 0.f), 3.f);

    ggo::sphere3d_float sphere3 = ggo::sphere3d_float::merge(sphere1, sphere2);

    GGO_CHECK_FABS(sphere3.center().x(), 0);
    GGO_CHECK_FABS(sphere3.center().y(), -0.5);
    GGO_CHECK_FABS(sphere3.center().z(), 0);
    GGO_CHECK_FABS(sphere3.radius(), 3.5);
  }

  // Ray <=> sphere
  {
    ggo::sphere3d_float sphere(ggo::pos3f(10.f, 11.f, 12.f), 2.f);
    float t = 0;
    ggo::ray3d_float normal;

    ggo::ray3d_float ray1(ggo::pos3f(10.f, 11.f, 3.f), ggo::vec3f(0.f, 0.f, 1.f));
    GGO_CHECK(sphere.intersect_ray(ray1, t, normal) == true);
    GGO_CHECK_FABS(t, 7);
    GGO_CHECK_FABS(normal.pos().x(), 10);
    GGO_CHECK_FABS(normal.pos().y(), 11);
    GGO_CHECK_FABS(normal.pos().z(), 10);
    GGO_CHECK_FABS(normal.dir().x(), 0);
    GGO_CHECK_FABS(normal.dir().y(), 0);
    GGO_CHECK_FABS(normal.dir().z(), -1);

    ggo::ray3d_float ray2(ggo::pos3f(10.f, 2.f, 3.f), ggo::vec3f(1.f, 1.f, 1.f));
    GGO_CHECK(sphere.intersect_ray(ray2, t, normal) == false);

    ggo::ray3d_float ray4(ggo::pos3f(8.f, 11.f, 8.f), ggo::vec3f(1.f, 0.f, 1.f));
    GGO_CHECK(sphere.intersect_ray(ray4, t, normal) == true);
    GGO_CHECK_FABS(t, 2 * std::sqrt(2));
    GGO_CHECK_FABS(normal.pos().x(), 10);
    GGO_CHECK_FABS(normal.pos().y(), 11);
    GGO_CHECK_FABS(normal.pos().z(), 10);
    GGO_CHECK_FABS(normal.dir().x(), 0);
    GGO_CHECK_FABS(normal.dir().y(), 0);
    GGO_CHECK_FABS(normal.dir().z(), -1);

    ggo::ray3d_float ray5(ggo::pos3f(13.f, 16.f, 12.f), ggo::vec3f(-1.f, -1.f, 0.f));
    GGO_CHECK(sphere.intersect_ray(ray5, t, normal) == true);
    GGO_CHECK_FABS(t, 3 * std::sqrt(2));
    GGO_CHECK_FABS(normal.pos().x(), 10);
    GGO_CHECK_FABS(normal.pos().y(), 13);
    GGO_CHECK_FABS(normal.pos().z(), 12);
    GGO_CHECK_FABS(normal.dir().x(), 0);
    GGO_CHECK_FABS(normal.dir().y(), 1);
    GGO_CHECK_FABS(normal.dir().z(), 0);

    // Ray's origin inside the shpere.
    ggo::ray3d_float ray6(ggo::pos3f(10.f, 11.f, 12.f), ggo::vec3f(0.f, 1.f, 0.f));
    GGO_CHECK(sphere.intersect_ray(ray6, t, normal) == true);
    GGO_CHECK_FABS(t, 2);
    GGO_CHECK_FABS(normal.pos().x(), 10);
    GGO_CHECK_FABS(normal.pos().y(), 13);
    GGO_CHECK_FABS(normal.pos().z(), 12);
    GGO_CHECK_FABS(normal.dir().x(), 0);
    GGO_CHECK_FABS(normal.dir().y(), -1);
    GGO_CHECK_FABS(normal.dir().z(), 0);
  }

  {
    ggo::sphere3d_float s({ 3.0f, 1.0f, 0.0f }, 2.f);
    GGO_CHECK(s.is_point_inside(0.0f, 0.0f, 0.0f) == false);
    GGO_CHECK(s.is_point_inside(3.0f, 1.0f, 3.0f) == false);
    GGO_CHECK(s.is_point_inside(3.0f, 1.0f, 0.0f) == true);
    GGO_CHECK(s.is_point_inside(2.0f, 2.0f, 0.0f) == true);
  }
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(shapes3d, basis)
{
  {
    ggo::basis3d_float basis;

    basis.move(1, 2, 1);

    ggo::vec3f v1 = basis.vector_from_local_to_world(ggo::vec3f(1.f, 2.f, 3.f));
    GGO_CHECK_FABS(v1.x(), 1);
    GGO_CHECK_FABS(v1.y(), 2);
    GGO_CHECK_FABS(v1.z(), 3);

    ggo::vec3f v2 = basis.vector_from_world_to_local(ggo::vec3f(1.f, 2.f, 3.f));
    GGO_CHECK_FABS(v2.x(), 1);
    GGO_CHECK_FABS(v2.y(), 2);
    GGO_CHECK_FABS(v2.z(), 3);

    ggo::pos3f p1 = basis.point_from_world_to_local(ggo::pos3f(3.f, 2.f, 0.f));
    GGO_CHECK_FABS(p1.x(), 2);
    GGO_CHECK_FABS(p1.y(), 0);
    GGO_CHECK_FABS(p1.z(), -1);

    ggo::pos3f p2 = basis.point_from_world_to_local(ggo::pos3f(0.f, 2.f, 0.f));
    GGO_CHECK_FABS(p2.x(), -1);
    GGO_CHECK_FABS(p2.y(), 0);
    GGO_CHECK_FABS(p2.z(), -1);    

    basis.rotate_z(ggo::pi<float>() / 4);
    GGO_CHECK_FABS(basis.x().x(), 1 / std::sqrt(2));
    GGO_CHECK_FABS(basis.x().y(), 1 / std::sqrt(2));
    GGO_CHECK_FABS(basis.x().z(), 0);
    GGO_CHECK_FABS(basis.y().x(), -1 / std::sqrt(2));
    GGO_CHECK_FABS(basis.y().y(), 1 / std::sqrt(2));
    GGO_CHECK_FABS(basis.y().z(), 0);
    GGO_CHECK_FABS(basis.z().x(), 0);
    GGO_CHECK_FABS(basis.z().y(), 0);
    GGO_CHECK_FABS(basis.z().z(), 1);

    ggo::pos3f p3 = basis.point_from_world_to_local(ggo::pos3f(2.f, 4.f, 0.f));
    GGO_CHECK_FABS(p3.x(), 1.5 * std::sqrt(2));
    GGO_CHECK_FABS(p3.y(), 0.5 * std::sqrt(2));
    GGO_CHECK_FABS(p3.z(), -1);

    ggo::pos3f p4 = basis.point_from_local_to_world(ggo::pos3f(1.f, 2.f, 0.f));
    GGO_CHECK_FABS(p4.x(), 1 - 1 / std::sqrt(2));
    GGO_CHECK_FABS(p4.y(), 2 + 3 / std::sqrt(2));
    GGO_CHECK_FABS(p4.z(), 1);
  }
  
  {
    ggo::basis3d_float basis;
    
    basis.pos() = {0.f, 0.f, 1.f };
    basis.look_at({0.f, 0.f, 0.f });
    
    GGO_CHECK_FABS(basis.z().x(), 0);
    GGO_CHECK_FABS(basis.z().y(), 0);
    GGO_CHECK_FABS(basis.z().z(), 1);
  }

  {
    ggo::basis3d_float basis;

    ggo::pos2f proj1 = basis.project({ 0.f, 0.f, -2.f }, 0.5f, 200, 100);
    GGO_CHECK_FABS(proj1.x(), 100.f);
    GGO_CHECK_FABS(proj1.y(), 50.f);

    ggo::pos2f proj2 = basis.project({ 2.f, 2.f, -2.f }, 1.0f, 200, 100);
    GGO_CHECK_FABS(proj2.x(), 150.f);
    GGO_CHECK_FABS(proj2.y(), 100.f);
  }
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(shapes3d, axis_aligned_box3d)
{
  ggo::aabox3d_float box(-1, 1, -1, 2, 2, 5);
  
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
  
  // Merbe axis aligned box.
  box.merge_with(ggo::aabox3d_float(-3, -2, -10, 20, 3, 4));
  GGO_CHECK_FABS(box.x_min(), -3); 
  GGO_CHECK_FABS(box.x_max(), 1); 
  GGO_CHECK_FABS(box.y_min(), -10); 
  GGO_CHECK_FABS(box.y_max(), 20); 
  GGO_CHECK_FABS(box.z_min(), 2); 
  GGO_CHECK_FABS(box.z_max(), 5);

  // Intersection.
  const std::vector<std::tuple<ggo::aabox3d_float, ggo::aabox3d_float, bool>> intersect_tests{
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
    GGO_CHECK(ggo::intersect(std::get<0>(test), std::get<1>(test)) == std::get<2>(test));
  }
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(shapes3d, plane3d)
{
  {
    ggo::plane3d_float plane({ 0.f, 0.f, 1.f }, -1.f); // z = -1
    float dist = 0;
    ggo::ray3d_float normal;

    ggo::ray3d_float ray1(ggo::pos3f(2.f, 3.f, 5.f), ggo::vec3f(0.f, 0.f, -1.f));
    GGO_CHECK(plane.intersect_ray(ray1, dist, normal) == true);
    GGO_CHECK_FABS(dist, 6);
    GGO_CHECK_FABS(normal.pos().x(), 2);
    GGO_CHECK_FABS(normal.pos().y(), 3);
    GGO_CHECK_FABS(normal.pos().z(), -1);
    GGO_CHECK_FABS(normal.dir().x(), 0);
    GGO_CHECK_FABS(normal.dir().y(), 0);
    GGO_CHECK_FABS(normal.dir().z(), 1);

    ggo::ray3d_float ray2(ggo::pos3f(2.f, 4.f, -2.f), ggo::vec3f(-1.f, 2.f, -1.f));
    GGO_CHECK(plane.intersect_ray(ray2, dist, normal) == false);

    ggo::ray3d_float ray3(ggo::pos3f(1.f, 5.f, -2.f), ggo::vec3f(0.f, 0.f, 1.f));
    GGO_CHECK(plane.intersect_ray(ray3, dist, normal) == false);
  }

  {
    ggo::plane3d_float plane({ 1.f, 2.f, -1.f }, 1 / std::sqrt(6.f));
    float dist = 0;
    ggo::ray3d_float normal;

    ggo::ray3d_float ray(ggo::pos3f(1.f, 3.f, 2.f), ggo::vec3f(1.f, -2.f, 1.f));
    GGO_CHECK(plane.intersect_ray(ray, dist, normal) == true);
    GGO_CHECK_FABS(dist, std::sqrt(6));
    GGO_CHECK_FABS(normal.pos().x(), 2);
    GGO_CHECK_FABS(normal.pos().y(), 1);
    GGO_CHECK_FABS(normal.pos().z(), 3);
    GGO_CHECK_FABS(normal.dir().x(), 1 / std::sqrt(6.f));
    GGO_CHECK_FABS(normal.dir().y(), 2 / std::sqrt(6.f));
    GGO_CHECK_FABS(normal.dir().z(), -1 / std::sqrt(6.f));
  }

  {
    ggo::const_plane3d<float, 0, 0, 1, -1> plane; // z = -1
    float dist = 0;
    ggo::ray3d_float normal;

    ggo::ray3d_float ray1(ggo::pos3f(2.f, 3.f, 5.f), ggo::vec3f(0.f, 0.f, -1.f));
    GGO_CHECK(plane.intersect_ray(ray1, dist, normal) == true);
    GGO_CHECK_FABS(dist, 6);
    GGO_CHECK_FABS(normal.pos().x(), 2);
    GGO_CHECK_FABS(normal.pos().y(), 3);
    GGO_CHECK_FABS(normal.pos().z(), -1);
    GGO_CHECK_FABS(normal.dir().x(), 0);
    GGO_CHECK_FABS(normal.dir().y(), 0);
    GGO_CHECK_FABS(normal.dir().z(), 1);

    ggo::ray3d_float ray2(ggo::pos3f(2.f, 4.f, -2.f), ggo::vec3f(-1.f, 2.f, -1.f));
    GGO_CHECK(plane.intersect_ray(ray2, dist, normal) == false);

    ggo::ray3d_float ray3(ggo::pos3f(1.f, 5.f, -2.f), ggo::vec3f(0.f, 0.f, 1.f));
    GGO_CHECK(plane.intersect_ray(ray3, dist, normal) == false);
  }
}

/////////////////////////////////////////////////////////////////////  
GGO_TEST(shapes3d, polygon3d)
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
    GGO_CHECK_FABS(dist, 1);
    GGO_CHECK_FABS(normal.pos().x(), 1);
    GGO_CHECK_FABS(normal.pos().y(), 2);
    GGO_CHECK_FABS(normal.pos().z(), 5);
    GGO_CHECK_FABS(normal.dir().x(), 0);
    GGO_CHECK_FABS(normal.dir().y(), 0);
    GGO_CHECK_FABS(normal.dir().z(), 1);
  }
  
  // Hit vertex 2
  {
    ggo::ray3d_float normal, ray(ggo::pos3f(2.f, 2.f, 10.f), ggo::vec3f(0.f, 0.f, -1.f));
    float dist = 0;
    GGO_CHECK(polygon.intersect_ray(ray, dist, normal));
    GGO_CHECK_FABS(dist, 5);
    GGO_CHECK_FABS(normal.pos().x(), 2);
    GGO_CHECK_FABS(normal.pos().y(), 2);
    GGO_CHECK_FABS(normal.pos().z(), 5);
    GGO_CHECK_FABS(normal.dir().x(), 1);
    GGO_CHECK_FABS(normal.dir().y(), 0);
    GGO_CHECK_FABS(normal.dir().z(), 0);
  }
  
  // Hit vertex 3
  {
    ggo::ray3d_float normal, ray(ggo::pos3f(1.f, 3.f, 7.f), ggo::vec3f(0.f, 0.f, -1.f));
    float dist = 0;
    GGO_CHECK(polygon.intersect_ray(ray, dist, normal));
    GGO_CHECK_FABS(dist, 2);
    GGO_CHECK_FABS(normal.pos().x(), 1);
    GGO_CHECK_FABS(normal.pos().y(), 3);
    GGO_CHECK_FABS(normal.pos().z(), 5);
    GGO_CHECK_FABS(normal.dir().x(), 0);
    GGO_CHECK_FABS(normal.dir().y(), 1);
    GGO_CHECK_FABS(normal.dir().z(), 0);
  }
  
  // Hit between vertex 1 and vertex 2
  {
    ggo::ray3d_float normal, ray(ggo::pos3f(1.5f, 2.f, 10.f), ggo::vec3f(0.f, 0.f, -1.f));
    float dist = 0;
    GGO_CHECK(polygon.intersect_ray(ray, dist, normal));
    GGO_CHECK_FABS(dist, 5);
    GGO_CHECK_FABS(normal.pos().x(), 1.5);
    GGO_CHECK_FABS(normal.pos().y(), 2);
    GGO_CHECK_FABS(normal.pos().z(), 5);
    GGO_CHECK_FABS(normal.dir().x(), 1 / std::sqrt(2));
    GGO_CHECK_FABS(normal.dir().y(), 0);
    GGO_CHECK_FABS(normal.dir().z(), 1 / std::sqrt(2));
  }
  
  // Hit between vertex 2 and vertex 3
  {
    ggo::ray3d_float normal, ray(ggo::pos3f(1.5f, 2.5f, 10.f), ggo::vec3f(0.f, 0.f, -1.f));
    float dist = 0;
    GGO_CHECK(polygon.intersect_ray(ray, dist, normal));
    GGO_CHECK_FABS(dist, 5);
    GGO_CHECK_FABS(normal.pos().x(), 1.5);
    GGO_CHECK_FABS(normal.pos().y(), 2.5);
    GGO_CHECK_FABS(normal.pos().z(), 5);
    GGO_CHECK_FABS(normal.dir().x(), 1 / std::sqrt(2));
    GGO_CHECK_FABS(normal.dir().y(), 1 / std::sqrt(2));
    GGO_CHECK_FABS(normal.dir().z(), 0);
  }
  
  // Hit between vertex 3 and vertex 1
  {
    ggo::ray3d_float normal, ray(ggo::pos3f(1.f, 2.5f, 10.f), ggo::vec3f(0.f, 0.f, -1.f));
    float dist = 0;
    GGO_CHECK(polygon.intersect_ray(ray, dist, normal));
    GGO_CHECK_FABS(dist, 5);
    GGO_CHECK_FABS(normal.pos().x(), 1);
    GGO_CHECK_FABS(normal.pos().y(), 2.5);
    GGO_CHECK_FABS(normal.pos().z(), 5);
    GGO_CHECK_FABS(normal.dir().x(), 0);
    GGO_CHECK_FABS(normal.dir().y(), 1 / std::sqrt(2));
    GGO_CHECK_FABS(normal.dir().z(), 1 / std::sqrt(2));
  }
  
  // Hit inside the triangle
  {
    ggo::ray3d_float normal, ray(ggo::pos3f(1.25f, 2.25f, 10.f), ggo::vec3f(0.f, 0.f, -1.f));
    float dist = 0;
    GGO_CHECK(polygon.intersect_ray(ray, dist, normal));
    GGO_CHECK_FABS(dist, 5);
    GGO_CHECK_FABS(normal.pos().x(), 1.25);
    GGO_CHECK_FABS(normal.pos().y(), 2.25);
    GGO_CHECK_FABS(normal.pos().z(), 5);
    GGO_CHECK_FABS(normal.dir().x(), 0.25 / std::sqrt(0.5 * 0.5 + 0.25 * 0.25 + 0.25 * 0.25));
    GGO_CHECK_FABS(normal.dir().y(), 0.25 / std::sqrt(0.5 * 0.5 + 0.25 * 0.25 + 0.25 * 0.25));
    GGO_CHECK_FABS(normal.dir().z(), 0.5 / std::sqrt(0.5 * 0.5 + 0.25 * 0.25 + 0.25 * 0.25));
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
GGO_TEST(shapes3d, triangle3d)
{
  // Not double sided.
  {    
    ggo::triangle3d<float, false> triangle(1, 2, 5, 2, 2, 5, 1, 4, 5);
    float dist = 0;
    ggo::ray3d_float normal;
    
    GGO_CHECK_FABS(triangle.area(), 1);
    
    GGO_CHECK(triangle.intersect_ray(ggo::ray3d_float(1.5, 2.5, 10, 0, 0, -1), dist, normal) == true);
    GGO_CHECK_FABS(dist, 5);
    GGO_CHECK_FABS(normal.pos().x(), 1.5);
    GGO_CHECK_FABS(normal.pos().y(), 2.5);

    GGO_CHECK(triangle.intersect_ray(ggo::ray3d_float(1.5, 2.5, 2, 0, 0, 1), dist, normal) == false);
    
    GGO_CHECK(triangle.intersect_ray(ggo::ray3d_float(1.5, 3.5, 6, 0, 0, -1), dist, normal) == false);
  }
  
  // Not double sided.
  {    
    ggo::triangle3d<float, false> triangle(1, 4, 5, 2, 2, 5, 1, 2, 5);
    float dist = 0;
    ggo::ray3d_float normal;
    
    GGO_CHECK(triangle.intersect_ray(ggo::ray3d_float(1.5, 2.5, 10, 0, 0, -1), dist, normal) == false);

    GGO_CHECK(triangle.intersect_ray(ggo::ray3d_float(1.5, 2.5, 2, 0, 0, 1), dist, normal) == true);
    GGO_CHECK_FABS(dist, 3);
    GGO_CHECK_FABS(normal.pos().x(), 1.5);
    GGO_CHECK_FABS(normal.pos().y(), 2.5);
    GGO_CHECK_FABS(normal.pos().z(), 5);
    GGO_CHECK_FABS(normal.dir().x(), 0);
    GGO_CHECK_FABS(normal.dir().y(), 0);
    GGO_CHECK_FABS(normal.dir().z(), -1);
    
    GGO_CHECK(triangle.intersect_ray(ggo::ray3d_float(1.5, 3.5, 6, 0, 0, -1), dist, normal) == false);
  }
  
  // Double sided.
  {    
    ggo::triangle3d<float, true> triangle(1, 2, 5, 2, 2, 5, 1, 4, 5);
    float dist = 0;
    ggo::ray3d_float normal;
    
    GGO_CHECK(triangle.intersect_ray(ggo::ray3d_float(1.5, 2.5, 10, 0, 0, -1), dist, normal) == true);
    GGO_CHECK_FABS(normal.pos().x(), 1.5);
    GGO_CHECK_FABS(normal.pos().y(), 2.5);
    GGO_CHECK_FABS(normal.pos().z(), 5);
    GGO_CHECK_FABS(normal.dir().x(), 0);
    GGO_CHECK_FABS(normal.dir().y(), 0);
    GGO_CHECK_FABS(normal.dir().z(), 1);

    GGO_CHECK(triangle.intersect_ray(ggo::ray3d_float(1.5, 2.5, 2, 0, 0, 1), dist, normal) == true);
    GGO_CHECK_FABS(dist, 3);
    GGO_CHECK_FABS(normal.pos().x(), 1.5);
    GGO_CHECK_FABS(normal.pos().y(), 2.5);
    GGO_CHECK_FABS(normal.pos().z(), 5);
    GGO_CHECK_FABS(normal.dir().x(), 0);
    GGO_CHECK_FABS(normal.dir().y(), 0);
    GGO_CHECK_FABS(normal.dir().z(), -1);

    GGO_CHECK(triangle.intersect_ray(ggo::ray3d_float(1.5, 3.5, 6, 0, 0, -1), dist, normal) == false);
  }
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(shapes3d, disc3d)
{
  ggo::disc3d_float disc(ggo::ray3d_float({5.f, 2.f, 0.f }, {0.f, 1.f, 0.f }, false), 3);
  
  ggo::ray3d_float normal;
  float dist = 0;
  GGO_CHECK(disc.intersect_ray(ggo::ray3d_float({3.f, 5.f, 0.f }, {0.f, -1.f, 0.f }, false), dist, normal) == true);
  GGO_CHECK_FABS(dist, 3);
  GGO_CHECK_FABS(normal.pos().x(), 3);
  GGO_CHECK_FABS(normal.pos().y(), 2);
  GGO_CHECK_FABS(normal.pos().z(), 0);
  GGO_CHECK_FABS(normal.dir().x(), 0);
  GGO_CHECK_FABS(normal.dir().y(), 1);
  GGO_CHECK_FABS(normal.dir().z(), 0);
  
  GGO_CHECK(disc.intersect_ray(ggo::ray3d_float({1.f, 5.f, 0.f}, {0.f, -1.f, 0.f }, false), dist, normal) == false);
  GGO_CHECK(disc.intersect_ray(ggo::ray3d_float({4.f, 1.f, 0.f}, {0.f, -1.f, 0.f }, false), dist, normal) == false);
  GGO_CHECK(disc.intersect_ray(ggo::ray3d_float({4.f, 1.f, 0.f}, {0.f, 1.f, 0.f }, false), dist, normal) == false);
}

/////////////////////////////////////////////////////////////////////  
GGO_TEST(shapes3d, parallelogram3d)
{
  ggo::parallelogram3d_float parallelogram({4.f, 3.f, 1.f }, {3.f, 0.f, 0.f }, {2.f, 2.f, 0.f });
  
  ggo::ray3d_float normal;
  float dist = 0;
  
  GGO_CHECK(parallelogram.intersect_ray(ggo::ray3d_float({5.f, 2.f, 2.f }, {0.f, 0.f, -1.f }, false), dist, normal) == true);
  GGO_CHECK_FABS(dist, 1);
  GGO_CHECK_FABS(normal.pos().x(), 5);
  GGO_CHECK_FABS(normal.pos().y(), 2);
  GGO_CHECK_FABS(normal.pos().z(), 1);
  GGO_CHECK_FABS(normal.dir().x(), 0);
  GGO_CHECK_FABS(normal.dir().y(), 0);
  GGO_CHECK_FABS(normal.dir().z(), 1);
  
  GGO_CHECK(parallelogram.intersect_ray(ggo::ray3d_float({1.f, 2.f, 3.f }, {0.f, 0.f, -1.f }, false), dist, normal) == true);
  GGO_CHECK_FABS(dist, 2);
  GGO_CHECK_FABS(normal.pos().x(), 1);
  GGO_CHECK_FABS(normal.pos().y(), 2);
  GGO_CHECK_FABS(normal.pos().z(), 1);
  GGO_CHECK_FABS(normal.dir().x(), 0);
  GGO_CHECK_FABS(normal.dir().y(), 0);
  GGO_CHECK_FABS(normal.dir().z(), 1);

  GGO_CHECK(parallelogram.intersect_ray(ggo::ray3d_float({5.f, 2.f, 0.f}, {0.f, 0.f, -1.f }, false), dist, normal) == false);
  GGO_CHECK(parallelogram.intersect_ray(ggo::ray3d_float({5.f, 2.f, 2.f}, {0.f, 0.f, 1.f }, false), dist, normal) == false);
  GGO_CHECK(parallelogram.intersect_ray(ggo::ray3d_float({7.f, 2.f, 2.f}, {0.f, 0.f, -1.f }, false), dist, normal) == false);
  GGO_CHECK(parallelogram.intersect_ray(ggo::ray3d_float({1.f, 4.f, 2.f}, {0.f, 0.f, -1.f }, false), dist, normal) == false);
}

/////////////////////////////////////////////////////////////////////  
GGO_TEST(shapes3d, metaball)
{
  ggo::metaball<float> metaball(1.f);
  std::shared_ptr<ggo::influence_shape3d_abc<float>> sphere(new ggo::sphere3d_float(ggo::pos3f(3.0f, 0.0f, 0.0f), 2.0f));
  metaball.add_influence_data(sphere, 3.0f);

  float dist = 0.f;
  ggo::ray3d_float normal;
  ggo::ray3d_float ray({ 0.f, 0.f, 0.f }, { 1.f, 0.f, 0.f }, false);
  GGO_CHECK(metaball.intersect_ray(ray, dist, normal) == true);
  GGO_CHECK_FABS(normal.pos().x(), dist);
  GGO_CHECK_FABS(normal.pos().y(), 0.f);
  GGO_CHECK_FABS(normal.pos().z(), 0.f);
  GGO_CHECK_FABS(normal.dir().x(), -1.f);
  GGO_CHECK_FABS(normal.dir().y(), 0.f);
  GGO_CHECK_FABS(normal.dir().z(), 0.f);
}

/////////////////////////////////////////////////////////////////////  
GGO_TEST(shapes3d, line3d)
{
  // line1 = orig1 + t1 * dir1
  // line2 = orig2 + t2 * dir2

  {
    const ggo::pos3f orig1(-1.f, 2.f, 0.f);
    const ggo::pos3f orig2(4.f, -1.f, 1.f);
    const ggo::vec3f dir1(1.f, 0.f, 0.f);
    const ggo::vec3f dir2(0.f, 0.f, 1.f);

    float dist1 = 0.f;
    float dist2 = 0.f;
    GGO_CHECK(ggo::find_closest_lines_points({ orig1, dir1 }, { orig2, dir2 }, dist1, dist2) == true);

    GGO_CHECK_FABS(dist1, 5.f);
    GGO_CHECK_FABS(dist2, -1.f);
  }

  {
    const ggo::pos3f orig1(-1.f, 2.f, 0.f);
    const ggo::pos3f orig2(4.f, -1.f, 1.f);
    const ggo::vec3f dir1(1.f, 0.f, 0.f);
    const ggo::vec3f dir2(-ggo::inv_sqrt2<float>(), ggo::inv_sqrt2<float>(), 0.f);

    ggo::pos3f p1, p2;
    GGO_CHECK(ggo::find_closest_lines_points(ggo::line3d<float>(orig1, dir1), ggo::line3d<float>(orig2, dir2), p1, p2) == true);

    GGO_CHECK_FABS(p1.x(), 1.f);
    GGO_CHECK_FABS(p1.y(), 2.f);
    GGO_CHECK_FABS(p1.z(), 0.f);

    GGO_CHECK_FABS(p2.x(), 1.f);
    GGO_CHECK_FABS(p2.y(), 2.f);
    GGO_CHECK_FABS(p2.z(), 1.f);
  }

  {
    const ggo::line3d<float> line({ 2.f, 1.f, 4.f }, { 0.f, 0.f, 1.f }, false);
    GGO_CHECK_FABS(ggo::hypot(line, ggo::pos3f(4.f, 1.f, 1.f)), 4.f);
  }
}

/////////////////////////////////////////////////////////////////////  
GGO_TEST(shapes3d, cylinder3d)
{
  ggo::cylinder3d<float> cylinder({ 2.f, 4.f, 0.f }, { 0.f, 0.f, 1.f }, 2.f);
  ggo::ray3d<float> ray({ 2.f, 1.f, 0.f }, { 0.f, 1.f, 0.f });

  float dist_inf = 0.f;
  float dist_sup = 0.f;
  GGO_CHECK(cylinder.intersect_line(ray, dist_inf, dist_sup) == true);
  GGO_CHECK_FABS(dist_inf, 1.f);
  GGO_CHECK_FABS(dist_sup, 5.f);

  // Ray outside the cyulinder.
  {
    float dist = 0.f;
    ggo::ray3d_float normal;
    GGO_CHECK(cylinder.intersect_ray(ray, dist, normal) == true);
    GGO_CHECK_FABS(dist, 1.f);
    GGO_CHECK_FABS(normal.pos().x(), 2.f);
    GGO_CHECK_FABS(normal.pos().y(), 2.f);
    GGO_CHECK_FABS(normal.pos().z(), 0.f);
    GGO_CHECK_FABS(normal.dir().x(), 0.f);
    GGO_CHECK_FABS(normal.dir().y(), -1.f);
    GGO_CHECK_FABS(normal.dir().z(), 0.f);
  }

  // Ray inside the cyulinder.
  {
    float dist = 0.f;
    ggo::ray3d_float normal;
    GGO_CHECK(cylinder.intersect_ray(ggo::ray3d_float({ 2.f, 4.f, 1.f }, { 0.f, 1.f, 0.f }), dist, normal) == true);
    GGO_CHECK_FABS(dist, 2.f);
    GGO_CHECK_FABS(normal.pos().x(), 2.f);
    GGO_CHECK_FABS(normal.pos().y(), 6.f);
    GGO_CHECK_FABS(normal.pos().z(), 1.f);
    GGO_CHECK_FABS(normal.dir().x(), 0.f);
    GGO_CHECK_FABS(normal.dir().y(), -1.f);
    GGO_CHECK_FABS(normal.dir().z(), 0.f);
  }
}

/////////////////////////////////////////////////////////////////////  
GGO_TEST(shapes3d, influence_plane3d)
{
  ggo::influence_plane3d<float> plane({ 0.f, 1.f, 0.f }, 3.f, 1.f);

  auto intersections = plane.intersect_ray({ {3.f, 5.f, 0.f}, {0.f, -1.f, 0.f} });
  GGO_CHECK(intersections.size() == 2);
  GGO_CHECK_FABS(intersections[0], 1.f);
  GGO_CHECK_FABS(intersections[1], 3.f);

  intersections = plane.intersect_ray({ { 3.f, -1.f, 0.f }, { 0.f, 1.f, 0.f } });
  GGO_CHECK(intersections.size() == 2);
  GGO_CHECK_FABS(intersections[0], 3.f);
  GGO_CHECK_FABS(intersections[1], 5.f);

  intersections = plane.intersect_ray({ { -2.f, 1.f, 0.f }, { 1.f, 1.f, 0.f } });
  GGO_CHECK(intersections.size() == 2);
  GGO_CHECK_FABS(intersections[0], ggo::sqrt2<float>());
  GGO_CHECK_FABS(intersections[1], 3 * ggo::sqrt2<float>());

  GGO_CHECK_FABS(plane.hypot_to_center({ 2.1f, 2.5f, 3.14f }), 0.25f);
  GGO_CHECK_FABS(plane.hypot_to_center({ 2.1f, 3.0f, 3.14f }), 0.0f);
}

/////////////////////////////////////////////////////////////////////  
GGO_TEST(shapes3d, rectangle3d)
{
  {
    ggo::rectangle3d<0, 0, 1, float> rect({ 4.f, 2.f, 1.f }, 2.f, 1.f);
    float dist = 0.f;
    ggo::ray3d_float normal;

    GGO_CHECK(rect.intersect_ray({ {3.f, 2.f, 2.f},{0.f, 0.f, -1.f} }, dist, normal) == true);
    GGO_CHECK_FABS(dist, 1.f);
    GGO_CHECK_FABS(normal.pos().x(), 3.f);
    GGO_CHECK_FABS(normal.pos().y(), 2.f);
    GGO_CHECK_FABS(normal.pos().z(), 1.f);
    GGO_CHECK_FABS(normal.dir().x(), 0.f);
    GGO_CHECK_FABS(normal.dir().y(), 0.f);
    GGO_CHECK_FABS(normal.dir().z(), 1.f);

    GGO_CHECK(rect.intersect_ray({ { 3.f, 2.f, 2.f },{ 1.f, 0.f, -1.f } }, dist, normal) == true);
    GGO_CHECK_FABS(dist, ggo::sqrt2<float>());
    GGO_CHECK_FABS(normal.pos().x(), 4.f);
    GGO_CHECK_FABS(normal.pos().y(), 2.f);
    GGO_CHECK_FABS(normal.pos().z(), 1.f);
    GGO_CHECK_FABS(normal.dir().x(), 0.f);
    GGO_CHECK_FABS(normal.dir().y(), 0.f);
    GGO_CHECK_FABS(normal.dir().z(), 1.f);

    GGO_CHECK(rect.intersect_ray({ { 3.f, 2.f, 0.f },{ 0.f, 0.f, -1.f } }, dist, normal) == false);
    GGO_CHECK(rect.intersect_ray({ { 1.f, 2.f, 2.f },{ 0.f, 0.f, -1.f } }, dist, normal) == false);
    GGO_CHECK(rect.intersect_ray({ { 3.f, 2.f, 0.f },{ 0.f, 0.f, 1.f } }, dist, normal) == false);
  }

  {
    ggo::rectangle3d<0, 0, -1, float> rect({ 4.f, 2.f, 1.f }, 2.f, 1.f);
    float dist = 0.f;
    ggo::ray3d_float normal;

    GGO_CHECK(rect.intersect_ray({ { 3.f, 2.f, 0.f },{ 0.f, 0.f, 1.f } }, dist, normal) == true);
    GGO_CHECK_FABS(dist, 1.f);
    GGO_CHECK_FABS(normal.pos().x(), 3.f);
    GGO_CHECK_FABS(normal.pos().y(), 2.f);
    GGO_CHECK_FABS(normal.pos().z(), 1.f);
    GGO_CHECK_FABS(normal.dir().x(), 0.f);
    GGO_CHECK_FABS(normal.dir().y(), 0.f);
    GGO_CHECK_FABS(normal.dir().z(), -1.f);

    GGO_CHECK(rect.intersect_ray({ { 3.f, 2.f, 0.f },{ 1.f, 0.f, 1.f } }, dist, normal) == true);
    GGO_CHECK_FABS(dist, ggo::sqrt2<float>());
    GGO_CHECK_FABS(normal.pos().x(), 4.f);
    GGO_CHECK_FABS(normal.pos().y(), 2.f);
    GGO_CHECK_FABS(normal.pos().z(), 1.f);
    GGO_CHECK_FABS(normal.dir().x(), 0.f);
    GGO_CHECK_FABS(normal.dir().y(), 0.f);
    GGO_CHECK_FABS(normal.dir().z(), -1.f);

    GGO_CHECK(rect.intersect_ray({ { 3.f, 2.f, 2.f },{ 0.f, 0.f, 1.f } }, dist, normal) == false);
    GGO_CHECK(rect.intersect_ray({ { 1.f, 2.f, 0.f },{ 0.f, 0.f, 1.f } }, dist, normal) == false);
    GGO_CHECK(rect.intersect_ray({ { 3.f, 2.f, 2.f },{ 0.f, 0.f, -1.f } }, dist, normal) == false);
  }
}
