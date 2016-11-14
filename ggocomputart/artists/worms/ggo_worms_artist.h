#ifndef __GGO_WORMS_ARTIST__
#define __GGO_WORMS_ARTIST__

#include <ggo_bitmap_artist_abc.h>

namespace ggo
{
  class worms_artist : public bitmap_artist_abc
  {
  public:

          worms_artist(int render_width, int render_height);

  private:

    void  render_bitmap(void * buffer) const override;
  };
}

#endif

