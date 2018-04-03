#include "../../ggo_kernel_nonreg.h"
#include <ggo_shapes3d.h>

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
  GGO_CHECK_FLOAT_EQ(normal.pos().x(), dist);
  GGO_CHECK_FLOAT_EQ(normal.pos().y(), 0.f);
  GGO_CHECK_FLOAT_EQ(normal.pos().z(), 0.f);
  GGO_CHECK_FLOAT_EQ(normal.dir().x(), -1.f);
  GGO_CHECK_FLOAT_EQ(normal.dir().y(), 0.f);
  GGO_CHECK_FLOAT_EQ(normal.dir().z(), 0.f);
}
