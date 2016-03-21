#include <ggo_nonreg.h>
#include <ggo_collisions.h>

//////////////////////////////////////////////////////////////////
//GGO_TEST(collisions, ortho_dot)
//{
//  GGO_CHECK_FABS(ggo::ortho_dot({ 1.f, 2.f }, { 3.f, 4.f }), 2.f);
//  GGO_CHECK_FABS(ggo::ortho_dot({ 2.f, 1.f }, { 5.f, 4.f }), -3.f);
//}

//////////////////////////////////////////////////////////////////
GGO_TEST(collisions, half_plane_vs_oriented_box)
{
  ggo::half_plane_float half_plane({ 0.0f, 1.0f }, 2.f);
  ggo::oriented_box_float box({ 3.0f, 5.0f }, { 1.0f, 1.0f }, 2.f, 2.f);

  ggo::point2d_float pos;
  ggo::vector2d_float normal;
  GGO_CHECK(ggo::test_collision(half_plane, box, pos, normal) == false);

  box.move(0.f, -1.f);
  GGO_CHECK(ggo::test_collision(half_plane, box, pos, normal) == true);
  GGO_CHECK_FABS(pos.x(), 3.0f);
  GGO_CHECK_FABS(pos.y(), 2.0f);
  GGO_CHECK_FABS(normal.x(), 0.0f);
  GGO_CHECK_FABS(normal.y(), 1.0f);
}

//////////////////////////////////////////////////////////////////
GGO_TEST(collisions, half_plane_vs_oriented_box_horizontal)
{
  ggo::half_plane_float half_plane({ 0.0f, 1.0f }, 3.f);
  ggo::oriented_box_float box({ 4.0f, 4.0f }, { 1.0f, 0.0f }, 2.f, 2.f);

  ggo::point2d_float pos;
  ggo::vector2d_float normal;
  GGO_CHECK(ggo::test_collision(half_plane, box, pos, normal) == true);
  GGO_CHECK_FABS(pos.x(), 4.0f);
  GGO_CHECK_FABS(pos.y(), 3.0f);
  GGO_CHECK_FABS(normal.x(), 0.0f);
  GGO_CHECK_FABS(normal.y(), 1.0f);
}

//////////////////////////////////////////////////////////////////
GGO_TEST(collisions, oriented_box_vs_oriented_box_one_point_inside)
{
  ggo::oriented_box_float box1({ 5.0f, 3.0f }, { 1.0f, 0.0f }, 3.f, 2.f);

  // Intersection on top edge of box1.
  {
    ggo::oriented_box_float box2({ 6.0f, 6.0f }, { 1.0f, 1.0f }, std::sqrt(2.f), std::sqrt(2.f));

    ggo::point2d_float pos;
    ggo::vector2d_float normal;
    GGO_CHECK(ggo::test_collision(box1, box2, pos, normal) == true);
    GGO_CHECK_FABS(pos.x(), 6.0f);
    GGO_CHECK_FABS(pos.y(), 5.0f);
    GGO_CHECK_FABS(normal.x(), 0.0f);
    GGO_CHECK_FABS(normal.y(), 1.0f);
  }

  // Same test as the previous, but box1 and box2 are inverted when calling test_collision which should result in a reversed normal.
  {
    ggo::oriented_box_float box2({ 6.0f, 6.0f }, { 1.0f, 1.0f }, std::sqrt(2.f), std::sqrt(2.f));

    ggo::point2d_float pos;
    ggo::vector2d_float normal;
    GGO_CHECK(ggo::test_collision(box2, box1, pos, normal) == true);
    GGO_CHECK_FABS(pos.x(), 6.0f);
    GGO_CHECK_FABS(pos.y(), 5.0f);
    GGO_CHECK_FABS(normal.x(), 0.0f);
    GGO_CHECK_FABS(normal.y(), -1.0f);
  }

  // Intersection on bottom edge of box1.
  {
    ggo::oriented_box_float box2({ 4.0f, 0.0f }, { 1.0f, 1.0f }, std::sqrt(2.f), std::sqrt(2.f));

    ggo::point2d_float pos;
    ggo::vector2d_float normal;
    GGO_CHECK(ggo::test_collision(box1, box2, pos, normal) == true);
    GGO_CHECK_FABS(pos.x(), 4.0f);
    GGO_CHECK_FABS(pos.y(), 1.0f);
    GGO_CHECK_FABS(normal.x(), 0.0f);
    GGO_CHECK_FABS(normal.y(), -1.0f);
  }

  // Intersection on left edge of box1.
  {
    ggo::oriented_box_float box2({ 1.0f, 3.0f }, { 1.0f, 1.0f }, std::sqrt(2.f), std::sqrt(2.f));

    ggo::point2d_float pos;
    ggo::vector2d_float normal;
    GGO_CHECK(ggo::test_collision(box1, box2, pos, normal) == true);
    GGO_CHECK_FABS(pos.x(), 2.0f);
    GGO_CHECK_FABS(pos.y(), 3.0f);
    GGO_CHECK_FABS(normal.x(), -1.0f);
    GGO_CHECK_FABS(normal.y(), 0.0f);
  }
}

//////////////////////////////////////////////////////////////////
GGO_TEST(collisions, oriented_box_vs_oriented_box_two_points_inside)
{
  ggo::oriented_box_float box1({ 5.0f, 3.0f }, { 1.0f, 0.0f }, 3.f, 2.f);
  ggo::oriented_box_float box2({ 8.0f, 3.0f }, { 1.0f, 0.0f }, 1.0f, 1.0f);

  {
    ggo::point2d_float pos;
    ggo::vector2d_float normal;

    GGO_CHECK(ggo::test_collision(box1, box2, pos, normal) == true);
    GGO_CHECK_FABS(pos.x(), 8.0f);
    GGO_CHECK_FABS(pos.y(), 3.0f);
    GGO_CHECK_FABS(normal.x(), 1.0f);
    GGO_CHECK_FABS(normal.y(), 0.0f);
  }

  // Boxes are the same, but box1 and box2 are inverted when calling test_collision which should result in a reversed normal.
  {
    ggo::point2d_float pos;
    ggo::vector2d_float normal;

    GGO_CHECK(ggo::test_collision(box2, box1, pos, normal) == true);
    GGO_CHECK_FABS(pos.x(), 8.0f);
    GGO_CHECK_FABS(pos.y(), 3.0f);
    GGO_CHECK_FABS(normal.x(), -1.0f);
    GGO_CHECK_FABS(normal.y(), 0.0f);
  }
}
