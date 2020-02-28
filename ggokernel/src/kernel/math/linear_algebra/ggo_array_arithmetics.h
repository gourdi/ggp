#ifndef __GGO_VECTOR_SPACE__
#define __GGO_VECTOR_SPACE__

#include <kernel/ggo_kernel.h>
#include <kernel/memory/ggo_array.h>

/////////////////////////////////////////////////////////////////////
// Add arithmetic operations on ggo::array by considering them as matrices.

namespace ggo
{
  template <typename data_t>
  ggo::array2<data_t> operator*(const ggo::array2<data_t> & m1, const ggo::array2<data_t> & m2)
  {
    if (m1.width() != m2.height())
    {
      throw std::runtime_error("dimension mismatch");
    }

    ggo::array2<data_t> r(m2.width(), m1.height());

    for (int y = 0; y < m1.height(); ++y)
    {
      for (int x = 0; x < m2.width(); ++x)
      {
        r(x, y) = 0;
        for (int j = 0; j < m1.width(); ++j)
        {
          r(x, y) += m1(j, y) * m2(x, j);
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
  ggo::array2<data_t> operator+(const ggo::array2<data_t> & m1, const ggo::array2<data_t> & m2)
  {
    if (m1.width() != m2.width() || m1.height() != m2.height())
    {
      throw std::runtime_error("dimension mismatch");
    }

    ggo::array2<data_t> r(m1.width(), m2.height());

    for (int i = 0; i < r.count(); ++i)
    {
      r.data()[i] = m1.data()[i] + m2.data()[i];
    }

    return r;
  }

  template <typename data_t>
  ggo::array2<data_t> operator-(const ggo::array2<data_t> & m1, const ggo::array2<data_t> & m2)
  {
    if (m1.width() != m2.width() || m1.height() != m2.height())
    {
      throw std::runtime_error("dimension mismatch");
    }

    ggo::array2<data_t> r(m1.width(), m2.height());

    for (int i = 0; i < r.count(); ++i)
    {
      r.data()[i] = m1.data()[i] - m2.data()[i];
    }

    return r;
  }
}

#endif
