#ifndef __GGO_COORDINATES_CONVERSIONS__
#define __GGO_COORDINATES_CONVERSIONS__

#include <ggo_rect_int.h>
#include <ggo_shapes2d.h>
#include <optional>

namespace ggo
{
  //////////////////////////////////////////////////////////////
  template <typename data_t>
  int from_math_to_pixel(data_t v)
  {
    static_assert(std::is_floating_point<data_t>::value, "expecting floating point type");

    return v >= 0 ? static_cast<int>(v) : static_cast<int>(v) - 1;
  }

  //////////////////////////////////////////////////////////////
  template <typename data_t>
  data_t from_pixel_to_math(int v)
  {
    static_assert(std::is_floating_point<data_t>::value, "expecting floating point type");

    return static_cast<data_t>(v) + data_t(0.5);
  }

  //////////////////////////////////////////////////////////////
  template <typename data_t>
  ggo::pos2i from_math_to_pixel(const ggo::pos2<data_t> & p)
  {
    static_assert(std::is_floating_point<data_t>::value, "expecting floating point type");

    return { ggo::from_math_to_pixel(p.x()), ggo::from_math_to_pixel(p.y()) };
  }

  //////////////////////////////////////////////////////////////
  template <typename data_t>
  ggo::pos2<data_t> from_pixel_to_math(const pos2i & p)
  {
    static_assert(std::is_floating_point<data_t>::value, "expecting floating point type");

    return { ggo::from_pixel_to_math<data_t>(p.x()), ggo::from_pixel_to_math<data_t>(p.y()) };
  }

  //////////////////////////////////////////////////////////////
  template <typename data_t>
  rect_int from_math_to_pixel_exclusive(const ggo::rect_data<data_t> & rect)
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
  std::optional<rect_int> from_math_to_pixel_inclusive(const ggo::rect_data<data_t> & rect)
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
  ggo::rect_data<data_t> from_pixel_to_math(const ggo::rect_int & rect)
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
  data_t map_fit(data_t value, data_t inf, data_t sup, int render_width, int render_height)
  {
    if (render_width >= render_height)
    {
      return ggo::map(value, inf, sup, data_t(0), data_t(render_height));
    }
    else
    {
      return ggo::map(value, inf, sup, data_t(0), data_t(render_width));
    }
  }

  //////////////////////////////////////////////////////////////
  template <typename data_t>
  ggo::pos2<data_t> map_fit(const ggo::pos2<data_t> & point, data_t inf, data_t sup, int render_width, int render_height)
  {
    ggo::pos2<data_t> result;

    if (render_width >= render_height)
    {
      result.x() = ggo::map(point.x(), inf, sup, data_t(0), data_t(render_height));
      result.y() = ggo::map(point.y(), inf, sup, data_t(0), data_t(render_height));

      result.x() += (render_width - render_height) / data_t(2);
    }
    else
    {
      result.x() = ggo::map(point.x(), inf, sup, data_t(0), data_t(render_width));
      result.y() = ggo::map(point.y(), inf, sup, data_t(0), data_t(render_width));

      result.y() += (render_height - render_width) / data_t(2);
    }

    return result;
  }

  //////////////////////////////////////////////////////////////
  template <typename data_t>
  void map_fit(ggo::rect<data_t> & rect, data_t inf, data_t sup, int render_width, int render_height)
  {
    if (render_width >= render_height)
    {
      rect.left() = ggo::map(rect.left(), inf, sup, data_t(0), data_t(render_height));
      rect.bottom() = ggo::map(rect.bottom(), inf, sup, data_t(0), data_t(render_height));
      rect.width() = ggo::map(rect.width(), inf, sup, data_t(0), data_t(render_height));
      rect.height() = ggo::map(rect.height(), inf, sup, data_t(0), data_t(render_height));

      rect.left() += (render_width - render_height) / data_t(2);
    }
    else
    {
      rect.left() = ggo::map(rect.left(), inf, sup, 0.f, data_t(render_width));
      rect.bottom() = ggo::map(rect.bottom(), inf, sup, 0.f, data_t(render_width));
      rect.width() = ggo::map(rect.width(), inf, sup, 0.f, data_t(render_width));
      rect.height() = ggo::map(rect.height(), inf, sup, 0.f, data_t(render_width));

      rect.bottom() += 0.5f * (render_height - render_width);
    }
  }

  //////////////////////////////////////////////////////////////
  template <typename data_t>
  void map_fit(ggo::disc<data_t> & disc, data_t inf, data_t sup, int render_width, int render_height)
  {
    disc.center() = map_fit(disc.center(), inf, sup, render_width, render_height);
    disc.radius() = map_fit(disc.radius(), inf, sup, render_width, render_height);
  }
}

#endif
