#ifndef __GGO_DUFFING_BITMAP_ARTIST__
#define __GGO_DUFFING_BITMAP_ARTIST__

#include "ggo_bitmap_artist_abc.h"

namespace ggo
{
  class duffing_bitmap_artist : public bitmap_artist_abc
  {
  public:

          duffing_bitmap_artist(int width, int height, int line_step, ggo::pixel_buffer_format pbf);

  private:

    void	render_bitmap(void * buffer) const override;

    void	apply_shadow(float * buffer, const float * shadow_buffer) const;
  };
}

#endif
