#include "ggo_dupecheck_bitmap_artist.h"
#include "ggo_dupecheck_animation_artist.h"

//////////////////////////////////////////////////////////////
ggo_dupecheck_bitmap_artist::ggo_dupecheck_bitmap_artist(int render_width, int render_height)
:
ggo_bitmap_artist_abc(render_width, render_height)
{
	
}

//////////////////////////////////////////////////////////////
void ggo_dupecheck_bitmap_artist::render_bitmap(uint8_t * buffer)
{
	const int FRAME_INDEX = 100;
	
	ggo_dupecheck_animation_artist artist(get_render_width(), get_render_height());

	artist.init_sub();
	for (int i = 0; i < FRAME_INDEX; ++i)
	{
		artist.render_next_frame_sub(nullptr, i);
	}
	
	artist.render_next_frame_sub(buffer, FRAME_INDEX);
}

