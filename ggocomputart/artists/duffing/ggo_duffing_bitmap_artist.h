#ifndef __GGO_DUFFING_BITMAP_ARTIST__
#define __GGO_DUFFING_BITMAP_ARTIST__

#include "ggo_bitmap_artist_abc.h"
#include <ggo_array.h>

class ggo_duffing_bitmap_artist : public ggo_bitmap_artist_abc
{
public:

			ggo_duffing_bitmap_artist(int render_width, int render_height);

private:

	void	render_bitmap(uint8_t * buffer) override;
	
	void	apply_shadow(float * buffer, const float * shadow_buffer) const;
};

#endif
