#include "ggo_canvas.h"

/////////////////////////////////////////////////////////////////////
void ggo::canvas::render(void * buffer, main_direction main_dir, int width, int height, int line_byte_step, pixel_buffer_format pbf) const
{

}

/////////////////////////////////////////////////////////////////////
ggo::pos2i ggo::canvas::from_canvas_to_render(const ggo::pos2f & p, main_direction main_dir, int render_width, int render_height)
{
  switch (main_dir)
  {
  case main_direction::vertical:
  {
    float ratio = float(render_width) / float(render_height);
    int x = ggo::to<int>(ggo::map<float>(p.x(), -ratio, ratio, 0.f, float(render_width)) - 0.5f);
    int y = ggo::to<int>(ggo::map<float>(p.y(), -1.f, 1.f, 0.f, float(render_height)) - 0.5f);
    return pos2i(x, y);
  }
  case main_direction::horizontal:
  {
    float ratio = float(render_height) / float(render_width);
    int x = ggo::to<int>(ggo::map<float>(p.x(), -1.f, 1.f, 0.f, float(render_width)) - 0.5f);
    int y = ggo::to<int>(ggo::map<float>(p.y(), -ratio, ratio, 0.f, float(render_height)) - 0.5f);
    return pos2i(x, y);
  }
  default:
    GGO_FAIL();
    return ggo::pos2i(0, 0);
  }
}

/////////////////////////////////////////////////////////////////////
ggo::pos2f ggo::canvas::from_render_to_canvas(const ggo::pos2i & p, main_direction main_dir, int render_width, int render_height)
{
  switch (main_dir)
  {
  case main_direction::vertical:
  {
    float ratio = float(render_width) / float(render_height);
    float x = ggo::map<float>(p.x() + 0.5f, 0.f, float(render_width), -ratio, ratio);
    float y = ggo::map<float>(p.y() + 0.5f, 0.f, float(render_height), -1.f, 1.f);
    return pos2f(x, y);
  }
  case main_direction::horizontal:
  {
    float ratio = float(render_height) / float(render_width);
    float x = ggo::map<float>(p.x() + 0.5f, 0.f, float(render_width), -1.f, 1.f);
    float y = ggo::map<float>(p.y() + 0.5f, 0.f, float(render_height), -ratio, ratio);
    return pos2f(x, y);
  }
  default:
    GGO_FAIL();
    return ggo::pos2f(0.f, 0.f);
  }
}

