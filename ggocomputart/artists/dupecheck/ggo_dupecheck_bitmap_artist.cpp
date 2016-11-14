#include "ggo_dupecheck_bitmap_artist.h"
#include "ggo_dupecheck_animation_artist.h"

//////////////////////////////////////////////////////////////
ggo::dupecheck_bitmap_artist::dupecheck_bitmap_artist(int render_width, int render_height)
:
bitmap_artist_abc(render_width, render_height)
{
	
}

//////////////////////////////////////////////////////////////
void ggo::dupecheck_bitmap_artist::render_bitmap(void * buffer) const
{
	const int frame_index = 100;
	
	ggo::dupecheck_animation_artist artist(get_render_width(), get_render_height());

	artist.init_sub();
	for (int i = 0; i < frame_index; ++i)
	{
		artist.render_next_frame_sub(nullptr, i);
	}
	
	artist.render_next_frame_sub(buffer, frame_index);
}

