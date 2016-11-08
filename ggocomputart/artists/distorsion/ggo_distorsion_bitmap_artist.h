#ifndef __GGO_DISTORSION_ARTIST__
#define __GGO_DISTORSION_ARTIST__

#include <ggo_bitmap_artist_abc.h>

namespace ggo
{
  class distorsion_bitmap_artist : public bitmap_artist_abc
  {
  public:

    distorsion_bitmap_artist(int render_width, int render_height);

  private:

    void  render_bitmap(void * buffer) const override;
  };
}

#endif
