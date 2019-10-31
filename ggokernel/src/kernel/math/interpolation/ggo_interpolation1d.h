#ifndef __GGO_INTERPOLATE_1D__
#define __GGO_INTERPOLATE_1D__

#include <stdexcept>
#include <kernel/ggo_kernel.h>

namespace ggo
{
  //////////////////////////////////////////////////////////////////
  template <typename t_x, typename t_y>
  t_y linear_interpolation(t_x x0, t_y y0, t_x x1, t_y y1, t_x x)
  {
    t_x det = x0 - x1;
    if (std::abs(det) < t_x(0.0000001))
    {
      return (y0 + y1) / t_x(2);
    }

    t_y a = y0 - y1;
    t_y b = x0 * y1 - x1 * y0;

    return (a * x + b) / det;
  }
}

namespace ggo
{
  template <typename data_t, typename scalar_t>
  struct cubic
  {
    static_assert(std::is_floating_point_v<scalar_t>);

    data_t _a;
    data_t _b;
    data_t _c;
    data_t _d;

    //////////////////////////////////////////////////////////////////
    static cubic make_smooth(scalar_t x0, data_t y0, scalar_t x1, data_t y1, scalar_t x2, data_t y2, scalar_t x3, data_t y3)
    {
      if (std::abs(x2 - x0) <= scalar_t(0.0000001))
      {
        throw std::runtime_error("invalid data");
      }

      if (std::abs(x3 - x1) <= scalar_t(0.0000001))
      {
        throw std::runtime_error("invalid data");
      }

      scalar_t x12 = x1 * x1;
      scalar_t x13 = x1 * x12;
      scalar_t x22 = x2 * x2;
      scalar_t x23 = x2 * x22;

      scalar_t det = 3 * (x1 * x22 - x12 * x2) + x13 - x23;
      if (std::abs(det) <= scalar_t(0.0000001))
      {
        throw std::runtime_error("invalid data");
      }

      data_t t1 = (y2 - y0) / (x2 - x0);
      data_t t2 = (y3 - y1) / (x3 - x1);
      
      return {
        -(x2*(t2 + t1) + x1 * (-t2 - t1) - scalar_t(2)*y2 + scalar_t(2)*y1) / det,
        (x1*(x2*(t2 - t1) - scalar_t(3)*y2) + x22 * (t2 + scalar_t(2)*t1) + x12 * (scalar_t(-2)*t2 - t1) - scalar_t(3)*x2*y2 + (scalar_t(3)*x2 + 3 * x1)*y1) / det,
        -(x1*(x22*(scalar_t(2)*t2 + t1) - scalar_t(6)*x2*y2) - x13 * t2 + x12 * x2*(-t2 - scalar_t(2)*t1) + x23 * t1 + scalar_t(6)*x1*x2*y1) / det,
        (x12*(x22*(t2 - t1) - scalar_t(3)*x2*y2) + x13 * (y2 - x2 * t2) + x1 * x23*t1 + (scalar_t(3)*x1*x22 - x23)*y1) / det };
    }

    //////////////////////////////////////////////////////////////////
    // Optimized version where we assume that:
    //   x0 = -1
    //   x1 = 0
    //   x2 = 1
    //   x3 = 2
    static cubic make_smooth(data_t y0, data_t y1, data_t y2, data_t y3)
    {
      static_assert(std::is_floating_point_v<scalar_t>);

      // Cubic derivatives.
      data_t d1 = (y2 - y0) / scalar_t(2); // Derative at x=0.
      data_t d2 = (y3 - y1) / scalar_t(2); // Derative at x=1.

      // Now we have a linear system with 4 equations:
      // y1 = d;
      // y2 = a+b+c+d
      // d1 = c
      // d2 = 3*a+2*c+b
      // So we can compute the cubic's coefs.
      return {
        scalar_t(2) * (y1 - y2) + d1 + d2,
        scalar_t(3) * (y2 - y1) - scalar_t(2) * d1 - d2,
        d1,
        y1 };
    }

    //////////////////////////////////////////////////////////////////
    data_t evaluate(scalar_t x) const
    {
      scalar_t xx = x * x;

      return _a * x * xx + _b * xx + _c * x + _d;
    }

    //////////////////////////////////////////////////////////////////
    data_t integrate(scalar_t from, scalar_t to) const
    {
      GGO_ASSERT_LE(from, to);

      auto eval_cubic_integrale = [&](scalar_t x)
      {
        scalar_t xx = x * x;
        return _a * xx * xx / scalar_t(4) + _b * xx * x / scalar_t(3) + _c * xx / scalar_t(2) + _d * x;
      };

      return eval_cubic_integrale(to) - eval_cubic_integrale(from);
    }

    //////////////////////////////////////////////////////////////////
    data_t derivate(scalar_t x) const
    {
      return scalar_t(3) * _a * x * x + scalar_t(2) * _b * x + _c;
    }
  };

  //////////////////////////////////////////////////////////////////
  template <typename t_x, typename t_y>
  auto cubic_interpolation(t_x x0, t_y y0, t_x x1, t_y y1, t_x x2, t_y y2, t_x x3, t_y y3, t_x x)
  {
    return cubic<t_y, t_x>::make_smooth(x0, y0, x1, y1, x2, y2, x3, y3).evaluate(x);
  }

  //////////////////////////////////////////////////////////////////
  template <typename t_x, typename t_y>
  constexpr t_y cubic_interpolation(t_y y0, t_y y1, t_y y2, t_y y3, t_x x)
  {
    return cubic<t_y, t_x>::make_smooth(y0, y1, y2, y3).evaluate(x);
  }

  //////////////////////////////////////////////////////////////////
  template <typename t_x, typename t_y>
  t_y cubic_integration(t_x x0, t_y y0, t_x x1, t_y y1, t_x x2, t_y y2, t_x x3, t_y y3, t_x from, t_x to)
  {
    return cubic<t_y, t_x>::make_smooth(x0, y0, x1, y1, x2, y2, x3, y3).integrate(from, to);
  }

  //////////////////////////////////////////////////////////////////
  template <typename t_x, typename t_y>
  t_y cubic_integration(t_y y0, t_y y1, t_y y2, t_y y3, t_x from, t_x to)
  {
    return cubic<t_y, t_x>::make_smooth(y0, y1, y2, y3).integrate(from, to);
  }
}

#endif
