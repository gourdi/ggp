#ifndef __GGO_WORMS_ARTIST__
#define __GGO_WORMS_ARTIST__

#include <ggo_bitmap_artist_abc.h>

namespace ggo
{
  class worms_artist : public bitmap_artist_abc
  {
  public:

          worms_artist(int width, int height, int line_byte_step, ggo::pixel_type pixel_type, ggo::lines_order memory_lines_order);

  private:

    void  render_bitmap(void * buffer) const override;
  };
}

#endif

