#ifndef __GGO_CRYSTAL_BITMAP_ARTIST__
#define __GGO_CRYSTAL_BITMAP_ARTIST__

#include "ggo_bitmap_artist_abc.h"
#include "ggo_crystal_artist.h"

class ggo_crystal_bitmap_artist : public ggo_bitmap_artist_abc
{
public:
	
        ggo_crystal_bitmap_artist(int render_width, int render_height);

private:

	void	render_bitmap(uint8_t * buffer) override;
};

#endif
