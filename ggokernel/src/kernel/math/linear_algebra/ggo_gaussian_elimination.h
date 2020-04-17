#pragma once

#include <kernel/memory/ggo_array.h>

namespace ggo
{
  /////////////////////////////////////////////////////////////////////
  template <typename data_t, typename is_zero_t>
  ggo::array<data_t, 1> gaussian_elimination(ggo::array<data_t, 2> A, ggo::array<data_t, 1> b, data_t zero, is_zero_t && is_zero)
  {
    if (A.dim(0) != A.dim(1))
    {
      throw std::runtime_error("non-square matrix");
    }
    if (A.dim(0) != b.dim(0))
    {
      throw std::runtime_error("dimension mismatch");
    }

    int size = A.dim(0);

    // First pass: make the matrix upper triangular.
    for (int step = 1; step < size; ++step)
    {
      for (int row = step; row < size; ++row)
      {
        if (is_zero(A(row, step - 1)) == false)
        {
          auto r = A(step - 1, step - 1) / A(row, step - 1);

          for (int col = 0; col < size; ++col)
          {
            A(row, col) = col < step ? zero : A(step - 1, col) - r * A(row, col);
          }
          b(row) = b(step - 1) - r * b(row);
        }
      }
    }

    // Then compute solution vector.
    ggo::array<data_t, 1> s(size);
    for (int row = size - 1; row >= 0; --row)
    {
      s(row) = b(row);
      for (int col = size - 1; col > row; --col)
      {
        s(row) = s(row) - A(row, col) * s(col);
      }
      s(row) = s(row) / A(row, row);
    }

    return s;
  }

  /////////////////////////////////////////////////////////////////////
  template <typename data_t>
  ggo::array<data_t, 1> gaussian_elimination(ggo::array<data_t, 2> A, ggo::array<data_t, 1> b)
  {
    return gaussian_elimination(A, b, data_t(0), [](data_t v) { return v == data_t(0); });
  }
}