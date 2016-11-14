#ifndef __GGO_TOPODOKO_BITMAP_ARTIST__
#define __GGO_TOPODOKO_BITMAP_ARTIST__

#include <ggo_bitmap_artist_abc.h>

namespace ggo
{
  class topodoko_artist : public bitmap_artist_abc
  {
  public:

          topodoko_artist(int render_width, int render_height);

    void	render_bitmap(void * buffer) const override;
  };
}

#endif
