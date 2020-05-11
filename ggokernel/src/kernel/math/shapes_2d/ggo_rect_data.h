#ifndef __GGO_RECT_DATA__
#define __GGO_RECT_DATA__

#include <kernel/ggo_vec2.h>

namespace ggo
{
  template <typename data_t>
  struct rect_data
  {
    static_assert(std::is_floating_point_v<data_t>);

    rect_data() = default;
    rect_data(const ggo::pos2<data_t> & pos, data_t width, data_t height) : _pos(pos), _width(width), _height(height) {}

    static rect_data<data_t> from_left_right_bottom_top(data_t left, data_t right, data_t bottom, data_t top)
    {
      GGO_ASSERT(left <= right);
      GGO_ASSERT(bottom <= top);

      return rect_data<data_t>({ left, bottom }, right - left, top - bottom);
    }

    static rect_data<data_t> from_points(pos2<data_t> p1, pos2<data_t> p2)
    {
      auto [left, right] = std::minmax(p1.x(), p2.x());
      auto [bottom, top] = std::minmax(p1.y(), p2.y());

      return from_left_right_bottom_top(left, right, bottom, top);
    }

    pos2<data_t>  _pos;
    data_t        _width;
    data_t        _height;
  };

  template <typename data_t>
  rect_data<data_t> extend(const rect_data<data_t> & rect, data_t e)
  {
    return { { rect._pos.x() - e, rect._pos.y() - e }, rect._width + 2 * e, rect._height + 2 * e };
  }

  template <typename data_t>
  rect_data<data_t> get_union(const rect_data<data_t> & rect1, const rect_data<data_t> & rect2)
  {
    data_t left1 = rect1._pos.x();
    data_t bottom1 = rect1._pos.y();
    data_t right1 = left1 + rect1._width;
    data_t top1 = bottom1 + rect1._height;

    data_t left2 = rect2._pos.x();
    data_t bottom2 = rect2._pos.y();
    data_t right2 = left2 + rect2._width;
    data_t top2 = bottom2 + rect2._height;

    data_t left = std::min(left1, left2);
    data_t bottom = std::min(bottom1, bottom2);
    data_t right = std::max(right1, right2);
    data_t top = std::max(top1, top2);

    return{ { left, bottom }, right - left, top - bottom };
  }

  template <typename data_t>
  rect_data<data_t> get_union(const rect_data<data_t> & rect, const pos2<data_t> & p)
  {
    data_t left = rect._pos.x();
    data_t bottom = rect._pos.y();
    data_t right = left + rect._width;
    data_t top = bottom + rect._height;

    left = std::min(left, p.x());
    bottom = std::min(bottom, p.y());
    right = std::max(right, p.x());
    top = std::max(top, p.y());

    return rect_data_from_left_right_bottom_top(left, right, bottom, top);
  }

  template <typename data_t>
  bool test_intersection(const rect_data<data_t> & rect1, const rect_data<data_t> & rect2)
  {
    const data_t left1 = rect1._pos.x();
    const data_t right1 = left1 + rect1._width;
    const data_t left2 = rect2._pos.x();
    const data_t right2 = left2 + rect2._width;

    if (right1 < left2 || right2 < left1)
    {
      return false;
    }

    const data_t bottom1 = rect1._pos.y();
    const data_t top1 = bottom1 + rect1._height;
    const data_t bottom2 = rect2._pos.y();
    const data_t top2 = bottom2 + rect2._height;

    if (bottom1 > top2 || bottom2 > top1)
    {
      return false;
    }

    return true;
  }

  template <typename data_t>
  std::optional<rect_data<data_t>> get_intersection(const rect_data<data_t> & rect1, const rect_data<data_t> & rect2)
  {
    const data_t left1 = rect1._pos.x();
    const data_t right1 = left1 + rect1._width;
    const data_t left2 = rect2._pos.x();
    const data_t right2 = left2 + rect2._width;

    if (right1 < left2 || right2 < left1)
    {
      return {};
    }

    const data_t bottom1 = rect1._pos.y();
    const data_t top1 = bottom1 + rect1._height;
    const data_t bottom2 = rect2._pos.y();
    const data_t top2 = bottom2 + rect2._height;

    if (bottom1 > top2 || bottom2 > top1)
    {
      return {};
    }

    const data_t left = std::max(left1, left2);
    const data_t bottom = std::max(bottom1, bottom2);
    const data_t right = std::min(right1, right2);
    const data_t top = std::min(top1, top2);

    return rect_data<data_t>{ { left, bottom }, right - left, top - bottom };
  }
}

namespace ggo
{
  template <typename data_t>
  std::ostream & operator<<(std::ostream & os, const ggo::rect_data<data_t> & r)
  {
    os << "(l: " << r._pos.x() << ", b: " << r._pos.y() << ", w: " << r._width << ", h: " << r._height << ")";
    return os;
  }
}

#endif
