#ifndef GGO_MATRIX_H_
#define GGO_MATRIX_H_

#include <array>
#include <functional>
#include <ggo_vec.h>

//////////////////////////////////////////////////////////////
// Matrices.
namespace ggo
{
  template <typename data_t, int size_y, int size_x>
  class matrix
  {
  public:

    template <int y, int x>
    data_t &  get()
    {
      static_assert(x >= 0 && x < size_x, "invalid x index");
      static_assert(y >= 0 && x < size_y, "invalid y index");
      return _data[y * size_x + x];
    }

    template <int y, int x>
    const data_t &  get() const
    {
      static_assert(x >= 0 && x < size_x, "invalid x index");
      static_assert(y >= 0 && x < size_y, "invalid y index");
      return _data[y * size_x + x];
    }

    data_t &			  operator()(unsigned y, unsigned x) { return _data[y * size_x + x]; }
    const data_t &  operator()(unsigned y, unsigned x) const { return _data[y * size_x + x]; }
    
    matrix &        operator=(const matrix<data_t, size_y, size_x> & m);

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
  matrix<data_t, size_y, size_x> operator+(const matrix<data_t, size_y, size_x> & m1, const matrix<data_t, size_y, size_x> & m2);

  template <typename data_t, int size_y, int size_x>
  matrix<data_t, size_y, size_x> operator-(const matrix<data_t, size_y, size_x> & m1, const matrix<data_t, size_y, size_x> & m2);

  template <typename data_t, int size_1, int size_2, int size_3>
  matrix<data_t, size_1, size_3> operator*(const matrix<data_t, size_1, size_2> & m1, const matrix<data_t, size_2, size_3> & m2);
}

/////////////////////////////////////////////////////////////////////
// Scalar operators.
namespace ggo
{
  template <int size_x, int size_y, typename data_t>
  matrix<data_t, size_y, size_x> operator*(const matrix<data_t, size_y, size_x> & m, data_t k);

  template <int size_x, int size_y, typename data_t>
  matrix<data_t, size_y, size_x> operator*(data_t k, const matrix<data_t, size_y, size_x> & m);
}

/////////////////////////////////////////////////////////////////////
// Square matrix.
namespace ggo
{
  template <typename data_t, int size>
  class square_matrix : public matrix<data_t, size, size>
  {
  public:

    bool 	is_diagonally_dominant() const;

    bool  is_symmetric() const;

    template <typename compare_func>
    bool  is_symmetric(compare_func compare) const;
  };
}

#if 0

/////////////////////////////////////////////////////////////////////
// Square matrix 2D.
namespace ggo
{
  template <typename data_t>
  class square_matrix2d : public square_matrix<data_t, 2>
  {
  public:
      
    data_t  det() const;
    
    void    set_rotation(float angle);
  };	

  template <typename T>
  ggo::vec2<T> operator*(const square_matrix2d<T> & m, const ggo::vec2<T> & v);
}

#endif

#include <ggo_matrix.imp.h>

#endif
