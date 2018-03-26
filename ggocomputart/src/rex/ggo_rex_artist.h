#ifndef __GGO_REX_ARTIST__
#define __GGO_REX_ARTIST__

#include <ggo_bitmap_artist_abc.h>

namespace ggo
{
  class rex_artist : public bitmap_artist_abc
  {
  public:

          rex_artist(int width, int height, int line_step, ggo::pixel_buffer_format pbf);

  private:

    void	render_bitmap(void * buffer) const override;
  };
}

#endif
