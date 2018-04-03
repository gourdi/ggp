#include "../../ggo_kernel_nonreg.h"

/////////////////////////////////////////////////////////////////////
GGO_TEST(half_plane, is_point_inside)
{
  ggo::half_plane<float> half_plane({ 1.f, 1.f }, 1.f);

  GGO_CHECK(half_plane.is_point_inside({ 0.f, 0.f }) == true);
  GGO_CHECK(half_plane.is_point_inside({ 1.f, 1.f }) == false);
  GGO_CHECK(half_plane.is_point_inside({ 2.f, 0.f }) == false);
  GGO_CHECK(half_plane.is_point_inside({ 0.f, 2.f }) == false);
  GGO_CHECK(half_plane.is_point_inside({ -1.f, 0.f }) == true);
  GGO_CHECK(half_plane.is_point_inside({ 0.f, -1.f }) == true);
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(half_plane, dist_to_point)
{
  ggo::half_plane<float> half_plane({ 1.f, 1.f }, 1.f);

  GGO_CHECK_FLOAT_EQ(half_plane.dist_to_point({ 0, 0 }), 0);
  GGO_CHECK_FLOAT_EQ(half_plane.dist_to_point({ 0, 1 }), 0);
  GGO_CHECK_FLOAT_EQ(half_plane.dist_to_point({ 1, 0 }), 0);
  GGO_CHECK_FLOAT_EQ(half_plane.dist_to_point({ 1, 1 }), std::sqrt(2.f) - 1.f);
  GGO_CHECK_FLOAT_EQ(half_plane.dist_to_point({ 2, 0 }), std::sqrt(2.f) - 1.f);
  GGO_CHECK_FLOAT_EQ(half_plane.dist_to_point({ 0, 2 }), std::sqrt(2.f) - 1.f);
}
