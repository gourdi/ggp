#ifndef __GGO_ARTIST_FLOWER__
#define __GGO_ARTIST_FLOWER__

#include <ggo_bitmap_artist_abc.h>

namespace ggo
{
  class flower_bitmap_artist : public bitmap_artist_abc
  {
    void  render_bitmap(void * buffer, int width, int height, int line_byte_step, ggo::pixel_type pixel_type, ggo::lines_order memory_lines_order) const override;
  };
}

#endif


