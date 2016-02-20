#include <ggo_nonreg.h>
#include <ggo_intersections.h>

GGO_TEST(intersections, disc_disc)
{
  ggo::vector2d_float normal;

  GGO_CHECK(ggo::test_intersection(ggo::disc<float>(-2, 0, 1), ggo::disc<float>(2, 0, 1), normal) == false);

  GGO_CHECK(ggo::test_intersection(ggo::disc<float>(-1, 0, 2), ggo::disc<float>(1, 0, 2), normal) == true);
  GGO_CHECK_FABS(normal.x(), 1.f);
  GGO_CHECK_FABS(normal.y(), 0.f);

  GGO_CHECK(ggo::test_intersection(ggo::disc<float>(0, 1, 2), ggo::disc<float>(0, -1, 2), normal) == true);
  GGO_CHECK_FABS(normal.x(), 0.f);
  GGO_CHECK_FABS(normal.y(), -1.f);
}

GGO_TEST(intersections, half_plane_disc)
{
  ggo::vector2d_float normal;

  GGO_CHECK(ggo::test_intersection(ggo::half_plane<float>({ 0, 1 }, 1), ggo::disc<float>(0, 3, 1), normal) == false);

  GGO_CHECK(ggo::test_intersection(ggo::half_plane<float>({ 0, 1 }, 1), ggo::disc<float>(0, 2, 2), normal) == true);
  GGO_CHECK_FABS(normal.x(), 0.f);
  GGO_CHECK_FABS(normal.y(), 1.f);
}
