#ifndef __GGO_MARBLES_ARTIST__
#define __GGO_MARBLES_ARTIST__

#include <ggo_bitmap_artist_abc.h>

namespace ggo
{
  class marbles_artist : public bitmap_artist_abc
  {
  public:

          marbles_artist(int width, int height, int line_byte_step, ggo::pixel_type pixel_type, ggo::lines_order memory_lines_order);

  private:

    void	render_bitmap(void * buffer) const override;
  };
}

#endif
