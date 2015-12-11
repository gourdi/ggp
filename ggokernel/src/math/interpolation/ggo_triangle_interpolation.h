#ifndef __GGO_TRIANGLE_INTERPOLATION__
#define __GGO_TRIANGLE_INTERPOLATION__

#include <ggo_linear_algebra2d.h>

namespace ggo
{
  template <typename REAL_T, typename VALUE_T, bool clip = true>
  bool triangular_interpolation(const ggo::set2<REAL_T> & p0, const VALUE_T & v0,
                                const ggo::set2<REAL_T> & p1, const VALUE_T & v1,
                                const ggo::set2<REAL_T> & p2, const VALUE_T & v2,
                                const ggo::set2<REAL_T> & p, VALUE_T & output)
  {
    ggo::set2<REAL_T> d(p - p0);
    ggo::set2<REAL_T> d1(p1 - p0);
    ggo::set2<REAL_T> d2(p2 - p0);
    
    REAL_T m[2][2] = {{d1.x(), d2.x()},
                      {d1.y(), d2.y()}};
    REAL_T c[2] = {d.x(), d.y()};
    REAL_T s[2] = {0, 0};
    
    if (ggo::linsolve2d(m, c, s) == false)
    {
      return false;
    }

    if (clip == true)
    {
      if (s[0] < 0 || s[1] < 0)
      {
        return false;
      }
      
      if (s[0] + s[1] > 1)
      {
        return false;
      }
    }
    
    output = (1 - s[0] - s[1]) * v0 + s[0] * v1 + s[1] * v2;
    
    return true;
  }
}

#endif