#ifndef __GGO_MARBLES_ARTIST__
#define __GGO_MARBLES_ARTIST__

#include "ggo_bitmap_artist_abc.h"

class ggo_marbles_artist : public ggo_bitmap_artist_abc
{
public:
	
        ggo_marbles_artist(int render_width, int render_height);
	
private:
	
	void	render_bitmap(uint8_t * buffer) override;
};

#endif
