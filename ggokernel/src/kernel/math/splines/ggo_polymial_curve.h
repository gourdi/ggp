#pragma once

#include <stdexcept>
#include <kernel/ggo_kernel.h>

namespace ggo
{
  //////////////////////////////////////////////////////////////
  template <typename data_t, typename point_it_t, typename knot_it_t, typename scalar_t>
  data_t polynomial_curve_aux(int i, int r, point_it_t points_begin, knot_it_t knots_begin, scalar_t t)
  {
    if (r == 0)
    {
      return *(points_begin + i);
    }
    else
    {
      scalar_t t1 = *(knots_begin + i);
      scalar_t t2 = *(knots_begin + i + r);
      GGO_ASSERT_LT(t1, t2);

      scalar_t w1 = t2 - t;
      scalar_t w2 = t - t1;
      scalar_t den = t2 - t1;
      GGO_ASSERT_FLOAT_EQ(w1 + w2, den);

      data_t p1 = polynomial_curve_aux<data_t>(i, r - 1, points_begin, knots_begin, t);
      data_t p2 = polynomial_curve_aux<data_t>(i + 1, r - 1, points_begin, knots_begin, t);

      return (w1 * p1 + w2 * p2) / den;
    }
  }

  //////////////////////////////////////////////////////////////
  template <typename data_t, typename point_it_t, typename knot_it_t, typename scalar_t>
  data_t polynomial_curve(point_it_t points_begin, point_it_t points_end, knot_it_t knots_begin, knot_it_t knots_end, scalar_t t)
  {
    int points_count = static_cast<int>(std::distance(points_begin, points_end));
    int knots_count  = static_cast<int>(std::distance(knots_begin, knots_end));

    if (points_count != knots_count)
    {
      throw std::runtime_error("knots count and points count mismatch");
    }

    return polynomial_curve_aux<data_t>(0, points_count - 1, points_begin, knots_begin, t);
  }

  //////////////////////////////////////////////////////////////
  template <typename points_t, typename knots_t, typename scalar_t>
  auto polynomial_curve(const points_t & points, const knots_t & knots, scalar_t t)
  {
    return polynomial_curve<points_t::value_type>(points.begin(), points.end(), knots.begin(), knots.end(), t);
  }
}

