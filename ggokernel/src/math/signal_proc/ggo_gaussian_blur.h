#ifndef __GGO_GAUSSIAN_BLUR__
#define __GGO_GAUSSIAN_BLUR__

#include <stdint.h>
#include <vector>
#include <ggo_array.h>
#include <ggo_data_fetcher.h>
#include <ggo_convolution1d.h>

/////////////////////////////////////////////////////////////////////
// Definitions.
namespace ggo
{
  template <typename input_type,
            typename filter_type = input_type,
            typename data_type = input_type,
            typename output_type = input_type,
            input_type(fetch_func)(const input_type *, int, int, int) = ggo::fetch_data_duplicated_edge_mirror1d_const<input_type>>
  void gaussian_blur_1d(const input_type * in,
                        output_type * out,
                        int size,
                        filter_type stddev,
                        int stride_in = 1, int stride_out = 1, filter_type filter_threshold = filter_type(0.01));

  template <typename input_type,
            typename filter_type = input_type,
            typename data_type = input_type,
            typename output_type = input_type,
            input_type(fetch_func_input)(const input_type *, int, int, int) = ggo::fetch_data_duplicated_edge_mirror1d_const<input_type>,
            data_type(fetch_func_data)(const data_type *, int, int, int) = ggo::fetch_data_duplicated_edge_mirror1d_const<data_type>>
  void gaussian_blur_2d(const input_type * in,
                        output_type * out,
                        int width,
                        int height,
                        filter_type stddev,
                        int stride_in = 1, int stride_out = 1, filter_type filter_threshold = filter_type(0.01));
}

/////////////////////////////////////////////////////////////////////
// Some usefull shortcuts.
namespace ggo
{
  void gaussian_blur_1d_uint8(const uint8_t * in, uint8_t * out, int size, float stddev, int stride_in = 1, int stride_out = 1, float filter_threshold = 0.01f)
  {
    gaussian_blur_1d<uint8_t, float, float, uint8_t>(in, out, size, stddev, stride_in, stride_out, filter_threshold);
  }

  void gaussian_blur_2d_uint8(const uint8_t * in, uint8_t * out, int width, int height, float stddev, int stride_in = 1, int stride_out = 1, float filter_threshold = 0.01f)
  {
    gaussian_blur_2d<uint8_t, float, float, uint8_t>(in, out, width, height, stddev, stride_in, stride_out, filter_threshold);
  }
}

/////////////////////////////////////////////////////////////////////
// Implementation.
namespace ggo
{
  template <typename filter_type>
  std::vector<filter_type> build_gaussian_filter(filter_type stddev, filter_type filter_threshold)
  {
    std::vector<filter_type> filter;

    // Fill filter and compute norm at the same time.
    bool first = true;
    filter_type i = 0;
    filter_type norm = 0;
    filter_type variance2 = 2 * stddev * stddev;
    while (true)
    {
      filter_type coef = std::exp(-i * i / variance2);
      if (coef < filter_threshold)
      {
        break;
      }
      filter.push_back(coef);

      if (first == true)
      {
        norm = coef;
        first = false;
      }
      else
      {
        norm += 2 * coef;
      }

      ++i;
    }

    // Normalize.
    for (auto & coef : filter)
    {
      coef /= norm;
#ifdef GGO_GAUSSIAN_DEBUG		
      std::cout << coef << ", ";
#endif
    }
#ifdef GGO_GAUSSIAN_DEBUG		
    std::cout << std::endl;
#endif

    return filter;
  }

  /////////////////////////////////////////////////////////////////////
  template <typename input_type,
            typename filter_type,
            typename data_type,
            typename output_type,
            input_type(fetch_func)(const input_type *, int, int, int)>
  void gaussian_blur_1d(const input_type * in,
                        output_type * out,
                        int size,
                        filter_type stddev,
                        int stride_in, int stride_out, filter_type filter_threshold)
  {
    // Setup the filter.
    auto filter = build_gaussian_filter<filter_type>(stddev, filter_threshold);

    // Apply the filter.
    apply_symetric_filter_1d
      <input_type, filter_type, data_type, output_type, fetch_func>
      (in, out, size, stride_in, stride_out, &filter[0], static_cast<int>(filter.size()));
  }

  /////////////////////////////////////////////////////////////////////
  template <typename input_type,
            typename filter_type,
            typename data_type,
            typename output_type,
            input_type(fetch_func_input)(const input_type *, int, int, int),
            data_type(fetch_func_data)(const data_type *, int, int, int)>
  void gaussian_blur_2d(const input_type * in,
                        output_type * out,
                        int width,
                        int height,
                        filter_type stddev,
                        int stride_in, int stride_out, filter_type filter_threshold)
  {
    // Setup the filter.
    auto filter = build_gaussian_filter<filter_type>(stddev, filter_threshold);

    if (filter.size() == 1 && in == out)
    {
      return;
    }

    ggo::array<data_type> tmp(width * height);

    // Horizontal filtering.
    {
      const input_type * it_in = in;
      data_type * it_out = tmp;
      for (int y = 0; y < height; ++y)
      {
        apply_symetric_filter_1d
          <input_type, filter_type, data_type, data_type, fetch_func_input>
          (it_in, it_out, width, stride_in, 1, &filter[0], static_cast<int>(filter.size()));
        it_in += width * stride_in;
        it_out += width;
      }
    }

    // Vertical filtering.
    {
      const data_type * it_in = tmp;
      output_type * it_out = out;
      for (int x = 0; x < width; ++x)
      {
        apply_symetric_filter_1d
          <data_type, filter_type, data_type, output_type, fetch_func_data>
          (it_in, it_out, height, width, stride_out * width, &filter[0], static_cast<int>(filter.size()));
        it_in += 1;
        it_out += stride_out;
      }
    }
  }
}

#endif

