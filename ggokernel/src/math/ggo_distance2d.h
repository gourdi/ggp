#ifndef __GGO_DISTANCE2D__
#define __GGO_DISTANCE2D__

#include <ggo_vec.h>
#include <array>

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
  T distance(T x, T y)
  {
    return std::sqrt(ggo::hypot(x, y));
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
  T distance(T x1, T y1, T x2, T y2)
  {
    return std::sqrt(hypot(x1, y1, x2, y2));
  }

  //////////////////////////////////////////////////////////////
  template <typename T>
  T hypot(const std::array<T, 2> & pos1, const std::array<T, 2> & pos2)
  {
    return hypot(pos1[0], pos1[1], pos2[0], pos2[1]);
  }

  //////////////////////////////////////////////////////////////
  template <typename T>
  T distance(const std::array<T, 2> & pos1, const std::array<T, 2> & pos2)
  {
    return distance(pos1[0], pos1[1], pos2[0], pos2[1]);
  }

  //////////////////////////////////////////////////////////////
  template <typename T>
  T hypot(const ggo::pos2<T> & p1, const ggo::pos2<T> & p2)
  {
    return hypot(p1.template get<0>(), p1.template get<1>(), p2.template get<0>(), p2.template get<1>());
  }

  //////////////////////////////////////////////////////////////
  template <typename T>
  T distance(const ggo::pos2<T> & p1, const ggo::pos2<T> & p2)
  {
    return distance(p1.template get<0>(), p1.template get<1>(), p2.template get<0>(), p2.template get<1>());
  }
}

#endif
