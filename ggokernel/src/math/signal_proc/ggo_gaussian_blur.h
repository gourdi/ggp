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
            typename filter_type,
            typename data_type,
            typename output_type,
            typename fetch_func>
  void gaussian_blur_1d(const input_type * in,
                        output_type * out,
                        int size,
                        filter_type stddev,
                        int stride_in, int stride_out, filter_type filter_threshold,
                        fetch_func fetch);

  template <typename data_type,
            typename filter_type>
  void gaussian_blur_1d(const data_type * in,
                        data_type * out,
                        int size,
                        filter_type stddev,
                        int stride_in = 1, int stride_out = 1, filter_type filter_threshold = filter_type(0.001));

  template <typename input_type,
            typename filter_type,
            typename data_type,
            typename output_type,
            typename fetch_func>
  void gaussian_blur_2d(const input_type * in,
                        output_type * out,
                        int width,
                        int height,
                        filter_type stddev,
                        int stride_in, int stride_out, filter_type filter_threshold,
                        fetch_func fetch);
}

/////////////////////////////////////////////////////////////////////
// Some usefull shortcuts.
namespace ggo
{
  inline void gaussian_blur_1d_uint8(const uint8_t * in, uint8_t * out, int size, float stddev, int stride_in = 1, int stride_out = 1, float filter_threshold = 0.01f)
  {
    gaussian_blur_1d<uint8_t, float, float, uint8_t>(in, out, size, stddev, stride_in, stride_out, filter_threshold, fetch_data_duplicated_edge_mirror1d_const_struct<uint8_t>());
  }

  inline void gaussian_blur_1d_float(const float * in, float * out, int size, float stddev, int stride_in = 1, int stride_out = 1, float filter_threshold = 0.01f)
  {
    gaussian_blur_1d<float, float>(in, out, size, stddev, stride_in, stride_out, filter_threshold);
  }

  inline void gaussian_blur_2d_uint8(const uint8_t * in, uint8_t * out, int width, int height, float stddev, int stride_in = 1, int stride_out = 1, float filter_threshold = 0.01f)
  {
    gaussian_blur_2d<uint8_t, float, float, uint8_t>(in, out, width, height, stddev, stride_in, stride_out, filter_threshold, fetch_data_duplicated_edge_mirror1d_const_struct<float>());
  }

  inline void gaussian_blur_2d_float(const float * in, float * out, int width, int height, float stddev, int stride_in = 1, int stride_out = 1, float filter_threshold = 0.01f)
  {
    //gaussian_blur_2d<float, float, float, float>(in, out, width, height, stddev, stride_in, stride_out, filter_threshold);
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
            typename fetch_func>
  void gaussian_blur_1d(const input_type * in,
                        output_type * out,
                        int size,
                        filter_type stddev,
                        int stride_in, int stride_out, filter_type filter_threshold,
                        fetch_func fetch)
  {
    // Setup the filter.
    auto filter = build_gaussian_filter<filter_type>(stddev, filter_threshold);

    // Apply the filter.
    apply_symetric_filter_1d
      <input_type, filter_type, data_type, output_type, fetch_func>
      (in, out, size, stride_in, stride_out, &filter[0], static_cast<int>(filter.size()), fetch);
  }
  
  /////////////////////////////////////////////////////////////////////
  template <typename data_type,
            typename filter_type>
  void gaussian_blur_1d(const data_type * in,
                        data_type * out,
                        int size,
                        filter_type stddev,
                        int stride_in, int stride_out, filter_type filter_threshold)
  {
    gaussian_blur_1d
    <data_type, filter_type, data_type, data_type>
    (in, out, size, stddev, stride_in, stride_out, filter_threshold, fetch_data_duplicated_edge_mirror1d_const_struct<data_type>());
  }

  /////////////////////////////////////////////////////////////////////
  template <typename input_type,
            typename filter_type,
            typename data_type,
            typename output_type,
            typename fetch_func>
  void gaussian_blur_2d(const input_type * in,
                        output_type * out,
                        int width,
                        int height,
                        filter_type stddev,
                        int stride_in, int stride_out, filter_type filter_threshold,
                        fetch_func fetch)
  {
    // Setup the filter.
    auto filter = build_gaussian_filter<filter_type>(stddev, filter_threshold);
    int filter_size = static_cast<int>(filter.size());

    if (filter.size() == 1 && in == out)
    {
      return;
    }

    // Cast input.
    ggo::array<data_type> tmp1(width * height);
    {
      const input_type * it_in = in;
      data_type * it_out = tmp1;
      for (int y = 0; y < height; ++y)
      {
        for (int x = 0; x < width; ++x)
        {
          *it_out = ggo::to<data_type>(*it_in);
          ++it_out;
          it_in += stride_in;
        }
      }
    }

    // Horizontal filtering.
    ggo::array<data_type> tmp2(width * height);
    {
      const data_type * it_in = tmp1;
      data_type * it_out = tmp2;
      for (int y = 0; y < height; ++y)
      {
        apply_symetric_filter_1d
          <data_type, filter_type, data_type, data_type, fetch_func>
          (it_in, it_out, width, 1, 1, &filter[0], filter_size, fetch);
        it_in += width;
        it_out += width;
      }
    }

    // Vertical filtering.
    {
      const data_type * it_in = tmp2;
      output_type * it_out = out;
      for (int x = 0; x < width; ++x)
      {
        apply_symetric_filter_1d
          <data_type, filter_type, data_type, output_type, fetch_func>
          (it_in, it_out, height, width, stride_out * width, &filter[0], filter_size, fetch);
        it_in += 1;
        it_out += stride_out;
      }
    }
  }
}

#endif

