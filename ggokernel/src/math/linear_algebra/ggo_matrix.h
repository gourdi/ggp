#ifndef GGO_MATRIX_H_
#define GGO_MATRIX_H_

#include <array>
#include <functional>
#include <ggo_set2.h>

//////////////////////////////////////////////////////////////
// Matrices.
namespace ggo
{
  template <int size_y, int size_x, typename T>
  class matrix
  {
  public:

    T &				operator()(unsigned y, unsigned x) { return _data[y * size_x + x]; }
    const T & operator()(unsigned y, unsigned x) const { return _data[y * size_x + x]; }
    
    matrix &  operator=(const matrix<size_y, size_x, T> & m);

    void			operator+=(const matrix & m);
    void			operator-=(const matrix & m);

    template <typename T2>
    void			operator*=(T2 k);

  protected:
    
    std::array<T, size_x * size_y> _data;
  };
}

/////////////////////////////////////////////////////////////////////
// Matrix operators.
namespace ggo
{
  template <int size_y, int size_x, typename T>
  matrix<size_y, size_x, T> operator+(const matrix<size_y, size_x, T> & m1, const matrix<size_y, size_x, T> & m2);

  template <int size_y, int size_x, typename T>
  matrix<size_y, size_x, T> operator-(const matrix<size_y, size_x, T> & m1, const matrix<size_y, size_x, T> & m2);

  template <int size_1, int size_2, int size_3, typename T>
  matrix<size_1, size_3, T> operator*(const matrix<size_1, size_2, T> & m1, const matrix<size_2, size_3, T> & m2);
}

/////////////////////////////////////////////////////////////////////
// Scalar operators.
namespace ggo
{
  template <int size_x, int size_y, typename T1, typename T2>
  matrix<size_y, size_x, T1> operator*(const matrix<size_y, size_x, T1> & m, T2 k);

  template <int size_x, int size_y, typename T1, typename T2>
  matrix<size_y, size_x, T1> operator*(T2 k, const matrix<size_y, size_x, T1> & m);
}

/////////////////////////////////////////////////////////////////////
// Square matrix.
namespace ggo
{
  template <int size, typename T>
  class square_matrix : public matrix<size, size, T>
  {
  public:

    bool 	is_diagonally_dominant() const;
    bool  is_symmetric(std::function<bool(T, T)> compare_func) const;
  };
}

/////////////////////////////////////////////////////////////////////
// Square matrix 2D.
namespace ggo
{
  template <typename T>
  class square_matrix2d : public square_matrix<2, T>
  {
  public:
      
    T     det() const;
    
    void  set_rotation(float angle);
  };	

  template <typename T>
  ggo::set2<T> operator*(const square_matrix2d<T> & m, ggo::set2<T> v);
}

#include <ggo_matrix.imp.h>

#endif
