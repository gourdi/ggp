#ifndef __GGO_CRYSTAL_BITMAP_ARTIST__
#define __GGO_CRYSTAL_BITMAP_ARTIST__

#include "ggo_bitmap_artist_abc.h"
#include "ggo_crystal_artist.h"

namespace ggo
{
  class crystal_bitmap_artist : public bitmap_artist_abc
  {
  public:

    crystal_bitmap_artist(int render_width, int render_height);

  private:

    void	render_bitmap(void * buffer) const override;
  };
}

#endif
