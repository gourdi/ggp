#include "ggo_julia_bitmap_artist.h"
#include "ggo_julia_artist.h"

//////////////////////////////////////////////////////////////
ggo::julia_bitmap_artist::julia_bitmap_artist(int width, int height, int line_step, ggo::pixel_buffer_format pbf)
:
bitmap_artist_abc(width, height, line_step, pbf)
{

}

//////////////////////////////////////////////////////////////
void ggo::julia_bitmap_artist::render_bitmap(void * buffer, const bool & quit) const
{	
	ggo::julia_artist	artist(get_width(), get_height());
	
	artist.render_bitmap(buffer, get_line_step(), get_pixel_buffer_format(), ggo::julia_artist::pickup_seed());
}
