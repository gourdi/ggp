#ifndef GGO_MATRIX_H_
#define GGO_MATRIX_H_

#include <functional>
//#include <kernel/ggo_vec.h>
//#include <kernel/ggo_details.h>

//////////////////////////////////////////////////////////////
// Matrices. Dimensions are known at compile - time.

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


    //void			      operator*=(data_t k);

    constexpr bool  operator==(const matrix<data_t, size_y, size_x> & m) const { return details::eq<data_t, size_y * size_x>(this->_data, m._data); }
    constexpr bool  operator!=(const matrix<data_t, size_y, size_x> & m) const { return !this->operator==(m); }

    //data_t *        data() { return _data.data(); };
    //const data_t *  data() const { return _data.data(); };

  protected:
    
    data_t _data[size_x * size_y];
  };
}

/////////////////////////////////////////////////////////////////////
// Matrix operators.
namespace ggo
{
  //template <typename data_t, int size_y, int size_x>
  //matrix<data_t, size_y, size_x> operator+(const matrix<data_t, size_y, size_x> & m1, const matrix<data_t, size_y, size_x> & m2);

  //template <typename data_t, int size_y, int size_x>
  //matrix<data_t, size_y, size_x> operator-(const matrix<data_t, size_y, size_x> & m1, const matrix<data_t, size_y, size_x> & m2);

#ifndef _MSC_VER
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
#endif
}

///////////////////////////////////////////////////////////////////////
//// Scalar operators.
//namespace ggo
//{
//  template <int size_x, int size_y, typename data_t>
//  matrix<data_t, size_y, size_x> operator*(const matrix<data_t, size_y, size_x> & m, data_t k);
//
//  template <int size_x, int size_y, typename data_t>
//  matrix<data_t, size_y, size_x> operator*(data_t k, const matrix<data_t, size_y, size_x> & m);
//}
//
///////////////////////////////////////////////////////////////////////
//// Square matrix.
//namespace ggo
//{
//  template <typename data_t, int size>
//  class square_matrix : public matrix<data_t, size, size>
//  {
//  public:
//
//    bool 	is_diagonally_dominant() const;
//
//    bool  is_symmetric() const;
//
//    template <typename compare_func>
//    bool  is_symmetric(compare_func compare) const;
//  };
//}
//
///////////////////////////////////////////////////////////////////////
//// Square matrix 2D.
//namespace ggo
//{
//  template <typename data_t>
//  class square_matrix2d : public square_matrix<data_t, 2>
//  {
//  public:
//      
//    data_t  det() const;
//    
//    void    set_rotation(float angle);
//  };
//}
//
//#include <kernel/math/linear_algebra/ggo_matrix.imp.h>

#endif
