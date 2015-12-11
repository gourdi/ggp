#ifndef __GGO_IFS_BITMAP_ARTIST__
#define __GGO_IFS_BITMAP_ARTIST__

#include "ggo_bitmap_artist_abc.h"
#include "ggo_ifs_artist.h"
#include <ggo_2d.h>

class ggo_ifs_bitmap_artist : public ggo_bitmap_artist_abc
{
public:

			ggo_ifs_bitmap_artist(int render_width, int render_height);

private:

	void	render_bitmap(uint8_t * buffer) override;
	
private:
	
	ggo_ifs_artist	_artist;
};

#endif


