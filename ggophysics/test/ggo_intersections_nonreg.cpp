//#include <ggo_nonreg.h>
//#include <ggo_intersections.h>
//
///////////////////////////////////////////////////////////////////////
//GGO_TEST(intersections, disc_disc)
//{
//  ggo::vector2d_float normal;
//  float penetration = 0.f;
//
//  GGO_CHECK(ggo::test_intersection(ggo::disc<float>(-2, 0, 1), ggo::disc<float>(2, 0, 1), normal, penetration) == false);
//
//  GGO_CHECK(ggo::test_intersection(ggo::disc<float>(-1, 0, 2), ggo::disc<float>(1, 0, 2), normal, penetration) == true);
//  GGO_CHECK_FABS(normal.x(), 1.f);
//  GGO_CHECK_FABS(normal.y(), 0.f);
//  GGO_CHECK_FABS(penetration, 2.f);
//
//  GGO_CHECK(ggo::test_intersection(ggo::disc<float>(0, 1, 2), ggo::disc<float>(0, -1, 2), normal, penetration) == true);
//  GGO_CHECK_FABS(normal.x(), 0.f);
//  GGO_CHECK_FABS(normal.y(), -1.f);
//  GGO_CHECK_FABS(penetration, 2.f);
//}
//
///////////////////////////////////////////////////////////////////////
//GGO_TEST(intersections, half_plane_disc)
//{
//  ggo::vector2d_float normal;
//  float penetration = 0.f;
//
//  GGO_CHECK(ggo::test_intersection(ggo::half_plane<float>({ 0, 1 }, 1), ggo::disc<float>(0, 3, 1), normal, penetration) == false);
//
//  GGO_CHECK(ggo::test_intersection(ggo::half_plane<float>({ 0, 1 }, 1), ggo::disc<float>(0, 2, 2), normal, penetration) == true);
//  GGO_CHECK_FABS(normal.x(), 0.f);
//  GGO_CHECK_FABS(normal.y(), 1.f);
//  GGO_CHECK_FABS(penetration, 1.f);
//}
//
///////////////////////////////////////////////////////////////////////
//GGO_TEST(intersections, half_plane_oriented_box)
//{
//  ggo::vector2d_float normal;
//  float penetration = 0.f;
//
//  ggo::oriented_box_float box({ 2.f, 3.f }, { 1.0f, 0.0f }, 1.f, 1.f);
//
//  GGO_CHECK(ggo::test_intersection(ggo::half_plane<float>({ 0, 1 }, 1), ggo::disc<float>(0, 3, 1), normal, penetration) == false);
//
//  box.move(0, -2.f);
//  GGO_CHECK(ggo::test_intersection(ggo::half_plane<float>({ 0, 1 }, 1), ggo::disc<float>(0, 2, 2), normal, penetration) == true);
//  GGO_CHECK_FABS(normal.x(), 0.f);
//  GGO_CHECK_FABS(normal.y(), 1.f);
//  GGO_CHECK_FABS(penetration, 1.f);
//}
