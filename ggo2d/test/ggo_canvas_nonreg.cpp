#include <ggo_nonreg.h>
#include <ggo_canvas.h>

/////////////////////////////////////////////////////////////////////
GGO_TEST(canvas, from_render_to_view_distance)
{
  {
    const float view_size = 2.f;
    const int render_width = 16;
    const int render_height = 8;

    GGO_CHECK_FABS(ggo::canvas::from_render_to_view(2.f, view_size, ggo::canvas::main_direction::vertical, render_width, render_height), 1.0f);
    GGO_CHECK_FABS(ggo::canvas::from_render_to_view(2.f, view_size, ggo::canvas::main_direction::horizontal, render_width, render_height), 0.5f);
  }

  {
    const float view_size = 2.f;
    const int render_width = 8;
    const int render_height = 16;

    GGO_CHECK_FABS(ggo::canvas::from_render_to_view(8.f, view_size, ggo::canvas::main_direction::vertical, render_width, render_height), 2.0f);
    GGO_CHECK_FABS(ggo::canvas::from_render_to_view(4.f, view_size, ggo::canvas::main_direction::horizontal, render_width, render_height), 2.0f);
  }
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(canvas, from_view_to_render_distance)
{
  {
    const float view_size = 2.f;
    const int render_width = 16;
    const int render_height = 8;

    GGO_CHECK_FABS(ggo::canvas::from_view_to_render(2.f, view_size, ggo::canvas::main_direction::vertical, render_width, render_height), 4.0f);
    GGO_CHECK_FABS(ggo::canvas::from_view_to_render(2.f, view_size, ggo::canvas::main_direction::horizontal, render_width, render_height), 8.0f);
  }

  {
    const float view_size = 2.f;
    const int render_width = 8;
    const int render_height = 16;

    GGO_CHECK_FABS(ggo::canvas::from_view_to_render(2.f, view_size, ggo::canvas::main_direction::vertical, render_width, render_height), 8.0f);
    GGO_CHECK_FABS(ggo::canvas::from_view_to_render(2.f, view_size, ggo::canvas::main_direction::horizontal, render_width, render_height), 4.0f);
  }
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(canvas, from_render_to_view_position)
{
  {
    const ggo::canvas::view view{ { 5.f, 3.f }, 1.f, ggo::canvas::main_direction::vertical };
    const int render_width = 8;
    const int render_height = 4;

    const ggo::pos2f p1 = ggo::canvas::from_render_to_view({ -0.5f, -0.5f }, view, render_width, render_height, false);
    GGO_CHECK_FABS(p1.x(), 3.0f);
    GGO_CHECK_FABS(p1.y(), 2.0f);

    const ggo::pos2f p2 = ggo::canvas::from_render_to_view({ 7.5f, 3.5f }, view, render_width, render_height, false);
    GGO_CHECK_FABS(p2.x(), 7.0f);
    GGO_CHECK_FABS(p2.y(), 4.0f);

    const ggo::pos2f p3 = ggo::canvas::from_render_pixel_to_view({0, 0}, view, render_width, render_height, false);
    GGO_CHECK_FABS(p3.x(), 3.25f);
    GGO_CHECK_FABS(p3.y(), 2.25f);

    const ggo::pos2f p4 = ggo::canvas::from_render_pixel_to_view({ 7, 3 }, view, render_width, render_height, false);
    GGO_CHECK_FABS(p4.x(), 6.75f);
    GGO_CHECK_FABS(p4.y(), 3.75f);
  }

  {
    const ggo::canvas::view view{ { 16.f, 12.f }, 8.f, ggo::canvas::main_direction::vertical };
    const int render_width = 4;
    const int render_height = 8;

    const ggo::pos2f p1 = ggo::canvas::from_render_to_view({ -0.5f, -0.5f }, view, render_width, render_height, false);
    GGO_CHECK_FABS(p1.x(), 12.0f);
    GGO_CHECK_FABS(p1.y(), 4.0f);

    const ggo::pos2f p2 = ggo::canvas::from_render_to_view({ 3.5f, 7.5f }, view, render_width, render_height, false);
    GGO_CHECK_FABS(p2.x(), 20.0f);
    GGO_CHECK_FABS(p2.y(), 20.0f);

    const ggo::pos2f p3 = ggo::canvas::from_render_pixel_to_view({ 0, 0 }, view, render_width, render_height, false);
    GGO_CHECK_FABS(p3.x(), 13.0f);
    GGO_CHECK_FABS(p3.y(), 5.0f);

    const ggo::pos2f p4 = ggo::canvas::from_render_pixel_to_view({ 3, 7 }, view, render_width, render_height, false);
    GGO_CHECK_FABS(p4.x(), 19.0f);
    GGO_CHECK_FABS(p4.y(), 19.0f);
  }

  {
    const ggo::canvas::view view{ { 6.f, 5.f }, 4.f, ggo::canvas::main_direction::horizontal };
    const int render_width = 8;
    const int render_height = 4;

    const ggo::pos2f p1 = ggo::canvas::from_render_to_view({ -0.5f, -0.5f }, view, render_width, render_height, false);
    GGO_CHECK_FABS(p1.x(), 2.0f);
    GGO_CHECK_FABS(p1.y(), 3.0f);

    const ggo::pos2f p2 = ggo::canvas::from_render_to_view({ 7.5f, 3.5f }, view, render_width, render_height, false);
    GGO_CHECK_FABS(p2.x(), 10.0f);
    GGO_CHECK_FABS(p2.y(), 7.0f);

    const ggo::pos2f p3 = ggo::canvas::from_render_pixel_to_view({ 0, 0 }, view, render_width, render_height, false);
    GGO_CHECK_FABS(p3.x(), 2.5f);
    GGO_CHECK_FABS(p3.y(), 3.5f);

    const ggo::pos2f p4 = ggo::canvas::from_render_pixel_to_view({ 7, 3 }, view, render_width, render_height, false);
    GGO_CHECK_FABS(p4.x(), 9.5f);
    GGO_CHECK_FABS(p4.y(), 6.5f);
  }
  
  {
    const ggo::canvas::view view{ { 16.f, 12.f }, 4.f, ggo::canvas::main_direction::horizontal };
    const int render_width = 4;
    const int render_height = 8;

    const ggo::pos2f p1 = ggo::canvas::from_render_to_view({ -0.5f, -0.5f }, view, render_width, render_height, false);
    GGO_CHECK_FABS(p1.x(), 12.0f);
    GGO_CHECK_FABS(p1.y(), 4.0f);

    const ggo::pos2f p2 = ggo::canvas::from_render_to_view({ 3.5f, 7.5f }, view, render_width, render_height, false);
    GGO_CHECK_FABS(p2.x(), 20.0f);
    GGO_CHECK_FABS(p2.y(), 20.0f);

    const ggo::pos2f p3 = ggo::canvas::from_render_pixel_to_view({ 0, 0 }, view, render_width, render_height, false);
    GGO_CHECK_FABS(p3.x(), 13.0f);
    GGO_CHECK_FABS(p3.y(), 5.0f);

    const ggo::pos2f p4 = ggo::canvas::from_render_pixel_to_view({ 3, 7 }, view, render_width, render_height, false);
    GGO_CHECK_FABS(p4.x(), 19.0f);
    GGO_CHECK_FABS(p4.y(), 19.0f);
  }
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(canvas, from_view_to_render_position)
{
  {
    const ggo::canvas::view view{ { 6.f, 5.f }, 2.f, ggo::canvas::main_direction::vertical };
    const int render_width = 8;
    const int render_height = 4;

    const ggo::pos2f p1 = ggo::canvas::from_view_to_render({ 2.0f, 3.0f }, view, render_width, render_height, false);
    GGO_CHECK_FABS(p1.x(), -0.5f);
    GGO_CHECK_FABS(p1.y(), -0.5f);

    const ggo::pos2f p2 = ggo::canvas::from_view_to_render({ 10.0f, 7.0f }, view, render_width, render_height, false);
    GGO_CHECK_FABS(p2.x(), 7.5f);
    GGO_CHECK_FABS(p2.y(), 3.5f);

    const ggo::pos2i p3 = ggo::canvas::from_view_to_render_pixel({ 2.9f, 3.9f }, view, render_width, render_height, false);
    GGO_CHECK_EQ(p3.x(), 0);
    GGO_CHECK_EQ(p3.y(), 0);

    const ggo::pos2i p4 = ggo::canvas::from_view_to_render_pixel({ 3.1f, 4.1f }, view, render_width, render_height, false);
    GGO_CHECK_EQ(p4.x(), 1);
    GGO_CHECK_EQ(p4.y(), 1);
  }

  {
    const ggo::canvas::view view{ { 16.f, 12.f }, 8.f, ggo::canvas::main_direction::vertical };
    const int render_width = 4;
    const int render_height = 8;

    const ggo::pos2f p1 = ggo::canvas::from_view_to_render({ 12.0f, 4.0f }, view, render_width, render_height, false);
    GGO_CHECK_FABS(p1.x(), -0.5f);
    GGO_CHECK_FABS(p1.y(), -0.5f);

    const ggo::pos2f p2 = ggo::canvas::from_view_to_render({ 20.0f, 20.0f }, view, render_width, render_height, false);
    GGO_CHECK_FABS(p2.x(), 3.5f);
    GGO_CHECK_FABS(p2.y(), 7.5f);

    const ggo::pos2i p3 = ggo::canvas::from_view_to_render_pixel({ 13.9f, 5.9f }, view, render_width, render_height, false);
    GGO_CHECK_EQ(p3.x(), 0);
    GGO_CHECK_EQ(p3.y(), 0);

    const ggo::pos2i p4 = ggo::canvas::from_view_to_render_pixel({ 14.1f, 6.1f }, view, render_width, render_height, false);
    GGO_CHECK_EQ(p4.x(), 1);
    GGO_CHECK_EQ(p4.y(), 1);
  }

  {
    const ggo::canvas::view view{ { 6.f, 5.f }, 4.f, ggo::canvas::main_direction::horizontal };
    const int render_width = 8;
    const int render_height = 4;

    const ggo::pos2f p1 = ggo::canvas::from_view_to_render({ 2.0f, 3.0f }, view, render_width, render_height, false);
    GGO_CHECK_FABS(p1.x(), -0.5f);
    GGO_CHECK_FABS(p1.y(), -0.5f);

    const ggo::pos2f p2 = ggo::canvas::from_view_to_render({ 10.0f, 7.0f }, view, render_width, render_height, false);
    GGO_CHECK_FABS(p2.x(), 7.5f);
    GGO_CHECK_FABS(p2.y(), 3.5f);

    const ggo::pos2i p3 = ggo::canvas::from_view_to_render_pixel({ 2.9f, 3.9f }, view, render_width, render_height, false);
    GGO_CHECK_EQ(p3.x(), 0);
    GGO_CHECK_EQ(p3.y(), 0);

    const ggo::pos2i p4 = ggo::canvas::from_view_to_render_pixel({ 9.1f, 6.1f }, view, render_width, render_height, false);
    GGO_CHECK_EQ(p4.x(), 7);
    GGO_CHECK_EQ(p4.y(), 3);
  }

  {
    const ggo::canvas::view view{ {16.f, 12.f}, 4.f, ggo::canvas::main_direction::horizontal };
    const int render_width = 4;
    const int render_height = 8;

    const ggo::pos2f p1 = ggo::canvas::from_view_to_render({ 12.0f, 4.0f }, view, render_width, render_height, false);
    GGO_CHECK_FABS(p1.x(), -0.5f);
    GGO_CHECK_FABS(p1.y(), -0.5f);

    const ggo::pos2f p2 = ggo::canvas::from_view_to_render({ 16.0f, 12.0f }, view, render_width, render_height, false);
    GGO_CHECK_FABS(p2.x(), 1.5f);
    GGO_CHECK_FABS(p2.y(), 3.5f);

    const ggo::pos2i p3 = ggo::canvas::from_view_to_render_pixel({ 15.9f, 11.9f }, view, render_width, render_height, false);
    GGO_CHECK_EQ(p3.x(), 1);
    GGO_CHECK_EQ(p3.y(), 3);

    const ggo::pos2i p4 = ggo::canvas::from_view_to_render_pixel({ 16.1f, 12.1f }, view, render_width, render_height, false);
    GGO_CHECK_EQ(p4.x(), 2);
    GGO_CHECK_EQ(p4.y(), 4);
  }
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(canvas, io)
{
  ggo::canvas canvas;

  auto * disc = canvas.create_disc();
  disc->_color = ggo::white_8u();
  disc->get_disc().center() = { 1.f, 2.f };
  disc->get_disc().radius() = 3.f;

  auto * polygon = canvas.create_polygon();
  polygon->_color = ggo::red_8u();
  polygon->get_polygon().add_point({ 1.f, 2.f });
  polygon->get_polygon().add_point({ 3.f, 4.f });
  polygon->get_polygon().add_point({ 5.f, 6.f });

  std::string xml = canvas.to_string();

  ggo::canvas canvas2(xml);
}