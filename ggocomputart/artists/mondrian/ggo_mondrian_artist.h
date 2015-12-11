#ifndef __GGO_BITMAP_ARTIST_MONDRIAN__
#define __GGO_BITMAP_ARTIST_MONDRIAN__

#include "ggo_bitmap_artist_abc.h"
#include <ggo_2d.h>

class ggo_mondrian_artist : public ggo_bitmap_artist_abc
{
public:

        ggo_mondrian_artist(int render_width, int render_height);

private:

	void	render_bitmap(uint8_t * buffer) override;

private:

	struct ggo_area
	{
		ggo::polygon2d_float  _rect;
		ggo::color			      _color;
	};
};

#endif

