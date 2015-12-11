#include "ggo_kanji_animation_artist.h"

//////////////////////////////////////////////////////////////
ggo_kanji_animation_artist::ggo_kanji_animation_artist(int render_width, int render_height)
:
ggo_accumulation_animation_artist_abc(render_width, render_height),
_kanji_artist(render_width, render_height)
{
	
}

//////////////////////////////////////////////////////////////
void ggo_kanji_animation_artist::init_sub()
{
	_kanji_artist.init();
}

//////////////////////////////////////////////////////////////
void ggo_kanji_animation_artist::init_output_buffer(uint8_t * buffer)
{
	_kanji_artist.init_output_buffer(buffer);
}

//////////////////////////////////////////////////////////////
bool ggo_kanji_animation_artist::render_next_frame_acc(uint8_t * buffer, int frame_index)
{
	for (int i = 0; i < 50; ++i)
	{
		if (_kanji_artist.render_frame(buffer, 50 * frame_index + i) == false)
		{
			return false;
		}
	}
	
	return true;
}