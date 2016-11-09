#include "ggo_kanji_bitmap_artist.h"
#include "ggo_kanji_artist.h"

//////////////////////////////////////////////////////////////
ggo::kanji_bitmap_artist::kanji_bitmap_artist(int render_width, int render_height)
:
bitmap_artist_abc(render_width, render_height)
{
	
}

//////////////////////////////////////////////////////////////
void ggo::kanji_bitmap_artist::render_bitmap(void * buffer) const
{
  ggo::kanji_artist	artist(get_render_width(), get_render_height());

	artist.init();
  artist.init_output_buffer(buffer);
	
	int frame_index = 0;
	while (artist.render_frame(buffer, frame_index) == true)
	{
		++frame_index;
	}
}