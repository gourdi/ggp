#include "ggo_color_reduction.h"
#include <vector>
#include <algorithm>

#include <ggo_kernel.h>
using namespace std;

//////////////////////////////////////////////////////////////
class ggo_pixel_block
{
public:

	ggo_pixel_block() : _dist_max(-1), _dist_max_channel(GGO_CHANNEL_NONE) {};

public:

	int					_dist_max;
	ggo_channel			_dist_max_channel;
	vector<ggo_color>	_pixels;
};

//////////////////////////////////////////////////////////////
void ggo_compute_dist_max(ggo_pixel_block & pixel_block)
{
printf("toto");

	GGO_ASSERT( pixel_block._pixels.empty() == false );

	int									min_r, min_g, min_b, max_r, max_g, max_b;
	int									dist_r, dist_g, dist_b;
	vector<ggo_color>::const_iterator	it;

	// Distance has already been calculated.
	if ( pixel_block._dist_max >= 0 )
	{
		return;
	}

	// Perform full computation.
	min_r = min_g = min_b = 0xFF;
	max_r = max_g = max_b = 0x00;

	for ( it = pixel_block._pixels.begin(); it != pixel_block._pixels.end(); ++it )
	{
		if ( it->r() < min_r ) { min_r = it->r(); }
		if ( it->g() < min_g ) { min_g = it->g(); }
		if ( it->b() < min_b ) { min_b = it->b(); }

		if ( it->r() > max_r ) { max_r = it->r(); }
		if ( it->g() > max_g ) { max_g = it->g(); }
		if ( it->b() > max_b ) { max_b = it->b(); }
	}

	dist_r = max_r - min_r; GGO_ASSERT( dist_r >= 0 );
	dist_g = max_g - min_g; GGO_ASSERT( dist_g >= 0 );
	dist_b = max_b - min_b; GGO_ASSERT( dist_b >= 0 );

	if ( ( dist_r >= dist_g ) && ( dist_r >= dist_b ) )
	{
		pixel_block._dist_max_channel	= GGO_CHANNEL_RED;
		pixel_block._dist_max			= dist_r;
	}
	else
	if ( ( dist_g >= dist_r ) && ( dist_g >= dist_b ) )
	{
		pixel_block._dist_max_channel	= GGO_CHANNEL_GREEN;
		pixel_block._dist_max			= dist_g;
	}
	else
	if ( ( dist_b >= dist_r ) && ( dist_b >= dist_g ) )
	{
		pixel_block._dist_max_channel	= GGO_CHANNEL_BLUE;
		pixel_block._dist_max			= dist_b;
	}
	else
	{
		GGO_FAIL();
	}
}

//////////////////////////////////////////////////////////////
list<ggo_pixel_block>::iterator	ggo_find_largest_pixel_block(list<ggo_pixel_block> & pixel_blocks)
{
	list<ggo_pixel_block>::iterator	it, split_it;
	int								dist_cur, dist_max;

	dist_max = 0;
	for ( it = pixel_blocks.begin(); it != pixel_blocks.end(); ++it )
	{
		ggo_compute_dist_max( *it );
		dist_cur = it->_dist_max;
		if ( dist_cur > dist_max )
		{
			split_it = it;
			dist_max = dist_cur;
		}
	}

	if ( split_it->_pixels.size() <= 1 )
	{
		GGO_FAIL();
		return ( pixel_blocks.end() );
	}

	return ( split_it );
}

//////////////////////////////////////////////////////////////
void	ggo_split_pixel_block(ggo_pixel_block & pixel_block, ggo_pixel_block & pixel_block_inf, ggo_pixel_block & pixel_block_sup)
{
	size_t		i;
	int			dist_to_inf, dist_to_sup;
	ggo_color	middle_pixel;

	switch ( pixel_block._dist_max_channel )
	{
	case GGO_CHANNEL_RED:
		sort( pixel_block._pixels.begin(), pixel_block._pixels.end(), ggo_pixel_sort_r );
		break;
	case GGO_CHANNEL_GREEN:
		sort( pixel_block._pixels.begin(), pixel_block._pixels.end(), ggo_pixel_sort_g );
		break;
	case GGO_CHANNEL_BLUE:
		sort( pixel_block._pixels.begin(), pixel_block._pixels.end(), ggo_pixel_sort_b );
		break;
	default:
		GGO_FAIL();
		break;
	}

	if ( ggo_is_even( pixel_block._pixels.size() ) == true )
	{
		for ( i = 0; i < pixel_block._pixels.size()/2; ++i )
		{
			pixel_block_inf._pixels.push_back( pixel_block._pixels[ i ] );
			pixel_block_sup._pixels.push_back( pixel_block._pixels[ pixel_block._pixels.size()-i-1 ] );
		}
	}
	else
	{
		for ( i = 0; i < (pixel_block._pixels.size()+1)/2; ++i )
		{
			pixel_block_inf._pixels.push_back( pixel_block._pixels[ i ] );
			pixel_block_sup._pixels.push_back( pixel_block._pixels[ pixel_block._pixels.size()-i-1 ] );
		}

		// Handle the middle pixel properly.
		middle_pixel = pixel_block._pixels[ pixel_block._pixels.size()/2 ];
		switch ( pixel_block._dist_max_channel )
		{
		case GGO_CHANNEL_RED:
			dist_to_inf = middle_pixel.r() - pixel_block._pixels.front().r();
			dist_to_sup = pixel_block._pixels.back().r() - middle_pixel.r();
			break;
		case GGO_CHANNEL_GREEN:
			dist_to_inf = middle_pixel.g() - pixel_block._pixels.front().g();
			dist_to_sup = pixel_block._pixels.back().g() - middle_pixel.g();
			break;
		case GGO_CHANNEL_BLUE:
			dist_to_inf = middle_pixel.b() - pixel_block._pixels.front().b();
			dist_to_sup = pixel_block._pixels.back().b() - middle_pixel.b();
			break;
		default:
			GGO_FAIL();
			break;
		}
		GGO_ASSERT( ( dist_to_inf >= 0 ) && ( dist_to_sup >= 0 ) );

		if ( dist_to_inf < dist_to_sup )
		{
			pixel_block_inf._pixels.push_back( middle_pixel );
		}
		else
		{
			pixel_block_sup._pixels.push_back( middle_pixel );
		}
	}
}

//////////////////////////////////////////////////////////////
void	ggo_median_cut_split(list<ggo_pixel_block> & pixel_blocks)
{
	list<ggo_pixel_block>::iterator	it, split_it;
	ggo_pixel_block					split_block_inf, split_block_sup;

	// Find the largest pixel block.
	split_it = ggo_find_largest_pixel_block( pixel_blocks );
	if ( split_it == pixel_blocks.end() )
	{
		GGO_FAIL();
		return;
	}

	// Split it.
	split_block_inf._pixels.reserve( 1+(split_it->_pixels.size()/2) );
	split_block_sup._pixels.reserve( 1+(split_it->_pixels.size()/2) );
	ggo_split_pixel_block( *split_it, split_block_inf, split_block_sup );

	// Remove old pixels block and add now ones.
	pixel_blocks.erase( split_it );
	pixel_blocks.push_back( split_block_inf );
	pixel_blocks.push_back( split_block_sup );
}

//////////////////////////////////////////////////////////////
ggo_color	ggo_pixel_average(const vector<ggo_color> & pixels)
{
	vector<ggo_color>::const_iterator	it;
	ggo_color							mean;
	int									r, g, b;

	r = g = b = 0;
	for ( it = pixels.begin(); it != pixels.end(); ++it )
	{
		r += it->r();
		g += it->g();
		b += it->b();
	}

	mean.r() = ggo_clamp<int>( ggo_round_div( r, pixels.size() ), 0, 0xFF );
	mean.g() = ggo_clamp<int>( ggo_round_div( g, pixels.size() ), 0, 0xFF );
	mean.b() = ggo_clamp<int>( ggo_round_div( b, pixels.size() ), 0, 0xFF );

	return ( mean );
}

//////////////////////////////////////////////////////////////
bool	ggo_trivial_color_reduction_median_cut(const uint8_t * r, const uint8_t * g, const uint8_t * b, int size, ggo_palette & palette, int palette_size)
{
	int	i;

	if ( palette_size <= 0 )
	{
		GGO_FAIL();
		return ( true );
	}

	if ( palette_size >= size )
	{
		for ( i = 0; i < size; ++i )
		{
			palette[i].r() = *r++;
			palette[i].g() = *g++;
			palette[i].b() = *b++;
		}
		return ( true );
	}

	return ( false );
}

//////////////////////////////////////////////////////////////
void	ggo_color_reduction_median_cut(const uint8_t * r, const uint8_t * g, const uint8_t * b, int size, ggo_palette & palette, int palette_size, ggo_user_feedback_abc * feedback)
{
	ggo_color				pixel;
	ggo_pixel_block			pixel_block;
	list<ggo_pixel_block>	pixel_blocks;
	int						i;

	palette = ggo_palette( palette_size );

	// Trivial processing.
	if ( ggo_trivial_color_reduction_median_cut( r, g, b, size, palette, palette_size ) == true )
	{
		return;
	}

	// Initialization.
	if ( feedback != NULL )
	{
		feedback->update( 0, palette_size );
	}
	pixel_block._pixels.reserve( size );
	for ( i = 0; i < size; ++i )
	{
		pixel.r() = *r++;
		pixel.g() = *g++;
		pixel.b() = *b++;
		pixel_block._pixels.push_back( pixel );
	}
	pixel_blocks.push_back( pixel_block );

	// Split the pixels.
	while ( distance( pixel_blocks.begin(), pixel_blocks.end() ) < palette_size )
	{
		if ( feedback != NULL )
		{
			feedback->update( distance( pixel_blocks.begin(), pixel_blocks.end() ), palette_size );
		}
		ggo_median_cut_split( pixel_blocks );
	}

	// Fill the palette with the pixels blocks average.
	GGO_ASSERT( distance( pixel_blocks.begin(), pixel_blocks.end() ) == palette_size );
	for ( i = 0; i < palette_size; ++i )
	{
		palette[i] = ggo_pixel_average( pixel_blocks.front()._pixels );

		if ( feedback != NULL )
		{
			feedback->update( i, palette_size );
		}

		pixel_blocks.pop_front();
		if ( pixel_blocks.empty() == true )
		{
			break;
		}
	}
}
