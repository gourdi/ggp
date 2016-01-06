#ifndef __GGO_MOSAIC_ARTIST__
#define __GGO_MOSAIC_ARTIST__

#include "ggo_bitmap_artist_abc.h"

class ggo_mosaic_artist : public ggo_bitmap_artist_abc
{
public:
	
	ggo_mosaic_artist(int render_width, int render_height);

private:

	struct ggo_seed_positions
	{
		float _left, _right, _top, _bottom;
		std::vector<ggo::point2d_float> _points;
	};

private:

          void	render_bitmap(uint8_t * buffer) override;
	
	static	bool	polygon_intersection(const ggo::polygon2d_float & polygon1, const ggo::polygon2d_float & polygon2);
};

#endif