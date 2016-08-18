#include "ggo_dct.h"
#include <cmath>
#include <ggo_kernel.h>

namespace
{
  //////////////////////////////////////////////////////////////
  template<typename T>
  void dct_T(const T * src, T * dst, int size)
  {
    T norm = std::pow(T(2.0 / size), T(0.5));

    for (int k = 0; k < size; ++k) 
    {
      dst[k] = 0;

      for (int n = 0; n < size; ++n) 
      {
        dst[k] += src[n] * std::cos(ggo::pi<T>() * (n + T(0.5)) * (k + T(0.5)) / size);
      }

      dst[k] *= norm;
    }
  }
}

namespace ggo
{
  //////////////////////////////////////////////////////////////
  void dct(const double* src, double* dst, int size)
  {
    dct_T<double>(src, dst, size);
  }

  //////////////////////////////////////////////////////////////
  void dct(const float* src, float* dst, int size)
  {
    dct_T<float>(src, dst, size);
  }
}