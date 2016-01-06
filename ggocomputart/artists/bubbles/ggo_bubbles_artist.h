#ifndef __GGO_BUBBLES_ARTIST__
#define __GGO_BUBBLES_ARTIST__

#include "ggo_bitmap_artist_abc.h"

class ggo_bubbles_artist : public ggo_bitmap_artist_abc
{
public:

        ggo_bubbles_artist(int render_width, int render_height);

private:

  void  render_bitmap(uint8_t * buffer) override;
};

#endif
