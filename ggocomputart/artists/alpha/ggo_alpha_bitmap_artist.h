#ifndef __GGO_ALPHA_BITMAP_ARTIST__
#define __GGO_ALPHA_BITMAP_ARTIST__

#include "ggo_bitmap_artist_abc.h"

class ggo_alpha_bitmap_artist : public ggo_bitmap_artist_abc
{
public:
	
        ggo_alpha_bitmap_artist(int render_width, int render_height);
	
	void	render_bitmap(uint8_t * buffer) override;
};

#endif