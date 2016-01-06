#include "ggo_rotate.h"
#include "../ggo_2d_helpers.h"
#include "../ggo_pixel_interpolation.h"
#include <math.h>

//////////////////////////////////////////////////////////////
void ggo_rotate(const uint8_t * input, int width_in, int height_in, uint8_t * output, int width_out, int height_out, float angle, uint8_t backgrnd)
{
	int		x, y;
	float	src_x, src_y, dx, dy, cur_dist, cur_angle;

	for ( y = 0; y < height_out; ++y )
	{
		for ( x = 0; x < width_out; ++x )
		{
			dx			= x- width_out/2;
			dy			= y-height_out/2;
			cur_dist	= sqrt(dx*dx+dy*dy);
			cur_angle	= atan2(dy, dx);

			src_x =  width_in/2 + cos(cur_angle+angle)*cur_dist;
			src_y = height_in/2 + sin(cur_angle+angle)*cur_dist;

			*output++ = ggo_interpolate_backgrnd(input, width_in, height_in, src_x, src_y, 1, backgrnd);
		}
	}
}

//////////////////////////////////////////////////////////////
void ggo_flip_horz(uint8_t * buffer, int width, int height, ggo_user_feedback_abc * feedback)
{
	int					x, y;
	ggo_array_uint8	clone( buffer, width*height );

	for ( y = 0; y < height; ++y )
	{
		if ( ( feedback != NULL ) && ( feedback->update( y, height ) == false ) )
		{
			break;
		}

		for ( x = 0; x < width; ++x )
		{
			buffer[ ggo_get_2d_index(x,y,width) ] = clone[ ggo_get_2d_index(width-x-1,y,width) ];
		}
	}
}

//////////////////////////////////////////////////////////////
void ggo_flip_vert(uint8_t * buffer, int width, int height, ggo_user_feedback_abc * feedback)
{
	int				x, y;
	ggo_array_uint8	clone( buffer, width*height );

	for ( y = 0; y < height; ++y )
	{
		if ( ( feedback != NULL ) && ( feedback->update( y, height ) == false ) )
		{
			break;
		}

		for ( x = 0; x < width; ++x )
		{
			buffer[ ggo_get_2d_index(x,y,width) ] = clone[ ggo_get_2d_index(x,height-y-1,width) ];
		}
	}
}

//////////////////////////////////////////////////////////////
void ggo_rotate_90_cw(const uint8_t * in, uint8_t * out, int width, int height, ggo_user_feedback_abc * feedback)
{
	int	x, y;

	for ( y = 0; y < height; ++y )
	{
		if ( ( feedback != NULL ) && ( feedback->update( y, height ) == false ) )
		{
			break;
		}

		for ( x = 0; x < width; ++x )
		{
			out[ ggo_get_2d_index(height-y-1,x,height) ] = in[ ggo_get_2d_index(x,y,width) ];
		}
	}
}

//////////////////////////////////////////////////////////////
void ggo_rotate_90_ccw(const uint8_t * in, uint8_t * out, int width, int height, ggo_user_feedback_abc * feedback)
{
	int	x, y;

	for ( y = 0; y < height; ++y )
	{
		if ( ( feedback != NULL ) && ( feedback->update( y, height ) == false ) )
		{
			break;
		}

		for ( x = 0; x < width; ++x )
		{
			out[ ggo_get_2d_index(y,width-x-1,height) ] = in[ ggo_get_2d_index(x,y,width) ];
		}
	}
}

//////////////////////////////////////////////////////////////
void ggo_rotate_180(const uint8_t * in, uint8_t * out, int width, int height, ggo_user_feedback_abc * feedback)
{
	int	x, y;

	for ( y = 0; y < height; ++y )
	{
		if ( ( feedback != NULL ) && ( feedback->update( y, height ) == false ) )
		{
			break;
		}

		for ( x = 0; x < width; ++x )
		{
			out[ ggo_get_2d_index(x,y,width) ] = in[ ggo_get_2d_index(width-x-1,height-y-1,width) ];
		}
	}
}


