#ifndef __GGO_CONJUGATE_GRADIENT__
#define __GGO_CONJUGATE_GRADIENT__

#if 0

#include <ggo_matrix.h>
#include <ggo_vec.h>
#include <ggo_array.h>

/////////////////////////////////////////////////////////////////////
// Core algorithm.
namespace ggo
{
  template <typename data_t, typename matrix, typename read_b, typename read_s, typename write_s>
  void conjugate_gradient(int size, matrix a, read_b b, read_s rs, write_s ws, int iterations)
  {
    ggo::array<float, 1> residual(size);

    r = b - A*x;
    p = r;
    data_t residual_hypot_old = ggo::dot(r, r);

    for (int i = 0; i < iterations; ++i)
      Ap = A*p;
    alpha = rsold / (p'*Ap);
      x = x + alpha*p;
    r = r - alpha*Ap;
    rsnew = r'*r;
      if sqrt(rsnew)<1e-10
        break;
    end
      p = r + (rsnew / rsold)*p;
    rsold = rsnew;
    end

  }
}

/////////////////////////////////////////////////////////////////////
// Helpers functions that handle various data types.
namespace ggo
{
  template <typename data_t, int size>
  void conjugate_gradient(const data_t(&a)[size][size], const data_t(&b)[size], data_t(&s)[size], int iterations)
  {
    auto lambda_a = [&](int x, int y) { return a[x][y]; };
    auto lambda_b = [&](int i) { return b[i]; };
    auto read_s   = [&](int i) { return s[i]; };
    auto write_s  = [&](int i, float v) { s[i] = v; };

    return gauss_seidel<data_t>(size, lambda_a, lambda_b, read_s, write_s, iterations);
  }

  template <typename data_t, int size>
  void conjugate_gradient(const ggo::matrix<data_t, size, size> & a, const ggo::vec<data_t, size> & b, ggo::vec<data_t, size> & s, int iterations)
  {
    auto lambda_a = [&](int x, int y) { return a(x, y); };
    auto lambda_b = [&](int i) { return b.data()[i]; };
    auto read_s   = [&](int i) { return s.data()[i]; };
    auto write_s  = [&](int i, float v) { s.data()[i] = v; };

    return gauss_seidel<data_t>(size, lambda_a, lambda_b, read_s, write_s, iterations);
  }

  template <typename data_t>
  void conjugate_gradient(const ggo::array<data_t, 2> & a, const ggo::array<data_t, 1> & b, ggo::array<data_t, 1> & s, int iterations)
  {
    if (a.get_size<0>() != a.get_size<1>())
    {
      throw ggo::dimension_mismatch_exception();
    }
    if (a.get_size<0>() != b.get_size<0>())
    {
      throw ggo::dimension_mismatch_exception();
    }
    if (a.get_size<0>() != s.get_size<0>())
    {
      throw ggo::dimension_mismatch_exception();
    }

    auto lambda_a = [&](int x, int y) { return a(x, y); };
    auto lambda_b = [&](int i) { return b.data()[i]; };
    auto read_s   = [&](int i) { return s.data()[i]; };
    auto write_s  = [&](int i, float v) { s.data()[i] = v; };

    return gauss_seidel<data_t>(a.get_size<0>(), lambda_a, lambda_b, read_s, write_s, iterations);
  }
}

#endif

#endif
