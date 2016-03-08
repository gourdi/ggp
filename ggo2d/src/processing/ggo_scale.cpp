#if 0

#include "ggo_scale.h"

//////////////////////////////////////////////////////////////
double	eval_cubic_integrale(double a, double b, double c, double d, double x)
{
	GGO_ASSERT( x >= 0 );
	GGO_ASSERT( x <= 1 );
	
	return ( a*x*x*x*x/4 + b*x*x*x/3 + c*x*x/2 + d*x ); 
}

//////////////////////////////////////////////////////////////
// Here what we do is building a cubic curve which value at x=0 is
// the one is input[i] and which value at x=1 is input[i+1].
// Furthermore, to have a smooth global curve, we also make sure
// that that the derivatuve of the cubic at x=0 is the slope
// between the points at input[i-1] and input[i+1], and that the
// derivative at x=1 is the slope between the points at input[i]
// and input[i+2].
//////////////////////////////////////////////////////////////
template <typename T>
double	integrate_cubic_single(const T * input, int zero_offset, double from, double to, int size, int stride)
{
	GGO_ASSERT( from >= zero_offset );
	GGO_ASSERT( to   <= zero_offset+1 );
	GGO_ASSERT( from <= to );
	
	double	a, b, c, d, y0, y1, d0, d1;
	int		i_p, i_c, i_n, i_nn;	
	
	// Compute points coordinates.
	i_p  = ggo_clamp( zero_offset-1, 0, size-1 ) * stride;
	i_c	 = ggo_clamp( zero_offset,   0, size-1 ) * stride;
	i_n	 = ggo_clamp( zero_offset+1, 0, size-1 ) * stride;
	i_nn = ggo_clamp( zero_offset+2, 0, size-1 ) * stride;
	
	// Retrieve the values of the cubic and its derivatives.
	y0	= input[i_c];	// Value at x=0
	y1	= input[i_n];	// Value at x=1.
	d0	= (input[i_n ]-input[i_p])/2.0; // Derative at x=0.
	d1	= (input[i_nn]-input[i_c])/2.0; // Derative at x=1.
	
	// Now we have a linear system with 4 equations:
	// y0 = d;
	// y1 = a+b+c+d
	// d0 = c
	// d1 = 3*a+2*c+b
	// So we can compute the cubic's coefs.
	a	= 2*y0-2*y1+d0+d1;
	b	= -3*y0+3*y1-2*d0-d1;
	c	= d0;
	d	= y0;
	
	// Finally, we can integrate between 0 and 1.
	return ( eval_cubic_integrale( a, b, c, d, to-zero_offset ) - eval_cubic_integrale( a, b, c, d, from-zero_offset ) );
}

//////////////////////////////////////////////////////////////
template <typename T>
double	integrate_cubic(const T * input, double from, double to, int size, int stride)
{
	GGO_ASSERT( from < to );
	
	double	val;
	int		i, from_i, to_i;
	
	val 	= 0;
	from_i	= static_cast<int>( from  );
	to_i	= static_cast<int>( to + 1.0 );
	GGO_ASSERT( from_i < to_i );
	
	// Integrate the middle parts.
	for ( i = from_i; i < to_i; ++i  )
	{
		val += integrate_cubic_single( input, i, i, i+1, size, stride );
	}

	// Remove first block.
	val -= integrate_cubic_single( input, from_i, from_i, from, size, stride );
	
	// Remove last block.
	val -= integrate_cubic_single( input, to_i-1, to, to_i, size, stride );

	return ( val );
}

//////////////////////////////////////////////////////////////
template <typename T>
double	integrate_linear(const T * input, double from, double to, int size, int stride)
{
	GGO_ASSERT( from < to );
	
	double	val, a, b;
	int		i, from_i, to_i;
	
	val 	= 0;
	from_i	= static_cast<int>( from  );
	to_i	= static_cast<int>( to + 1.0 );
	GGO_ASSERT( from_i < to_i );
	
	// Integrate the middle parts.
	for ( i = from_i; i < to_i; ++i  )
	{
		val += input[stride*i]+input[stride*(i+1)];
	}
	val /= 2;
	
	// Remove first block.
	a    = input[stride*(from_i+1)]-input[stride*from_i];
	b    = input[stride*from_i]-a*from_i;
	val -= a*(from*from-from_i*from_i)/2 + b*(from-from_i);
	
	// Remove last block.
	a    = input[stride*to_i]-input[stride*(to_i-1)];
	b    = input[stride*(to_i-1)]-a*(to_i-1);
	val -= a*(to_i*to_i-to*to)/2 + b*(to_i-to);

	return ( val );
}

//////////////////////////////////////////////////////////////
template <typename T>
void	scale_1d_t(const T * input, int size_in, int stride_in, T * output, int size_out, int stride_out, ggo_resample_algo algo, T clamp_min, T clamp_max)
{
	int		i;
	double	from, to, val;
	
	for ( i = 0; i < size_out; ++i )
	{
		from	= static_cast<double>(i  )*size_in/size_out;
		to		= static_cast<double>(i+1)*size_in/size_out;
		switch ( algo )
		{
			case GGO_RESAMPLE_ALGO_LINEAR:
				val	= integrate_linear( input, from, to, size_in, stride_in ) / ( to - from );
				break;
			case GGO_RESAMPLE_ALGO_CUBIC:
				val	= integrate_cubic( input, from, to, size_in, stride_in ) / ( to - from );
				break;
			default:
				val = 0;
				GGO_FAIL();
				break;
		}
		
		*output	= ggo_clamp<int>( ggo_round( val ), clamp_min, clamp_max );
		output += stride_out;
	}
}

//////////////////////////////////////////////////////////////
template <typename T>
void	scale_fast_t(const T * input, int width_in, int height_in, T * output, int width_out, int height_out)
{
	int	x, y, x_in, y_in;
	
	for ( y = 0; y < height_out; ++y )
	{
		for ( x = 0; x < width_out; ++x )
		{
			x_in = x* width_in/ width_out;
			y_in = y*height_in/height_out;
			
			output[ y*width_out+x ] = input[ y_in*width_in+x_in ];
		}
	}
}

//////////////////////////////////////////////////////////////
template <typename T>
void	ggo_scale2d_t(const T * input, int width_in, int height_in, T * output, int width_out, int height_out, ggo_resample_algo algo, T clamp_min, T clamp_max)
{
	int	x, y;
	T *	tmp;
	
	if ( algo == GGO_RESAMPLE_ALGO_FAST )
	{
		scale_fast_t( input, width_in, height_in, output, width_out, height_out );
		return;
	}
	
	tmp = new T[ width_in*height_out ];
	
	// First perform vertical scaling.
	for ( x = 0; x < width_in; ++x )
	{
		scale_1d_t( input+x, height_in, width_in, tmp+x, height_out, width_in, algo, clamp_min, clamp_max );
	}
	
	// Then perform the horizontal scaling.
	for ( y = 0; y < height_out; ++y )
	{
		scale_1d_t( tmp+y*width_in, width_in, 1, output+y*width_out, width_out, 1, algo, clamp_min, clamp_max );
	}
	
	delete[] tmp;
}

//////////////////////////////////////////////////////////////
void	ggo_scale_2d(const uint8_t * input, int width_in, int height_in, uint8_t * output, int width_out, int height_out, ggo_resample_algo algo)
{
	ggo_scale2d_t<uint8_t>( input, width_in, height_in, output, width_out, height_out, algo, 0, 255 );
}

#endif

