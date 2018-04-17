#ifndef __GGO_DCT__
#define __GGO_DCT__

#include <kernel/ggo_kernel.h>

namespace ggo
{
  // A straightforward DCT transform, not optimized at all, being its own inverse.
  // No restrictions over the data dimension.
  template<typename data_t>
  void dct(const data_t * src, data_t * dst, int size)
  {
    static_assert(std::is_floating_point<data_t>::value);

    data_t norm = std::pow(data_t(2.0 / size), data_t(0.5));

    for (int k = 0; k < size; ++k)
    {
      dst[k] = 0;

      for (int n = 0; n < size; ++n)
      {
        dst[k] += src[n] * std::cos(ggo::pi<data_t>() * (n + data_t(0.5)) * (k + data_t(0.5)) / size);
      }

      dst[k] *= norm;
    }
  }
}

#endif
