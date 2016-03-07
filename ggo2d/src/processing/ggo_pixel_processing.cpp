#if 0

#include "ggo_pixel_processing.h"
#include "ggo_color_reduction.h"
#include <ggo_kernel.h>
#include <ggo_helpers.h>
#include <ggo_math.h>
#include <ggo_histogram.h>
#include <ggo_2d_helpers.h>
#include <ggo_pixel_interpolation.h>
#include <utility>
using namespace std;

#ifdef GGO_MMX
#include <emmintrin.h>
#endif

//////////////////////////////////////////////////////////////
void	ggo_brightness_constrast(uint8_t * buffer, int size, float brightness, float contrast, ggo_user_feedback_abc * feedback)
{
	int	i, brightness_aux, contrast_aux;

	brightness		= ggo_clamp<float>( brightness, -1, 1 );
	contrast		= ggo_clamp<float>( contrast,   -1, 1 );

	brightness_aux	=       ggo_round( 256 * brightness );
	contrast_aux	= 256 + ggo_round( 256 * contrast );

	for ( i = 0; i < size; ++i )
	{
		if ( ( feedback != NULL ) && ( (i&0xFF) == 0 ) && ( feedback->update( i>>8, size>>8 ) == false ) )
		{
			break;
		}

		buffer[i] = ggo_clamp( 128 + ( ( contrast_aux * ( buffer[i] + brightness_aux - 128 ) ) >> 8 ), 0, 255 );
	}
}

//////////////////////////////////////////////////////////////
void	ggo_threshold(uint8_t * r, uint8_t * g, uint8_t * b, int size, float threshold, ggo_user_feedback_abc * feedback)
{
	int	i, luma, threshold_aux;

	threshold_aux = static_cast<int>( 3*256*threshold );

	for ( i = 0; i < size; ++i )
	{
		if ( ( feedback != NULL ) && ( (i&0xFF) == 0 ) && ( feedback->update( i>>8, size>>8 ) == false ) )
		{
			break;
		}

		luma = (*r) + (*g) + (*b);
		if ( luma > threshold_aux )
		{
			*r++ = *g++ = *b++ = 0xFF;
		}
		else
		{
			*r++ = *g++ = *b++ = 0;
		}
	}
}

//////////////////////////////////////////////////////////////
void	ggo_zoom_merge_line(const uint8_t * r, const uint8_t * g, const uint8_t * b, uint8_t * out, int width, ggo_zoom_factor zoom_factor)
{
	switch ( zoom_factor )
	{
	case GGO_ZOOM_12:
		while ( width > 0 )
		{
			*out++ = *r; r += 8;
			*out++ = *g; g += 8;
			*out++ = *b; b += 8;

			width -= 8;
		}
		break;
	case GGO_ZOOM_25:
		while ( width > 0 )
		{
			*out++ = *r; r += 4;
			*out++ = *g; g += 4;
			*out++ = *b; b += 4;

			width -= 4;
		}
		break;
	case GGO_ZOOM_50:
		while ( width > 0 )
		{
			*out++ = *r; r += 2;
			*out++ = *g; g += 2;
			*out++ = *b; b += 2;

			width -= 2;
		}
		break;
	case GGO_ZOOM_NONE:
		while ( width-- > 0 )
		{
			*out++ = *r++;
			*out++ = *g++;
			*out++ = *b++;
		}
		break;
	case GGO_ZOOM_200:
		while ( width-- > 0 )
		{
			out[0] = out[3] = *r++;
			out[1] = out[4] = *g++;
			out[2] = out[5] = *b++;

			out += 6;
		}
		break;
	case GGO_ZOOM_400:
		while ( width-- > 0 )
		{
			out[0] = out[3] = out[6] = out[ 9] = *r++;
			out[1] = out[4] = out[7] = out[10] = *g++;
			out[2] = out[5] = out[8] = out[11] = *b++;

			out += 12;
		}
		break;
	case GGO_ZOOM_800:
		while ( width-- > 0 )
		{
			out[0] = out[3] = out[6] = out[ 9] = out[12] = out[15] = out[18] = out[21] = *r++;
			out[1] = out[4] = out[7] = out[10] = out[13] = out[16] = out[19] = out[22] = *g++;
			out[2] = out[5] = out[8] = out[11] = out[14] = out[17] = out[20] = out[23] = *b++;

			out += 24;
		}
		break;
	}
}

//////////////////////////////////////////////////////////////
#ifdef GGO_MMX
static inline __m128i	ggo_blend_aux(__m128i out, __m128i in, __m128i a)
{
	__m128i	in_8_1, in_8_2, out_8_1, out_8_2, a_1, a_2, a_inv, a_inv_1,a_inv_2;

	a_1			= _mm_unpacklo_epi8( a,_mm_setzero_si128() );
	a_2			= _mm_unpackhi_epi8( a,_mm_setzero_si128() );

	a_inv		= _mm_sub_epi8( _mm_set1_epi8( 0xFF ), a );
	a_inv_1		= _mm_unpacklo_epi8( a_inv,_mm_setzero_si128() );
	a_inv_2		= _mm_unpackhi_epi8( a_inv,_mm_setzero_si128() );

	in_8_1		= _mm_unpacklo_epi8( in,_mm_setzero_si128() );
	in_8_2		= _mm_unpackhi_epi8( in,_mm_setzero_si128() );

	out_8_1		= _mm_unpacklo_epi8( out, _mm_setzero_si128() );
	out_8_2		= _mm_unpackhi_epi8( out, _mm_setzero_si128() );

	out_8_1		= _mm_adds_epu16( _mm_mullo_epi16( a_inv_1, in_8_1 ), _mm_mullo_epi16( a_1, out_8_1 ) );
	out_8_2		= _mm_adds_epu16( _mm_mullo_epi16( a_inv_2, in_8_2 ), _mm_mullo_epi16( a_2, out_8_2 ) );

	return ( _mm_packus_epi16( _mm_srli_epi16( out_8_1, 8 ), _mm_srli_epi16( out_8_2, 8 ) ) );
}
#endif

//////////////////////////////////////////////////////////////
void	ggo_blend(uint8_t * r_out, uint8_t * g_out, uint8_t * b_out, const uint8_t * r_in, const uint8_t * g_in, const uint8_t * b_in, const uint8_t * alpha, int size)
{
#if 0

	GGO_ASSERT( (size%16) == 0 );

	int		x;
	__m128i	a;

	for ( x = 0; x < size; x += 16 )
	{
		a = _mm_load_si128( (__m128i*)(alpha+x) );

		_mm_store_si128( (__m128i*)(r_out+x), ggo_blend_aux( _mm_load_si128( (__m128i*)(r_out+x) ), _mm_load_si128( (__m128i*)(r_in+x) ), a ) );
		_mm_store_si128( (__m128i*)(g_out+x), ggo_blend_aux( _mm_load_si128( (__m128i*)(g_out+x) ), _mm_load_si128( (__m128i*)(g_in+x) ), a ) );
		_mm_store_si128( (__m128i*)(b_out+x), ggo_blend_aux( _mm_load_si128( (__m128i*)(b_out+x) ), _mm_load_si128( (__m128i*)(b_in+x) ), a ) );
	}

#else

	int	x, a, a_inv;

	for ( x = 0; x < size; ++x )
	{
		a			= alpha[x];
		a_inv		= 0xFF-a;

		r_out[x]	= (a_inv*r_in[x]+a*r_out[x])>>8;
		g_out[x]	= (a_inv*g_in[x]+a*g_out[x])>>8;
		b_out[x]	= (a_inv*b_in[x]+a*b_out[x])>>8;
	}

#endif
}

//////////////////////////////////////////////////////////////
void	ggo_uniform_noise(uint8_t * r, uint8_t * g, uint8_t * b, int size, float amp, bool luma_only, ggo_user_feedback_abc * feedback)
{
	int	i, noise, amp_aux;

	amp_aux = ggo_round( 256*amp );

	if ( luma_only == true )
	{
		for ( i = 0; i < size; ++i )
		{
			if ( ( feedback != NULL ) && ( (i&0xFF) == 0 ) && ( feedback->update( i>>8, size>>8 ) == false ) )
			{
				break;
			}

			noise = (rand()%(2*amp_aux+1))-amp_aux;
			*r++ = ggo_clamp( (*r)+noise, 0, 0xFF );
			*g++ = ggo_clamp( (*g)+noise, 0, 0xFF );
			*b++ = ggo_clamp( (*b)+noise, 0, 0xFF );
		}
	}
	else
	{
		for ( i = 0; i < size; ++i )
		{
			if ( ( feedback != NULL ) && ( (i&0xFF) == 0 ) && ( feedback->update( i>>8, size>>8 ) == false ) )
			{
				break;
			}

			*r++ = ggo_clamp( (*r)+((rand()%(2*amp_aux+1))-amp_aux), 0, 0xFF );
			*g++ = ggo_clamp( (*g)+((rand()%(2*amp_aux+1))-amp_aux), 0, 0xFF );
			*b++ = ggo_clamp( (*b)+((rand()%(2*amp_aux+1))-amp_aux), 0, 0xFF );
		}
	}
}

//////////////////////////////////////////////////////////////
void	ggo_blur_1d(const uint8_t * in, uint8_t * out, int size, float variance, int stride)
{
	int sum_out, coef_gaus, sum_gaus, i, delta;

	for ( i = 0; i < size; ++i )
	{
		sum_out		= 0;
		sum_gaus	= 0;

		coef_gaus	= 256;
		sum_out		= in[stride*i] * coef_gaus;
		sum_gaus	= coef_gaus;

		// Left.
		for ( delta = -1; i+delta >= 0; --delta )
		{
			coef_gaus	= ggo_round( 256.0 * exp( -delta*delta/variance ) );
			if ( coef_gaus == 0 ) { break; }
			sum_gaus 	+= coef_gaus;
			sum_out		+= in[stride*(i+delta)] * coef_gaus;
		}

		// Right.
		for ( delta = 1; i+delta < size; ++delta )
		{
			coef_gaus	= ggo_round( 256.0 * exp( -delta*delta/variance ) );
			if ( coef_gaus == 0 ) { break; }
			sum_gaus 	+= coef_gaus;
			sum_out		+= in[stride*(i+delta)] * coef_gaus;
		}

		out[stride*i] = ggo_clamp( (sum_out+sum_gaus/2)/sum_gaus, 0, 0xFF );
	}
}

//////////////////////////////////////////////////////////////
void ggo_blur(uint8_t * buffer, int width, int height, float variance, ggo_user_feedback_abc * feedback)
{
	int 				x, y;
	ggo_array_uint8	tmp;

	tmp = ggo_array_uint8( width*height );

	for ( y = 0; y < height; ++y )
	{
		if ( feedback != NULL )
		{
			feedback->update( y, height );
		}

		ggo_blur_1d( buffer+ggo_get_2d_index( 0, y, width ), tmp+ggo_get_2d_index( 0, y, width ), width, variance, 1 );
	}

	for ( x = 0; x < width; ++x )
	{
		if ( feedback != NULL )
		{
			feedback->update( x, width );
		}

		ggo_blur_1d( tmp+x, buffer+x, height, variance, width );
	}
}

//////////////////////////////////////////////////////////////
void	ggo_grayscale(uint8_t * r, uint8_t * g, uint8_t * b, int size, ggo_user_feedback_abc * feedback)
{
	int	i, luma;

	for ( i = 0; i < size; ++i )
	{
		if ( ( feedback != NULL ) && ( (i&0xFF) == 0 ) && ( feedback->update( i>>8, size>>8 ) == false ) )
		{
			break;
		}

		luma = ( (*r) + (*g) + (*b) ) / 3;
		*r++ = *g++ = *b++ = luma;
	}
}

//////////////////////////////////////////////////////////////
void	ggo_invert(uint8_t * buffer, int size, ggo_user_feedback_abc * feedback)
{
	int	i;

	for ( i = 0; i < size; ++i )
	{
		if ( ( feedback != NULL ) && ( (i&0xFF) == 0 ) && ( feedback->update( i>>8, size>>8 ) == false ) )
		{
			break;
		}

		*buffer++ = 0xFF-(*buffer);
	}
}

//////////////////////////////////////////////////////////////
// http://en.wikipedia.org/wiki/Histogram_equalization
void	ggo_equalize_histogram(uint8_t * r, uint8_t * g, uint8_t * b, int size, ggo_user_feedback_abc * feedback)
{
	ggo_histogram	histogram;
	ggo_array_int	cdfs;
	int				i, luma, cdf_cur, size_aux;
	double			cdf_new;

	size_aux = size;
	histogram.build( r, g, b, size );
	histogram.fill_cumulative_distribution( cdfs );

	for ( i = 0; i < size; ++i )
	{
		if ( ( feedback != NULL ) && ( (i&0xFF) == 0 ) && ( feedback->update( i>>8, size>>8 ) == false ) )
		{
			break;
		}

		luma	= (*r) + (*g) + (*b);
		cdf_cur	= cdfs[luma];
		cdf_new	= ggo_round( ( cdfs.get_size() - 1 ) * static_cast<double>( cdf_cur - cdfs[0] ) / static_cast<double>( size_aux - cdfs[0] ) );

		*r++ = luma == 0 ? 0 : ggo_clamp( ggo_round( (*r)*cdf_new/luma ), 0, 0xFF );
		*g++ = luma == 0 ? 0 : ggo_clamp( ggo_round( (*g)*cdf_new/luma ), 0, 0xFF );
		*b++ = luma == 0 ? 0 : ggo_clamp( ggo_round( (*b)*cdf_new/luma ), 0, 0xFF );
	}
}

//////////////////////////////////////////////////////////////
void	ggo_apply_mapping(uint8_t * buffer, int size, const ggo_mapping & mapping, ggo_interpolation interpolation, ggo_user_feedback_abc * feedback)
{
	int		i;
	float	old_val, new_val;

	for ( i = 0; i < size; ++i )
	{
		if ( ( feedback != NULL ) && ( (i&0xFF) == 0 ) && ( feedback->update( i>>8, size>>8 ) == false ) )
		{
			break;
		}

		old_val = (*buffer) / 255.0f;
		new_val = mapping.get_value( old_val, interpolation );

		if ( *buffer == 0 )
		{
			*buffer++ = ggo_clamp( ggo_round( 255*new_val ), 0, 0xFF );
		}
		else
		{
			*buffer++ = ggo_clamp( ggo_round( (*buffer)*new_val/old_val ), 0, 0xFF );
		}
	}
}

//////////////////////////////////////////////////////////////
void	ggo_apply_palette(uint8_t * r, uint8_t * g, uint8_t * b, int size, const ggo_palette & palette, bool optimize_palette, ggo_user_feedback_abc * feedback)
{
	int			error_cur, error_min, i, j;
	ggo_color	color_cur, color_min;
	ggo_palette	palette_aux( palette );

	if ( optimize_palette == true )
	{
		ggo_color_reduction_median_cut( r, g, b, size, palette_aux, palette.get_size(), feedback );
	}

	for ( i = 0; i < size; ++i )
	{
		if ( ( feedback != NULL ) && ( (i&0xFF) == 0 ) && ( feedback->update( i>>8, size>>8 ) == false ) )
		{
			break;
		}

		color_min = palette_aux[0];
		error_min = std::abs((*r)-color_min.r() ) + std::abs( (*g)-color_min.g()) + std::abs( (*b)-color_min.b());

		for ( j = 1; j < palette_aux.get_size(); ++j )
		{
			color_cur = palette_aux[j];
			error_cur = std::abs((*r)-color_cur.r() ) + std::abs( (*g)-color_cur.g() ) + std::abs( (*b)-color_cur.b());

			if ( error_cur < error_min )
			{
				color_min = color_cur;
				error_min = error_cur;
			}
		}

		*r++ = color_min.r();
		*g++ = color_min.g();
		*b++ = color_min.b();
	}
}

//////////////////////////////////////////////////////////////
void	ggo_colorize(uint8_t * r, uint8_t * g, uint8_t * b, int size, ggo_color color, float factor, ggo_user_feedback_abc * feedback)
{
	ggo_color	new_color;
	int			i, luma, factor_aux1, factor_aux2;

	factor_aux1 = ggo_round( factor*0xFF );
	factor_aux2 = 0xFF - factor_aux1;

	for ( i = 0; i < size; ++i )
	{
		if ( ( feedback != NULL ) && ( (i&0xFF) == 0 ) && ( feedback->update( i>>8, size>>8 ) == false ) )
		{
			break;
		}

		luma = (*r) + (*g) + (*b);

		new_color.r() = ggo_round_div( color.r()*luma, 3*0xFF );
		new_color.g() = ggo_round_div( color.g()*luma, 3*0xFF );
		new_color.b() = ggo_round_div( color.b()*luma, 3*0xFF );

		*r++ = ggo_round_div( factor_aux1*new_color.r() + factor_aux2*(*r), 0xFF );
		*g++ = ggo_round_div( factor_aux1*new_color.g() + factor_aux2*(*g), 0xFF );
		*b++ = ggo_round_div( factor_aux1*new_color.b() + factor_aux2*(*b), 0xFF );
	}
}

//////////////////////////////////////////////////////////////
void	ggo_mozaic(uint8_t * buffer, int width, int height, int tile_width, int tile_height, ggo_user_feedback_abc * feedback)
{
	int	mean, tile_x, tile_y, tile_count_x, tile_count_y, x_from, x_to, y_from, y_to, x, y;

	tile_count_x = ((width -1)/tile_width )+1;
	tile_count_y = ((height-1)/tile_height)+1;
	for ( tile_y = 0; tile_y < tile_count_y; ++tile_y )
	{
		if ( ( feedback != NULL ) && ( feedback->update( tile_y, tile_count_y ) == false ) )
		{
			break;
		}

		for ( tile_x = 0; tile_x < tile_count_x; ++tile_x )
		{
			x_from	= tile_x * tile_width;
			y_from	= tile_y * tile_height;
			x_to	= std::min( (tile_x+1)*tile_width,  width );
			y_to	= std::min( (tile_y+1)*tile_height, height );

			// Compute the mean.
			mean	= 0;
			for ( y = y_from; y < y_to; ++y )
			{
				for ( x = x_from; x < x_to; ++x )
				{
					mean += buffer[ ggo_get_2d_index( x, y, width ) ];
				}
			}
			mean = ggo_round_div( mean, (x_to-x_from)*(y_to-y_from) );

			// Set the mean.
			for ( y = y_from; y < y_to; ++y )
			{
				for ( x = x_from; x < x_to; ++x )
				{
					buffer[ ggo_get_2d_index( x, y, width ) ] = mean;
				}
			}
		}
	}
}

//////////////////////////////////////////////////////////////
void ggo_wind(uint8_t * buffer, int width, int height, float direction, int length, ggo_user_feedback_abc * feedback)
{
	int							x, y, i, weight, weights_sum, new_pixel;
	float						x_src, y_src;
	ggo_array_uint8				clone;
	uint8_t						pixel;
	vector<pair<uint8_t, int> >	pixels;

	clone = ggo_array_uint8( width*height );
	memcpy( clone, buffer, width*height );

	for ( y = 0; y < height; ++y )
	{
		if ( ( feedback != NULL ) && ( feedback->update( y, height ) == false ) )
		{
			break;
		}

		for ( x = 0; x < width; ++x )
		{
			weights_sum = 0;
			pixels.clear();

			// Get pixels and their weights.
			// The first pixel which is the current one.
			pixel = clone[ ggo_get_2d_index( x, y, width ) ];
			pixels.push_back( pair<int,int>( pixel, length ) );
			weights_sum	+= length;
			// The other pixel.
			for ( i = 1; i < length; ++i )
			{
				x_src = x + i*cos(direction);
				y_src = y + i*sin(direction);

				if ( ggo_interpolate(clone, width, height, x_src, y_src, 1, pixel) == true )
				{
					weight		= length - i;
					weights_sum	+= weight;
					pixels.push_back( pair<uint8_t,int>( pixel, weight ) );
				}
			}

			// Compute new pixels value which is a weighted sum.
			new_pixel = 0;
			for ( i = 0; i < (int)pixels.size(); ++i )
			{
				new_pixel += pixels[i].first * pixels[i].second;
			}
			buffer[ ggo_get_2d_index( x, y, width ) ] = ggo_round_div( new_pixel, weights_sum );
		}
	}
}

#endif


