#ifndef __GGO_KANJI_ANIMATION_ARTIST__
#define __GGO_KANJI_ANIMATION_ARTIST__

#include "ggo_animation_artist_abc.h"
#include "ggo_kanji_artist.h"

class ggo_kanji_animation_artist : public ggo_accumulation_animation_artist_abc
{
public:
	
        ggo_kanji_animation_artist(int render_width, int render_height);		

private:

	void	init_sub() override;
  void  init_output_buffer(uint8_t * buffer) override;
	bool	render_next_frame_acc(uint8_t * buffer, int frame_index) override;
	
private:
	
	ggo_kanji_artist	_kanji_artist;
};

#endif
