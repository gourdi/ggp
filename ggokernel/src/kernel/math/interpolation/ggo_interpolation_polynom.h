#pragma once

#include <kernel/ggo_kernel.h>
#include <kernel/math/ggo_polynom.h>

// Linear interpolation.
namespace ggo
{
  //////////////////////////////////////////////////////////////////
  template <typename data_t, typename scalar_t>
  polynom<data_t, 1> make_interpolation_linear(scalar_t x0, data_t y0, scalar_t x1, data_t y1)
  {
    if (std::abs(x1 - x0) <= scalar_t(0.0000001))
    {
      throw std::runtime_error("invalid data");
    }

    data_t deg1 = (y1 - y0) / (x1 - x0);
    data_t deg0 = y0 - deg1 * x0;

    return polynom<data_t, 1>({ deg0, deg1 });
  }

  //////////////////////////////////////////////////////////////////
  // Optimized version where we assume that:
  //   x0 = 0
  //   x1 = 1
  template <typename data_t>
  polynom<data_t, 1> make_interpolation_linear(data_t y0, data_t y1)
  {
    data_t deg1 = y1 - y0;
    data_t deg0 = y0;

    return polynom<data_t, 1>({ deg0, deg1 });
  }
}

// Cubic interpolation.
namespace ggo
{
  //////////////////////////////////////////////////////////////////
  template <typename data_t, typename scalar_t>
  polynom<data_t, 3> make_interpolation_cubic(scalar_t x0, data_t y0, scalar_t x1, data_t y1, scalar_t x2, data_t y2, scalar_t x3, data_t y3)
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

    return polynom<data_t, 3>({
      (x12 * (x22 * (t2 - t1) - scalar_t(3) * x2 * y2) + x13 * (y2 - x2 * t2) + x1 * x23 * t1 + (scalar_t(3) * x1 * x22 - x23) * y1) / det,
      -(x1 * (x22 * (scalar_t(2) * t2 + t1) - scalar_t(6) * x2 * y2) - x13 * t2 + x12 * x2 * (-t2 - scalar_t(2) * t1) + x23 * t1 + scalar_t(6) * x1 * x2 * y1) / det,
      (x1 * (x2 * (t2 - t1) - scalar_t(3) * y2) + x22 * (t2 + scalar_t(2) * t1) + x12 * (scalar_t(-2) * t2 - t1) - scalar_t(3) * x2 * y2 + (scalar_t(3) * x2 + 3 * x1) * y1) / det,
      -(x2 * (t2 + t1) + x1 * (-t2 - t1) - scalar_t(2) * y2 + scalar_t(2) * y1) / det });
  }

  //////////////////////////////////////////////////////////////////
  // Optimized version where we assume that:
  //   x0 = -1
  //   x1 = 0
  //   x2 = 1
  //   x3 = 2
  template <typename data_t, typename scalar_t = data_t>
  polynom<data_t, 3> make_interpolation_cubic(data_t y0, data_t y1, data_t y2, data_t y3)
  {
    // Cubic derivatives.
    data_t d1 = (y2 - y0) / scalar_t(2); // Derative at x=0.
    data_t d2 = (y3 - y1) / scalar_t(2); // Derative at x=1.

    // Now we have a linear system with 4 equations:
    // y1 = d;
    // y2 = a+b+c+d
    // d1 = c
    // d2 = 3*a+2*c+b
    // So we can compute the cubic's coefs.
    return polynom<data_t, 3>({
      y1,
      d1,
      scalar_t(3)* (y2 - y1) - scalar_t(2) * d1 - d2,
      scalar_t(2) * (y1 - y2) + d1 + d2 });
  }
}
