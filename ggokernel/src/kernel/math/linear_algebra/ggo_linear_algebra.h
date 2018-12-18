#ifndef __GGO_LINEAR_ALGEBRA__
#define __GGO_LINEAR_ALGEBRA__

#include <math.h>
#include <kernel/ggo_kernel.h>
#include <kernel/memory/ggo_array.h>

/////////////////////////////////////////////////////////////////////
// Square matrices
namespace ggo
{
  template <typename input, typename compare_func>
  constexpr bool is_symmetric(const input & m, int size, compare_func compare)
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

  template <typename input>
  constexpr bool is_symmetric(const input & m, int size)
  {
    return is_symmetric(m, size, [](auto v1, auto v2) { return v1 == v2; });
  }
}

#endif


