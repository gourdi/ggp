#include <ggo_linear_algebra.h>
#include <ggo_kernel.h>
#include <cstring>

//////////////////////////////////////////////////////////////
// Matrices.

namespace ggo
{
  /////////////////////////////////////////////////////////////////////
  template <typename data_t, int size_y, int size_x>
  matrix<data_t, size_y, size_x> & matrix<data_t, size_y, size_x>::operator=(const matrix<data_t, size_y, size_x> & m)
  {
    if (&m != this)
    {
      ggo::copy<size_x * size_y, data_t>(_data.data(), m._data.data());
    }
    
    return *this;
  }

  /////////////////////////////////////////////////////////////////////
  template <typename data_t, int size_y, int size_x>
  void matrix<data_t, size_y, size_x>::operator*=(data_t k)
  {
    ggo::mul<size_x * size_y>(_data.data(), k);
  }
}

/////////////////////////////////////////////////////////////////////
// Matrix operators.

namespace ggo
{
  /////////////////////////////////////////////////////////////////////
  template <typename data_t, int size_y, int size_x>
  matrix<data_t, size_y, size_x> operator+(const matrix<data_t, size_y, size_x> & m1, const matrix<data_t, size_y, size_x> & m2)
  {
    matrix<data_t, size_y, size_x> result;

    ggo::binary_operation<size_x * size_y>(result.data(), m1.data(), m2.data(), [](data_t v1, data_t v2) { return v1 + v2; });

    return result;
  }

  /////////////////////////////////////////////////////////////////////
  template <typename data_t, int size_y, int size_x>
  matrix<data_t, size_y, size_x> operator-(const matrix<data_t, size_y, size_x> & m1, const matrix<data_t, size_y, size_x> & m2)
  {
    matrix<data_t, size_y, size_x> result;

    ggo::binary_operation<size_x * size_y>(result.data(), m1.data(), m2.data(), [](data_t v1, data_t v2) { return v1 - v2; });

    return result;
  }

  /////////////////////////////////////////////////////////////////////
  template <typename data_t, int size_1, int size_2, int size_3>
  matrix<data_t, size_1, size_3> operator*(const matrix<data_t, size_1, size_2> & m1, const matrix<data_t, size_2, size_3> & m2)
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
// Scalar operators.

namespace ggo
{
  /////////////////////////////////////////////////////////////////////
  template <typename data_t, int size_y, int size_x>
  matrix<data_t, size_y, size_x> operator*(const matrix<data_t, size_y, size_x> & m, data_t k)
  {
    matrix<data_t, size_y, size_x> result;
    
    for (int y = 0; y < size_y; ++y)
    {
      for (int x = 0; x < size_x; ++x)
      {
        result(y, x) = k * m(y, x);
      }
    }
    
    return result;
  }

  /////////////////////////////////////////////////////////////////////
  template <typename data_t, int size_y, int size_x>
  matrix<data_t, size_y, size_x> operator*(data_t k, const matrix<data_t, size_y, size_x> & m)
  {
    return m * k;
  }
}

/////////////////////////////////////////////////////////////////////
// Square matrix.

namespace ggo
{
  /////////////////////////////////////////////////////////////////////
  template <typename data_t, int size>
  bool square_matrix<data_t, size>::is_diagonally_dominant() const
  {
    return ggo::is_matrix_diagonally_dominant<data_t>(*this, size);
  }
  
  /////////////////////////////////////////////////////////////////////
  template <typename data_t, int size>
  bool square_matrix<data_t, size>::is_symmetric() const
  {
    auto compare = [](data_t v1, data_t v2) { return v1 == v2; };

    return ggo::is_matrix_symmetric(*this, size, compare);
  }

  /////////////////////////////////////////////////////////////////////
  template <typename data_t, int size>
  template <typename compare_func>
  bool square_matrix<data_t, size>::is_symmetric(compare_func compare) const
  {
    return ggo::is_matrix_symmetric(*this, size, compare);
  }
}

#if 0

/////////////////////////////////////////////////////////////////////
// Square matrix 2D.

namespace ggo
{
  /////////////////////////////////////////////////////////////////////
  template <typename T>
  ggo::vec2<T> operator*(const square_matrix2d<T> & m, const ggo::vec2<T> & v) 
  {
    return ggo::vec2<T>(m(0, 0) * v.template get<0>() + m(0, 1) * v.template get<1>(),
                        m(1, 0) * v.template get<0>() + m(1, 1) * v.template get<1>());
  }

  /////////////////////////////////////////////////////////////////////
  template <typename T>
  T square_matrix2d<T>::det() const
  {
    return this->operator()(0, 0)*this->operator()(1, 1) - this->operator()(1, 0)*this->operator()(0, 1); 
  }

  /////////////////////////////////////////////////////////////////////
  template <typename T>
  void square_matrix2d<T>::set_rotation(float angle) 
  {
    this->operator()(0, 0) =  cos(angle);
    this->operator()(0, 1) = -sin(angle);
    this->operator()(1, 0) =  sin(angle);
    this->operator()(1, 1) =  cos(angle);
  }
}

#endif

