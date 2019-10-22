#ifndef __GGO_PLASTIC_BITMAP_ARTIST__
#define __GGO_PLASTIC_BITMAP_ARTIST__

#include <ggo_bitmap_artist_abc.h>
#include "ggo_plastic_artist.h"

namespace ggo
{
  class plastic_bitmap_artist : public bitmap_artist_abc
  {
  public:

          plastic_bitmap_artist(int width, int height, int line_byte_step, ggo::pixel_type pixel_type, ggo::lines_order memory_lines_order);

    void	render_bitmap(void * buffer) const override;
  };
}

#endif