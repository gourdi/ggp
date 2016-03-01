#ifndef __GGO_JULIA_BITMAP_ARTIST__
#define __GGO_JULIA_BITMAP_ARTIST__

#include "ggo_bitmap_artist_abc.h"
#include <complex>
#include <ggo_array.h>

class ggo_julia_bitmap_artist : public ggo_bitmap_artist_abc
{
public:
	
        ggo_julia_bitmap_artist(int render_width, int render_height);

private:

	void	render_bitmap(uint8_t * buffer) override;
};

#endif