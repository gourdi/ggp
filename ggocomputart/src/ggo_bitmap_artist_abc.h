#ifndef __GGO_BITMAP_ARTIST_ABC__
#define __GGO_BITMAP_ARTIST_ABC__

#include <ggo_artist_ids.h>
#include <2d/ggo_pixel_type.h>

namespace ggo
{
  class bitmap_artist_abc
  {
  public:

    static bitmap_artist_abc * create(ggo::bitmap_artist_id artist_id);

    virtual	void  render_bitmap(void * buffer, int width, int height, int line_byte_step, ggo::pixel_type pixel_type, ggo::lines_order memory_lines_order) const = 0;
  };
}

#endif
