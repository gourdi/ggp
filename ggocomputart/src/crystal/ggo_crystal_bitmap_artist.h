#ifndef __GGO_CRYSTAL_BITMAP_ARTIST__
#define __GGO_CRYSTAL_BITMAP_ARTIST__

#include "ggo_bitmap_artist_abc.h"
#include "ggo_crystal_artist.h"

namespace ggo
{
  class crystal_bitmap_artist : public bitmap_artist_abc
  {
  public:

          crystal_bitmap_artist(int width, int height, int line_step, ggo::pixel_type pixel_type, ggo::lines_order memory_lines_order);

  private:

    void	render_bitmap(void * buffer) const override;
  };
}

#endif
