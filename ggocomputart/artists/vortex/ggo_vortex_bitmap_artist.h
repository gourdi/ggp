#ifndef __GGO_VORTEX_BITMAP_ARTIST__
#define __GGO_VORTEX_BITMAP_ARTIST__

#include <ggo_bitmap_artist_abc.h>

namespace ggo
{
  class vortex_bitmap_artist : public bitmap_artist_abc
  {
  public:

          vortex_bitmap_artist(int width, int height, int line_step, ggo::pixel_buffer_format pbf);

  private:

    void	render_bitmap(void * buffer) const override;
  };
}

#endif
