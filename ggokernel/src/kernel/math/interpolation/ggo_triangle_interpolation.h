#pragma once

#include <kernel/math/linear_algebra/ggo_linear_algebra2d.h>

namespace ggo
{
  template <typename real_t, typename value_t, bool clip = true>
  std::optional<value_t> triangular_interpolation(const ggo::pos2<real_t> & p0, const value_t & v0,
                                                  const ggo::pos2<real_t> & p1, const value_t & v1,
                                                  const ggo::pos2<real_t> & p2, const value_t & v2,
                                                  const ggo::pos2<real_t> & p)
  {
    static_assert(std::is_floating_point<real_t>::value, "expecting floating point type");

    ggo::pos2<real_t> d(p - p0);
    ggo::pos2<real_t> d1(p1 - p0);
    ggo::pos2<real_t> d2(p2 - p0);
    
    real_t m[2][2] = { { d1.x(), d2.x() },
                       { d1.y(), d2.y() } };
    real_t c[2] = { d.x(), d.y() };
    real_t s[2] = { 0, 0 };
    
    if (ggo::linsolve2d(m, c, s) == false)
    {
      return {};
    }

    if (clip == true)
    {
      if (s[0] < 0 || s[1] < 0)
      {
        return {};
      }
      
      if (s[0] + s[1] > 1)
      {
        return {};
      }
    }
    
    return (1 - s[0] - s[1]) * v0 + s[0] * v1 + s[1] * v2;
  }
}

