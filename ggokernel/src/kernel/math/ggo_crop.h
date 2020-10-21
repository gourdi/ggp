#pragma once

#include <kernel/ggo_size.h>
#include <kernel/math/shapes_2d/ggo_shapes2d.h>

namespace ggo
{
  //////////////////////////////////////////////////////////////
  template <typename data_t>
  [[nodiscard]] data_t fit(data_t l, ggo::rect<data_t> roi, ggo::size render_size)
  {
    static_assert(std::is_floating_point_v<data_t>);

    data_t ratio_roi = roi.width() / roi.height();
    data_t ratio_render = static_cast<data_t>(render_size.width()) / static_cast<data_t>(render_size.height());

    if (ratio_render > ratio_roi)
    {
      return l * render_size.height() / roi.height();
    }
    else
    {
      return l * render_size.width() / roi.width();
    }
  }

  //////////////////////////////////////////////////////////////
  template <typename data_t>
  [[nodiscard]] ggo::pos2<data_t> fit(ggo::pos2<data_t> p, ggo::rect<data_t> roi, ggo::size render_size)
  {
    static_assert(std::is_floating_point_v<data_t>);

    data_t ratio_roi = roi.width() / roi.height();
    data_t ratio_render = static_cast<data_t>(render_size.width()) / static_cast<data_t>(render_size.height());

    data_t scale = 0.f;
    data_t offset_x = 0.f;
    data_t offset_y = 0.f;
    if (ratio_render > ratio_roi)
    {
      scale = render_size.height() / roi.height();
      offset_x = (render_size.width() - roi.width() * scale) / 2;
    }
    else
    {
      scale = render_size.width() / roi.width();
      offset_y = (render_size.height() - roi.height() * scale) / 2;
    }

    data_t x = scale * (p.x() - roi.left()) + offset_x;
    data_t y = scale * (p.y() - roi.bottom()) + offset_y;
    return { x, y };
  }


  //////////////////////////////////////////////////////////////
  template <typename data_t>
  [[nodiscard]] ggo::oriented_box<data_t> fit(ggo::oriented_box<data_t> box, ggo::rect<data_t> roi, ggo::size render_size)
  {
    ggo::pos2<data_t> pos = map_fit<data_t>(box.pos(), roi, render_size);
    data_t half_size_x = map_fit<data_t>(box.half_size_x(), roi, render_size);
    data_t half_size_y = map_fit<data_t>(box.half_size_y(), roi, render_size);

    return ggo::oriented_box<data_t>(pos, box.angle(), half_size_x, half_size_y);
  }

  //////////////////////////////////////////////////////////////
  template <typename data_t>
  [[nodiscard]] data_t fit(data_t l, data_t inf, data_t sup, int render_width, int render_height)
  {
    return map_fit(l, ggo::rect<data_t>::from_left_right_bottom_top(inf, sup, inf, sup), ggo::size(render_width, render_height));
  }

  //////////////////////////////////////////////////////////////
  template <typename data_t>
  [[nodiscard]] ggo::pos2<data_t> fit(ggo::pos2<data_t> p, data_t inf, data_t sup, int render_width, int render_height)
  {
    return map_fit<data_t>(p, ggo::rect<data_t>::from_left_right_bottom_top(inf, sup, inf, sup), ggo::size(render_width, render_height));
  }

  //////////////////////////////////////////////////////////////
  template <typename data_t>
  [[nodiscard]] ggo::rect<data_t> fit(ggo::rect<data_t> rect, data_t inf, data_t sup, int render_width, int render_height)
  {
    const ggo::rect<data_t> roi = ggo::rect<data_t>::from_left_right_bottom_top(inf, sup, inf, sup);

    ggo::pos2<data_t> left_bottom{ rect.left(), rect.bottom() };
    ggo::pos2<data_t> right_top{ rect.right(), rect.top() };
    
    left_bottom = map_fit<data_t>(left_bottom, roi, ggo::size(render_width, render_height));
    right_top = map_fit<data_t>(right_top, roi, ggo::size(render_width, render_height));

    return ggo::rect<data_t>::from_left_right_bottom_top(left_bottom.x(), right_top.x(), left_bottom.y(), right_top.y());
  }

  //////////////////////////////////////////////////////////////
  template <typename data_t>
  [[nodiscard]] ggo::disc<data_t> fit(ggo::disc<data_t> disc, data_t inf, data_t sup, int render_width, int render_height)
  {
    const ggo::rect<data_t> roi = ggo::rect<data_t>::from_left_right_bottom_top(inf, sup, inf, sup);

    ggo::pos2<data_t> center = map_fit<data_t>(disc.center(), roi, ggo::size(render_width, render_height));
    data_t radius = map_fit<data_t>(disc.radius(), roi, ggo::size(render_width, render_height));

    return ggo::disc<data_t>(center, radius);
  }
}