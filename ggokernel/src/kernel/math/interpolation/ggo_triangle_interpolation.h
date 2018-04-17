#ifndef __GGO_TRIANGLE_INTERPOLATION__
#define __GGO_TRIANGLE_INTERPOLATION__

#include <ggo_linear_algebra2d.h>

namespace ggo
{
  template <typename real_t, typename value_t, bool clip = true>
  bool triangular_interpolation(const ggo::pos2<real_t> & p0, const value_t & v0,
                                const ggo::pos2<real_t> & p1, const value_t & v1,
                                const ggo::pos2<real_t> & p2, const value_t & v2,
                                const ggo::pos2<real_t> & p, value_t & output)
  {
    static_assert(std::is_floating_point<real_t>::value, "expecting floating point type");

    ggo::pos2<real_t> d(p - p0);
    ggo::pos2<real_t> d1(p1 - p0);
    ggo::pos2<real_t> d2(p2 - p0);
    
    real_t m[2][2] = {{d1.template get<0>(), d2.template get<0>()},
                      {d1.template get<1>(), d2.template get<1>()}};
    real_t c[2] = {d.template get<0>(), d.template get<1>()};
    real_t s[2] = {0, 0};
    
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

  template <typename real_t, typename value_t, bool clip = true>
  value_t triangular_interpolation(const ggo::pos2<real_t> & p0, const value_t & v0,
    const ggo::pos2<real_t> & p1, const value_t & v1,
    const ggo::pos2<real_t> & p2, const value_t & v2,
    const ggo::pos2<real_t> & p)
  {
    value_t result;

    if (triangular_interpolation<real_t, value_t, clip>(p0, v0, p1, v1, p2, v2, p, result) == false)
    {
      throw std::runtime_error("failed computing triangular interpolation");
    }

    return result;
  }
}

#endif