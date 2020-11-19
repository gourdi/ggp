#pragma once

#include <kernel/ggo_vec2.h>

namespace ggo
{
  template <typename scalar_t>
  struct rect_data
  {
    static_assert(std::is_floating_point_v<scalar_t>);

    rect_data() = default;
    rect_data(const ggo::pos2<scalar_t> & pos, scalar_t width, scalar_t height) : _pos(pos), _width(width), _height(height) {}

    static rect_data<scalar_t> from_left_right_bottom_top(scalar_t left, scalar_t right, scalar_t bottom, scalar_t top)
    {
      GGO_ASSERT(left <= right);
      GGO_ASSERT(bottom <= top);

      return rect_data<scalar_t>({ left, bottom }, right - left, top - bottom);
    }

    static rect_data<scalar_t> from_points(pos2<scalar_t> p1, pos2<scalar_t> p2)
    {
      auto [left, right] = std::minmax(p1.x(), p2.x());
      auto [bottom, top] = std::minmax(p1.y(), p2.y());

      return from_left_right_bottom_top(left, right, bottom, top);
    }

    pos2<scalar_t>  _pos;
    scalar_t        _width;
    scalar_t        _height;
  };

  template <typename scalar_t>
  rect_data<scalar_t> extend(const rect_data<scalar_t> & rect, scalar_t e)
  {
    return { { rect._pos.x() - e, rect._pos.y() - e }, rect._width + 2 * e, rect._height + 2 * e };
  }

  template <typename scalar_t>
  rect_data<scalar_t> get_union(const rect_data<scalar_t> & rect1, const rect_data<scalar_t> & rect2)
  {
    scalar_t left1 = rect1._pos.x();
    scalar_t bottom1 = rect1._pos.y();
    scalar_t right1 = left1 + rect1._width;
    scalar_t top1 = bottom1 + rect1._height;

    scalar_t left2 = rect2._pos.x();
    scalar_t bottom2 = rect2._pos.y();
    scalar_t right2 = left2 + rect2._width;
    scalar_t top2 = bottom2 + rect2._height;

    scalar_t left = std::min(left1, left2);
    scalar_t bottom = std::min(bottom1, bottom2);
    scalar_t right = std::max(right1, right2);
    scalar_t top = std::max(top1, top2);

    return{ { left, bottom }, right - left, top - bottom };
  }

  template <typename scalar_t>
  rect_data<scalar_t> get_union(const rect_data<scalar_t> & rect, const pos2<scalar_t> & p)
  {
    scalar_t left = rect._pos.x();
    scalar_t bottom = rect._pos.y();
    scalar_t right = left + rect._width;
    scalar_t top = bottom + rect._height;

    left = std::min(left, p.x());
    bottom = std::min(bottom, p.y());
    right = std::max(right, p.x());
    top = std::max(top, p.y());

    return rect_data_from_left_right_bottom_top(left, right, bottom, top);
  }

  template <typename scalar_t>
  bool test_intersection(const rect_data<scalar_t> & rect1, const rect_data<scalar_t> & rect2)
  {
    const scalar_t left1 = rect1._pos.x();
    const scalar_t right1 = left1 + rect1._width;
    const scalar_t left2 = rect2._pos.x();
    const scalar_t right2 = left2 + rect2._width;

    if (right1 < left2 || right2 < left1)
    {
      return false;
    }

    const scalar_t bottom1 = rect1._pos.y();
    const scalar_t top1 = bottom1 + rect1._height;
    const scalar_t bottom2 = rect2._pos.y();
    const scalar_t top2 = bottom2 + rect2._height;

    if (bottom1 > top2 || bottom2 > top1)
    {
      return false;
    }

    return true;
  }

  template <typename scalar_t>
  std::optional<rect_data<scalar_t>> get_intersection(const rect_data<scalar_t> & rect1, const rect_data<scalar_t> & rect2)
  {
    const scalar_t left1 = rect1._pos.x();
    const scalar_t right1 = left1 + rect1._width;
    const scalar_t left2 = rect2._pos.x();
    const scalar_t right2 = left2 + rect2._width;

    if (right1 < left2 || right2 < left1)
    {
      return {};
    }

    const scalar_t bottom1 = rect1._pos.y();
    const scalar_t top1 = bottom1 + rect1._height;
    const scalar_t bottom2 = rect2._pos.y();
    const scalar_t top2 = bottom2 + rect2._height;

    if (bottom1 > top2 || bottom2 > top1)
    {
      return {};
    }

    const scalar_t left = std::max(left1, left2);
    const scalar_t bottom = std::max(bottom1, bottom2);
    const scalar_t right = std::min(right1, right2);
    const scalar_t top = std::min(top1, top2);

    return rect_data<scalar_t>{ { left, bottom }, right - left, top - bottom };
  }
}

namespace ggo
{
  template <typename scalar_t>
  std::ostream & operator<<(std::ostream & os, const ggo::rect_data<scalar_t> & r)
  {
    os << "(l: " << r._pos.x() << ", b: " << r._pos.y() << ", w: " << r._width << ", h: " << r._height << ")";
    return os;
  }
}

