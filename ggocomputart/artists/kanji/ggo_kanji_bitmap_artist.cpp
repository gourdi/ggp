#include "ggo_kanji_bitmap_artist.h"
#include "ggo_kanji_artist.h"

//////////////////////////////////////////////////////////////
ggo::kanji_bitmap_artist::kanji_bitmap_artist(int width, int height, int line_step, ggo::pixel_buffer_format pbf)
:
bitmap_artist_abc(width, height, line_step, pbf)
{
	
}

//////////////////////////////////////////////////////////////
void ggo::kanji_bitmap_artist::render_bitmap(void * buffer) const
{
  ggo::kanji_artist	artist(get_width(), get_height(), get_line_step(), get_pixel_buffer_format());

	artist.init();
  artist.init_output_buffer(buffer);
	
	int frame_index = 0;
	while (artist.render_frame(buffer, frame_index) == true)
	{
		++frame_index;
	}
}