#ifndef __GGO_LINEAR_ALGEBRA__
#define __GGO_LINEAR_ALGEBRA__

#include <math.h>
#include <functional>

/////////////////////////////////////////////////////////////////////
// Square matrices
namespace ggo
{
  template <typename MATRIX_TYPE, typename DATA_TYPE>
  bool is_matrix_symmetric(const MATRIX_TYPE & m, int size, std::function<bool(DATA_TYPE, DATA_TYPE)> compare_func)
  {
    for (int y = 0; y < size; ++y)
    {
      for (int x = y + 1; x < size; ++x)
      {
        DATA_TYPE k1 = m(x, y);
        DATA_TYPE k2 = m(y, x);
        if (compare_func(k1, k2) == false)
        {
          return false;
        }
      }
    }
    
    return true;
  }
  
  template <typename MATRIX_TYPE, typename DATA_TYPE>
  bool is_matrix_diagonally_dominant(const MATRIX_TYPE & m, int size)
  {
    for (int y = 0; y < size; ++y)
    {
      DATA_TYPE v2 = 0;
      for (int x = 0; x < size; ++x)
      {
        if (x != y)
        {
          DATA_TYPE v3 = m(y, x);
          if (v3 > 0)
          {
            v2 += v3;
          }
          else
          {
            v2 -= v3;
          }
        }
      }
      
      DATA_TYPE v1 = m(y, y);
      if (v1 < DATA_TYPE(0)) 
      {
        v1 = -v1;
      }
      
      if (v2 > v1)
      {
        return false;
      }
    }
    
    return true;
  }
}

#endif
