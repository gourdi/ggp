//#define GGO_GAUSSIAN_DEBUG 1

#include <kernel/nonreg/ggo_nonreg.h>
#include <kernel/math/signal_proc/ggo_gaussian_blur.h>
#include <kernel/math/signal_proc/ggo_convolution1d.h>
#include <numeric>

/////////////////////////////////////////////////////////////////////
GGO_TEST(gaussian_blur, kernel_floating)
{
  auto compute_norm = [](const std::vector<float> & kernel)
  {
    auto norm = std::accumulate(kernel.begin(), kernel.end(), 0.f);
    return std::accumulate(kernel.begin() + 1, kernel.end(), norm);
  };

  std::vector<std::pair<float, float>> stddev_thresholds{ { 1.f, 0.01f }, { 2.f, 0.01f }, { 5.f, 0.01f }, { 5.f, 0.0001f } };

  for (const auto & stddev_threshold : stddev_thresholds)
  {
    auto kernel = ggo::build_gaussian_kernel(stddev_threshold.first, stddev_threshold.second);
    GGO_CHECK_FLOAT_EQ(compute_norm(kernel), 1.f);
  }
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(gaussian_blur, convolution)
{
  const float in[7] = { 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.f };
  float out[7] = { 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f };

  auto kernel = ggo::build_gaussian_kernel<float>(0.8f, 0.01f);
  GGO_CHECK_EQ(kernel.size(), 3);

  ggo::symmetric_convolution<ggo::border_mode::mirror>(in, out, 7, kernel.data(), int(kernel.size()));

  GGO_CHECK_FLOAT_EQ(out[0], 0.f);
  GGO_CHECK_FLOAT_EQ(out[1], 0.0219296496f);
  GGO_CHECK_FLOAT_EQ(out[2], 0.228512138f);
  GGO_CHECK_FLOAT_EQ(out[3], 0.499116361f);
  GGO_CHECK_FLOAT_EQ(out[4], 0.228512138f);
  GGO_CHECK_FLOAT_EQ(out[5], 0.0219296496f);
  GGO_CHECK_FLOAT_EQ(out[6], 0.f);
}
