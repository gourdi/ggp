#ifndef __GGO_LAPLACIAN__
#define __GGO_LAPLACIAN__

namespace
{
  void laplacian_2d(const float * input, float * output, int width, int height, int stride_in = 1, int stride_out = 1);
}

#endif
