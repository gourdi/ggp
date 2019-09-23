//#define GGO_GAUSSIAN_DEBUG 1

#include <kernel/nonreg/ggo_nonreg.h>
#include <kernel/math/signal_processing/ggo_gaussian_blur.h>
#include <numeric>

/////////////////////////////////////////////////////////////////////
GGO_TEST(gaussian_blur, kernel_floating)
{
  auto compute_norm = [](const std::vector<float> & kernel)
  {
    auto norm = std::accumulate(kernel.begin(), kernel.end(), 0.f);
    return std::accumulate(kernel.begin() + 1, kernel.end(), norm);
  };

  std::vector<float> stddevs{ 1.f, 2.f, 5.f };

  for (const auto & stddev : stddevs)
  {
    auto kernel = ggo::build_gaussian_kernel(stddev);
    GGO_CHECK_FLOAT_EQ(compute_norm(kernel), 1.f);
  }
}

