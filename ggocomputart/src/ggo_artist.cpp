#include "ggo_artist.h"
#include <kernel/math/ggo_coordinates_conversions.h>

namespace ggo
{
  //////////////////////////////////////////////////////////////
  artist::artist(int width, int height, int line_byte_step, ggo::pixel_type pixel_type, lines_order memory_lines_order)
    :
    _width(width), _height(height), _line_byte_step(line_byte_step), _pixel_type(pixel_type), _memory_lines_order(memory_lines_order)
  {
  }

  //////////////////////////////////////////////////////////////
  ggo::pos2_f artist::map_fill(const ggo::pos2_f & point, float inf, float sup, int render_width, int render_height)
  {
    ggo::pos2_f result;

    if (render_width >= render_height)
    {
      result.x() = ggo::map(point.x(), inf, sup, 0.f, static_cast<float>(render_width));
      result.y() = ggo::map(point.y(), inf, sup, 0.f, static_cast<float>(render_width));

      result.y() -= 0.5f * (render_width - render_height);
    }
    else
    {
      result.x() = ggo::map(point.x(), inf, sup, 0.f, static_cast<float>(render_height));
      result.y() = ggo::map(point.y(), inf, sup, 0.f, static_cast<float>(render_height));

      result.x() -= 0.5f * (render_height - render_width);
    }

    return result;
  }

  //////////////////////////////////////////////////////////////
  ggo::pos2_f artist::get_random_point(int render_width, int render_height)
  {
    return artist::get_random_point(0.f, 0.f, 0.f, 0.f, render_width, render_height);
  }

  //////////////////////////////////////////////////////////////
  ggo::pos2_f artist::get_random_point(float margin, int render_width, int render_height)
  {
    return artist::get_random_point(margin, margin, margin, margin, render_width, render_height);
  }

  //////////////////////////////////////////////////////////////
  ggo::pos2_f artist::get_random_point(float margin_left, float margin_right, float margin_top, float margin_bottom, int render_width, int render_height)
  {
    return ggo::pos2_f(ggo::rand<float>(margin_left, render_width - margin_right), ggo::rand<float>(margin_bottom, render_height - margin_top));
  }

  //////////////////////////////////////////////////////////////
  ggo::pos2_f artist::map_fill(const ggo::pos2_f & point, float inf, float sup) const
  {
    return map_fill(point, inf, sup, _width, _height);
  }

  //////////////////////////////////////////////////////////////
  ggo::pos2_f artist::get_center() const
  {
    return ggo::pos2_f(static_cast<float>(_width - 1) / 2, static_cast<float>(_height - 1) / 2);
  }

  //////////////////////////////////////////////////////////////
  ggo::pos2_f artist::horz_mirror(const ggo::pos2_f & p, int render_height)
  {
    return ggo::pos2_f(p.x(), render_height - p.y());
  }

  //////////////////////////////////////////////////////////////
  ggo::pos2_f artist::vert_mirror(const ggo::pos2_f & p, int render_width)
  {
    return ggo::pos2_f(render_width - p.x(), p.y());
  }

  //////////////////////////////////////////////////////////////
  float artist::map_fit(float value, float inf, float sup) const
  {
    return ggo::map_fit(value, inf, sup, _width, _height);
  }

  //////////////////////////////////////////////////////////////
  ggo::pos2_f artist::map_fit(const ggo::pos2_f & point, float inf, float sup) const
  {
    return ggo::map_fit(point, inf, sup, _width, _height);
  }

  //////////////////////////////////////////////////////////////
  ggo::rect_f artist::map_fit(ggo::rect_f rect, float inf, float sup) const
  {
    return ggo::map_fit(rect, inf, sup, _width, _height);
  }

  //////////////////////////////////////////////////////////////
  ggo::disc_f artist::map_fit(ggo::disc_f disc, float inf, float sup) const
  {
    return ggo::map_fit(disc, inf, sup, _width, _height);
  }

  //////////////////////////////////////////////////////////////
  ggo::disc_d artist::map_fit(ggo::disc_d disc, double inf, double sup) const
  {
    return ggo::map_fit(disc, inf, sup, _width, _height);
  }
}

