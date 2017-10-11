#include "../ggo_kernel_nonreg.h"

/////////////////////////////////////////////////////////////////////
GGO_TEST(oriented_box, construction)
{
  ggo::oriented_box<float> box({ 3.f, 2.f }, { 2.f, 0.f }, 2.f, 1.f);

  // Check second direction.
  GGO_CHECK(box.dir().is_normalized());

  // Check second direction.
  GGO_CHECK(box.dir2().is_normalized());
  GGO_CHECK_FABS(ggo::dot(box.dir(), box.dir2()), 0.f);
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(oriented_box, vertices)
{
  ggo::oriented_box<float> box({ 3.f, 2.f }, { 2.f, 0.f }, 2.f, 1.f);

  auto points = box.get_points();
  GGO_CHECK(find_point(points, 1.0f, 1.0f));
  GGO_CHECK(find_point(points, 1.0f, 3.0f));
  GGO_CHECK(find_point(points, 5.0f, 1.0f));
  GGO_CHECK(find_point(points, 5.0f, 3.0f));
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(oriented_box, is_point_inside)
{
  ggo::oriented_box<float> box({ 3.f, 2.f }, { 2.f, 0.f }, 2.f, 1.f);

  GGO_CHECK(box.is_point_inside({ 4.f, 1.5f }) == true);
  GGO_CHECK(box.is_point_inside({ 4.f, 2.5f }) == true);
  GGO_CHECK(box.is_point_inside({ 4.5f, 1.5f }) == true);
  GGO_CHECK(box.is_point_inside({ 2.5f, 2.5f }) == true);
  GGO_CHECK(box.is_point_inside({ 3.f, 3.5f }) == false);
  GGO_CHECK(box.is_point_inside({ 3.f, 0.5f }) == false);
  GGO_CHECK(box.is_point_inside({ 0.5f, 2.f }) == false);
  GGO_CHECK(box.is_point_inside({ 5.5f, 2.f }) == false);
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(oriented_box, rotation)
{
  ggo::oriented_box<float> box({ 3.f, 2.f }, { 2.f, 0.f }, 2.f, 1.f);

  box.rotate(ggo::pi<float>() / 4.f, { 1.f, 1.f });
  auto points2 = box.get_points();
  GGO_CHECK(find_point(points2, 1.0f, 1.0f));
  GGO_CHECK(find_point(points2, 1.0f - std::sqrt(2.f), 1.0f + std::sqrt(2.f)));
  GGO_CHECK(find_point(points2, 1.0f + 2.f * std::sqrt(2.f), 1.0f + 2.f * std::sqrt(2.f)));
  GGO_CHECK(find_point(points2, 1.0f + std::sqrt(2.f), 1.0f + 3.0f * std::sqrt(2.f)));
}
