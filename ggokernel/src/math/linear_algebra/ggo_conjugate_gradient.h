#ifndef __GGO_CONJUGATE_GRADIENT__
#define __GGO_CONJUGATE_GRADIENT__

#include <ggo_array.h>
#include <ggo_array_arithmetics.h>

/////////////////////////////////////////////////////////////////////
// Core algorithm. Only for ggo::array.
namespace ggo
{
  template <typename data_t>
  void conjugate_gradient(const ggo::array<data_t, 2> & a, const ggo::array<data_t, 2> & b, ggo::array<data_t, 2> & s, int iterations)
  {
    // Check a is a square matrix.
    if (a.template get_size<0>() != a.template get_size<1>())
    {
      throw ggo::dimension_mismatch_exception();
    }

    // Check b is a n x 1 vertical vector.
    if (b.template get_size<0>() != a.template get_size<0>())
    {
      throw ggo::dimension_mismatch_exception();
    }
    if (b.template get_size<1>() != 1)
    {
      throw ggo::dimension_mismatch_exception();
    }

    // Check s is a n x 1 vertical vector.
    if (s.template get_size<0>() != a.template get_size<0>())
    {
      throw ggo::dimension_mismatch_exception();
    }
    if (s.template get_size<1>() != 1)
    {
      throw ggo::dimension_mismatch_exception();
    }

    auto cg_dot = [](const ggo::array<data_t, 2> & v1, const ggo::array<data_t, 2> & v2)
    {
      GGO_ASSERT(v1.template get_size<1>() == 1);
      GGO_ASSERT(v2.template get_size<1>() == 1);
      GGO_ASSERT(v1.template get_size<1>() == v2.template get_size<1>());

      data_t h = 0;
      for (int y = 0; y < v1.template get_size<0>(); ++y)
      {
        h += v1(y, 0) * v2(y, 0);
      }
      return h;
    };

    ggo::array<float, 2> r(b - a * s);
    ggo::array<float, 2> p(r);
    data_t rh_old = cg_dot(r, r);

    for (int i = 0; i < iterations; ++i)
    {
      auto ap = a * p;
      data_t alpha = rh_old / cg_dot(p, ap);
      s = s + alpha * p;
      r = r - alpha * ap;
      data_t rh_new = cg_dot(r, r);
      p = r + (rh_new / rh_old) * p;
      rh_old = rh_new;
    }
  }
}

#endif
