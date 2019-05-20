#ifndef __GGO_COORDINATES_CONVERSIONS__
#define __GGO_COORDINATES_CONVERSIONS__

#include <kernel/ggo_rect_int.h>
#include <kernel/math/shapes_2d/ggo_shapes2d.h>
#include <optional>

namespace ggo
{
  //////////////////////////////////////////////////////////////
  template <typename data_t>
  int from_continuous_to_pixel(data_t v)
  {
    static_assert(std::is_floating_point<data_t>::value, "expecting floating point type");

    return v >= 0 ? static_cast<int>(v) : static_cast<int>(v) - 1;
  }

  //////////////////////////////////////////////////////////////
  template <typename data_t>
  data_t from_pixel_to_continuous(int v)
  {
    static_assert(std::is_floating_point<data_t>::value, "expecting floating point type");

    return static_cast<data_t>(v) + data_t(0.5);
  }

  //////////////////////////////////////////////////////////////
  template <typename data_t>
  ggo::pos2_i from_continuous_to_pixel(const ggo::pos2<data_t> & p)
  {
    static_assert(std::is_floating_point<data_t>::value, "expecting floating point type");

    return { ggo::from_continuous_to_pixel(p.x()), ggo::from_continuous_to_pixel(p.y()) };
  }

  //////////////////////////////////////////////////////////////
  template <typename data_t>
  ggo::pos2<data_t> from_pixel_to_continuous(const pos2_i & p)
  {
    static_assert(std::is_floating_point<data_t>::value, "expecting floating point type");

    return { ggo::from_pixel_to_continuous<data_t>(p.x()), ggo::from_pixel_to_continuous<data_t>(p.y()) };
  }

  //////////////////////////////////////////////////////////////
  template <typename data_t>
  rect_int from_continuous_to_pixel_exclusive(const ggo::rect_data<data_t> & rect)
  {
    static_assert(std::is_floating_point<data_t>::value, "expecting floating point type");

    int left = static_cast<int>(std::floor(rect._pos.x()));
    int past_right = static_cast<int>(std::ceil(rect._pos.x() + rect._width));
    int bottom = static_cast<int>(std::floor(rect._pos.y()));
    int past_top = static_cast<int>(std::ceil(rect._pos.y() + rect._height));

    return ggo::rect_int::from_left_right_bottom_top(left, past_right - 1, bottom, past_top - 1);
  }

  //////////////////////////////////////////////////////////////
  template <typename data_t>
  std::optional<rect_int> from_continuous_to_pixel_inclusive(const ggo::rect_data<data_t> & rect)
  {
    static_assert(std::is_floating_point<data_t>::value, "expecting floating point type");

    int left = static_cast<int>(std::ceil(rect._pos.x()));
    int past_right = static_cast<int>(std::floor(rect ._pos.x() + rect._width));
    int bottom = static_cast<int>(std::ceil(rect._pos.y()));
    int past_top = static_cast<int>(std::floor(rect._pos.y() + rect._height));

    if (left >= past_right || bottom >= past_top)
    {
      return {};
    }

    return ggo::rect_int::from_left_right_bottom_top(left, past_right - 1, bottom, past_top - 1);
  }

  //////////////////////////////////////////////////////////////
  template <typename data_t>
  ggo::rect_data<data_t> from_pixel_to_continuous(const ggo::rect_int & rect)
  {
    static_assert(std::is_floating_point<data_t>::value, "expecting floating point type");

    data_t left = static_cast<data_t>(rect.left());
    data_t right = static_cast<data_t>(rect.right()) + 1;
    data_t bottom = static_cast<data_t>(rect.bottom());
    data_t top = static_cast<data_t>(rect.top()) + 1;

    return ggo::rect_data_from_left_right_bottom_top<data_t>(left, right, bottom, top);
  }
}

namespace ggo
{
  //////////////////////////////////////////////////////////////
  template <typename data_t>
  [[nodiscard]] data_t map_fit(data_t l, ggo::rect<data_t> roi, ggo::size render_size)
  {
    static_assert(std::is_floating_point_v<data_t>);

    data_t ratio_roi = roi.width() / roi.height();
    data_t ratio_render = static_cast<data_t>(render_size.width()) / static_cast<data_t>(render_size.height());

    data_t scale = 0.f;
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
  [[nodiscard]] ggo::pos2<data_t> map_fit(ggo::pos2<data_t> p, ggo::rect<data_t> roi, ggo::size render_size)
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
  [[nodiscard]] data_t map_fit(data_t l, data_t inf, data_t sup, int render_width, int render_height)
  {
    return map_fit(l, ggo::rect<data_t>::from_left_right_bottom_top(inf, sup, inf, sup), ggo::size(render_width, render_height));
  }

  //////////////////////////////////////////////////////////////
  template <typename data_t>
  [[nodiscard]] ggo::pos2<data_t> map_fit(ggo::pos2<data_t> p, data_t inf, data_t sup, int render_width, int render_height)
  {
    return map_fit<data_t>(p, ggo::rect<data_t>::from_left_right_bottom_top(inf, sup, inf, sup), ggo::size(render_width, render_height));
  }

  //////////////////////////////////////////////////////////////
  template <typename data_t>
  [[nodiscard]] ggo::rect<data_t> map_fit(ggo::rect<data_t> rect, data_t inf, data_t sup, int render_width, int render_height)
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
  [[nodiscard]] ggo::disc<data_t> map_fit(ggo::disc<data_t> disc, data_t inf, data_t sup, int render_width, int render_height)
  {
    const ggo::rect<data_t> roi = ggo::rect<data_t>::from_left_right_bottom_top(inf, sup, inf, sup);

    ggo::pos2<data_t> center = map_fit<data_t>(disc.center(), roi, ggo::size(render_width, render_height));
    data_t radius = map_fit<data_t>(disc.radius(), roi, ggo::size(render_width, render_height));

    return ggo::disc<data_t>(center, radius);
  }
}

#endif
