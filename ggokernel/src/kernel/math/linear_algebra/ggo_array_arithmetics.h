#ifndef __GGO_VECTOR_SPACE__
#define __GGO_VECTOR_SPACE__

#include <kernel/ggo_kernel.h>
#include <kernel/memory/ggo_array.h>

/////////////////////////////////////////////////////////////////////
// Add arithmetic operations on ggo::array by considering them as matrices.
//
// WARNING: The first index of the matrix is the row (y) and the second one is the column (x).
// So m.size<0>() returns the numbers of rows (height) and m.size<1>() the number of columns (width).

namespace ggo
{
  template <typename data_t>
  ggo::array<data_t, 2> operator*(const ggo::array<data_t, 2> & m1, const ggo::array<data_t, 2> & m2)
  {
    if (m1.height() != m2.width())
    {
      throw ggo::dimension_mismatch_exception();
    }

    ggo::array<data_t, 2> r(m1.width(), m2.height());

    for (int y = 0; y < m1.width(); ++y)
    {
      for (int x = 0; x < m2.height(); ++x)
      {
        r(y, x) = 0;
        for (int j = 0; j < m1.height(); ++j)
        {
          r(y, x) += m1(y, j) * m2(j, x);
        }
      }
    }

    return r;
  }

  template <typename data_t>
  ggo::array<data_t, 2> operator*(data_t k, const ggo::array<data_t, 2> & m)
  {
    ggo::array<data_t, 2> r(m.width(), m.height());

    for (int i = 0; i < r.count(); ++i)
    {
      r.data()[i] = k * m.data()[i];
    }

    return r;
  }

  template <typename data_t>
  ggo::array<data_t, 2> operator+(const ggo::array<data_t, 2> & m1, const ggo::array<data_t, 2> & m2)
  {
    if (m1.width() != m2.width() || m1.height() != m2.height())
    {
      throw ggo::dimension_mismatch_exception();
    }

    ggo::array<data_t, 2> r(m1.width(), m2.height());

    for (int i = 0; i < r.count(); ++i)
    {
      r.data()[i] = m1.data()[i] + m2.data()[i];
    }

    return r;
  }

  template <typename data_t>
  ggo::array<data_t, 2> operator-(const ggo::array<data_t, 2> & m1, const ggo::array<data_t, 2> & m2)
  {
    if (m1.width() != m2.width() || m1.height() != m2.height())
    {
      throw ggo::dimension_mismatch_exception();
    }

    ggo::array<data_t, 2> r(m1.width(), m2.height());

    for (int i = 0; i < r.count(); ++i)
    {
      r.data()[i] = m1.data()[i] - m2.data()[i];
    }

    return r;
  }
}

#endif
