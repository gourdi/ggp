#ifndef __GGO_FILLING_SQUARES_BITMAP_ARTIST__
#define __GGO_FILLING_SQUARES_BITMAP_ARTIST__

#include "ggo_bitmap_artist_abc.h"

namespace ggo
{
  class filling_squares_bitmap_artist : public bitmap_artist_abc
  {
    void	render_bitmap(void * buffer, int width, int height, int line_byte_step, ggo::pixel_type pixel_type, ggo::lines_order memory_lines_order) const override;
  };
}

#endif
