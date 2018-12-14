#include <kernel/nonreg/ggo_nonreg.h>
#include <physics/ggo_collisions.h>

//////////////////////////////////////////////////////////////////
GGO_TEST(collisions, half_plane_vs_oriented_box)
{
  ggo::half_plane_float half_plane({ 0.0f, 1.0f }, 2.f);
  ggo::oriented_box_float box({ 3.0f, 5.0f }, { 1.0f, 1.0f }, 2.f, 2.f);

  ggo::pos2f pos;
  ggo::vec2f normal;
  GGO_CHECK(ggo::test_collision(half_plane, box, pos, normal) == false);

  box.move({ 0.f, -1.f });
  GGO_CHECK(ggo::test_collision(half_plane, box, pos, normal) == true);
  GGO_CHECK_FLOAT_EQ(pos.x(), 3.0f);
  GGO_CHECK_FLOAT_EQ(pos.y(), 2.0f);
  GGO_CHECK_FLOAT_EQ(normal.x(), 0.0f);
  GGO_CHECK_FLOAT_EQ(normal.y(), 1.0f);
}

//////////////////////////////////////////////////////////////////
GGO_TEST(collisions, half_plane_vs_oriented_box_horizontal)
{
  ggo::half_plane_float half_plane({ 0.0f, 1.0f }, 3.f);
  ggo::oriented_box_float box({ 4.0f, 4.0f }, { 1.0f, 0.0f }, 2.f, 2.f);

  ggo::pos2f pos;
  ggo::vec2f normal;
  GGO_CHECK(ggo::test_collision(half_plane, box, pos, normal) == true);
  GGO_CHECK_FLOAT_EQ(pos.x(), 4.0f);
  GGO_CHECK_FLOAT_EQ(pos.y(), 3.0f);
  GGO_CHECK_FLOAT_EQ(normal.x(), 0.0f);
  GGO_CHECK_FLOAT_EQ(normal.y(), 1.0f);
}

//////////////////////////////////////////////////////////////////
GGO_TEST(collisions, oriented_box_vs_oriented_box_one_point_inside)
{
  ggo::oriented_box_float box1({ 5.0f, 3.0f }, { 1.0f, 0.0f }, 3.f, 2.f);

  // Intersection on top edge of box1.
  {
    ggo::oriented_box_float box2({ 6.0f, 6.0f }, { 1.0f, 1.0f }, std::sqrt(2.f), std::sqrt(2.f));

    ggo::pos2f pos;
    ggo::vec2f normal;
    GGO_CHECK(ggo::test_collision(box1, box2, pos, normal) == true);
    GGO_CHECK_FLOAT_EQ(pos.x(), 6.0f);
    GGO_CHECK_FLOAT_EQ(pos.y(), 5.0f);
    GGO_CHECK_FLOAT_EQ(normal.x(), 0.0f);
    GGO_CHECK_FLOAT_EQ(normal.y(), 1.0f);
  }

  // Same test as the previous, but box1 and box2 are inverted when calling test_collision which should result in a reversed normal.
  {
    ggo::oriented_box_float box2({ 6.0f, 6.0f }, { 1.0f, 1.0f }, std::sqrt(2.f), std::sqrt(2.f));

    ggo::pos2f pos;
    ggo::vec2f normal;
    GGO_CHECK(ggo::test_collision (box2, box1, pos, normal) == true);
    GGO_CHECK_FLOAT_EQ(pos.x(), 6.0f);
    GGO_CHECK_FLOAT_EQ(pos.y(), 5.0f);
    GGO_CHECK_FLOAT_EQ(normal.x(), 0.0f);
    GGO_CHECK_FLOAT_EQ(normal.y(), -1.0f);
  }

  // Intersection on bottom edge of box1.
  {
    ggo::oriented_box_float box2({ 4.0f, 0.0f }, { 1.0f, 1.0f }, std::sqrt(2.f), std::sqrt(2.f));

    ggo::pos2f pos;
    ggo::vec2f normal;
    GGO_CHECK(ggo::test_collision(box1, box2, pos, normal) == true);
    GGO_CHECK_FLOAT_EQ(pos.x(), 4.0f);
    GGO_CHECK_FLOAT_EQ(pos.y(), 1.0f);
    GGO_CHECK_FLOAT_EQ(normal.x(), 0.0f);
    GGO_CHECK_FLOAT_EQ(normal.y(), -1.0f);
  }

  // Intersection on left edge of box1.
  {
    ggo::oriented_box_float box2({ 1.0f, 3.0f }, { 1.0f, 1.0f }, std::sqrt(2.f), std::sqrt(2.f));

    ggo::pos2f pos;
    ggo::vec2f normal;
    GGO_CHECK(ggo::test_collision(box1, box2, pos, normal) == true);
    GGO_CHECK_FLOAT_EQ(pos.x(), 2.0f);
    GGO_CHECK_FLOAT_EQ(pos.y(), 3.0f);
    GGO_CHECK_FLOAT_EQ(normal.x(), -1.0f);
    GGO_CHECK_FLOAT_EQ(normal.y(), 0.0f);
  }
}

//////////////////////////////////////////////////////////////////
GGO_TEST(collisions, oriented_box_vs_oriented_box_two_points_inside)
{
  ggo::oriented_box_float box1({ 5.0f, 3.0f }, { 1.0f, 0.0f }, 3.f, 2.f);
  ggo::oriented_box_float box2({ 8.0f, 3.0f }, { 1.0f, 0.0f }, 1.0f, 1.0f);

  {
    ggo::pos2f pos;
    ggo::vec2f normal;

    GGO_CHECK(ggo::test_collision(box1, box2, pos, normal) == true);
    GGO_CHECK_FLOAT_EQ(pos.x(), 8.0f);
    GGO_CHECK_FLOAT_EQ(pos.y(), 3.0f);
    GGO_CHECK_FLOAT_EQ(normal.x(), 1.0f);
    GGO_CHECK_FLOAT_EQ(normal.y(), 0.0f);
  }

  // Boxes are the same, but box1 and box2 are inverted when calling test_collision which should result in a reversed normal.
  {
    ggo::pos2f pos;
    ggo::vec2f normal;

    GGO_CHECK(ggo::test_collision(box2, box1, pos, normal) == true);
    GGO_CHECK_FLOAT_EQ(pos.x(), 8.0f);
    GGO_CHECK_FLOAT_EQ(pos.y(), 3.0f);
    GGO_CHECK_FLOAT_EQ(normal.x(), -1.0f);
    GGO_CHECK_FLOAT_EQ(normal.y(), 0.0f);
  }
}
