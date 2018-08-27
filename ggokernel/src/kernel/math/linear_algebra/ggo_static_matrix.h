#ifndef GGO_STATIC_MATRIX_H_
#define GGO_STATIC_MATRIX_H_

#include <array>
#include <functional>
#include <kernel/ggo_vec.h>

//////////////////////////////////////////////////////////////
// Matrices. Dimensions are known at compile-time.
// Indices are y-first and x-second.

//////////////////////////////////////////////////////////////
// General matrices.
namespace ggo
{
  template <typename data_t, int size_y, int size_x>
  class static_matrix
  {
  public:

    // Compile time accessors.
    template <int y, int x>
    data_t &  get()
    {
      static_assert(x >= 0 && x < size_x && y >= 0 && x < size_y);
      return _data[y * size_x + x];
    }

    template <int y, int x>
    const data_t &  get() const
    {
      static_assert(x >= 0 && x < size_x && y >= 0 && x < size_y);
      return _data[y * size_x + x];
    }

    // Run time accessors.
    data_t &			  operator()(unsigned y, unsigned x) { return _data[y * size_x + x]; }
    const data_t &  operator()(unsigned y, unsigned x) const { return _data[y * size_x + x]; }
    
    static_matrix & operator=(const static_matrix<data_t, size_y, size_x> & m);

    void			      operator*=(data_t k);

    data_t *        data() { return _data.data(); };
    const data_t *  data() const { return _data.data(); };

  protected:
    
    std::array<data_t, size_x * size_y> _data;
  };
}

/////////////////////////////////////////////////////////////////////
// Matrix operators.
namespace ggo
{
  template <typename data_t, int size_y, int size_x>
  static_matrix<data_t, size_y, size_x> operator+(const static_matrix<data_t, size_y, size_x> & m1, const static_matrix<data_t, size_y, size_x> & m2);

  template <typename data_t, int size_y, int size_x>
  static_matrix<data_t, size_y, size_x> operator-(const static_matrix<data_t, size_y, size_x> & m1, const static_matrix<data_t, size_y, size_x> & m2);

  template <typename data_t, int size_1, int size_2, int size_3>
  static_matrix<data_t, size_1, size_3> operator*(const static_matrix<data_t, size_1, size_2> & m1, const static_matrix<data_t, size_2, size_3> & m2);
}

/////////////////////////////////////////////////////////////////////
// Scalar operators.
namespace ggo
{
  template <int size_x, int size_y, typename data_t>
  static_matrix<data_t, size_y, size_x> operator*(const static_matrix<data_t, size_y, size_x> & m, data_t k);

  template <int size_x, int size_y, typename data_t>
  static_matrix<data_t, size_y, size_x> operator*(data_t k, const static_matrix<data_t, size_y, size_x> & m);
}

/////////////////////////////////////////////////////////////////////
// Square matrix.
namespace ggo
{
  template <typename data_t, int size>
  class static_square_matrix : public static_matrix<data_t, size, size>
  {
  public:

    bool 	is_diagonally_dominant() const;

    bool  is_symmetric() const;

    template <typename compare_func>
    bool  is_symmetric(compare_func compare) const;
  };
}

/////////////////////////////////////////////////////////////////////
// Square matrix 2D.
namespace ggo
{
  template <typename data_t>
  class square_matrix2d : public static_square_matrix<data_t, 2>
  {
  public:
      
    data_t  det() const;
    
    void    set_rotation(float angle);
  };
}

#include <kernel/math/linear_algebra/ggo_static_matrix.imp.h>

#endif
