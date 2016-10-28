#ifndef __GGO_ALPHA_BITMAP_ARTIST__
#define __GGO_ALPHA_BITMAP_ARTIST__

#include "ggo_bitmap_artist_abc.h"

namespace ggo
{
  class alpha_bitmap_artist : public bitmap_artist_abc
  {
  public:

    void render_bitmap(void * buffer, int render_width, int render_height) const override;
  };
}

#endif