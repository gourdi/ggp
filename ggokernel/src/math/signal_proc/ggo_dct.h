#ifndef __GGO_DCT__
#define __GGO_DCT__

namespace ggo
{
  // A straightforward DCT transform, not optimized at all, being its own inverse.
  // No restrictions over the data dimension.
  void dct(const double * src, double * dst, int size);
  void dct(const float * src, float * dst, int size);
}

#endif
