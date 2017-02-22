#include "ggo_canvas.h"

///////////////////////////////////////////////////////////////////
void ggo::canvas::push_shape(const dyn_paint_shape<float, ggo::color_8u, ggo::color_8u> & shape)
{
  _shapes.push_back(shape);
}

/////////////////////////////////////////////////////////////////////
void ggo::canvas::render(void * buffer, main_direction main_dir, int width, int height, int line_byte_step, pixel_buffer_format pbf) const
{
 /* const ggo::pixel_rect clipping = ggo::pixel_rect::from_width_height(width, height);

  switch (pbf)
  {
  case bgra_8u_yd:
    paint_shapes<bgra_8u_yd, sampling_4x4>(buffer, width, height, line_byte_step, _shapes.begin(), _shapes.end(), clipping);
    break;
  default:
    GGO_FAIL();
    break;
  }*/
}

/////////////////////////////////////////////////////////////////////
float ggo::canvas::from_canvas_to_render(float dist, main_direction main_dir, int render_width, int render_height)
{
  switch (main_dir)
  {
  case main_direction::vertical:
    return render_height * dist / 2.0f;
  case main_direction::horizontal:
    return render_width * dist / 2.0f;
  default:
    GGO_FAIL();
    return 0.f;
  }
}

/////////////////////////////////////////////////////////////////////
float ggo::canvas::from_render_to_canvas(float dist, main_direction main_dir, int render_width, int render_height)
{
  switch (main_dir)
  {
  case main_direction::vertical:
    return 2.0f * dist / render_height;
  case main_direction::horizontal:
    return 2.0f * dist / render_width;
  default:
    GGO_FAIL();
    return 0.f;
  }
}

/////////////////////////////////////////////////////////////////////
ggo::pos2f ggo::canvas::from_canvas_to_render(const ggo::pos2f & p, main_direction main_dir, int render_width, int render_height)
{
  switch (main_dir)
  {
  case main_direction::vertical:
  {
    float ratio = float(render_width) / float(render_height);
    float x = ggo::map<float>(p.x(), -ratio, ratio, -0.5f, float(render_width) - 0.5f);
    float y = ggo::map<float>(p.y(), -1.f, 1.f, -0.5f, float(render_height) - 0.5f);
    return pos2f(x, y);
  }
  case main_direction::horizontal:
  {
    float ratio = float(render_height) / float(render_width);
    float x = ggo::map<float>(p.x(), -1.f, 1.f, -0.5f, float(render_width) - 0.5f);
    float y = ggo::map<float>(p.y(), -ratio, ratio, -0.5f, float(render_height) - 0.5f);
    return pos2f(x, y);
  }
  default:
    GGO_FAIL();
    return ggo::pos2f(0, 0);
  }
}

/////////////////////////////////////////////////////////////////////
ggo::pos2f ggo::canvas::from_render_to_canvas(const ggo::pos2f & p, main_direction main_dir, int render_width, int render_height)
{
  switch (main_dir)
  {
  case main_direction::vertical:
  {
    float ratio = float(render_width) / float(render_height);
    float x = ggo::map(p.x(), -0.5f, float(render_width) - 0.5f, -ratio, ratio);
    float y = ggo::map(p.y(), -0.5f, float(render_height) - 0.5f, -1.f, 1.f);
    return pos2f(x, y);
  }
  case main_direction::horizontal:
  {
    float ratio = float(render_height) / float(render_width);
    float x = ggo::map(p.x(), -0.5f, float(render_width) - 0.5f, -1.f, 1.f);
    float y = ggo::map(p.y(), -0.5f, float(render_height) - 0.5f, -ratio, ratio);
    return pos2f(x, y);
  }
  default:
    GGO_FAIL();
    return ggo::pos2f(0.f, 0.f);
  }
}

/////////////////////////////////////////////////////////////////////
ggo::pos2i ggo::canvas::from_canvas_to_render_pixel(const ggo::pos2f & p, main_direction main_dir, int render_width, int render_height)
{
  const ggo::pos2f pf = from_canvas_to_render(p, main_dir, render_width, render_height);

  return ggo::pos2i(ggo::to<int>(pf.x()), ggo::to<int>(pf.y()));
}

/////////////////////////////////////////////////////////////////////
ggo::pos2f ggo::canvas::from_render_pixel_to_canvas(const ggo::pos2i & p, main_direction main_dir, int render_width, int render_height)
{
  const ggo::pos2f pf(ggo::to<float>(p.x()), ggo::to<float>(p.y()));
  
  return from_render_to_canvas(pf, main_dir, render_width, render_height);
}

