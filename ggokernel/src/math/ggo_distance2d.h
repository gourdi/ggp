#ifndef __GGO_DISTANCE2D__
#define __GGO_DISTANCE2D__

namespace ggo
{
  //////////////////////////////////////////////////////////////
  template <typename T>
  T hypot(T x, T y)
  {
    return x * x + y * y;
  }

  //////////////////////////////////////////////////////////////
  template <typename T>
  T hypot(T x1, T y1, T x2, T y2)
  {
    T dx = x1 - x2;
    T dy = y1 - y2;

    return dx * dx + dy * dy;
  }

  //////////////////////////////////////////////////////////////
  template <typename T>
  T hypot(const ggo::set2<T> & pos1, const ggo::set2<T> & pos2)
  {
    return hypot(pos1.x(), pos1.y(), pos2.x(), pos2.y());
  }

  //////////////////////////////////////////////////////////////
  template <typename T>
  T distance(T x, T y)
  {
    return std::sqrt(ggo::hypot(x, y));
  }

  //////////////////////////////////////////////////////////////
  template <typename T>
  T distance(T x1, T y1, T x2, T y2)
  {
    return std::sqrt(hypot(x1, y1, x2, y2) );
  }

  //////////////////////////////////////////////////////////////
  template <typename T>
  T distance(const ggo::set2<T> & pos1, const ggo::set2<T> & pos2)
  {
    return distance(pos1.x(), pos1.y(), pos2.x(), pos2.y());
  }
}

#endif
