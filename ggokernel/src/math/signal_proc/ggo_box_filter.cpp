#include "ggo_box_filter.h"

namespace
{
  /////////////////////////////////////////////////////////////////////
  uint8_t average3_uint8(uint8_t v1, uint8_t v2, uint8_t v3)
  {
    return (v1 + v2 + v3 + 1) / 3;
  }

  /////////////////////////////////////////////////////////////////////
  float average3_float(float v1, float v2, float v3)
  {
    return (v1 + v2 + v3) / 3;
  }

  /////////////////////////////////////////////////////////////////////
  double average3_double(double v1, double v2, double v3)
  {
    return (v1 + v2 + v3) / 3;
  }
  
  /////////////////////////////////////////////////////////////////////
  uint8_t average5_uint8(uint8_t v1, uint8_t v2, uint8_t v3, uint8_t v4, uint8_t v5)
  {
    return (v1 + v2 + v3 + v4 + v4 + 2) / 5;
  }

  /////////////////////////////////////////////////////////////////////
  float average5_float(float v1, float v2, float v3, float v4, float v5)
  {
    return (v1 + v2 + v3 + v4 + v5) / 5;
  }

  /////////////////////////////////////////////////////////////////////
  double average5_double(double v1, double v2, double v3, double v4, double v5)
  {
    return (v1 + v2 + v3 + v4 + v5) / 5;
  }
    
  //////////////////////////////////////////////////////////////
  template <typename T, T average(T, T, T)>
  void box_filter3_1d_T(T * data, int size, int stride)
  {
    T prv = data[0];
    data[0] = (2 * data[stride] + data[0]) / 3;
    data += stride;
    
    for (int i = 1; i < size - 1; ++i)
    {
      T result = average(prv, data[0], data[stride]);
      prv = data[0];
      data[0] = result;
      data += stride;
    }
    
    data[0] = (2 * prv + data[0]) / 3;
  }
  
  //////////////////////////////////////////////////////////////
  template <typename T, T average(T, T, T)>
  void box_filter3_2d_T(T * data, int size_x, int size_y, int stride)
  {
    // First pass to perform 1D horizontal box filtering.
    for (int y = 0; y < size_y; ++y)
    {
      box_filter3_1d_T<T, average>(data + y * size_x * stride, size_x, stride);
    }
    
    // Second pass to perform 1D vertical box filtering.
    for (int x = 0; x < size_x; ++x)
    {
      box_filter3_1d_T<T, average>(data + x * stride, size_y, size_x * stride);
    }
  }
  
  //////////////////////////////////////////////////////////////
  template <typename T, T average(T, T, T, T, T)>
  void box_filter5_1d_T(T * data, int size, int stride)
  {
    T prv2 = data[0];
    T prv = data[stride];
    
    data[0] = average(data[0], data[stride], data[stride], data[2 * stride], data[2 * stride]);
    data += stride;
    
    data[0] = average(data[0], prv2, data[stride], data[0], data[2 * stride]);
    data += stride;

    for (int i = 2; i < size - 2; ++i)
    {
      T result = average(prv2, prv, data[0], data[stride], data[2 * stride]);
      prv2 = prv;
      prv = data[0];
      data[0] = result;
      data += stride;
    }
    
    T result = average(prv2, prv, data[0], data[stride], data[0]);
    prv2 = prv;
    prv = data[0];
    data[0] = result;
    data += stride;
    
    data[0] = average(data[0], prv, prv, prv2, prv2);
  }
  
  //////////////////////////////////////////////////////////////
  template <typename T, T average(T, T, T, T, T)>
  void box_filter5_2d_T(T * data, int size_x, int size_y, int stride)
  {
    // First pass to perform 1D horizontal box filtering.
    for (int y = 0; y < size_y; ++y)
    {
      box_filter5_1d_T<T, average>(data + y * size_x * stride, size_x, stride);
    }
    
    // Second pass to perform 1D vertical box filtering.
    for (int x = 0; x < size_x; ++x)
    {
      box_filter5_1d_T<T, average>(data + x * stride, size_y, size_x * stride);
    }
  }
}

namespace ggo
{
  //////////////////////////////////////////////////////////////
  void box_filter3_1d(uint8_t * data, int size, int stride)
  {
    box_filter3_1d_T<uint8_t, average3_uint8>(data, size, stride);
  }
  
  //////////////////////////////////////////////////////////////
  void box_filter3_1d(float * data, int size, int stride)
  {
    box_filter3_1d_T<float, average3_float>(data, size, stride);
  }
  
  //////////////////////////////////////////////////////////////
  void box_filter3_1d(double * data, int size, int stride)
  {
    box_filter3_1d_T<double, average3_double>(data, size, stride);
  }
  
    //////////////////////////////////////////////////////////////
  void box_filter3_2d(uint8_t * data, int size_x, int size_y, int stride)
  {
    box_filter3_2d_T<uint8_t, average3_uint8>(data, size_x, size_y, stride);
  }

  //////////////////////////////////////////////////////////////
  void box_filter3_2d(float * data, int size_x, int size_y, int stride)
  {
    box_filter3_2d_T<float, average3_float>(data, size_x, size_y, stride);
  }
  
  //////////////////////////////////////////////////////////////
  void box_filter3_2d(double * data, int size_x, int size_y, int stride)
  {
    box_filter3_2d_T<double, average3_double>(data, size_x, size_y, stride);
  }
  
  //////////////////////////////////////////////////////////////
  void box_filter5_1d(uint8_t * data, int size, int stride)
  {
    box_filter5_1d_T<uint8_t, average5_uint8>(data, size, stride);
  }
  
  //////////////////////////////////////////////////////////////
  void box_filter5_1d(float * data, int size, int stride)
  {
    box_filter5_1d_T<float, average5_float>(data, size, stride);
  }
  
  //////////////////////////////////////////////////////////////
  void box_filter5_1d(double * data, int size, int stride)
  {
    box_filter5_1d_T<double, average5_double>(data, size, stride);
  }
  
  //////////////////////////////////////////////////////////////
  void box_filter5_2d(uint8_t * data, int size_x, int size_y, int stride)
  {
    box_filter5_2d_T<uint8_t, average5_uint8>(data, size_x, size_y, stride);
  }
  
  //////////////////////////////////////////////////////////////
  void box_filter5_2d(float * data, int size_x, int size_y, int stride)
  {
    box_filter5_2d_T<float, average5_float>(data, size_x, size_y, stride);
  }
  
  //////////////////////////////////////////////////////////////
  void box_filter5_2d(double * data, int size_x, int size_y, int stride)
  {
    box_filter5_2d_T<double, average5_double>(data, size_x, size_y, stride);
  }
}