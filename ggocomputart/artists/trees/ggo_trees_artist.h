#ifndef __GGO_TREES_ARTIST__
#define __GGO_TREES_ARTIST__

#include <ggo_bitmap_artist_abc.h>

namespace ggo
{
  class trees_artist : public bitmap_artist_abc
  {
  public:

          trees_artist(int width, int height, int line_step, ggo::pixel_buffer_format pbf);

  private:

    void  render_bitmap(void * buffer) const override;
  };
}

#endif
