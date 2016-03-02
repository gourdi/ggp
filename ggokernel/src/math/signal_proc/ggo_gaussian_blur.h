#ifndef __GGO_GAUSSIAN_BLUR__
#define __GGO_GAUSSIAN_BLUR__

#include <stdint.h>
#include <memory>
#include <ggo_data_fetcher.h>

namespace ggo
{
  template <typename input_type, input_type(fetch_func)(const input_type *, int, int, int) = fetch_data_duplicated_edge_mirror1d_const<input_type>>
  struct gaussian_parameters
  {
    static input_type fetch(const input_type * data, int size, int x) { return fetch_func(data, size, _stride_in, x) }

    int   _stride_in = 1;
    int   _stride_out = 1;
    int   _max_filter_size = 128;
    float _filter_threshold = 0.01f;
  };
  
  void gaussian_blur_1d(const uint8_t * in, uint8_t * out, int size, float variance, const gaussian_parameters<uint8_t> & params = gaussian_parameters<uint8_t>());
  void gaussian_blur_1d(const uint16_t * in, uint16_t * out, int size, float variance, const gaussian_parameters<uint16_t> & params = gaussian_parameters<uint16_t>());
  void gaussian_blur_1d(const uint32_t * in, uint32_t * out, int size, float variance, const gaussian_parameters<uint32_t> & params = gaussian_parameters<uint32_t>());
  void gaussian_blur_1d(const float * in, float * out, int size, float variance, const gaussian_parameters<float> & params = gaussian_parameters<float>());
  void gaussian_blur_1d(const double * in, double * out, int size, double variance, const gaussian_parameters<double> & params = gaussian_parameters<double>());

  void gaussian_blur_2d(const uint8_t * in, uint8_t * out, int width, int height, float variance, const gaussian_parameters<uint8_t> & params = gaussian_parameters<uint8_t>());
  void gaussian_blur_2d(const uint16_t * in, uint16_t * out, int width, int height, float variance, const gaussian_parameters<uint16_t> & params = gaussian_parameters<uint16_t>());
  void gaussian_blur_2d(const uint32_t * in, uint32_t * out, int width, int height, float variance, const gaussian_parameters<uint32_t> & params = gaussian_parameters<uint32_t>());
  void gaussian_blur_2d(const float * in, float * out, int width, int height, float variance, const gaussian_parameters<float> & params = gaussian_parameters<float>());
  void gaussian_blur_2d(const double * in, double * out, int width, int height, double variance, const gaussian_parameters<double> & params = gaussian_parameters<double>());
}

#endif

