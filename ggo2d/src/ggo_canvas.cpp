#include "ggo_canvas.h"

/////////////////////////////////////////////////////////////////////
ggo::canvas::canvas(main_direction direction, void * buffer, int width, int height, int line_byte_step, pixel_buffer_format pbf)
:
_main_direction(direction),
_buffer(buffer),
_width(width),
_height(height),
_line_byte_step(line_byte_step),
_pbf(pbf)
{
}

/////////////////////////////////////////////////////////////////////
ggo::pos2i ggo::canvas::from_canvas_to_render(const ggo::pos2f & p) const
{
  switch (_main_direction)
  {
  case main_direction::vertical:
  {
    float ratio = float(_width) / float(_height);
    int x = ggo::to<int>(ggo::map<float>(p.x(), -ratio, ratio, 0.f, float(_width)) - 0.5f);
    int y = ggo::to<int>(ggo::map<float>(p.y(), -1.f, 1.f, 0.f, float(_height)) - 0.5f);
    return pos2i(x, y);
  }
  case main_direction::horizontal:
  {
    float ratio = float(_height) / float(_width);
    int x = ggo::to<int>(ggo::map<float>(p.x(), -1.f, 1.f, 0.f, float(_width)) - 0.5f);
    int y = ggo::to<int>(ggo::map<float>(p.y(), -ratio, ratio, 0.f, float(_height)) - 0.5f);
    return pos2i(x, y);
  }
  default:
    GGO_FAIL();
    return ggo::pos2i(0, 0);
  }
}

/////////////////////////////////////////////////////////////////////
ggo::pos2f ggo::canvas::from_render_to_canvas(const ggo::pos2i & p) const
{
  switch (_main_direction)
  {
  case main_direction::vertical:
  {
    float ratio = float(_width) / float(_height);
    float x = ggo::map<float>(p.x() + 0.5f, 0.f, float(_width), -ratio, ratio);
    float y = ggo::map<float>(p.y() + 0.5f, 0.f, float(_height), -1.f, 1.f);
    return pos2f(x, y);
  }
  case main_direction::horizontal:
  {
    float ratio = float(_height) / float(_width);
    float x = ggo::map<float>(p.x() + 0.5f, 0.f, float(_width), -1.f, 1.f);
    float y = ggo::map<float>(p.y() + 0.5f, 0.f, float(_height), -ratio, ratio);
    return pos2f(x, y);
  }
  default:
    GGO_FAIL();
    return ggo::pos2f(0.f, 0.f);
  }
}

