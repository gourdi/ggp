#ifndef __GGO_GAUSSIAN_BLUR__
#define __GGO_GAUSSIAN_BLUR__

#include <vector>

namespace ggo
{
  template <typename kernel_t>
  std::vector<kernel_t> build_gaussian_kernel(kernel_t stddev, kernel_t truncate = 4)
  {
    static_assert(std::is_floating_point<kernel_t>::value, "expecting floating point type");

    std::vector<kernel_t> kernel;

    // Fill kernel and compute norm at the same time.
    kernel.push_back(1);
    kernel_t i = 1;
    kernel_t norm = 1;
    kernel_t variance2 = 2 * stddev * stddev;
    truncate = stddev * truncate;
    while (true)
    {
      kernel_t coef = std::exp(-i * i / variance2);
      if (i > truncate)
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
}

#endif
