#ifndef __GGO_STOA_BITMAP_ARTIST__
#define __GGO_STOA_BITMAP_ARTIST__

#include <ggo_bitmap_artist_abc.h>

namespace ggo
{
  class stoa_bitmap_artist : public bitmap_artist_abc
  {
  public:

    stoa_bitmap_artist(int render_width, int render_height);

  private:

    void  render_bitmap(void * buffer) const override;
  };
}

#endif