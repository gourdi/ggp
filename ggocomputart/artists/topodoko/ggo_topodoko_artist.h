#ifndef __GGO_TOPODOKO_BITMAP_ARTIST__
#define __GGO_TOPODOKO_BITMAP_ARTIST__

#include "ggo_bitmap_artist_abc.h"

class ggo_topodoko_artist : public ggo_bitmap_artist_abc
{
public:

        ggo_topodoko_artist(int render_width, int render_height);

	void	render_bitmap(uint8_t * buffer) override;
 
private:
	
	struct ggo_color_square
	{
		ggo::polygon2d_float	_square;
		ggo::color		        _color;
	};
	
	std::vector<ggo_color_square>	_color_squares;
	float							            _shadow_offset;
	ggo::color						        _bkgd_color1;
	ggo::color						        _bkgd_color2;
	ggo::color						        _bkgd_color3;
	ggo::color						        _bkgd_color4;
};

#endif
