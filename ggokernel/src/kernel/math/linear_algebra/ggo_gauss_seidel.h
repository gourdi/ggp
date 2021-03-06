#ifndef __GGO_GAUSS_SEIDEL__
#define __GGO_GAUSS_SEIDEL__

#include <kernel/ggo_vec.h>
#include <kernel/memory/ggo_array.h>
#include <kernel/math/linear_algebra/ggo_matrix.h>

/////////////////////////////////////////////////////////////////////
// Core algorithm.
namespace ggo
{
  template <typename data_t, typename matrix, typename read_b, typename read_s, typename write_s>
  void gauss_seidel(const int size, matrix a, read_b b, read_s rs, write_s ws, const int iterations)
  {
    // todo precompute diagonal inverses

    for (int i = 0; i < iterations; ++i)
    {
      for (int y = 0; y < size; ++y)
      {
        data_t sigma = 0;
        for (int x = 0; x < size; ++x)
        {
          if (y != x)
          {
            sigma += a(y, x) * rs(x);
          }
        }
        ws(y, (b(y) - sigma) / a(y, y));
      }
    }
  }
}

/////////////////////////////////////////////////////////////////////
// Helpers functions that handle various data types.
namespace ggo
{
  template <typename data_t, int size>
  void gauss_seidel(const data_t(&a)[size][size], const data_t(&b)[size], data_t(&s)[size], int iterations)
  {
    auto lambda_a = [&](int x, int y) { return a[x][y]; };
    auto lambda_b = [&](int i) { return b[i]; };
    auto read_s   = [&](int i) { return s[i]; };
    auto write_s  = [&](int i, float v) { s[i] = v; };

    return gauss_seidel<data_t>(size, lambda_a, lambda_b, read_s, write_s, iterations);
  }

  //template <typename data_t, int size>
  //void gauss_seidel(const ggo::matrix<data_t, size, size> & a, const ggo::vec_base<data_t, size> & b, ggo::vec_base<data_t, size> & s, int iterations)
  //{
  //  auto lambda_a = [&](int x, int y) { return a(x, y); };
  //  auto lambda_b = [&](int i) { return b.data()[i]; };
  //  auto read_s   = [&](int i) { return s.data()[i]; };
  //  auto write_s  = [&](int i, float v) { s.data()[i] = v; };

  //  return gauss_seidel<data_t>(size, lambda_a, lambda_b, read_s, write_s, iterations);
  //}

  template <typename data_t>
  void gauss_seidel(const ggo::array2<data_t> & a, const ggo::array1<data_t> & b, ggo::array1<data_t> & s, int iterations)
  {
    if (a.width() != a.height())
    {
      throw std::runtime_error("dimension mismatch");
    }
    if (a.width() != b.size())
    {
      throw std::runtime_error("dimension mismatch");
    }
    if (a.width() != s.size())
    {
      throw std::runtime_error("dimension mismatch");
    }

    auto lambda_a = [&](int x, int y) { return a(x, y); };
    auto lambda_b = [&](int i) { return b.data()[i]; };
    auto read_s   = [&](int i) { return s.data()[i]; };
    auto write_s  = [&](int i, float v) { s.data()[i] = v; };

    return gauss_seidel<data_t>(a.width(), lambda_a, lambda_b, read_s, write_s, iterations);
  }
}

#endif
