#ifndef __GGO_MARBLES_ARTIST__
#define __GGO_MARBLES_ARTIST__

#include <ggo_bitmap_artist_abc.h>

namespace ggo
{
  class marbles_artist : public bitmap_artist_abc
  {
  public:

    marbles_artist(int render_width, int render_height);

  private:

    void	render_bitmap(void * buffer) const override;
  };
}

#endif
