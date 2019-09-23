#include "ggo_dwt.h"
#include <complex>
#include <kernel/ggo_assert.h>
#include <kernel/ggo_kernel.h>

namespace
{
  template <typename T> T D4_0() { return static_cast<T>( 0.6830127 * ggo::inv_sqrt2<T>()); }
  template <typename T> T D4_1() { return static_cast<T>( 1.1830127 * ggo::inv_sqrt2<T>()); }
  template <typename T> T D4_2() { return static_cast<T>( 0.3169873 * ggo::inv_sqrt2<T>()); }
  template <typename T> T D4_3() { return static_cast<T>(-0.1830127 * ggo::inv_sqrt2<T>()); }

  template <typename T> T D6_0() { return static_cast<T>( 0.47046721 * ggo::inv_sqrt2<T>()); }
  template <typename T> T D6_1() { return static_cast<T>( 1.14111692 * ggo::inv_sqrt2<T>()); }
  template <typename T> T D6_2() { return static_cast<T>( 0.650365   * ggo::inv_sqrt2<T>()); }
  template <typename T> T D6_3() { return static_cast<T>(-0.19093442 * ggo::inv_sqrt2<T>()); }
  template <typename T> T D6_4() { return static_cast<T>(-0.12083221 * ggo::inv_sqrt2<T>()); }
  template <typename T> T D6_5() { return static_cast<T>( 0.0498175  * ggo::inv_sqrt2<T>()); }

  template <typename T> T D8_0() { return static_cast<T>( 0.32580343 * ggo::inv_sqrt2<T>()); }
  template <typename T> T D8_1() { return static_cast<T>( 1.01094572 * ggo::inv_sqrt2<T>()); }
  template <typename T> T D8_2() { return static_cast<T>( 0.8922014  * ggo::inv_sqrt2<T>()); }
  template <typename T> T D8_3() { return static_cast<T>(-0.03957503 * ggo::inv_sqrt2<T>()); }
  template <typename T> T D8_4() { return static_cast<T>(-0.26450717 * ggo::inv_sqrt2<T>()); }
  template <typename T> T D8_5() { return static_cast<T>( 0.0436163  * ggo::inv_sqrt2<T>()); }
  template <typename T> T D8_6() { return static_cast<T>( 0.0465036  * ggo::inv_sqrt2<T>()); }
  template <typename T> T D8_7() { return static_cast<T>(-0.01498699 * ggo::inv_sqrt2<T>()); }
  
  //////////////////////////////////////////////////////////////
  template <typename T>
  void fdwt_haar_T(const T * input, T * low, T * high, int size)
  {
    for (int k = 0; k < size / 2; ++k) 
    {
      low[k]  = (input[2 * k] + input[2 * k + 1]) * T(ggo::inv_sqrt2<T>());
      high[k] = (input[2 * k] - input[2 * k + 1]) * T(ggo::inv_sqrt2<T>());
    }
  }

  //////////////////////////////////////////////////////////////
  template <typename T>
  void idwt_haar_T(const T * low, const T * high, T * output, int size)
  {
    for (int k = 0; k < size / 2; ++k) 
    {
      output[2 * k]     = (low[k] + high[k]) * T(ggo::inv_sqrt2<T>());
      output[2 * k + 1] = (low[k] - high[k]) * T(ggo::inv_sqrt2<T>());
    }
  }

  //////////////////////////////////////////////////////////////
  template <typename T>
  void fdwt_d4_T(const T * input, T * low, T * high, int size)
  {
    for (int k = 0; k < size/2-1; ++k) 
    {
      low[k]  = D4_0<T>()*input[2*k] + D4_1<T>()*input[2*k+1] + D4_2<T>()*input[2*k+2] + D4_3<T>()*input[2*k+3];
      high[k] = D4_3<T>()*input[2*k] - D4_2<T>()*input[2*k+1] + D4_1<T>()*input[2*k+2] - D4_0<T>()*input[2*k+3];
    }

    low[size/2-1]  = D4_0<T>()*input[size-2] + D4_1<T>()*input[size-1] + D4_2<T>()*input[0] + D4_3<T>()*input[1];
    high[size/2-1] = D4_3<T>()*input[size-2] - D4_2<T>()*input[size-1] + D4_1<T>()*input[0] - D4_0<T>()*input[1];
  }

  //////////////////////////////////////////////////////////////
  template <typename T>
  void idwt_d4_T(const T * low, const T * high, T * output, int size)
  {
    output[0] = D4_2<T>()*low[size/2-1] + D4_1<T>()*high[size/2-1] + D4_0<T>()*low[0] + D4_3<T>()*high[0];
    output[1] = D4_3<T>()*low[size/2-1] - D4_0<T>()*high[size/2-1] + D4_1<T>()*low[0] - D4_2<T>()*high[0];

    for (int k = 1; k < size/2; ++k) 
    {
      output[2*k]   = D4_2<T>()*low[k-1] + D4_1<T>()*high[k-1] + D4_0<T>()*low[k] + D4_3<T>()*high[k];
      output[2*k+1] = D4_3<T>()*low[k-1] - D4_0<T>()*high[k-1] + D4_1<T>()*low[k] - D4_2<T>()*high[k];
    }
  }

  //////////////////////////////////////////////////////////////
  template <typename T>
  void fdwt_d6_T(const T * input, T * low, T * high, int size)
  {
    for (int k = 0; k < size/2-2; ++k) 
    {
      low[k]  = D6_0<T>()*input[2*k]   + D6_1<T>()*input[2*k+1] +
                D6_2<T>()*input[2*k+2] + D6_3<T>()*input[2*k+3] +
                D6_4<T>()*input[2*k+4] + D6_5<T>()*input[2*k+5];
      high[k] = D6_5<T>()*input[2*k]   - D6_4<T>()*input[2*k+1] + 
                D6_3<T>()*input[2*k+2] - D6_2<T>()*input[2*k+3] +
                D6_1<T>()*input[2*k+4] - D6_0<T>()*input[2*k+5];
    }

    low[size/2-2]  = D6_0<T>()*input[size-4] + D6_1<T>()*input[size-3] + 
                     D6_2<T>()*input[size-2] + D6_3<T>()*input[size-1] +
                     D6_4<T>()*input[0]      + D6_5<T>()*input[1];
    high[size/2-2] = D6_5<T>()*input[size-4] - D6_4<T>()*input[size-3] + 
                     D6_3<T>()*input[size-2] - D6_2<T>()*input[size-1] + 
                     D6_1<T>()*input[0]      - D6_0<T>()*input[1];

    low[size/2-1]  = D6_0<T>()*input[size-2] + D6_1<T>()*input[size-1] + 
                     D6_2<T>()*input[0]      + D6_3<T>()*input[1] +
                     D6_4<T>()*input[2]      + D6_5<T>()*input[3];
    high[size/2-1] = D6_5<T>()*input[size-2] - D6_4<T>()*input[size-1] + 
                     D6_3<T>()*input[0]      - D6_2<T>()*input[1] + 
                     D6_1<T>()*input[2]      - D6_0<T>()*input[3];
  }

  //////////////////////////////////////////////////////////////
  template <typename T>
  void idwt_d6_T(const T * low, const T * high, T * output, int size)
  {
    output[0] = D6_4<T>()*low[size/2-2] + D6_1<T>()*high[size/2-2] + 
                D6_2<T>()*low[size/2-1] + D6_3<T>()*high[size/2-1] + 
                D6_0<T>()*low[0]        + D6_5<T>()*high[0];
    output[1] = D6_5<T>()*low[size/2-2] - D6_0<T>()*high[size/2-2] +
                D6_3<T>()*low[size/2-1] - D6_2<T>()*high[size/2-1] + 
                D6_1<T>()*low[0]        - D6_4<T>()*high[0];

    output[2] = D6_4<T>()*low[size/2-1] + D6_1<T>()*high[size/2-1] + 
                D6_2<T>()*low[0]        + D6_3<T>()*high[0] + 
                D6_0<T>()*low[1]        + D6_5<T>()*high[1];
    output[3] = D6_5<T>()*low[size/2-1] - D6_0<T>()*high[size/2-1] +
                D6_3<T>()*low[0]        - D6_2<T>()*high[0] + 
                D6_1<T>()*low[1]        - D6_4<T>()*high[1];

    for (int k = 2; k < size/2; ++k) 
    {
      output[2*k]   = D6_4<T>()*low[k-2] + D6_1<T>()*high[k-2] + 
                      D6_2<T>()*low[k-1] + D6_3<T>()*high[k-1] + 
                      D6_0<T>()*low[k]   + D6_5<T>()*high[k];
      output[2*k+1] = D6_5<T>()*low[k-2] - D6_0<T>()*high[k-2] + 
                      D6_3<T>()*low[k-1] - D6_2<T>()*high[k-1] + 
                      D6_1<T>()*low[k]   - D6_4<T>()*high[k];
    }
  }

  //////////////////////////////////////////////////////////////
  template <typename T>
  void fdwt_d8_T(const T * input, T * low, T * high, int size)
  {
    for (int k = 0; k < size/2-3; ++k) 
    {
      low[k]  = D8_0<T>()*input[2*k]   + D8_1<T>()*input[2*k+1] +
                D8_2<T>()*input[2*k+2] + D8_3<T>()*input[2*k+3] +
                D8_4<T>()*input[2*k+4] + D8_5<T>()*input[2*k+5] +
                D8_6<T>()*input[2*k+6] + D8_7<T>()*input[2*k+7];
      high[k] = D8_7<T>()*input[2*k]   - D8_6<T>()*input[2*k+1] + 
                D8_5<T>()*input[2*k+2] - D8_4<T>()*input[2*k+3] +
                D8_3<T>()*input[2*k+4] - D8_2<T>()*input[2*k+5] +
                D8_1<T>()*input[2*k+6] - D8_0<T>()*input[2*k+7];
    }

    low[size/2-3]  = D8_0<T>()*input[size-6] + D8_1<T>()*input[size-5] + 
                     D8_2<T>()*input[size-4] + D8_3<T>()*input[size-3] +
                     D8_4<T>()*input[size-2] + D8_5<T>()*input[size-1] +
                     D8_6<T>()*input[0]      + D8_7<T>()*input[1];
    high[size/2-3] = D8_7<T>()*input[size-6] - D8_6<T>()*input[size-5] + 
                     D8_5<T>()*input[size-4] - D8_4<T>()*input[size-3] + 
                     D8_3<T>()*input[size-2] - D8_2<T>()*input[size-1] + 
                     D8_1<T>()*input[0]      - D8_0<T>()*input[1];

    low[size/2-2]  = D8_0<T>()*input[size-4] + D8_1<T>()*input[size-3] + 
                     D8_2<T>()*input[size-2] + D8_3<T>()*input[size-1] +
                     D8_4<T>()*input[0]      + D8_5<T>()*input[1] +
                     D8_6<T>()*input[2]      + D8_7<T>()*input[3];
    high[size/2-2] = D8_7<T>()*input[size-4] - D8_6<T>()*input[size-3] + 
                     D8_5<T>()*input[size-2] - D8_4<T>()*input[size-1] + 
                     D8_3<T>()*input[0]      - D8_2<T>()*input[1] + 
                     D8_1<T>()*input[2]      - D8_0<T>()*input[3];

    low[size/2-1]  = D8_0<T>()*input[size-2] + D8_1<T>()*input[size-1] + 
                     D8_2<T>()*input[0]      + D8_3<T>()*input[1] +
                     D8_4<T>()*input[2]      + D8_5<T>()*input[3] +
                     D8_6<T>()*input[4]      + D8_7<T>()*input[5];
    high[size/2-1] = D8_7<T>()*input[size-2] - D8_6<T>()*input[size-1] + 
                     D8_5<T>()*input[0]      - D8_4<T>()*input[1] + 
                     D8_3<T>()*input[2]      - D8_2<T>()*input[3] + 
                     D8_1<T>()*input[4]      - D8_0<T>()*input[5];
  }

  //////////////////////////////////////////////////////////////
  template <typename T>
  void idwt_d8_T(const T * low, const T * high, T * output, int size)
  {
    output[0] = D8_6<T>()*low[size/2-3] + D8_1<T>()*high[size/2-3] + 
                D8_4<T>()*low[size/2-2] + D8_3<T>()*high[size/2-2] + 
                D8_2<T>()*low[size/2-1] + D8_5<T>()*high[size/2-1] + 
                D8_0<T>()*low[0]        + D8_7<T>()*high[0];
    output[1] = D8_7<T>()*low[size/2-3] - D8_0<T>()*high[size/2-3] +
                D8_5<T>()*low[size/2-2] - D8_2<T>()*high[size/2-2] +
                D8_3<T>()*low[size/2-1] - D8_4<T>()*high[size/2-1] + 
                D8_1<T>()*low[0]        - D8_6<T>()*high[0];

    output[2] = D8_6<T>()*low[size/2-2] + D8_1<T>()*high[size/2-2] +
                D8_4<T>()*low[size/2-1] + D8_3<T>()*high[size/2-1] +  
                D8_2<T>()*low[0]        + D8_5<T>()*high[0] + 
                D8_0<T>()*low[1]        + D8_7<T>()*high[1];
    output[3] = D8_7<T>()*low[size/2-2] - D8_0<T>()*high[size/2-2] +
                D8_5<T>()*low[size/2-1] - D8_2<T>()*high[size/2-1] +
                D8_3<T>()*low[0]        - D8_4<T>()*high[0] + 
                D8_1<T>()*low[1]        - D8_6<T>()*high[1];

    output[4] = D8_6<T>()*low[size/2-1] + D8_1<T>()*high[size/2-1] +
                D8_4<T>()*low[0]        + D8_3<T>()*high[0] +  
                D8_2<T>()*low[1]        + D8_5<T>()*high[1] + 
                D8_0<T>()*low[2]        + D8_7<T>()*high[2];
    output[5] = D8_7<T>()*low[size/2-1] - D8_0<T>()*high[size/2-1] +
                D8_5<T>()*low[0]        - D8_2<T>()*high[0] +
                D8_3<T>()*low[1]        - D8_4<T>()*high[1] + 
                D8_1<T>()*low[2]        - D8_6<T>()*high[2];

    for (int k = 3; k < size/2; ++k) 
    {
      output[2*k]   = D8_6<T>()*low[k-3] + D8_1<T>()*high[k-3] + 
                      D8_4<T>()*low[k-2] + D8_3<T>()*high[k-2] + 
                      D8_2<T>()*low[k-1] + D8_5<T>()*high[k-1] + 
                      D8_0<T>()*low[k]   + D8_7<T>()*high[k];
      output[2*k+1] = D8_7<T>()*low[k-3] - D8_0<T>()*high[k-3] +
                      D8_5<T>()*low[k-2] - D8_2<T>()*high[k-2] +  
                      D8_3<T>()*low[k-1] - D8_4<T>()*high[k-1] + 
                      D8_1<T>()*low[k]   - D8_6<T>()*high[k];
    }
  }

  //////////////////////////////////////////////////////////////
  template <typename T>
  void fdwt_T(const T * input, T * low, T * high, int size, ggo::dwt_algo algo)
  {
    GGO_ASSERT((size&1) == 0);

    switch (algo)
    {
    case ggo::DWT_HAAR:
      fdwt_haar_T(input, low, high, size);
      break;
    case ggo::DWT_D4:
      fdwt_d4_T(input, low, high, size);
      break;
    case ggo::DWT_D6:
      fdwt_d6_T(input, low, high, size);
      break;
    case ggo::DWT_D8:
      fdwt_d8_T(input, low, high, size);
      break;
    }
  }

  //////////////////////////////////////////////////////////////
  template <typename T>
  void idwt_T(const T * low, const T * high, T * output, int size, ggo::dwt_algo algo)
  {
    GGO_ASSERT((size&1) == 0);

    switch (algo)
    {
    case ggo::DWT_HAAR:
      idwt_haar_T(low, high, output, size);
      break;
    case ggo::DWT_D4:
      idwt_d4_T(low, high, output, size);
      break;
    case ggo::DWT_D6:
      idwt_d6_T(low, high, output, size);
      break;
    case ggo::DWT_D8:
      idwt_d8_T(low, high, output, size);
      break;
    }
  }
}

namespace ggo
{
  //////////////////////////////////////////////////////////////
  void fdwt(const double * input, double * low, double * high, int size, dwt_algo algo)
  {
    fdwt_T<double>(input, low, high, size, algo);
  }

  //////////////////////////////////////////////////////////////
  void idwt(const double * low, const double * high, double * output, int size, dwt_algo algo)
  {
    idwt_T<double>(low, high, output, size, algo);
  }

  //////////////////////////////////////////////////////////////
  void fdwt(const float * input, float * low, float * high, int size, dwt_algo algo)
  {
    fdwt_T<float>(input, low, high, size, algo);
  }

  //////////////////////////////////////////////////////////////
  void idwt(const float * low, const float * high, float * output, int size, dwt_algo algo)
  {
    idwt_T<float>(low, high, output, size, algo);
  }
}

