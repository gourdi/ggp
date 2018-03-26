#ifndef __GGO_DUPECHECK_BITMAP_ARTIST__
#define __GGO_DUPECHECK_BITMAP_ARTIST__

#include <ggo_bitmap_artist_abc.h>

namespace ggo
{
  class dupecheck_bitmap_artist : public bitmap_artist_abc
  {
  public:

          dupecheck_bitmap_artist(int width, int height, int line_step, ggo::pixel_buffer_format pbf);

    void	render_bitmap(void * buffer) const override;
  };
}

#endif