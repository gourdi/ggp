#ifndef __GGO_FOURIER__
#define __GGO_FOURIER__

#include <complex>

namespace ggo
{
  void fdft(const std::complex<float> * input, std::complex<float> * output, int size);
  void idft(const std::complex<float> * input, std::complex<float> * output, int size);

  void fdft(const std::complex<double> * input, std::complex<double> * output, int size);
  void idft(const std::complex<double> * input, std::complex<double> * output, int size);
}

#endif



