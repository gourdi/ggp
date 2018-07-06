#include "ggo_julia_bitmap_artist.h"
#include "ggo_julia_artist.h"

//////////////////////////////////////////////////////////////
ggo::julia_bitmap_artist::julia_bitmap_artist(int width, int height, int line_step, ggo::image_format format)
:
bitmap_artist_abc(width, height, line_step, format)
{

}

//////////////////////////////////////////////////////////////
void ggo::julia_bitmap_artist::render_bitmap(void * buffer) const
{	
	ggo::julia_artist	artist(width(), height(), line_step(), format());
	
	artist.render_bitmap(buffer, ggo::julia_artist::pickup_seed());
}
