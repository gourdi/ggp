#include "ggo_canvas.h"
#include <ggo_buffer_fill.h>

///////////////////////////////////////////////////////////////////
ggo::paintable_shape2d_abc<float> * ggo::canvas::disc::create_render_shape(const view & view, int render_width, int render_height) const
{
  const pos2f center = from_view_to_render(_disc.center(), view, render_width, render_height);
  const float radius = from_view_to_render(_disc.radius(), view._size, view._main_direction, render_width, render_height);

  return new disc_float(center, radius);
}

///////////////////////////////////////////////////////////////////
ggo::paintable_shape2d_abc<float> * ggo::canvas::polygon::create_render_shape(const view & view, int render_width, int render_height) const
{
  const int points_count = _polygon.get_points_count();

  std::vector<ggo::pos2f> mapped_points(points_count);
  for (int i = 0; i < points_count; ++i)
  {
    mapped_points[i] = from_view_to_render(_polygon.get_point(i), view, render_width, render_height);
  }

  return new polygon2d_float(mapped_points);
}

///////////////////////////////////////////////////////////////////
ggo::disc_float * ggo::canvas::create_disc()
{
  ggo::canvas::disc * canvas_disc = new ggo::canvas::disc();

  _shapes.emplace_back(std::unique_ptr<shape_abc>(canvas_disc));

  return &canvas_disc->_disc;
}

///////////////////////////////////////////////////////////////////
ggo::polygon2d_float * ggo::canvas::create_polygon()
{
  ggo::canvas::polygon * canvas_polygon = new ggo::canvas::polygon();

  _shapes.emplace_back(std::unique_ptr<shape_abc>(canvas_polygon));

  return &canvas_polygon->_polygon;
}

///////////////////////////////////////////////////////////////////
void ggo::canvas::remove_shape(const ggo::paintable_shape2d_abc<float> * shape)
{
  ggo::remove_first_if(_shapes, [&](std::unique_ptr<shape_abc> & canvas_shape) { return canvas_shape->get_shape() == shape; });
}

/////////////////////////////////////////////////////////////////////
void ggo::canvas::render(void * buffer, const view & view, int width, int height, int line_byte_step, pixel_buffer_format pbf) const
{
  const ggo::pixel_rect clipping = ggo::pixel_rect::from_width_height(width, height);

  using shape_t = dyn_paint_shape<float, ggo::color_8u, ggo::color_8u>;

  std::vector<shape_t> mapped_shapes;

  // Map from canvas to render.
  for (const auto & shape : _shapes)
  {
    shape_t mapped_shape;
    mapped_shape._shape = std::shared_ptr<ggo::paintable_shape2d_abc<float>>(shape->create_render_shape(view, width, height));
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
float ggo::canvas::from_view_to_render(float dist, float view_size, main_direction main_dir, int render_width, int render_height)
{
  switch (main_dir)
  {
  case main_direction::vertical:
    return render_height * dist / (2.0f * view_size);
  case main_direction::horizontal:
    return render_width * dist / (2.0f * view_size);
  default:
    GGO_FAIL();
    return 0.f;
  }
}

/////////////////////////////////////////////////////////////////////
float ggo::canvas::from_render_to_view(float dist, float view_size, main_direction main_dir, int render_width, int render_height)
{
  switch (main_dir)
  {
  case main_direction::vertical:
    return 2.0f * view_size * dist / render_height;
  case main_direction::horizontal:
    return 2.0f * view_size * dist / render_width;
  default:
    GGO_FAIL();
    return 0.f;
  }
}

/////////////////////////////////////////////////////////////////////
ggo::pos2f ggo::canvas::from_view_to_render(const ggo::pos2f & p, const view & view, int render_width, int render_height)
{
  switch (view._main_direction)
  {
  case main_direction::vertical:
  {
    float ratio = float(render_width) / float(render_height);
    float x = ggo::map<float>(p.x() - view._center.x(), -ratio * view._size, ratio * view._size, -0.5f, float(render_width) - 0.5f);
    float y = ggo::map<float>(p.y() - view._center.y(), -view._size, view._size, -0.5f, float(render_height) - 0.5f);
    return pos2f(x, y);
  }
  case main_direction::horizontal:
  {
    float ratio = float(render_height) / float(render_width);
    float x = ggo::map<float>(p.x() - view._center.x(), -view._size, view._size, -0.5f, float(render_width) - 0.5f);
    float y = ggo::map<float>(p.y() - view._center.y(), -ratio * view._size, ratio * view._size, -0.5f, float(render_height) - 0.5f);
    return pos2f(x, y);
  }
  default:
    GGO_FAIL();
    return ggo::pos2f(0, 0);
  }
}

/////////////////////////////////////////////////////////////////////
ggo::pos2f ggo::canvas::from_render_to_view(const ggo::pos2f & p, const view & view, int render_width, int render_height)
{
  switch (view._main_direction)
  {
  case main_direction::vertical:
  {
    float ratio = float(render_width) / float(render_height);
    float x = view._center.x() + ggo::map(p.x(), -0.5f, float(render_width) - 0.5f, -ratio * view._size, ratio * view._size);
    float y = view._center.y() + ggo::map(p.y(), -0.5f, float(render_height) - 0.5f, -view._size, view._size);
    return pos2f(x, y);
  }
  case main_direction::horizontal:
  {
    float ratio = float(render_height) / float(render_width);
    float x = view._center.x() + ggo::map(p.x(), -0.5f, float(render_width) - 0.5f, -view._size, view._size);
    float y = view._center.y() + ggo::map(p.y(), -0.5f, float(render_height) - 0.5f, -ratio * view._size, ratio * view._size);
    return pos2f(x, y);
  }
  default:
    GGO_FAIL();
    return ggo::pos2f(0.f, 0.f);
  }
}

/////////////////////////////////////////////////////////////////////
ggo::pos2i ggo::canvas::from_view_to_render_pixel(const ggo::pos2f & p, const view & view, int render_width, int render_height)
{
  const ggo::pos2f pf = from_view_to_render(p, view, render_width, render_height);

  return ggo::pos2i(ggo::to<int>(pf.x()), ggo::to<int>(pf.y()));
}

/////////////////////////////////////////////////////////////////////
ggo::pos2f ggo::canvas::from_render_pixel_to_view(const ggo::pos2i & p, const view & view, int render_width, int render_height)
{
  const ggo::pos2f pf(ggo::to<float>(p.x()), ggo::to<float>(p.y()));
  
  return from_render_to_view(pf, view, render_width, render_height);
}

