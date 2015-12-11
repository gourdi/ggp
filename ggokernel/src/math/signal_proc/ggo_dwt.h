#ifndef __GGO_DWT__
#define __GGO_DWT__

namespace ggo
{
  enum dwt_algo
  {
    DWT_HAAR,
    DWT_D4,
    DWT_D6,
    DWT_D8
  };

  void fdwt(const double * input, double * low, double * high, int size, dwt_algo algo);
  void idwt(const double * low, const double * high, double * output, int size, dwt_algo algo);

  void fdwt(const float * input, float * low, float * high, int size, dwt_algo algo);
  void idwt(const float * low, const float * high, float * output, int size, dwt_algo algo);
}

#endif

