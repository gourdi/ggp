#ifndef __GGO_VORTEX_ARTIST__
#define __GGO_VORTEX_ARTIST__

#include <ggo_bitmap_artist_abc.h>

namespace ggo
{
  class vortex_artist : public bitmap_artist_abc
  {
  public:

          vortex_artist(int width, int height, int line_step, ggo::pixel_buffer_format pbf);

  private:

    void	render_bitmap(void * buffer, const bool & quit) const override;
  };
}

#endif
