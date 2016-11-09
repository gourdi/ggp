#ifndef __GGO_ARTIST_FLOWER__
#define __GGO_ARTIST_FLOWER__

#include <ggo_bitmap_artist_abc.h>

namespace ggo
{
  class flower_artist : public bitmap_artist_abc
  {
  public:

    flower_artist(int render_width, int render_height);

  private:

    void render_bitmap(void * buffer) const override;
  };
}

#endif


