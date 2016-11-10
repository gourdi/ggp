#ifndef __GGO_MOSAIC_ARTIST__
#define __GGO_MOSAIC_ARTIST__

#include <ggo_bitmap_artist_abc.h>

namespace ggo
{
  class mosaic_artist : public bitmap_artist_abc
  {
  public:

    mosaic_artist(int render_width, int render_height);

  private:

    void	render_bitmap(void * buffer) const override;
  };
}

#endif