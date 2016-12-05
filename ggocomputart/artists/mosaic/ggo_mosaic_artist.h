#ifndef __GGO_MOSAIC_ARTIST__
#define __GGO_MOSAIC_ARTIST__

#include <ggo_bitmap_artist_abc.h>

namespace ggo
{
  class mosaic_artist : public bitmap_artist_abc
  {
  public:

          mosaic_artist(int width, int height, int line_step, ggo::pixel_buffer_format pbf);

  private:

    void	render_bitmap(void * buffer, const bool & quit) const override;
  };
}

#endif