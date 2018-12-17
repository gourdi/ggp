#include "../../ggo_kernel_nonreg.h"

/////////////////////////////////////////////////////////////////////
GGO_TEST(disc, get_bounding_rect)
{
  ggo::disc_f disc(ggo::pos2_f(2.f, 12.f), 10.f);

  ggo::rect_f rect = disc.get_bounding_rect();
  GGO_CHECK_FLOAT_EQ(rect.left(), -8);
  GGO_CHECK_FLOAT_EQ(rect.right(), 12);
  GGO_CHECK_FLOAT_EQ(rect.top(), 22);
  GGO_CHECK_FLOAT_EQ(rect.bottom(), 2);
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(disc, is_point_inside)
{
  ggo::disc_f disc(ggo::pos2_f(2.f, 12.f), 10.f);

  GGO_CHECK(disc.is_point_inside(disc.center()) == true);
  GGO_CHECK(disc.is_point_inside({ 0, 10 }) == true);
  GGO_CHECK(disc.is_point_inside({ 2, 1 }) == false);
  GGO_CHECK(disc.is_point_inside({ 3, 2 }) == false);
  GGO_CHECK(disc.is_point_inside({ 8, 21 }) == false);
  GGO_CHECK(disc.is_point_inside({ 8, 21 }) == false);
  GGO_CHECK(disc.is_point_inside({ -6, 21 }) == false);
  GGO_CHECK(disc.is_point_inside({ -4, -3 }) == false);
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(disc, dist_to_point)
{
  ggo::disc_f disc(ggo::pos2_f(2.f, 12.f), 10.f);

  GGO_CHECK_FLOAT_EQ(disc.dist_to_point(disc.center()), 0);
  GGO_CHECK_FLOAT_EQ(disc.dist_to_point({ 0, 10 }), 0);
  GGO_CHECK_FLOAT_EQ(disc.dist_to_point({ 14, 12 }), 2);
  GGO_CHECK_FLOAT_EQ(disc.dist_to_point({ -10, 12 }), 2);
  GGO_CHECK_FLOAT_EQ(disc.dist_to_point({ 2, 0 }), 2);
  GGO_CHECK_FLOAT_EQ(disc.dist_to_point({ 2, 24 }), 2);
  GGO_CHECK_FLOAT_EQ(disc.dist_to_point({ 2, 24 }), 2);
  GGO_CHECK_FLOAT_EQ(disc.dist_to_point({ 22, 36 }), ggo::distance(disc.center(), ggo::pos2_f(22.f, 36.f)) - disc.radius());
  GGO_CHECK_FLOAT_EQ(disc.dist_to_point({ -10, 100 }), ggo::distance(disc.center(), ggo::pos2_f(-10.f, 100.f)) - disc.radius());
  GGO_CHECK_FLOAT_EQ(disc.dist_to_point({ 0, -50 }), ggo::distance(disc.center(), ggo::pos2_f(0.f, -50.f)) - disc.radius());
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(disc, rect_intersection)
{
  ggo::disc_f disc(ggo::pos2_f(2.f, 12.f), 10.f);

  GGO_CHECK_RECT_INTERSECTION(disc, 0, 1, 11, 12, ggo::rect_intersection::rect_in_shape);
  GGO_CHECK_RECT_INTERSECTION(disc, -20, 20, 0, 40, ggo::rect_intersection::shape_in_rect);
  GGO_CHECK_RECT_INTERSECTION(disc, -20, -18, 0, 40, ggo::rect_intersection::disjoints);
  GGO_CHECK_RECT_INTERSECTION(disc, 13, 14, 0, 40, ggo::rect_intersection::disjoints);
  GGO_CHECK_RECT_INTERSECTION(disc, 0, 20, 23, 24, ggo::rect_intersection::disjoints);
  GGO_CHECK_RECT_INTERSECTION(disc, 13, 14, 0, 20, ggo::rect_intersection::disjoints);
  GGO_CHECK_RECT_INTERSECTION(disc, 10, 11, 3, 4, ggo::rect_intersection::disjoints);
  GGO_CHECK_RECT_INTERSECTION(disc, 10, 11, 20, 21, ggo::rect_intersection::disjoints);
  GGO_CHECK_RECT_INTERSECTION(disc, -7, -6, 20, 21, ggo::rect_intersection::disjoints);
  GGO_CHECK_RECT_INTERSECTION(disc, -7, -6, 3, 4, ggo::rect_intersection::disjoints);
  GGO_CHECK_RECT_INTERSECTION(disc, -10, -6, 0, 20, ggo::rect_intersection::partial_overlap);
  GGO_CHECK_RECT_INTERSECTION(disc, 0, 10, 4, 12, ggo::rect_intersection::partial_overlap);
  GGO_CHECK_RECT_INTERSECTION(disc, 0, 10, 18, 20, ggo::rect_intersection::partial_overlap);
  GGO_CHECK_RECT_INTERSECTION(disc, -6, -4, 18, 20, ggo::rect_intersection::partial_overlap);
  GGO_CHECK_RECT_INTERSECTION(disc, -6, -4, 4, 12, ggo::rect_intersection::partial_overlap);
  GGO_CHECK_RECT_INTERSECTION(disc, -6, 10, 4, 20, ggo::rect_intersection::partial_overlap);
  GGO_CHECK_RECT_INTERSECTION(disc, 2, 12, 4, 20, ggo::rect_intersection::partial_overlap);
  GGO_CHECK_RECT_INTERSECTION(disc, 0, 2, 0, 40, ggo::rect_intersection::partial_overlap);
  GGO_CHECK_RECT_INTERSECTION(disc, -10, 20, 10, 24, ggo::rect_intersection::partial_overlap);
  GGO_CHECK_RECT_INTERSECTION(disc, -10, 20, 0, 10, ggo::rect_intersection::partial_overlap);
  GGO_CHECK_RECT_INTERSECTION(disc, 0, 20, 10, 12, ggo::rect_intersection::partial_overlap);
  GGO_CHECK_RECT_INTERSECTION(disc, -20, 0, 10, 12, ggo::rect_intersection::partial_overlap);
  GGO_CHECK_RECT_INTERSECTION(disc, -10, 10, -2, 0, ggo::rect_intersection::disjoints);
  GGO_CHECK_RECT_INTERSECTION(disc, 14, 16, 0, 12, ggo::rect_intersection::disjoints);
  GGO_CHECK_RECT_INTERSECTION(disc, 0, 4, 23, 24, ggo::rect_intersection::disjoints);
  GGO_CHECK_RECT_INTERSECTION(disc, -20, -10, 0, 12, ggo::rect_intersection::disjoints);
  GGO_CHECK_RECT_INTERSECTION(disc, 10, 12, 0, 4, ggo::rect_intersection::disjoints);
  GGO_CHECK_RECT_INTERSECTION(disc, 10, 12, 21, 22, ggo::rect_intersection::disjoints);
  GGO_CHECK_RECT_INTERSECTION(disc, -10, -6, 21, 22, ggo::rect_intersection::disjoints);
  GGO_CHECK_RECT_INTERSECTION(disc, -10, -8, -4, -2, ggo::rect_intersection::disjoints);
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(disc, basis)
{
  auto world_disc = ggo::from_local_to_world(ggo::disc_f({ 2.f, 1.f }, 2.f), ggo::orthogonal_basis2d<float>({ 3.0f, 1.f }, { 2.0f, 0.f }));
  GGO_CHECK_FLOAT_EQ(world_disc.center().x(), 7.f);
  GGO_CHECK_FLOAT_EQ(world_disc.center().y(), 3.f);
  GGO_CHECK_FLOAT_EQ(world_disc.radius(), 4.f);
}