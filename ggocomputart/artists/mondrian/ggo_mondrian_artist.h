#ifndef __GGO_BITMAP_ARTIST_MONDRIAN__
#define __GGO_BITMAP_ARTIST_MONDRIAN__

#include <ggo_bitmap_artist_abc.h>

namespace ggo
{
  class mondrian_artist : public bitmap_artist_abc
  {
  public:

    mondrian_artist(int render_width, int render_height);

  private:

    void	render_bitmap(void * buffer) const override;
  };
}

#endif

