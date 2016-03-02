#include "ggo_gaussian_blur.h"
#include <iostream>
#include <ggo_kernel.h>
#include <ggo_array.h>
#include <ggo_data_fetcher.h>

#define GGO_GAUSSIAN_DEBUG 0

/////////////////////////////////////////////////////////////////////
// COMMON FUNCTIONS
namespace
{
  /////////////////////////////////////////////////////////////////////
  template<typename T>
  int setup_gaussian_filter_T(T * filter, int max_filter_size, T variance, T threshold)
  {
    int filter_size = 0;
      
  #if GGO_GAUSSIAN_DEBUG		
    std::cout << "threshold: " << threshold << std::endl;
    std::cout << "max_filter_size: " << max_filter_size << std::endl << "coefs before norm.: ";
  #endif

    // Fill coefs.
    T norm = 0;
    for (int i = 0; i < max_filter_size; ++i)
    {
      filter[i] = std::exp(-i * i / variance);
          
      if (filter[i] < threshold)
      {
  #if GGO_GAUSSIAN_DEBUG		
        std::cout << '(' << filter[i] << ")";
  #endif
        break;
      }
          
  #if GGO_GAUSSIAN_DEBUG		
      std::cout << filter[i] << ", ";
  #endif

      if (i == 0)
      {
        norm += filter[i];
      }
      else
      {
         norm += 2 * filter[i];
      }
          
      ++filter_size;
    }
    
  #if GGO_GAUSSIAN_DEBUG		
    std::cout << std::endl << "filter_size: " << filter_size << std::endl << "coefs after norm.: ";
  #endif
      
    // Normalize.
    for (int i = 0; i < filter_size; ++i)
    {
      filter[i] /= norm;
  #if GGO_GAUSSIAN_DEBUG		
      std::cout << filter[i] << ", ";
  #endif
    }
  #if GGO_GAUSSIAN_DEBUG		
    std::cout << std::endl << "----" << std::endl;
  #endif

    return filter_size;
  }

  /////////////////////////////////////////////////////////////////////
  // Apply 1D gaussian filter.
  template<typename data_type, typename filter_type, data_type(fetch_func)(const data_type *, int, int, int)>
  void apply_filter_1d_T(const data_type * in,
                         data_type * out,
                         int size,
                         int stride_in,
                         int stride_out,
                         const filter_type * filter,
                         int filter_size)
  {
    for (int x = 0; x < size; ++x)
    {
      filter_type v = in[x * stride_in] * filter[0];
      for (int i = 1; i < filter_size; ++i)
      {
        int i1 = x + i;
        int i2 = x - i;
        filter_type tmp1 = ggo::to<filter_type>(data_fetcher.fetch(in, size, stride_in, i1));
        filter_type tmp2 = ggo::to<filter_type>(data_fetcher.fetch(in, size, stride_in, i2));
        v += (tmp1 + tmp2) * filter[i];
      }
      
      *out = ggo::to<data_type>(v);
      
      out += stride_out;
    }
  }
  
  /////////////////////////////////////////////////////////////////////
  // Setup and apply 1D gaussian filter.
  template<typename T_DATA, typename T_FILTER>
  void apply_filter_1d_T(const T_DATA * in,
                         T_DATA * out,
                         int size,
                         T_FILTER variance,
                         const ggo::gaussian_parameters<T_DATA> & params)
  {
    // Setup the filter.
    ggo::array<T_FILTER> filter(params._max_filter_size);
    int filter_size = setup_gaussian_filter_T<T_FILTER>(filter, params._max_filter_size, variance, params._filter_threshold);
      
    // Apply the filter.
    apply_filter_1d_T<T_DATA, T_FILTER>(in, out, size, params._stride_in, params._stride_out, filter, filter_size, params._data_fetcher);
  }
  
  /////////////////////////////////////////////////////////////////////
  // Setup and apply 2D gaussian filter.
  template<typename T_DATA, typename T_FILTER>
  void apply_filter_2d_T(const T_DATA * in,
                         T_DATA * out,
                         int width,
                         int height,
                         T_FILTER variance,
                         const ggo::gaussian_parameters<T_DATA> & params)
  {
    ggo::array<T_DATA> tmp(width * height);
    
    // Setup the filter.
    ggo::array<T_FILTER> filter(params._max_filter_size);
    int filter_size = setup_gaussian_filter_T<T_FILTER>(filter, params._max_filter_size, variance, params._filter_threshold);
      
    if (filter_size == 1 && in == out)
    {
      return;
    }

    // Horizontal filtering.
    const T_DATA * it_in = in;
    T_DATA * it_out = tmp;
    for (int y = 0; y < height; ++y)
    {
      apply_filter_1d_T<T_DATA, T_FILTER>(it_in, it_out, width, params._stride_in, 1, filter, filter_size, params._data_fetcher);
      it_in += width * params._stride_in;
      it_out += width;
    }
    
    // Vertical filtering.
    it_in = tmp;
    it_out = out;
    for (int x = 0; x < width; ++x)
    {
      apply_filter_1d_T<T_DATA, T_FILTER>(it_in, it_out, height, width, params._stride_out * width, filter, filter_size, params._data_fetcher);
      it_in += 1;
      it_out += params._stride_out;
    }
  }
}

/////////////////////////////////////////////////////////////////////
// 1D
namespace ggo
{
  gaussian_parameters<float> p;

  /////////////////////////////////////////////////////////////////////
  void gaussian_blur_1d(const uint8_t * in, uint8_t * out, int size, float variance, const gaussian_parameters<uint8_t> & params)
  {
    apply_filter_1d_T<uint8_t, float>(in, out, size, variance, params);
  }

  /////////////////////////////////////////////////////////////////////
  void gaussian_blur_1d(const uint16_t * in, uint16_t * out, int size, float variance, const gaussian_parameters<uint16_t> & params)
  {
    apply_filter_1d_T<uint16_t, float>(in, out, size, variance, params);
  }

  /////////////////////////////////////////////////////////////////////
  void gaussian_blur_1d(const uint32_t * in, uint32_t * out, int size, float variance, const gaussian_parameters<uint32_t> & params)
  {
    apply_filter_1d_T<uint32_t, float>(in, out, size, variance, params);
  }

  /////////////////////////////////////////////////////////////////////
  void gaussian_blur_1d(const float * in, float * out, int size, float variance, const gaussian_parameters<float> & params)
  {
    apply_filter_1d_T<float, float>(in, out, size, variance, params);
  }

  /////////////////////////////////////////////////////////////////////
  void gaussian_blur_1d(const double * in, double * out, int size, double variance, const gaussian_parameters<double> & params)
  {
    apply_filter_1d_T<double, double>(in, out, size, variance, params);
  }
}

/////////////////////////////////////////////////////////////////////
// 2D
namespace ggo
{
  /////////////////////////////////////////////////////////////////////
  void gaussian_blur_2d(const uint8_t * in, uint8_t * out, int width, int height, float variance, const gaussian_parameters<uint8_t> & params)
  {
    apply_filter_2d_T<uint8_t, float>(in, out, width, height, variance, params);
  }

  /////////////////////////////////////////////////////////////////////
  void gaussian_blur_2d(const uint16_t * in, uint16_t * out, int width, int height, float variance, const gaussian_parameters<uint16_t> & params)
  {
    apply_filter_2d_T<uint16_t, float>(in, out, width, height, variance, params);
  }

  /////////////////////////////////////////////////////////////////////
  void gaussian_blur_2d(const uint32_t * in, uint32_t * out, int width, int height, float variance, const gaussian_parameters<uint32_t> & params)
  {
    apply_filter_2d_T<uint32_t, float>(in, out, width, height, variance, params);
  }

  /////////////////////////////////////////////////////////////////////
  void gaussian_blur_2d(const float * in, float * out, int width, int height, float variance, const gaussian_parameters<float> & params)
  {
    apply_filter_2d_T<float, float>(in, out, width, height, variance, params);
  }

  /////////////////////////////////////////////////////////////////////
  void gaussian_blur_2d(const double * in, double * out, int width, int height, double variance, const gaussian_parameters<double> & params)
  {
    apply_filter_2d_T<double, double>(in, out, width, height, variance, params);
  }
}

