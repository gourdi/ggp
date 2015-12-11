#ifndef __GGO_BITMAP_ARTIST_ABC__
#define __GGO_BITMAP_ARTIST_ABC__

#include <ggo_artist_abc.h>
#include <ggo_artist_ids.h>

//////////////////////////////////////////////////////////////
class ggo_bitmap_artist_abc : public ggo_artist_abc
{
public:
	
	static  ggo_bitmap_artist_abc *	create(ggo_bitmap_artist_id artist_id, int render_width, int render_height);
    
  virtual	void                    render_bitmap(uint8_t * buffer) = 0;

protected:
	
	ggo_bitmap_artist_abc(int render_width, int render_height);
};

#endif
