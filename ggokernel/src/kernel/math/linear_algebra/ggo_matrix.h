#ifndef GGO_MATRIX_H_
#define GGO_MATRIX_H_

#include <functional>
#include <kernel/math/linear_algebra/ggo_linear_algebra.h>

//////////////////////////////////////////////////////////////
// Matrices. Dimensions are known at compile-time.

//////////////////////////////////////////////////////////////
// General matrices.
namespace ggo
{
  template <typename data_t, int size_y, int size_x>
  class matrix
  {
  public:

    constexpr matrix() : _data() {}
    constexpr matrix(const matrix & m) = default;
    constexpr matrix(matrix && m) = default;
    constexpr matrix(data_t const (&coefs)[size_y][size_x]) : _data()
    {
      for (int y = 0; y < size_y; ++y)
      {
        for (int x = 0; x < size_x; ++x)
        {
          _data[y *size_x + x] = coefs[y][x];
        }
      }
    }

    constexpr data_t &        operator()(unsigned y, unsigned x) { return _data[y * size_x + x]; }
    constexpr const data_t &  operator()(unsigned y, unsigned x) const { return _data[y * size_x + x]; }
    
    matrix &                  operator=(const matrix<data_t, size_y, size_x> & m) = default;
    matrix &                  operator=(matrix<data_t, size_y, size_x> && m) = default;

    constexpr bool  operator==(const matrix<data_t, size_y, size_x> & m) const { return details::eq<data_t, size_y * size_x>(this->_data, m._data); }
    constexpr bool  operator!=(const matrix<data_t, size_y, size_x> & m) const { return !this->operator==(m); }

  private:
    
    data_t _data[size_x * size_y];
  };
}

/////////////////////////////////////////////////////////////////////
// Matrix operators.
namespace ggo
{
  template <typename data_t, int size_1, int size_2, int size_3>
  constexpr matrix<data_t, size_1, size_3> operator*(const matrix<data_t, size_1, size_2> & m1, const matrix<data_t, size_2, size_3> & m2)
  {
    matrix<data_t, size_1, size_3> result;

    for (int y = 0; y < size_1; ++y)
    {
      for (int x = 0; x < size_3; ++x)
      {
        result(y, x) = 0;
        for (int j = 0; j < size_2; ++j)
        {
          result(y, x) += m1(y, j) * m2(j, x);
        }
      }
    }

    return result;
  }
}

/////////////////////////////////////////////////////////////////////
// Square matrix.
namespace ggo
{
  template <typename data_t, int size>
  using square_matrix = matrix<data_t, size, size>;

  template <typename data_t, int size>
  constexpr bool is_symmetric(const square_matrix<data_t, size> & m)
  {
    return is_symmetric(m, size);
  }

  template <typename vec_t,
    typename data_t = typename vec_t::_data_t, int n_dims = vec_t::_n_dims,
    typename = std::enable_if_t<std::is_base_of_v<vec_base<data_t, n_dims>, vec_t>>>
    constexpr vec_t operator*(const square_matrix<data_t, n_dims> & m, const vec_t & v)
  {
    vec_t result;

    for (int y = 0; y < n_dims; ++y)
    {
      result[y] = 0;
      for (int x = 0; x < n_dims; ++x)
      {
        result[y] += m(y, x) * v[x];
      }
    }

    return result;
  }
}

/////////////////////////////////////////////////////////////////////
// Square matrix 2D.
namespace ggo
{
  template <typename data_t>
  using square_matrix2d = square_matrix<data_t, 2>;

  template <typename data_t>
  constexpr data_t det(const square_matrix2d<data_t> & m)
  {
    return m(0, 0) * m(1, 1) - m(0, 1) * m(1, 0);
  }
}

#endif
