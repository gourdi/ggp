#include "../ggo_kernel_nonreg.h"
#include <ggo_shapes3d.h>

/////////////////////////////////////////////////////////////////////  
GGO_TEST(cylinder3d, ray_intersection)
{
  ggo::cylinder3d_float cylinder({ 2.f, 4.f, 0.f }, { 0.f, 0.f, 1.f }, 10.f, 2.f);
  ggo::ray3d_float ray({ 2.f, 1.f, 0.f }, { 0.f, 1.f, 0.f });

  float dist_inf = 0.f;
  float dist_sup = 0.f;
  GGO_CHECK(cylinder.intersect_line(ray, dist_inf, dist_sup) == true);
  GGO_CHECK_FLOAT_EQ(dist_inf, 1.f);
  GGO_CHECK_FLOAT_EQ(dist_sup, 5.f);

  // Ray outside the cylinder.
  {
    float dist = 0.f;
    ggo::ray3d_float normal;
    GGO_CHECK(cylinder.intersect_ray(ray, dist, normal) == true);
    GGO_CHECK_FLOAT_EQ(dist, 1.f);
    GGO_CHECK_FLOAT_EQ(normal.pos().x(), 2.f);
    GGO_CHECK_FLOAT_EQ(normal.pos().y(), 2.f);
    GGO_CHECK_FLOAT_EQ(normal.pos().z(), 0.f);
    GGO_CHECK_FLOAT_EQ(normal.dir().x(), 0.f);
    GGO_CHECK_FLOAT_EQ(normal.dir().y(), -1.f);
    GGO_CHECK_FLOAT_EQ(normal.dir().z(), 0.f);
  }

  // Ray inside the cylinder.
  {
    float dist = 0.f;
    ggo::ray3d_float normal;
    GGO_CHECK(cylinder.intersect_ray(ggo::ray3d_float({ 2.f, 4.f, 1.f }, { 0.f, 1.f, 0.f }), dist, normal) == true);
    GGO_CHECK_FLOAT_EQ(dist, 2.f);
    GGO_CHECK_FLOAT_EQ(normal.pos().x(), 2.f);
    GGO_CHECK_FLOAT_EQ(normal.pos().y(), 6.f);
    GGO_CHECK_FLOAT_EQ(normal.pos().z(), 1.f);
    GGO_CHECK_FLOAT_EQ(normal.dir().x(), 0.f);
    GGO_CHECK_FLOAT_EQ(normal.dir().y(), -1.f);
    GGO_CHECK_FLOAT_EQ(normal.dir().z(), 0.f);
  }
}
