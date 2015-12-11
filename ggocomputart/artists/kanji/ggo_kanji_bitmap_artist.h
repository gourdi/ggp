#ifndef __GGO_KANJI_BITMAP_ARTIST__
#define __GGO_KANJI_BITMAP_ARTIST__

#include "ggo_bitmap_artist_abc.h"
#include "ggo_kanji_artist.h"

class ggo_kanji_bitmap_artist : public ggo_bitmap_artist_abc
{
public:
	
        ggo_kanji_bitmap_artist(int render_width, int render_height);		

private:

	void	render_bitmap(uint8_t * buffer) override;
	
private:
	
	ggo_kanji_artist	_kanji_artist;
};

#endif
