#include "ggo_fourier.h"
#include <ggo_kernel.h>

//////////////////////////////////////////////////////////////
template<typename T>
void fdft(const std::complex<T> * input, std::complex<T> * output, int size)
{
	T	norm = 1 / T(size);

	for (int k = 0; k < size; ++k)
	{
		output[k] = std::complex<T>(0, 0);

		for (int n = 0; n < size; ++n)
		{
			T angle = -2 * ggo::pi<T>() * k * n / size;
			output[k] += input[n] * std::complex<T>(std::cos(angle), std::sin(angle));
		}

		output[k] *= norm;
	}
}

//////////////////////////////////////////////////////////////
template<typename T>
void idft(const std::complex<T> * input, std::complex<T> * output, int size)
{
	T	norm = 1 / T(size);

	for (int k = 0; k < size; ++k)
	{
		output[k] = std::complex<T>(0, 0);

		for (int n = 0; n < size; ++n)
		{
			T angle = 2 *ggo::pi<T>() * k * n / size;
			output[k] += input[n] * std::complex<T>(std::cos(angle), std::sin(angle));
		}

		output[k] *= norm;
	}
}

//////////////////////////////////////////////////////////////
void fdft(const std::complex<float>* input, std::complex<float>* output, int size)
{
	fdft<float>(input, output, size);
}

//////////////////////////////////////////////////////////////
void idft(const std::complex<float>* input, std::complex<float>* output, int size)
{
	idft<float>(input, output, size);
}

//////////////////////////////////////////////////////////////
void fdft(const std::complex<double>* input, std::complex<double>* output, int size)
{
	fdft<double>(input, output, size);
}

//////////////////////////////////////////////////////////////
void idft(const std::complex<double>* input, std::complex<double>* output, int size)
{
	idft<double>(input, output, size);
}



