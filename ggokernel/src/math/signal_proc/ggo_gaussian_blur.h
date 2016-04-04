#ifndef __GGO_GAUSSIAN_BLUR__
#define __GGO_GAUSSIAN_BLUR__

#include <stdint.h>
#include <vector>
#include <ggo_array.h>
#include <ggo_data_fetcher.h>
#include <ggo_convolution.h>

/////////////////////////////////////////////////////////////////////
// Definitions.
namespace ggo
{
  // Gauddien filter coefs.
  template <typename filter_type>
  std::vector<filter_type> build_gaussian_filter(filter_type stddev, filter_type filter_threshold);

  // 1D.
  template <typename getter, typename setter, typename filter_type>
  void gaussian_blur_1d(getter & in,
                        setter & out,
                        int size,
                        filter_type stddev,
                        filter_type filter_threshold = filter_type(0.001));

  // 2D horizontal.
  template <typename getter, typename setter, typename filter_type>
  void gaussian_blur_2d_horz(const getter & in,
                             setter & out,
                             int width, int height,
                             filter_type stddev,
                             filter_type filter_threshold = filter_type(0.001));

  // 2D vertical.
  template <typename getter, typename setter, typename filter_type>
  void gaussian_blur_2d_vert(const getter & in,
                             setter & out,
                             int width, int height,
                             filter_type stddev,
                             filter_type filter_threshold = filter_type(0.001));
}

/////////////////////////////////////////////////////////////////////
// Some usefull specializations.
namespace ggo
{
  /////////////////////////////////////////////////////////////////////
  // Mirroring generic gaussian 2D.
  template <typename data_type, typename filter_type>
  void gaussian_blur_2d_mirror(const data_type * in, data_type * out, int width, int height, float stddev, int stride_in, int stride_out, filter_type filter_threshold)
  {
    ggo::array<data_type> tmp(width * height);

    // Setup the filter.
    auto filter = build_gaussian_filter<filter_type>(stddev, filter_threshold);

    // Horizontal filtering.
    auto in_horz = [&](int x, int y) { return ggo::get2d_duplicated_edge_mirror(in, x, y, width, height, stride_in); };
    auto out_horz = [&](int x, int y, float v) { ggo::set2d_standard(tmp.get_pointer(), x, y, width, height, v, 1); };
    ggo::apply_symetric_filter_2d_horz(in_horz, out_horz, width, height, &filter[0], static_cast<int>(filter.size()));

    // Vertical filtering.
    auto in_vert = [&](int x, int y) { return ggo::get2d_duplicated_edge_mirror(tmp.get_pointer(), x, y, width, height, 1); };
    auto out_vert = [&](int x, int y, float v) { ggo::set2d_standard(out, x, y, width, height, v, stride_out); };
    ggo::apply_symetric_filter_2d_vert(in_vert, out_vert, width, height, &filter[0], static_cast<int>(filter.size()));
  }

  /////////////////////////////////////////////////////////////////////
  // Mirroring uint8_t specialization.
  template <>
  void gaussian_blur_2d_mirror(const uint8_t * in, uint8_t * out, int width, int height, float stddev, int stride_in, int stride_out, float filter_threshold)
  {
    ggo::array<float> tmp(width * height);

    // Setup the filter.
    auto filter = build_gaussian_filter<float>(stddev, filter_threshold);

    // Horizontal filtering.
    auto in_horz = [&](int x, int y) { return ggo::to<float>(ggo::get2d_duplicated_edge_mirror(in, x, y, width, height, stride_in)); };
    auto out_horz = [&](int x, int y, float v) { ggo::set2d_standard(tmp.get_pointer(), x, y, width, height, v, 1); };
    ggo::apply_symetric_filter_2d_horz(in_horz, out_horz, width, height, &filter[0], static_cast<int>(filter.size()));

    // Vertical filtering.
    auto in_vert = [&](int x, int y) { return ggo::get2d_duplicated_edge_mirror(tmp.get_pointer(), x, y, width, height, 1); };
    auto out_vert = [&](int x, int y, float v) { ggo::set2d_standard(out, x, y, width, height, ggo::to<uint8_t>(v), stride_out); };
    ggo::apply_symetric_filter_2d_vert(in_vert, out_vert, width, height, &filter[0], static_cast<int>(filter.size()));
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

    // fill filter and compute norm at the same time.
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

    // normalize.
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
  template <typename getter, typename setter, typename filter_type>
  void gaussian_blur_1d(getter & in,
                        setter & out,
                        int size,
                        filter_type stddev,
                        filter_type filter_threshold)
  {
    // Setup the filter.
    auto filter = build_gaussian_filter<filter_type>(stddev, filter_threshold);

    // Apply the filter.
    apply_symetric_filter_1d(in, out, size, &filter[0], static_cast<int>(filter.size()));
  }

  /////////////////////////////////////////////////////////////////////
  template <typename getter, typename setter, typename filter_type>
  void gaussian_blur_2d_horz(const getter & in,
                             setter & out,
                             int width, int height,
                             filter_type stddev,
                             filter_type filter_threshold)
    {
      // Setup the filter.
      auto filter = build_gaussian_filter<filter_type>(stddev, filter_threshold);

      // Horizontal filtering.
      apply_symetric_filter_2d_horz(in, out, width, height, &filter[0], static_cast<int>(filter.size()));
    }

  /////////////////////////////////////////////////////////////////////
  template <typename getter, typename setter, typename filter_type>
  void gaussian_blur_2d_vert(const getter & in,
                             setter & out,
                             int width, int height,
                             filter_type stddev,
                             filter_type filter_threshold)
  {
    // Setup the filter.
    auto filter = build_gaussian_filter<filter_type>(stddev, filter_threshold);

    // Horizontal filtering.
    apply_symetric_filter_2d_vert(in, out, width, height, &filter[0], static_cast<int>(filter.size()));
  }
}

#endif

