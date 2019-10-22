#ifndef __GGO_MOSAIC_ARTIST__
#define __GGO_MOSAIC_ARTIST__

#include <ggo_bitmap_artist_abc.h>

namespace ggo
{
  class mosaic_artist : public bitmap_artist_abc
  {
  public:

          mosaic_artist(int width, int height, int line_byte_step, ggo::pixel_type pixel_type, ggo::lines_order memory_lines_order);

  private:

    void	render_bitmap(void * buffer) const override;
  };
}

#endif