#include "ggo_kanji_animation_artist.h"

//////////////////////////////////////////////////////////////
ggo::kanji_animation_artist::kanji_animation_artist(int width, int height, int line_step, ggo::pixel_buffer_format pbf)
:
accumulation_animation_artist_abc(width, height, line_step, pbf),
_kanji_artist(width, height, line_step, pbf)
{
	
}

//////////////////////////////////////////////////////////////
void ggo::kanji_animation_artist::init_sub()
{
	_kanji_artist.init();
}

//////////////////////////////////////////////////////////////
void ggo::kanji_animation_artist::init_output_buffer(void * buffer) const
{
	_kanji_artist.init_output_buffer(buffer);
}

//////////////////////////////////////////////////////////////
bool ggo::kanji_animation_artist::render_next_frame_acc(void * buffer, int frame_index)
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