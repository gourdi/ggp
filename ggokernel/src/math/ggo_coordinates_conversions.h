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

#endif
