#ifndef __GGO_GAUSSIAN_BLUR__
#define __GGO_GAUSSIAN_BLUR__

#include <stdint.h>
#include <vector>
#include <ggo_array.h>
#include <ggo_buffer_access.h>
#include <ggo_convolution.h>

/////////////////////////////////////////////////////////////////////
// Definitions.
namespace ggo
{
  // Gaussian filter coefs.
  template <typename filter_t>
  std::vector<filter_t> build_gaussian_filter(filter_t stddev, filter_t filter_threshold);

  // 1D.
  template <typename getter, typename setter, typename filter_t>
  void gaussian_blur_1d(getter & in,
                        setter & out,
                        int size,
                        filter_t stddev,
                        filter_t filter_threshold = filter_t(0.001));

  // 2D horizontal.
  template <typename getter, typename setter, typename filter_t>
  void gaussian_blur_2d_horz(const getter & in,
                             setter & out,
                             int width, int height,
                             filter_t stddev,
                             filter_t filter_threshold = filter_t(0.001));

  // 2D vertical.
  template <typename getter, typename setter, typename filter_t>
  void gaussian_blur_2d_vert(const getter & in,
                             setter & out,
                             int width, int height,
                             filter_t stddev,
                             filter_t filter_threshold = filter_t(0.001));
}

/////////////////////////////////////////////////////////////////////
// Some usefull specializations.
namespace ggo
{
  /////////////////////////////////////////////////////////////////////
  // Mirroring generic gaussian 2D.
  template <typename data_t, typename filter_t, int stride_in = 1, int stride_out = 1>
  void gaussian_blur_2d_mirror(const data_t * in, data_t * out, int width, int height, float stddev, filter_t filter_threshold)
  {
    ggo::array<data_t> tmp(width * height);

    // Setup the filter.
    auto filter = build_gaussian_filter<filter_t>(stddev, filter_threshold);

    // Horizontal filtering.
    auto in_horz = [&](int x, int y) { return ggo::get2d_mirror_ref<const data_t, stride_in>(in, x, y, width, height); };
    auto out_horz = [&](int x, int y, const data_t & v) { ggo::set2d(tmp.data(), x, y, width, height, v); };
    ggo::apply_symetric_filter_2d_horz(in_horz, out_horz, width, height, &filter[0], static_cast<int>(filter.size()));

    // Vertical filtering.
    auto in_vert = [&](int x, int y) { return ggo::get2d_mirror_ref(tmp.data(), x, y, width, height); };
    auto out_vert = [&](int x, int y, const data_t & v) { ggo::set2d<data_t, stride_out>(out, x, y, width, height, v); };
    ggo::apply_symetric_filter_2d_vert(in_vert, out_vert, width, height, &filter[0], static_cast<int>(filter.size()));
  }

  /////////////////////////////////////////////////////////////////////
  // Mirroring uint8_t specialization.
  template <int stride_in = 1, int stride_out = 1>
  void gaussian_blur_2d_mirror(const uint8_t * in, uint8_t * out, int width, int height, float stddev, float filter_threshold)
  {
    ggo::array<float> tmp(width * height);

    // Setup the filter.
    auto filter = build_gaussian_filter<float>(stddev, filter_threshold);

    // Horizontal filtering.
    auto in_horz = [&](int x, int y) { return ggo::to<float>(ggo::get2d_mirror<const uint8_t, stride_in>(in, x, y, width, height)); };
    auto out_horz = [&](int x, int y, float v) { ggo::set2d(tmp.data(), x, y, width, height, v); };
    ggo::apply_symetric_filter_2d_horz(in_horz, out_horz, width, height, &filter[0], static_cast<int>(filter.size()));

    // Vertical filtering.
    auto in_vert = [&](int x, int y) { return ggo::get2d_mirror(tmp.data(), x, y, width, height); };
    auto out_vert = [&](int x, int y, float v) { ggo::set2d<uint8_t, stride_out>(out, x, y, width, height, ggo::to<uint8_t>(v)); };
    ggo::apply_symetric_filter_2d_vert(in_vert, out_vert, width, height, &filter[0], static_cast<int>(filter.size()));
  }
}

/////////////////////////////////////////////////////////////////////
// Implementation.
namespace ggo
{
  template <typename filter_t>
  std::vector<filter_t> build_gaussian_filter(filter_t stddev, filter_t filter_threshold)
  {
    std::vector<filter_t> filter;

    // Fill filter and compute norm at the same time.
    filter.push_back(1);
    filter_t i = 1;
    filter_t norm = 1;
    filter_t variance2 = 2 * stddev * stddev;
    while (true)
    {
      filter_t coef = std::exp(-i * i / variance2);
      if (coef < filter_threshold)
      {
        break;
      }
      filter.push_back(coef);
      norm += 2 * coef;

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
  template <typename getter, typename setter, typename filter_t>
  void gaussian_blur_1d(getter & in,
                        setter & out,
                        int size,
                        filter_t stddev,
                        filter_t filter_threshold)
  {
    // Setup the filter.
    auto filter = build_gaussian_filter<filter_t>(stddev, filter_threshold);

    // Apply the filter.
    apply_symetric_filter_1d(in, out, size, &filter[0], static_cast<int>(filter.size()));
  }

  /////////////////////////////////////////////////////////////////////
  template <typename getter, typename setter, typename filter_t>
  void gaussian_blur_2d_horz(const getter & in,
                             setter & out,
                             int width, int height,
                             filter_t stddev,
                             filter_t filter_threshold)
    {
      // Setup the filter.
      auto filter = build_gaussian_filter<filter_t>(stddev, filter_threshold);

      // Horizontal filtering.
      apply_symetric_filter_2d_horz(in, out, width, height, &filter[0], static_cast<int>(filter.size()));
    }

  /////////////////////////////////////////////////////////////////////
  template <typename getter, typename setter, typename filter_t>
  void gaussian_blur_2d_vert(const getter & in,
                             setter & out,
                             int width, int height,
                             filter_t stddev,
                             filter_t filter_threshold)
  {
    // Setup the filter.
    auto filter = build_gaussian_filter<filter_t>(stddev, filter_threshold);

    // Horizontal filtering.
    apply_symetric_filter_2d_vert(in, out, width, height, &filter[0], static_cast<int>(filter.size()));
  }
}

#endif

