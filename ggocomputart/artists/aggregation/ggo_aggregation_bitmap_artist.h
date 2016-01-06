#ifndef __GGO_AGGREGATION_BITMAP_ARTIST__
#define __GGO_AGGREGATION_BITMAP_ARTIST__

#include <ggo_bitmap_artist_abc.h>

class ggo_aggregation_bitmap_artist : public ggo_bitmap_artist_abc
{
public:

        ggo_aggregation_bitmap_artist(int render_width, int render_height);

private:

  void	render_bitmap(uint8_t * buffer) override;
};

#endif
