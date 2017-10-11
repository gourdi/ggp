#include "../ggo_kernel_nonreg.h"
#include <ggo_shapes3d.h>

/////////////////////////////////////////////////////////////////////  
GGO_TEST(triangle3d, not_double_sided_ray_intersection)
{
  {
    ggo::triangle3d<float, false> triangle({ 1.f, 2.f, 5.f }, { 2.f, 2.f, 5.f }, { 1.f, 4.f, 5.f });
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

  {
    ggo::triangle3d<float, false> triangle({ 1.f, 4.f, 5.f }, { 2.f, 2.f, 5.f }, { 1.f, 2.f, 5.f });
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
}

/////////////////////////////////////////////////////////////////////  
GGO_TEST(triangle3d, double_sided_ray_intersection)
{
  ggo::triangle3d<float, true> triangle({ 1.f, 2.f, 5.f }, { 2.f, 2.f, 5.f }, { 1.f, 4.f, 6.f });

  ggo::basis3d_float basis({ 0.f, 0.f, 1.f });
  basis.rotate_z(ggo::pi<float>() / 2);
  auto aabb = triangle.get_bounding_box(basis);
  GGO_CHECK(aabb.has_value() == true);

  GGO_CHECK_FABS(aabb->_x_min, -4.0f);
  GGO_CHECK_FABS(aabb->_x_max, -2.0f);
  GGO_CHECK_FABS(aabb->_y_min, 1.0f);
  GGO_CHECK_FABS(aabb->_y_max, 2.0f);
  GGO_CHECK_FABS(aabb->_z_min, 6.0f);
  GGO_CHECK_FABS(aabb->_z_max, 7.0f);
}
