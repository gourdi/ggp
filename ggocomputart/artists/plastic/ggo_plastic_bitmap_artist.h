#ifndef __GGO_PLASTIC_BITMAP_ARTIST__
#define __GGO_PLASTIC_BITMAP_ARTIST__

#include "ggo_bitmap_artist_abc.h"
#include "ggo_plastic_artist.h"
#include <ggo_color.h>
#include <vector>

class ggo_plastic_bitmap_artist : public ggo_bitmap_artist_abc
{
public:
	
        ggo_plastic_bitmap_artist(int render_width, int render_height);
						
	void	render_bitmap(uint8_t * buffer) override;
};

#endif