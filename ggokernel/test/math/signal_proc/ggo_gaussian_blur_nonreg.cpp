//#define GGO_GAUSSIAN_DEBUG 1

#include <kernel/nonreg/ggo_nonreg.h>
#include <kernel/math/signal_proc/ggo_gaussian_blur.h>
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

