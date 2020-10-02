#pragma once

namespace ggo
{
  //////////////////////////////////////////////////////////////
  template <typename data_t, typename scalar_t>
  data_t quadratic_bezier(const data_t & p0, const data_t & p1, const data_t & p2, scalar_t t)
  {
    static_assert(std::is_floating_point_v<scalar_t>);

    scalar_t u = 1 - t;
    return u * u * p0 + 2 * u * t * p1 + t * t * p2;
  }

  //////////////////////////////////////////////////////////////
  template <typename data_t, typename scalar_t>
  data_t cubic_bezier(const data_t & p0, const data_t & p1, const data_t & p2, const data_t & p3, scalar_t t)
  {
    static_assert(std::is_floating_point_v<scalar_t>);

    scalar_t u = 1 - t;
    return u * u * u * p0 + 3 * u * u * t * p1 + 3 * u * t * t * p2 + t * t * t * p3;
  }
}
