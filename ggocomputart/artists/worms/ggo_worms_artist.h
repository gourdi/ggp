#ifndef __GGO_WORMS_ARTIST__
#define __GGO_WORMS_ARTIST__

#include <ggo_bitmap_artist_abc.h>

class ggo_worms_artist : public ggo_bitmap_artist_abc
{
public:

        ggo_worms_artist(int render_width, int render_height);

private:

	void  render_bitmap(uint8_t * buffer) override;
};

#endif

