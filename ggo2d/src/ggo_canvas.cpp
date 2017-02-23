#include "ggo_canvas.h"
#include <ggo_buffer_fill.h>

///////////////////////////////////////////////////////////////////
ggo::paintable_shape2d_abc<float> * ggo::canvas::disc::create_render_shape(main_direction main_dir, int render_width, int render_height) const
{
  const pos2f center = from_canvas_to_render(_disc.center(), main_dir, render_width, render_height);
  const float radius = from_canvas_to_render(_disc.radius(), main_dir, render_width, render_height);

  return new disc_float(center, radius);
}

///////////////////////////////////////////////////////////////////
ggo::disc_float * ggo::canvas::create_disc()
{
  ggo::canvas::disc * canvas_disc = new ggo::canvas::disc();

  _shapes.emplace_back(std::unique_ptr<shape_abc>(canvas_disc));

  return &canvas_disc->_disc;
}

///////////////////////////////////////////////////////////////////
void ggo::canvas::remove_shape(const ggo::paintable_shape2d_abc<float> * shape)
{
  ggo::remove_first_if(_shapes, [&](std::unique_ptr<shape_abc> & canvas_shape) { return canvas_shape->get_shape() == shape; });
}

/////////////////////////////////////////////////////////////////////
void ggo::canvas::render(void * buffer, main_direction main_dir, int width, int height, int line_byte_step, pixel_buffer_format pbf) const
{
  const ggo::pixel_rect clipping = ggo::pixel_rect::from_width_height(width, height);

  using shape_t = dyn_paint_shape<float, ggo::color_8u, ggo::color_8u>;

  std::vector<shape_t> mapped_shapes;

  // Map from canvas to render.
  for (const auto & shape : _shapes)
  {
    shape_t mapped_shape;
    mapped_shape._shape = std::shared_ptr<ggo::paintable_shape2d_abc<float>>(shape->create_render_shape(main_dir, width, height));
    mapped_shape._brush = std::make_shared<ggo::solid_dyn_brush<ggo::color_8u>>(ggo::red_8u());
    mapped_shape._blender = std::make_shared<ggo::overwrite_dyn_blender<ggo::color_8u, ggo::color_8u>>();
    
    mapped_shapes.push_back(mapped_shape);
  }

  // Paint mapped shapes.
  switch (pbf)
  {
  case bgra_8u_yd:
    fill_solid<bgra_8u_yd>(buffer, width, height, line_byte_step, ggo::yellow_8u());
    paint_shapes<bgra_8u_yd, sampling_4x4>(buffer, width, height, line_byte_step, mapped_shapes.begin(), mapped_shapes.end(), clipping);
    break;
  default:
    GGO_FAIL();
    break;
  }
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

