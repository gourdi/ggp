#include "ggo_dupecheck_bitmap_artist.h"
#include "ggo_dupecheck_animation_artist.h"

//////////////////////////////////////////////////////////////
ggo::dupecheck_bitmap_artist::dupecheck_bitmap_artist(int width, int height, int line_step, ggo::pixel_buffer_format pbf)
:
bitmap_artist_abc(width, height, line_step, pbf)
{
	
}

//////////////////////////////////////////////////////////////
void ggo::dupecheck_bitmap_artist::render_bitmap(void * buffer) const
{
	const int frame_index = 100;
	
	ggo::dupecheck_animation_artist artist(get_width(), get_height(), get_line_step(), get_pixel_buffer_format());

	artist.init_sub();
	for (int i = 0; i < frame_index; ++i)
	{
		artist.render_next_frame_sub(nullptr, i);
	}
	
	artist.render_next_frame_sub(buffer, frame_index);
}

