#ifndef __GGO_REX_ARTIST__
#define __GGO_REX_ARTIST__

#include <ggo_bitmap_artist_abc.h>
#include <kernel/ggo_unordered_pair.h>
#include <2d/paint/ggo_color_triangle.h>
#include <2d/paint/ggo_multi_scale_paint.h>

namespace ggo
{
  class rex_bitmap_artist : public bitmap_artist_abc
  {
    void	render_bitmap(void * buffer, int width, int height, int line_byte_step, ggo::pixel_type pixel_type, ggo::lines_order memory_lines_order) const override;
  };
}

#endif
