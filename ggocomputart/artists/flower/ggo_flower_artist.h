#ifndef __GGO_ARTIST_FLOWER__
#define __GGO_ARTIST_FLOWER__

#include "ggo_bitmap_artist_abc.h"

class ggo_flower_artist : public ggo_bitmap_artist_abc
{
public:

			ggo_flower_artist(int render_width, int render_height);

private:

	void	render_bitmap(uint8_t * buffer) override;
};

#endif


