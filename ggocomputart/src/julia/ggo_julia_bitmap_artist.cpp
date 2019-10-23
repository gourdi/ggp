#include "ggo_julia_bitmap_artist.h"
#include "ggo_julia_artist.h"

//////////////////////////////////////////////////////////////
void ggo::julia_bitmap_artist::render_bitmap(void * buffer, int width, int height, int line_byte_step, ggo::pixel_type pixel_type, ggo::lines_order memory_lines_order) const
{	
	ggo::julia_artist	artist;
	
	artist.render_bitmap(buffer, width, height, line_byte_step, pixel_type, memory_lines_order, ggo::julia_artist::pickup_seed());
}
