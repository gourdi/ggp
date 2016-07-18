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

    ggo::binary_operation<size_x * size_y>(result.data(), m1.data(), m2.data(), [](data_t & dst, const data_t & src1, const data_t & src2) { dst = src1 + src2; });

    return result;
  }

  /////////////////////////////////////////////////////////////////////
  template <typename data_t, int size_y, int size_x>
  matrix<data_t, size_y, size_x> operator-(const matrix<data_t, size_y, size_x> & m1, const matrix<data_t, size_y, size_x> & m2)
  {
    matrix<data_t, size_y, size_x> result;

    ggo::binary_operation<size_x * size_y>(result.data(), m1.data(), m2.data(), [](data_t & dst, const data_t & src1, const data_t & src2) { dst = src1 - src2; });

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

  template <typename data_t, int row_index, int current, int size>
  struct compute_row_t
  {
    static data_t compute_row(const ggo::square_matrix<data_t, size> & m, const ggo::vec<data_t, size> & v)
    {
      return m.template get<row_index, current>() * v.template get<current>() + ggo::compute_row_t<data_t, row_index, current + 1, size>::compute_row(m, v);
    }
  };

  template <typename data_t, int row_index, int size>
  struct compute_row_t<data_t, row_index, size, size>
  {
    static data_t compute_row(const ggo::square_matrix<data_t, size> & m, const ggo::vec<data_t, size> & v)
    {
      return 0;
    }
  };

  template <typename data_t, int row_index, int size>
  struct process_row_t
  {
    static void process_row(ggo::vec<data_t, size> & r, const ggo::square_matrix<data_t, size> & m, const ggo::vec<data_t, size> & v)
    {
      // Compute coef.
      r.template get<row_index>() = ggo::compute_row_t<data_t, row_index, 0, size>::compute_row(m, v);

      // Process next row.
      ggo::process_row_t<data_t, row_index + 1, size>::process_row(r, m, v);
    }
  };

  template <typename data_t, int size>
  struct process_row_t<data_t, size, size>
  {
    static void process_row(ggo::vec<data_t, size> & r, const ggo::square_matrix<data_t, size> & m, const ggo::vec<data_t, size> & v)
    {
      // Do nothing.
    }
  };

  template <typename data_t, int size>
  ggo::vec<data_t, size> operator*(const ggo::square_matrix<data_t, size> & m, const ggo::vec<data_t, size> & v)
  {
    ggo::vec<data_t, size> r;

    process_row_t<data_t, 0, size>::process_row(r, m, v);

    return r;
  }
}

/////////////////////////////////////////////////////////////////////
// Square matrix 2D.

namespace ggo
{

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
