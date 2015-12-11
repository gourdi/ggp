#ifndef __GGO_VORTEX2_ARTIST__
#define __GGO_VORTEX2_ARTIST__

#include "ggo_bitmap_artist_abc.h"
#include <ggo_2d.h>
#include <ggo_array.h>

class ggo_vortex2_artist
{
public:
	
	struct ggo_vortex
	{
		ggo::point2d_float	_pos;
		float				        _angle;
		float				        _speed;
		float				        _power;
	};
	
	struct ggo_vortex2_params
	{
		ggo::array<ggo_vortex> 	_vortices;
		ggo::color 				      _color1;
		ggo::color 				      _color2;
		ggo::color 				      _color3;
		ggo::color 				      _color4;
		float 					        _split_horz;
		float 					        _split_vert;
	};
	
	static	void	render(uint8_t * buffer, int render_width, int render_height, const ggo_vortex2_params & params);
};

#endif
