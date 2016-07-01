#ifndef __GGO_LINEAR_ALGEBRA__
#define __GGO_LINEAR_ALGEBRA__

#include <math.h>
#include <functional>

/////////////////////////////////////////////////////////////////////
// Square matrices
namespace ggo
{
  template <typename input, typename compare_func>
  bool is_matrix_symmetric(const input & m, int size, compare_func compare)
  {
    for (int y = 0; y < size; ++y)
    {
      for (int x = y + 1; x < size; ++x)
      {
        auto v1 = m(x, y);
        auto v2 = m(y, x);
        if (compare(v1, v2) == false)
        {
          return false;
        }
      }
    }
    
    return true;
  }
  
  template <typename data_t, typename input>
  bool is_matrix_diagonally_dominant(const input & m, int size)
  {
    for (int y = 0; y < size; ++y)
    {
      data_t v2 = 0;
      for (int x = 0; x < size; ++x)
      {
        if (x != y)
        {
          v2 += std::abs(m(y, x));
        }
      }
      
      data_t v1 = std::abs(m(y, y));
      
      if (v2 > v1)
      {
        return false;
      }
    }
    
    return true;
  }
}

#endif


