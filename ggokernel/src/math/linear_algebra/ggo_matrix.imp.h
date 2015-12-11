#include <ggo_linear_algebra.h>
#include <cstring>

//////////////////////////////////////////////////////////////
// Matrices.

namespace ggo
{
  /////////////////////////////////////////////////////////////////////
  template <int size_y, int size_x, typename T>
  matrix<size_y, size_x, T> & matrix<size_y, size_x, T>::operator=(const matrix<size_y, size_x, T> & m)
  {
    if (&m != this)
    {
      std::memcpy(_data.data(), m._data.data(), size_x * size_y * sizeof(T));
    }
    
    return *this;
  }

  /////////////////////////////////////////////////////////////////////
  template <int size_y, int size_x, typename T>
  template <typename T2>
  void matrix<size_y, size_x, T>::operator*=(T2 k)
  {
    for (int i = 0; i < size_x * size_y; ++i)
    {
      _data[i] *= k;
    }
  }
}

/////////////////////////////////////////////////////////////////////
// Matrix operators.

namespace ggo
{
  /////////////////////////////////////////////////////////////////////
  template <int size_y, int size_x, typename T>
  matrix<size_y, size_x, T> operator+(const matrix<size_y, size_x, T> & m1, const matrix<size_y, size_x, T> & m2)
  {
    matrix<size_y, size_x, T> result;
    
    for (int y = 0; y < size_y; ++y)
    {
      for (int x = 0; x < size_x; ++x)
      {
        result(y, x) = m1(y, x) + m2(y, x);
      }
    }
    
    return result;
  }

  /////////////////////////////////////////////////////////////////////
  template <int size_y, int size_x, typename T>
  matrix<size_y, size_x, T> operator-(const matrix<size_y, size_x, T> & m1, const matrix<size_y, size_x, T> & m2)
  {
    matrix<size_y, size_x, T> result;
    
    for (int y = 0; y < size_y; ++y)
    {
      for (int x = 0; x < size_x; ++x)
      {
        result(y, x) = m1(y, x) - m2(y, x);
      }
    }
    
    return result;
  }

  /////////////////////////////////////////////////////////////////////
  template <int size_1, int size_2, int size_3, typename T>
  matrix<size_1, size_3, T> operator*(const matrix<size_1, size_2, T> & m1, const matrix<size_2, size_3, T> & m2)
  {
    matrix<size_1, size_3, T> result;
    
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
  template <int size_y, int size_x, typename T1, typename T2>
  matrix<size_y, size_x, T1> operator*(const matrix<size_y, size_x, T1> & m, T2 k)
  {
    matrix<size_y, size_x, T1> result;
    
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
  template <int size_y, int size_x, typename T1, typename T2>
  matrix<size_y, size_x, T1> operator*(T2 k, const matrix<size_y, size_x, T1> & m)
  {
    return m * k;
  }
}

/////////////////////////////////////////////////////////////////////
// Square matrix.

namespace ggo
{
  /////////////////////////////////////////////////////////////////////
  template <int size, typename T>
  bool square_matrix<size, T>::is_diagonally_dominant() const
  {
    return ggo::is_matrix_diagonally_dominant<square_matrix<size, T>, T>(*this, size);
  }
  
  /////////////////////////////////////////////////////////////////////
  template <int size, typename T>
  bool square_matrix<size, T>::is_symmetric(std::function<bool(T, T)> compare_func) const
  {
   return ggo::is_matrix_symmetric<square_matrix<size, T>, T>(*this, size, compare_func);
  }
}

/////////////////////////////////////////////////////////////////////
// Square matrix 2D.

namespace ggo
{
  /////////////////////////////////////////////////////////////////////
  template <typename T>
  ggo::set2<T> operator*(const square_matrix2d<T> & m, ggo::set2<T> v) 
  {
    return ggo::set2<T>(m(0, 0)*v.x() + m(0, 1)*v.y(), m(1, 0)*v.x() + m(1, 1)*v.y());
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

