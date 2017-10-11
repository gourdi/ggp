#include "../ggo_kernel_nonreg.h"
#include <ggo_shapes3d.h>

/////////////////////////////////////////////////////////////////////  
GGO_TEST(line3d, find_closest_lines_points)
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
}

/////////////////////////////////////////////////////////////////////  
GGO_TEST(line3d, hypot)
{
  const ggo::line3d<float> line({ 2.f, 1.f, 4.f }, { 0.f, 0.f, 1.f }, false);
  GGO_CHECK_FABS(ggo::hypot(line, ggo::pos3f(4.f, 1.f, 1.f)), 4.f);
}
