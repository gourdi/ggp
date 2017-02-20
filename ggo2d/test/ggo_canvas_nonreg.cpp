#include <ggo_nonreg.h>
#include <ggo_canvas.h>

/////////////////////////////////////////////////////////////////////
GGO_TEST(canvas, from_render_to_canvas)
{
  {
    const int width = 8;
    const int height = 4;
    const ggo::canvas::main_direction main_dir = ggo::canvas::main_direction::vertical;

    const ggo::pos2f p1 = ggo::canvas::from_render_to_canvas({ 0, 0 }, main_dir, width, height);
    GGO_CHECK_FABS(p1.x(), -1.75f);
    GGO_CHECK_FABS(p1.y(), -0.75f);

    const ggo::pos2f p2 = ggo::canvas::from_render_to_canvas({ 7, 3 }, main_dir, width, height);
    GGO_CHECK_FABS(p2.x(), 1.75f);
    GGO_CHECK_FABS(p2.y(), 0.75f);
  }

  {
    const int width = 4;
    const int height = 8;
    const ggo::canvas::main_direction main_dir = ggo::canvas::main_direction::vertical;

    const ggo::pos2f p1 = ggo::canvas::from_render_to_canvas({ 0, 0 }, main_dir, width, height);
    GGO_CHECK_FABS(p1.x(), -3.f / 8.f);
    GGO_CHECK_FABS(p1.y(), -7.f / 8.f);

    const ggo::pos2f p2 = ggo::canvas::from_render_to_canvas({ 3, 7 }, main_dir, width, height);
    GGO_CHECK_FABS(p2.x(), 3.f / 8.f);
    GGO_CHECK_FABS(p2.y(), 7.f / 8.f);
  }

  {
    const int width = 4;
    const int height = 8;
    const ggo::canvas::main_direction main_dir = ggo::canvas::main_direction::horizontal;

    const ggo::pos2f p1 = ggo::canvas::from_render_to_canvas({ 0, 0 }, main_dir, width, height);
    GGO_CHECK_FABS(p1.x(), -0.75f );
    GGO_CHECK_FABS(p1.y(), -1.75f);

    const ggo::pos2f p2 = ggo::canvas::from_render_to_canvas({ 3, 7 }, main_dir, width, height);
    GGO_CHECK_FABS(p2.x(), 0.75f);
    GGO_CHECK_FABS(p2.y(), 1.75f);
  }

  {
    const int width = 8;
    const int height = 4;
    const ggo::canvas::main_direction main_dir = ggo::canvas::main_direction::horizontal;

    const ggo::pos2f p1 = ggo::canvas::from_render_to_canvas({ 0, 0 }, main_dir, width, height);
    GGO_CHECK_FABS(p1.x(), -7.f / 8.f);
    GGO_CHECK_FABS(p1.y(), -3.f / 8.f);

    const ggo::pos2f p2 = ggo::canvas::from_render_to_canvas({ 7, 3 }, main_dir, width, height);
    GGO_CHECK_FABS(p2.x(), 7.f / 8.f);
    GGO_CHECK_FABS(p2.y(), 3.f / 8.f);
  }
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(canvas, from_canvas_to_render)
{
  {
    const int width = 8;
    const int height = 4;
    const ggo::canvas::main_direction main_dir = ggo::canvas::main_direction::vertical;

    const ggo::pos2i p1 = ggo::canvas::from_canvas_to_render({ -1.1f, -0.9f }, main_dir, width, height);
    GGO_CHECK_EQ(p1.x(), 1);
    GGO_CHECK_EQ(p1.y(), 0);

    const ggo::pos2i p2 = ggo::canvas::from_canvas_to_render({ -0.9f, -0.9f }, main_dir, width, height);
    GGO_CHECK_EQ(p2.x(), 2);
    GGO_CHECK_EQ(p2.y(), 0);

    const ggo::pos2i p3 = ggo::canvas::from_canvas_to_render({ 1.9f, 0.9f }, main_dir, width, height);
    GGO_CHECK_EQ(p3.x(), 7);
    GGO_CHECK_EQ(p3.y(), 3);

    const ggo::pos2i p4 = ggo::canvas::from_canvas_to_render({ -2.1f, -1.1f }, main_dir, width, height);
    GGO_CHECK_EQ(p4.x(), -1);
    GGO_CHECK_EQ(p4.y(), -1);
  }

  {
    const int width = 4;
    const int height = 8;
    const ggo::canvas::main_direction main_dir = ggo::canvas::main_direction::vertical;

    const ggo::pos2i p1 = ggo::canvas::from_canvas_to_render({ -0.26f, -0.76f }, main_dir, width, height);
    GGO_CHECK_EQ(p1.x(), 0);
    GGO_CHECK_EQ(p1.y(), 0);

    const ggo::pos2i p2 = ggo::canvas::from_canvas_to_render({ -0.24f, -0.74f }, main_dir, width, height);
    GGO_CHECK_EQ(p2.x(), 1);
    GGO_CHECK_EQ(p2.y(), 1);

    const ggo::pos2i p3 = ggo::canvas::from_canvas_to_render({ -0.51f, -1.1f }, main_dir, width, height);
    GGO_CHECK_EQ(p3.x(), -1);
    GGO_CHECK_EQ(p3.y(), -1);

    const ggo::pos2i p4 = ggo::canvas::from_canvas_to_render({ 0.49f, 0.9f }, main_dir, width, height);
    GGO_CHECK_EQ(p4.x(), 3);
    GGO_CHECK_EQ(p4.y(), 7);
  }

  {
    const int width = 8;
    const int height = 4;
    const ggo::canvas::main_direction main_dir = ggo::canvas::main_direction::horizontal;

    const ggo::pos2i p1 = ggo::canvas::from_canvas_to_render({ -1.1f, -0.51f }, main_dir, width, height);
    GGO_CHECK_EQ(p1.x(), -1);
    GGO_CHECK_EQ(p1.y(), -1);

    const ggo::pos2i p2 = ggo::canvas::from_canvas_to_render({ -0.9f, -0.49f }, main_dir, width, height);
    GGO_CHECK_EQ(p2.x(), 0);
    GGO_CHECK_EQ(p2.y(), 0);

    const ggo::pos2i p3 = ggo::canvas::from_canvas_to_render({ 0.9f, 0.49f }, main_dir, width, height);
    GGO_CHECK_EQ(p3.x(), 7);
    GGO_CHECK_EQ(p3.y(), 3);

    const ggo::pos2i p4 = ggo::canvas::from_canvas_to_render({ 1.1f, 0.51f }, main_dir, width, height);
    GGO_CHECK_EQ(p4.x(), 8);
    GGO_CHECK_EQ(p4.y(), 4);
  }

  {
    const int width = 4;
    const int height = 8;
    const ggo::canvas::main_direction main_dir = ggo::canvas::main_direction::horizontal;

    const ggo::pos2i p1 = ggo::canvas::from_canvas_to_render({ -1.1f, -2.1f }, main_dir, width, height);
    GGO_CHECK_EQ(p1.x(), -1);
    GGO_CHECK_EQ(p1.y(), -1);

    const ggo::pos2i p2 = ggo::canvas::from_canvas_to_render({ -0.9f, -1.9f }, main_dir, width, height);
    GGO_CHECK_EQ(p2.x(), 0);
    GGO_CHECK_EQ(p2.y(), 0);

    const ggo::pos2i p3 = ggo::canvas::from_canvas_to_render({ 0.9f, 1.9f }, main_dir, width, height);
    GGO_CHECK_EQ(p3.x(), 3);
    GGO_CHECK_EQ(p3.y(), 7);

    const ggo::pos2i p4 = ggo::canvas::from_canvas_to_render({ 1.1f, 2.1f }, main_dir, width, height);
    GGO_CHECK_EQ(p4.x(), 4);
    GGO_CHECK_EQ(p4.y(), 8);
  }
}
