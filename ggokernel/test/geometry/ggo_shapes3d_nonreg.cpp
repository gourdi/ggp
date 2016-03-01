#include "../ggo_kernel_nonreg.h"
#include <ggo_shapes3d.h>

/////////////////////////////////////////////////////////////////////
GGO_TEST(shapes3d, set3)
{
  {
    ggo::vector3d_float b1(1, 1, 0), b2, b3;
    b1.normalize();
    b1.build_basis(b2, b3);

    GGO_CHECK(ggo::is_basis(b1, b2, b3));
    GGO_CHECK_FABS(b2.x(), 1 / std::sqrt(2));
    GGO_CHECK_FABS(b2.y(), -1 / std::sqrt(2));
    GGO_CHECK_FABS(b2.z(), 0);
    GGO_CHECK_FABS(b3.x(), 0);
    GGO_CHECK_FABS(b3.y(), 0);
    GGO_CHECK_FABS(b3.z(), -1);
  }

  {
    ggo::vector3d_float b1(1, 0, 1), b2, b3;
    b1.normalize();
    b1.build_basis(b2, b3);

    GGO_CHECK(ggo::is_basis(b1, b2, b3));
    GGO_CHECK_FABS(b2.x(), 1 / std::sqrt(2));
    GGO_CHECK_FABS(b2.y(), 0);
    GGO_CHECK_FABS(b2.z(), -1 / std::sqrt(2));
    GGO_CHECK_FABS(b3.x(), 0);
    GGO_CHECK_FABS(b3.y(), 1);
    GGO_CHECK_FABS(b3.z(), 0);
  }

  {
    ggo::vector3d_float b1(0, 1, 1), b2, b3;
    b1.normalize();
    b1.build_basis(b2, b3);

    GGO_CHECK(ggo::is_basis(b1, b2, b3));
    GGO_CHECK_FABS(b2.x(), 0);
    GGO_CHECK_FABS(b2.y(), 1 / std::sqrt(2));
    GGO_CHECK_FABS(b2.z(), -1 / std::sqrt(2));
    GGO_CHECK_FABS(b3.x(), -1);
    GGO_CHECK_FABS(b3.y(), 0);
    GGO_CHECK_FABS(b3.z(), 0);
  }
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(shapes3d, centered_spheres)
{
  // Rays intersection tests.
  {
    ggo::centered_sphere3d_float sphere(2);

    float dist = 0;
    ggo::ray3d_float ray(ggo::point3d_float(0, 0, 10), ggo::vector3d_float(0, 0, -1)), normal;
    GGO_CHECK(sphere.intersect_ray(ray, dist, normal) == true);

    GGO_CHECK_FABS(dist, 8);
    GGO_CHECK_FABS(normal.pos().x(), 0);
    GGO_CHECK_FABS(normal.pos().y(), 0);
    GGO_CHECK_FABS(normal.pos().z(), 2);
    GGO_CHECK_FABS(normal.dir().x(), 0);
    GGO_CHECK_FABS(normal.dir().y(), 0);
    GGO_CHECK_FABS(normal.dir().z(), 1);

    ray.pos() = ggo::point3d_float(5, 0, 5);
    GGO_CHECK(sphere.intersect_ray(ray, dist, normal) == false);

    ray.pos() = ggo::point3d_float(0, 0, -5);
    GGO_CHECK(sphere.intersect_ray(ray, dist, normal) == false);

    // Ray's origin inside the shpere.
    ggo::ray3d_float ray2(ggo::point3d_float(0, 0, 0), ggo::vector3d_float(0, 1, 0));
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
    ggo::sphere3d_float sphere1(ggo::point3d_float(1, 2, 3), 10);
    ggo::sphere3d_float sphere2(ggo::point3d_float(3, 2, 1), 1);

    ggo::sphere3d_float sphere3 = ggo::sphere3d_float::merge(sphere1, sphere2);

    GGO_CHECK_FABS(sphere3.center().x(), sphere1.center().x());
    GGO_CHECK_FABS(sphere3.center().y(), sphere1.center().y());
    GGO_CHECK_FABS(sphere3.center().z(), sphere1.center().z());
    GGO_CHECK_FABS(sphere3.radius(), sphere1.radius());
  }

  // Sphere1 inside sphere2.
  {
    ggo::sphere3d_float sphere1(ggo::point3d_float(10, 10, 10), 1);
    ggo::sphere3d_float sphere2(ggo::point3d_float(5, 6, 7), 100);

    ggo::sphere3d_float sphere3 = ggo::sphere3d_float::merge(sphere1, sphere2);

    GGO_CHECK_FABS(sphere3.center().x(), sphere2.center().x());
    GGO_CHECK_FABS(sphere3.center().y(), sphere2.center().y());
    GGO_CHECK_FABS(sphere3.center().z(), sphere2.center().z());
    GGO_CHECK_FABS(sphere3.radius(), sphere2.radius());
  }

  // Disjoints spheres.
  {
    ggo::sphere3d_float sphere1(ggo::point3d_float(-3, 0, 0), 2);
    ggo::sphere3d_float sphere2(ggo::point3d_float(3, 0, 0), 1);

    ggo::sphere3d_float sphere3 = ggo::sphere3d_float::merge(sphere1, sphere2);

    GGO_CHECK_FABS(sphere3.center().x(), -0.5);
    GGO_CHECK_FABS(sphere3.center().y(), 0);
    GGO_CHECK_FABS(sphere3.center().z(), 0);
    GGO_CHECK_FABS(sphere3.radius(), 4.5);
  }

  // Overlapping spheres.
  {
    ggo::sphere3d_float sphere1(ggo::point3d_float(0, 2, 0), 1);
    ggo::sphere3d_float sphere2(ggo::point3d_float(0, -1, 0), 3);

    ggo::sphere3d_float sphere3 = ggo::sphere3d_float::merge(sphere1, sphere2);

    GGO_CHECK_FABS(sphere3.center().x(), 0);
    GGO_CHECK_FABS(sphere3.center().y(), -0.5);
    GGO_CHECK_FABS(sphere3.center().z(), 0);
    GGO_CHECK_FABS(sphere3.radius(), 3.5);
  }

  // Ray <=> sphere
  {
    ggo::sphere3d_float sphere(ggo::point3d_float(10, 11, 12), 2);
    float t = 0;
    ggo::ray3d_float normal;

    ggo::ray3d_float ray1(ggo::point3d_float(10, 11, 3), ggo::vector3d_float(0, 0, 1));
    GGO_CHECK(sphere.intersect_ray(ray1, t, normal) == true);
    GGO_CHECK_FABS(t, 7);
    GGO_CHECK_FABS(normal.pos().x(), 10);
    GGO_CHECK_FABS(normal.pos().y(), 11);
    GGO_CHECK_FABS(normal.pos().z(), 10);
    GGO_CHECK_FABS(normal.dir().x(), 0);
    GGO_CHECK_FABS(normal.dir().y(), 0);
    GGO_CHECK_FABS(normal.dir().z(), -1);

    ggo::ray3d_float ray2(ggo::point3d_float(10, 2, 3), ggo::vector3d_float(1, 1, 1));
    GGO_CHECK(sphere.intersect_ray(ray2, t, normal) == false);

    ggo::ray3d_float ray4(ggo::point3d_float(8, 11, 8), ggo::vector3d_float(1, 0, 1));
    GGO_CHECK(sphere.intersect_ray(ray4, t, normal) == true);
    GGO_CHECK_FABS(t, 2 * std::sqrt(2));
    GGO_CHECK_FABS(normal.pos().x(), 10);
    GGO_CHECK_FABS(normal.pos().y(), 11);
    GGO_CHECK_FABS(normal.pos().z(), 10);
    GGO_CHECK_FABS(normal.dir().x(), 0);
    GGO_CHECK_FABS(normal.dir().y(), 0);
    GGO_CHECK_FABS(normal.dir().z(), -1);

    ggo::ray3d_float ray5(ggo::point3d_float(13, 16, 12), ggo::vector3d_float(-1, -1, 0));
    GGO_CHECK(sphere.intersect_ray(ray5, t, normal) == true);
    GGO_CHECK_FABS(t, 3 * std::sqrt(2));
    GGO_CHECK_FABS(normal.pos().x(), 10);
    GGO_CHECK_FABS(normal.pos().y(), 13);
    GGO_CHECK_FABS(normal.pos().z(), 12);
    GGO_CHECK_FABS(normal.dir().x(), 0);
    GGO_CHECK_FABS(normal.dir().y(), 1);
    GGO_CHECK_FABS(normal.dir().z(), 0);

    // Ray's origin inside the shpere.
    ggo::ray3d_float ray6(ggo::point3d_float(10, 11, 12), ggo::vector3d_float(0, 1, 0));
    GGO_CHECK(sphere.intersect_ray(ray6, t, normal) == true);
    GGO_CHECK_FABS(t, 2);
    GGO_CHECK_FABS(normal.pos().x(), 10);
    GGO_CHECK_FABS(normal.pos().y(), 13);
    GGO_CHECK_FABS(normal.pos().z(), 12);
    GGO_CHECK_FABS(normal.dir().x(), 0);
    GGO_CHECK_FABS(normal.dir().y(), -1);
    GGO_CHECK_FABS(normal.dir().z(), 0);
  }
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(shapes3d, basis)
{
  {
    ggo::basis3d_float basis;

    basis.move(1, 2, 1);

    ggo::vector3d_float v1 = basis.vector_from_local_to_world(ggo::vector3d_float(1, 2, 3));
    GGO_CHECK_FABS(v1.x(), 1);
    GGO_CHECK_FABS(v1.y(), 2);
    GGO_CHECK_FABS(v1.z(), 3);

    ggo::vector3d_float v2 = basis.vector_from_world_to_local(ggo::vector3d_float(1, 2, 3));
    GGO_CHECK_FABS(v2.x(), 1);
    GGO_CHECK_FABS(v2.y(), 2);
    GGO_CHECK_FABS(v2.z(), 3);

    ggo::point3d_float p1 = basis.point_from_world_to_local(ggo::point3d_float(3, 2, 0));
    GGO_CHECK_FABS(p1.x(), 2);
    GGO_CHECK_FABS(p1.y(), 0);
    GGO_CHECK_FABS(p1.z(), -1);

    ggo::point3d_float p2 = basis.point_from_world_to_local(ggo::point3d_float(0, 2, 0));
    GGO_CHECK_FABS(p2.x(), -1);
    GGO_CHECK_FABS(p2.y(), 0);
    GGO_CHECK_FABS(p2.z(), -1);    

    basis.rotate_z(ggo::PI<float>() / 4);
    GGO_CHECK_FABS(basis.x().x(), 1 / std::sqrt(2));
    GGO_CHECK_FABS(basis.x().y(), 1 / std::sqrt(2));
    GGO_CHECK_FABS(basis.x().z(), 0);
    GGO_CHECK_FABS(basis.y().x(), -1 / std::sqrt(2));
    GGO_CHECK_FABS(basis.y().y(), 1 / std::sqrt(2));
    GGO_CHECK_FABS(basis.y().z(), 0);
    GGO_CHECK_FABS(basis.z().x(), 0);
    GGO_CHECK_FABS(basis.z().y(), 0);
    GGO_CHECK_FABS(basis.z().z(), 1);

    ggo::point3d_float p3 = basis.point_from_world_to_local(ggo::point3d_float(2, 4, 0));
    GGO_CHECK_FABS(p3.x(), 1.5 * std::sqrt(2));
    GGO_CHECK_FABS(p3.y(), 0.5 * std::sqrt(2));
    GGO_CHECK_FABS(p3.z(), -1);

    ggo::point3d_float p4 = basis.point_from_local_to_world(ggo::point3d_float(1, 2, 0));
    GGO_CHECK_FABS(p4.x(), 1 - 1 / std::sqrt(2));
    GGO_CHECK_FABS(p4.y(), 2 + 3 / std::sqrt(2));
    GGO_CHECK_FABS(p4.z(), 1);
  }
  
  {
    ggo::basis3d_float basis;
    
    basis.pos() = {0, 0, 1};
    basis.look_at({0, 0, 0});
    
    GGO_CHECK_FABS(basis.z().x(), 0);
    GGO_CHECK_FABS(basis.z().y(), 0);
    GGO_CHECK_FABS(basis.z().z(), 1);
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
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(shapes3d, plane3d)
{
  {
    ggo::plane3d_float plane({ 0, 0, 1 }, -1); // z = -1
    float dist = 0;
    ggo::ray3d_float normal;

    ggo::ray3d_float ray1(ggo::point3d_float(2, 3, 5), ggo::vector3d_float(0, 0, -1));
    GGO_CHECK(plane.intersect_ray(ray1, dist, normal) == true);
    GGO_CHECK_FABS(dist, 6);
    GGO_CHECK_FABS(normal.pos().x(), 2);
    GGO_CHECK_FABS(normal.pos().y(), 3);
    GGO_CHECK_FABS(normal.pos().z(), -1);
    GGO_CHECK_FABS(normal.dir().x(), 0);
    GGO_CHECK_FABS(normal.dir().y(), 0);
    GGO_CHECK_FABS(normal.dir().z(), 1);

    ggo::ray3d_float ray2(ggo::point3d_float(2, 4, -2), ggo::vector3d_float(-1, 2, -1));
    GGO_CHECK(plane.intersect_ray(ray2, dist, normal) == false);

    ggo::ray3d_float ray3(ggo::point3d_float(1, 5, -2), ggo::vector3d_float(0, 0, 1));
    GGO_CHECK(plane.intersect_ray(ray3, dist, normal) == false);
  }

  {
    ggo::plane3d_float plane({ 1, 2, -1 }, 1 / std::sqrt(6.f));
    float dist = 0;
    ggo::ray3d_float normal;

    ggo::ray3d_float ray(ggo::point3d_float(1, 3, 2), ggo::vector3d_float(1, -2, 1));
    GGO_CHECK(plane.intersect_ray(ray, dist, normal) == true);
    GGO_CHECK_FABS(dist, std::sqrt(6));
    GGO_CHECK_FABS(normal.pos().x(), 2);
    GGO_CHECK_FABS(normal.pos().y(), 1);
    GGO_CHECK_FABS(normal.pos().z(), 3);
    GGO_CHECK_FABS(normal.dir().x(), 1 / std::sqrt(6.f));
    GGO_CHECK_FABS(normal.dir().y(), 2 / std::sqrt(6.f));
    GGO_CHECK_FABS(normal.dir().z(), -1 / std::sqrt(6.f));
  }
}

/////////////////////////////////////////////////////////////////////  
GGO_TEST(shapes3d, polygon3d)
{
  std::vector<ggo::vertex<float>> vertices;
  vertices.emplace_back(ggo::point3d_float(1, 2, 5), ggo::vector3d_float(0, 0, 1));
  vertices.emplace_back(ggo::point3d_float(2, 2, 5), ggo::vector3d_float(1, 0, 0));
  vertices.emplace_back(ggo::point3d_float(1, 3, 5), ggo::vector3d_float(0, 1, 0));
  
  std::vector<ggo::polygon3d_float::face> faces;
  faces.emplace_back(0, 1, 2);
  
  ggo::polygon3d_float polygon(vertices, faces);
  
  // Hit vertex 1
  {
    ggo::ray3d_float normal, ray(ggo::point3d_float(1, 2, 6), ggo::vector3d_float(0, 0, -1));
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
    ggo::ray3d_float normal, ray(ggo::point3d_float(2, 2, 10), ggo::vector3d_float(0, 0, -1));
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
    ggo::ray3d_float normal, ray(ggo::point3d_float(1, 3, 7), ggo::vector3d_float(0, 0, -1));
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
    ggo::ray3d_float normal, ray(ggo::point3d_float(1.5, 2, 10), ggo::vector3d_float(0, 0, -1));
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
    ggo::ray3d_float normal, ray(ggo::point3d_float(1.5, 2.5, 10), ggo::vector3d_float(0, 0, -1));
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
    ggo::ray3d_float normal, ray(ggo::point3d_float(1, 2.5, 10), ggo::vector3d_float(0, 0, -1));
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
    ggo::ray3d_float normal, ray(ggo::point3d_float(1.25, 2.25, 10), ggo::vector3d_float(0, 0, -1));
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

    GGO_CHECK(polygon.intersect_ray(ggo::ray3d_float(ggo::point3d_float(1.25, 2.25, 4), ggo::vector3d_float(0, 0, -1)), dist, normal) == false);
    GGO_CHECK(polygon.intersect_ray(ggo::ray3d_float(ggo::point3d_float(0, 0, 10), ggo::vector3d_float(0, 0, -1)), dist, normal) == false);
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
  ggo::disc3d_float disc(ggo::ray3d_float({5, 2, 0}, {0, 1, 0}, false), 3);
  
  ggo::ray3d_float normal;
  float dist = 0;
  GGO_CHECK(disc.intersect_ray(ggo::ray3d_float({3, 5, 0}, {0, -1, 0}, false), dist, normal) == true);
  GGO_CHECK_FABS(dist, 3);
  GGO_CHECK_FABS(normal.pos().x(), 3);
  GGO_CHECK_FABS(normal.pos().y(), 2);
  GGO_CHECK_FABS(normal.pos().z(), 0);
  GGO_CHECK_FABS(normal.dir().x(), 0);
  GGO_CHECK_FABS(normal.dir().y(), 1);
  GGO_CHECK_FABS(normal.dir().z(), 0);
  
  GGO_CHECK(disc.intersect_ray(ggo::ray3d_float({1, 5, 0}, {0, -1, 0}, false), dist, normal) == false);
  GGO_CHECK(disc.intersect_ray(ggo::ray3d_float({4, 1, 0}, {0, -1, 0}, false), dist, normal) == false);
  GGO_CHECK(disc.intersect_ray(ggo::ray3d_float({4, 1, 0}, {0, 1, 0}, false), dist, normal) == false);
}

/////////////////////////////////////////////////////////////////////  
GGO_TEST(shapes3d, parallelogram3d)
{
  ggo::parallelogram3d_float parallelogram({4, 3, 1}, {3, 0, 0}, {2, 2, 0});
  
  ggo::ray3d_float normal;
  float dist = 0;
  
  GGO_CHECK(parallelogram.intersect_ray(ggo::ray3d_float({5, 2, 2}, {0, 0, -1}, false), dist, normal) == true);
  GGO_CHECK_FABS(dist, 1);
  GGO_CHECK_FABS(normal.pos().x(), 5);
  GGO_CHECK_FABS(normal.pos().y(), 2);
  GGO_CHECK_FABS(normal.pos().z(), 1);
  GGO_CHECK_FABS(normal.dir().x(), 0);
  GGO_CHECK_FABS(normal.dir().y(), 0);
  GGO_CHECK_FABS(normal.dir().z(), 1);
  
  GGO_CHECK(parallelogram.intersect_ray(ggo::ray3d_float({1, 2, 3}, {0, 0, -1}, false), dist, normal) == true);
  GGO_CHECK_FABS(dist, 2);
  GGO_CHECK_FABS(normal.pos().x(), 1);
  GGO_CHECK_FABS(normal.pos().y(), 2);
  GGO_CHECK_FABS(normal.pos().z(), 1);
  GGO_CHECK_FABS(normal.dir().x(), 0);
  GGO_CHECK_FABS(normal.dir().y(), 0);
  GGO_CHECK_FABS(normal.dir().z(), 1);

  GGO_CHECK(parallelogram.intersect_ray(ggo::ray3d_float({5, 2, 0}, {0, 0, -1}, false), dist, normal) == false);
  GGO_CHECK(parallelogram.intersect_ray(ggo::ray3d_float({5, 2, 2}, {0, 0, 1}, false), dist, normal) == false);
  GGO_CHECK(parallelogram.intersect_ray(ggo::ray3d_float({7, 2, 2}, {0, 0, -1}, false), dist, normal) == false);
  GGO_CHECK(parallelogram.intersect_ray(ggo::ray3d_float({1, 4, 2}, {0, 0, -1}, false), dist, normal) == false);
}
