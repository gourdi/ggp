#include "ggo_julia_bitmap_artist.h"
#include "ggo_julia_artist.h"

//////////////////////////////////////////////////////////////
ggo_julia_bitmap_artist::ggo_julia_bitmap_artist(int render_width, int render_height)
:
ggo_bitmap_artist_abc(render_width, render_height)
{

}

//////////////////////////////////////////////////////////////
void ggo_julia_bitmap_artist::render_bitmap(uint8_t * buffer)
{	
	ggo_julia_artist	artist(get_render_width(), get_render_height());
	
	artist.render_bitmap(buffer, ggo_julia_artist::pickup_seed());
}
