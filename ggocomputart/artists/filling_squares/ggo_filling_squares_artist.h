#ifndef __GGO_FILLING_SQUARES_ARTIST__
#define __GGO_FILLING_SQUARES_ARTIST__

#include <vector>
#include <ggo_set2.h>
#include <ggo_color.h>

class ggo_filling_squares_artist
{
public:

	struct ggo_colored_square
	{
		float		    _size;
		ggo::color	_color;
	};
	
	struct ggo_multi_square
	{
		std::vector<ggo_colored_square> _squares;
		ggo::point2d_float				      _pos;
	};

	static	void	build_squares(int render_width, int render_height, float & hue, std::vector<ggo_multi_square> & result);
};

#endif

