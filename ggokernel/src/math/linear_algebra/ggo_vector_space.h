#ifndef __GGO_VECTOR_SPACE__
#define __GGO_VECTOR_SPACE__

#include <ggo_kernel.h>
#include <ggo_array.h>

/////////////////////////////////////////////////////////////////////
// Add a vector space structure ovec ggo::array.
//
// WARNING: The first index of the matrix is the row (y) and the second one is the column (x).
// So m.get_size<0>() returns the numbers of rows (height) and m.get_size<1>() the number of columns (width).

/////////////////////////////////////////////////////////////////////
// Matrix/matrix multiplication. The matrix must ve square.
namespace ggo
{
  template <typename data_t>
  ggo::array<data_t, 2> operator*(const ggo::array<data_t, 2> & m1, const ggo::array<data_t, 2> & m2)
  {
    if (m1.get_size<1>() != m2.get_size<0>())
    {
      throw ggo::dimension_mismatch_exception();
    }

    ggo::array<data_t, 2> r(m1.get_size<0>(), m2.get_size<1>());

    for (int y = 0; y < m1.get_size<0>(); ++y)
    {
      for (int x = 0; x < m2.get_size<1>(); ++x)
      {
        r(y, x) = 0;
        for (int j = 0; j < m1.get_size<1>(); ++j)
        {
          r(y, x) += m1(y, j) * m2(j, x);
        }
      }
    }

    return r;
  }
}

#endif
