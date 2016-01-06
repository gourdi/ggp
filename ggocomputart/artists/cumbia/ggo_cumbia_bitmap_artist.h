#ifndef __GGO_CUMBIA_BITMAP_ARTIST__
#define __GGO_CUMBIA_BITMAP_ARTIST__

#include <ggo_bitmap_artist_abc.h>

class ggo_cumbia_bitmap_artist : public ggo_bitmap_artist_abc
{
public:

			ggo_cumbia_bitmap_artist(int render_width, int render_height);

private:

	void	render_bitmap(uint8_t * buffer) override;
};

#endif
