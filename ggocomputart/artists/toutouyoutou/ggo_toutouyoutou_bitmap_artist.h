#ifndef __GGO_TOUTOUYOUTOU_BITMPA_ARTIST__
#define __GGO_TOUTOUYOUTOU_BITMPA_ARTIST__

#include <ggo_bitmap_artist_abc.h>

namespace ggo
{
  class toutouyoutou_bitmap_artist : public bitmap_artist_abc
  {
  public:

          toutouyoutou_bitmap_artist(int render_width, int render_height);

  private:

    void  render_bitmap(void * buffer) const override;
  };
}

#endif
