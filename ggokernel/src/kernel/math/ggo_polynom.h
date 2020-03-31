#pragma once

#include <kernel/ggo_kernel.h>
#include <kernel/ggo_assign.h>

namespace ggo
{
  template <typename data_t, int degree>
  struct polynom
  {
    polynom() = default;

    polynom(data_t const (&coefs)[degree + 1])
    {
      ggo::assign(_coefs, coefs);
    }

    data_t _coefs[degree + 1];
  };

  //////////////////////////////////////////////////////////////////
  template <typename data_t, int degree, typename scalar_t>
  data_t evaluate(const polynom<data_t, degree>& polynom, scalar_t x)
  {
    data_t r = polynom._coefs[degree];

    for (int d = degree - 1; d >= 0; --d)
    {
      r = r * x + polynom._coefs[d];
    }

    return r;
  }

  //////////////////////////////////////////////////////////////////
  template <typename data_t, int degree, typename scalar_t = data_t>
  polynom<data_t, degree - 1> derivate(const polynom<data_t, degree>& p)
  {
    static_assert(degree > 0);

    polynom<data_t, degree - 1> r;

    for (int d = 0; d <= degree - 1; ++d)
    {
      r._coefs[d] = p._coefs[d + 1] * scalar_t(d + 1);
    }

    return r;
  }

  //////////////////////////////////////////////////////////////////
  template <typename data_t, int degree, typename scalar_t>
  data_t derivate(const polynom<data_t, degree>& p, scalar_t x)
  {
    return evaluate(derivate(p), x);
  }

  //////////////////////////////////////////////////////////////////
  template <typename data_t, int degree, typename scalar_t = data_t>
  polynom<data_t, degree + 1> integrate(const polynom<data_t, degree>& p, data_t k_deg0)
  {
    polynom<data_t, degree + 1> r;

    for (int d = 0; d <= degree; ++d)
    {
      r._coefs[d + 1] = p._coefs[d] / scalar_t(d + 1);
    }
    r._coefs[0] = k_deg0;

    return r;
  }

  //////////////////////////////////////////////////////////////////
  template <typename data_t, int degree, typename scalar_t>
  data_t integrate(const polynom<data_t, degree>& p, scalar_t from, scalar_t to)
  {
    GGO_ASSERT_LE(from, to);

    if constexpr (degree == 0)
    {
      return (to - from) * p._coefs[0];
    }
    else
    {
      data_t r_to = p._coefs[degree] * to / scalar_t(degree + 1) + p._coefs[degree - 1] / scalar_t(degree);
      data_t r_from = p._coefs[degree] * from / scalar_t(degree + 1) + p._coefs[degree - 1] / scalar_t(degree);

      for (int d = degree - 2; d >= 0; --d)
      {
        r_to = to * r_to + p._coefs[d] / scalar_t(d + 1);
        r_from = from * r_from + p._coefs[d] / scalar_t(d + 1);
      }

      return to * r_to - from * r_from;
    }
  }
}
