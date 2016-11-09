#include "ggo_julia_bitmap_artist.h"
#include "ggo_julia_artist.h"

//////////////////////////////////////////////////////////////
ggo::julia_bitmap_artist::julia_bitmap_artist(int render_width, int render_height)
:
bitmap_artist_abc(render_width, render_height)
{

}

//////////////////////////////////////////////////////////////
void ggo::julia_bitmap_artist::render_bitmap(void * buffer) const
{	
	ggo::julia_artist	artist(get_render_width(), get_render_height());
	
	artist.render_bitmap(buffer, ggo::julia_artist::pickup_seed());
}
