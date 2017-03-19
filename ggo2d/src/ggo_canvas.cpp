#include "ggo_canvas.h"
#include <ggo_buffer_fill.h>

// Shape interface
namespace ggo
{
  ///////////////////////////////////////////////////////////////////
  std::unique_ptr<ggo::paintable_shape2d_abc<float>> canvas::shape_abc::make_unique_render_shape(const view & view, int render_width, int render_height, bool vertical_flip) const
  {
    return std::unique_ptr<ggo::paintable_shape2d_abc<float>>(create_render_shape(view, render_width, render_height, vertical_flip));
  }
}

// Disc.
namespace ggo
{
  ///////////////////////////////////////////////////////////////////
  disc_float * canvas::disc::create_render_disc(const view & view, int render_width, int render_height, bool vertical_flip) const
  {
    const pos2f center = from_view_to_render(_disc.center(), view, render_width, render_height, vertical_flip);
    const float radius = from_view_to_render(_disc.radius(), view._size, view._main_direction, render_width, render_height);

    return new disc_float(center, radius);
  }

  ///////////////////////////////////////////////////////////////////
  void canvas::disc::set_from_render_points(const pos2f & p1, const pos2f & p2, const view & view, int render_width, int render_height, bool vertical_flip)
  {
    _disc = disc_float(
      canvas::from_render_to_view(p1, view, render_width, render_height, vertical_flip),
      canvas::from_render_to_view(p2, view, render_width, render_height, vertical_flip));
  }

  ///////////////////////////////////////////////////////////////////
  void canvas::disc::set_from_render_disc(const disc_float & disc, const view & view, int render_width, int render_height, bool vertical_flip)
  {
    _disc.center() = canvas::from_render_to_view(disc.center(), view, render_width, render_height, vertical_flip);
    _disc.radius() = canvas::from_render_to_view(disc.radius(), view._size, view._main_direction, render_width, render_height);
  }
}

// Polygon.
namespace ggo
{
  ///////////////////////////////////////////////////////////////////
  polygon2d_float * canvas::polygon::create_render_polygon(const view & view, int render_width, int render_height, bool vertical_flip) const
  {
    const int points_count = _polygon.get_points_count();

    std::vector<pos2f> mapped_points(points_count);
    for (int i = 0; i < points_count; ++i)
    {
      mapped_points[i] = from_view_to_render(_polygon.get_point(i), view, render_width, render_height, vertical_flip);
    }

    return new polygon2d_float(std::move(mapped_points));
  }

  ///////////////////////////////////////////////////////////////////
  void canvas::polygon::update_render_point(int index, const pos2f & p, const view & view, int render_width, int render_height, bool vertical_flip)
  {
    GGO_ASSERT_BTW(index, 0, _polygon.get_points_count() - 1);

    _polygon.get_point(index) = canvas::from_render_to_view(p, view, render_width, render_height, vertical_flip);
  }

  ///////////////////////////////////////////////////////////////////
  void canvas::polygon::set_from_render_polygon(const ggo::polygon2d_float & polygon, const view & view, int render_width, int render_height, bool vertical_flip)
  {
    const int points_count = polygon.get_points_count();

    std::vector<pos2f> mapped_points(points_count);
    for (int i = 0; i < points_count; ++i)
    {
      mapped_points[i] = from_render_to_view(polygon.get_point(i), view, render_width, render_height, vertical_flip);
    }

    _polygon.set_points(std::move(mapped_points));
  }
}

// Canvas.
namespace ggo
{
  ///////////////////////////////////////////////////////////////////
  canvas::disc * canvas::create_disc()
  {
    canvas::disc * disc = new canvas::disc();

    _shapes.emplace_back(std::unique_ptr<shape_abc>(disc));

    return disc;
  }

  ///////////////////////////////////////////////////////////////////
  canvas::polygon * canvas::create_polygon()
  {
    canvas::polygon * polygon = new canvas::polygon();

    _shapes.emplace_back(std::unique_ptr<shape_abc>(polygon));

    return polygon;
  }

  ///////////////////////////////////////////////////////////////////
  void canvas::remove_shape(const paintable_shape2d_abc<float> * shape)
  {
    remove_first_if(_shapes, [&](std::unique_ptr<shape_abc> & canvas_shape) { return canvas_shape->get_shape() == shape; });
  }

  /////////////////////////////////////////////////////////////////////
  void canvas::render(void * buffer, const view & view, int width, int height, int line_byte_step, pixel_buffer_format pbf) const
  {
    const pixel_rect clipping = pixel_rect::from_width_height(width, height);

    using shape_t = dyn_paint_shape<float, color_8u, color_8u>;

    std::vector<shape_t> mapped_shapes;

    // Map from canvas to render.
    for (const auto & shape : _shapes)
    {
      shape_t mapped_shape;
      mapped_shape._shape = std::shared_ptr<paintable_shape2d_abc<float>>(shape->create_render_shape(view, width, height, false));
      mapped_shape._brush = std::make_shared<solid_dyn_brush<color_8u>>(shape->_color);
      mapped_shape._blender = std::make_shared<overwrite_dyn_blender<color_8u, color_8u>>();

      mapped_shapes.push_back(mapped_shape);
    }

    // Paint mapped shapes.
    switch (pbf)
    {
    case bgra_8u_yd:
      fill_solid<bgra_8u_yd>(buffer, width, height, line_byte_step, yellow_8u());
      paint_shapes<bgra_8u_yd, sampling_4x4>(buffer, width, height, line_byte_step, mapped_shapes.begin(), mapped_shapes.end(), clipping);
      break;
    default:
      GGO_FAIL();
      break;
    }
  }

  /////////////////////////////////////////////////////////////////////
  float canvas::from_view_to_render(float dist, float view_size, main_direction main_dir, int render_width, int render_height)
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
  float canvas::from_render_to_view(float dist, float view_size, main_direction main_dir, int render_width, int render_height)
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
  pos2f canvas::from_view_to_render(const pos2f & p, const view & view, int render_width, int render_height, bool vertical_flip)
  {
    switch (view._main_direction)
    {
    case main_direction::vertical:
    {
      float ratio = float(render_width) / float(render_height);
      float x = ggo::map(p.x() - view._center.x(), -ratio * view._size, ratio * view._size, -0.5f, float(render_width) - 0.5f);
      float y = ggo::map(p.y() - view._center.y(), -view._size, view._size, -0.5f, float(render_height) - 0.5f);
      if (vertical_flip)
      {
        y = render_height - y - 1;
      }
      return pos2f(x, y);
    }
    case main_direction::horizontal:
    {
      float ratio = float(render_height) / float(render_width);
      float x = ggo::map(p.x() - view._center.x(), -view._size, view._size, -0.5f, float(render_width) - 0.5f);
      float y = ggo::map(p.y() - view._center.y(), -ratio * view._size, ratio * view._size, -0.5f, float(render_height) - 0.5f);
      if (vertical_flip)
      {
        y = render_height - y - 1;
      }
      return pos2f(x, y);
    }
    default:
      GGO_FAIL();
      return pos2f(0, 0);
    }
  }

  /////////////////////////////////////////////////////////////////////
  pos2f canvas::from_render_to_view(const pos2f & p, const view & view, int render_width, int render_height, bool vertical_flip)
  {
    float x = p.x();
    float y = vertical_flip ? render_height - p.y() - 1 : p.y();

    switch (view._main_direction)
    {
    case main_direction::vertical:
    {
      float ratio = float(render_width) / float(render_height);
      x = view._center.x() + ggo::map(x, -0.5f, float(render_width) - 0.5f, -ratio * view._size, ratio * view._size);
      y = view._center.y() + ggo::map(y, -0.5f, float(render_height) - 0.5f, -view._size, view._size);
      break;
    }
    case main_direction::horizontal:
    {
      float ratio = float(render_height) / float(render_width);
      x = view._center.x() + ggo::map(p.x(), -0.5f, float(render_width) - 0.5f, -view._size, view._size);
      y = view._center.y() + ggo::map(p.y(), -0.5f, float(render_height) - 0.5f, -ratio * view._size, ratio * view._size);
      break;
    }
    default:
      GGO_FAIL();
      break;
    }
    return pos2f(x, y);
  }

  /////////////////////////////////////////////////////////////////////
  pos2i canvas::from_view_to_render_pixel(const pos2f & p, const view & view, int render_width, int render_height, bool vertical_flip)
  {
    const pos2f pf = from_view_to_render(p, view, render_width, render_height, vertical_flip);

    return pos2i(ggo::to<int>(pf.x()), ggo::to<int>(pf.y()));
  }

  /////////////////////////////////////////////////////////////////////
  pos2f canvas::from_render_pixel_to_view(const pos2i & p, const view & view, int render_width, int render_height, bool vertical_flip)
  {
    const pos2f pf(ggo::to<float>(p.x()), ggo::to<float>(p.y()));

    return from_render_to_view(pf, view, render_width, render_height, vertical_flip);
  }
}

