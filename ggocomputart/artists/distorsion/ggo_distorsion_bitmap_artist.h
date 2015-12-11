#ifndef __GGO_DISTORSION_ARTIST__
#define __GGO_DISTORSION_ARTIST__

#include <ggo_bitmap_artist_abc.h>
#include <ggo_array.h>

class ggo_distorsion_bitmap_artist : public ggo_bitmap_artist_abc
{
public:

          ggo_distorsion_bitmap_artist(int render_width, int render_height);

private:

    void  render_bitmap(uint8_t * buffer) override;
};

#endif
