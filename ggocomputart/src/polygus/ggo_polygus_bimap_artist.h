#ifndef __GGO_POLYGUS_BITMAP_ARTIST__
#define __GGO_POLYGUS_BITMAP_ARTIST__

#include <ggo_bitmap_artist_abc.h>
#include <kernel/ggo_vec2.h>

namespace ggo
{
  class polygus_bitmap_artist : public bitmap_artist_abc
  {
    void  render_bitmap(void * buffer, int width, int height, int line_byte_step, ggo::pixel_type pixel_type, ggo::lines_order memory_lines_order) const override;

    template <ggo::pixel_type pixel_type, ggo::lines_order memory_lines_order>
    void  render_polygus(void * buffer) const;

    std::vector<ggo::pos2_f> create_head() const;
  };
}

#endif
