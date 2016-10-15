
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
    static_assert(std::is_integral<kernel_t>::value, "expecting integral type");
    static_assert(std::is_floating_point<real_t>::value, "expecting floating point type");

    auto kernel_real = build_gaussian_kernel(stddev, kernel_threshold);

    std::vector<kernel_t> kernel;
    real_t adjust = 1;

    while (true)
    {
      kernel.clear();
      for (const auto & coef_real : kernel_real)
      {
        kernel_t coef = ggo::to<kernel_t>((1 << bit_shift) * adjust * coef_real);
        if (coef == 0)
        {
          break;
        }
        kernel.push_back(coef);
#ifdef GGO_GAUSSIAN_DEBUG		
        std::cout << kernel.back() << ", ";
#endif
      }
#ifdef GGO_GAUSSIAN_DEBUG		
      std::cout << std::endl;
#endif  

      // Make sure the sum of integer coef is exactly 1<<bit_shift.
      kernel_t norm = std::accumulate(kernel.begin(), kernel.end(), kernel_t(0));
      norm = std::accumulate(kernel.begin() + 1, kernel.end(), norm);
      if (norm <= kernel_t(1 << bit_shift))
      {
        if (norm < kernel_t(1 << bit_shift))
        {
          kernel[0] += kernel_t(1 << bit_shift) - norm;
        }
        break;
      }

      adjust -= real_t(0.001);
    }

    return kernel;
  }
}

#endif
