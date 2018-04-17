#ifndef __GGO_GAUSSIAN_BLUR__
#define __GGO_GAUSSIAN_BLUR__

#include <stdint.h>
#include <vector>
#include <kernel/memory/ggo_array.h>
#include <kernel/math/signal_proc/ggo_convolution.h>

/////////////////////////////////////////////////////////////////////
// Definitions.
namespace ggo
{
  // Gaussian kernel coefs.
  template <typename kernel_t>
  std::vector<kernel_t> build_gaussian_kernel(kernel_t stddev, kernel_t kernel_threshold);

  template <typename kernel_t, typename real_t>
  std::vector<kernel_t> build_fixed_point_gaussian_kernel(real_t stddev, real_t kernel_threshold, int bit_shift);
}

/////////////////////////////////////////////////////////////////////
// Implementation.
namespace ggo
{
  /////////////////////////////////////////////////////////////////////
  template <typename kernel_t>
  std::vector<kernel_t> build_gaussian_kernel(kernel_t stddev, kernel_t kernel_threshold)
  {
    static_assert(std::is_floating_point<kernel_t>::value, "expecting floating point type");

    std::vector<kernel_t> kernel;

    // Fill kernel and compute norm at the same time.
    kernel.push_back(1);
    kernel_t i = 1;
    kernel_t norm = 1;
    kernel_t variance2 = 2 * stddev * stddev;
    while (true)
    {
      kernel_t coef = std::exp(-i * i / variance2);
      if (coef < kernel_threshold)
      {
        break;
      }
      kernel.push_back(coef);
      norm += 2 * coef;

      ++i;
    }

    // Normalize.
    for (auto & coef : kernel)
    {
      coef /= norm;
#ifdef GGO_GAUSSIAN_DEBUG		
      std::cout << coef << ", ";
#endif
    }
#ifdef GGO_GAUSSIAN_DEBUG		
    std::cout << std::endl;
#endif

    return kernel;
  }

  /////////////////////////////////////////////////////////////////////
  template <typename kernel_t, typename real_t>
  std::vector<kernel_t> build_fixed_point_gaussian_kernel(real_t stddev, real_t kernel_threshold, int bit_shift)
  {
    static_assert(std::is_integral<kernel_t>::value);
    static_assert(std::is_floating_point<real_t>::value);

    auto kernel_real = build_gaussian_kernel(stddev, kernel_threshold);

    real_t scale_inf = 0.5f;
    real_t scale_mid = 1.0f;
    real_t scale_sup = 2.0f;

    auto build_fixed_point_kernel = [&](real_t scale)
    {
      std::vector<kernel_t> kernel;

      for (const auto & coef_real : kernel_real)
      {
        kernel_t coef = ggo::round_to<kernel_t>((1 << bit_shift) * scale * coef_real);
        if (coef == 0)
        {
          break;
        }
        kernel.push_back(coef);
      }

      if (kernel.empty() == true)
      {
        throw std::runtime_error("failed building fixed point gaussian kernel");
      }

      return kernel;
    };

    auto compute_kernel_norm = [](const std::vector<kernel_t> & kernel)
    {
      return uint32_t(kernel[0] + 2 * std::accumulate(kernel.begin() + 1, kernel.end(), 0));
    };

    auto check_kernel = [&](std::vector<kernel_t> & kernel, int bit_shift)
    {
      size_t norm = compute_kernel_norm(kernel);

      if (norm == uint32_t(1 << bit_shift))
      {
        return true;
      }

      if (norm == uint32_t(1 << bit_shift) - 1)
      {
        kernel[0] += 1;
        return true;
      }

      return false;
    };

    for (int i = 0; i < 100; ++i)
    {
      auto kernel_inf = build_fixed_point_kernel(scale_inf);
      auto kernel_mid = build_fixed_point_kernel(scale_mid);
      auto kernel_sup = build_fixed_point_kernel(scale_sup);

      if (check_kernel(kernel_inf, bit_shift) == true)
      {
        return kernel_inf;
      }

      if (check_kernel(kernel_mid, bit_shift) == true)
      {
        return kernel_mid;
      }

      if (check_kernel(kernel_sup, bit_shift) == true)
      {
        return kernel_sup;
      }

      auto norm_mid = compute_kernel_norm(kernel_mid);
      if (norm_mid > uint32_t(1 << bit_shift))
      {
        scale_sup = scale_mid;
      }
      else
      {
        scale_inf = scale_mid;
      }
      scale_mid = 0.5f * (scale_inf + scale_sup);
    }

    throw std::runtime_error("failed building fixed point gaussian kernel");
    return {};
  }
}

#endif
