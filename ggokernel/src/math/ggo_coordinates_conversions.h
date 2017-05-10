#ifndef __GGO_COORDINATES_CONVERSIONS__
#define __GGO_COORDINATES_CONVERSIONS__

#include <ggo_rect_int.h>
#include <ggo_shapes2d.h>

namespace ggo
{
  //////////////////////////////////////////////////////////////
  template <typename data_t>
  int from_math_to_pixel(data_t v)
  {
    static_assert(std::is_floating_point<data_t>::value, "expecting floating point type");

    return ggo::to<int>(v)
  }

  //////////////////////////////////////////////////////////////
  template <typename data_t>
  data_t from_pixel_to_math(int v)
  {
    static_assert(std::is_floating_point<data_t>::value, "expecting floating point type");

    return ggo::to<data_t>(v);
  }

  //////////////////////////////////////////////////////////////
  template <typename data_t>
  ggo::pos2i from_math_to_pixel(const ggo::pos2<data_t> & p)
  {
    static_assert(std::is_floating_point<data_t>::value, "expecting floating point type");

    return { ggo::to<int>(p.x()), ggo::to<int>(p.y()) };
  }

  //////////////////////////////////////////////////////////////
  template <typename data_t>
  ggo::pos2<data_t> from_pixel_to_math(const pos2i & p)
  {
    static_assert(std::is_floating_point<data_t>::value, "expecting floating point type");

    return { ggo::to<data_t>(p.x()), ggo::to<data_t>(p.y()) };
  }

  //////////////////////////////////////////////////////////////
  template <typename data_t>
  rect_int from_math_to_pixel(const ggo::rect_data<data_t> & rect)
  {
    static_assert(std::is_floating_point<data_t>::value, "expecting floating point type");

    int left = ggo::to<int>(rect._pos.x());
    int right = ggo::to<int>(rect._pos.x() + rect._width);
    int bottom = ggo::to<int>(rect._pos.y());
    int top = ggo::to<int>(rect._pos.y() + rect._height);

    return ggo::rect_int::from_left_right_bottom_top(left, right, bottom, top);
  }

  //////////////////////////////////////////////////////////////
  template <typename data_t>
  ggo::rect_data<data_t> from_pixel_to_math(const ggo::rect_int & rect)
  {
    static_assert(std::is_floating_point<data_t>::value, "expecting floating point type");

    data_t left = static_cast<data_t>(rect.left()) - data_t(0.5);
    data_t right = static_cast<data_t>(rect.right()) + data_t(0.5);
    data_t bottom = static_cast<data_t>(rect.bottom()) - data_t(0.5);
    data_t top = static_cast<data_t>(rect.top()) + data_t(0.5);

    return ggo::rect_data_from_left_right_bottom_top<data_t>(left, right, bottom, top);
  }
}

#endif
