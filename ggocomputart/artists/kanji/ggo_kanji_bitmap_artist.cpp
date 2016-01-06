#include "ggo_kanji_bitmap_artist.h"

//////////////////////////////////////////////////////////////
ggo_kanji_bitmap_artist::ggo_kanji_bitmap_artist(int render_width, int render_height)
:
ggo_bitmap_artist_abc(render_width, render_height),
_kanji_artist(render_width, render_height)	
{
	
}

//////////////////////////////////////////////////////////////
void ggo_kanji_bitmap_artist::render_bitmap(uint8_t * buffer)
{
	_kanji_artist.init();
  _kanji_artist.init_output_buffer(buffer);
	
	int frame_index = 0;
	while (_kanji_artist.render_frame(buffer, frame_index) == true)
	{
		++frame_index;
	}
}