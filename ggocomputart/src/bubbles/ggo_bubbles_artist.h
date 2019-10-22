#ifndef __GGO_BUBBLES_ARTIST__
#define __GGO_BUBBLES_ARTIST__

#include "ggo_bitmap_artist_abc.h"

namespace ggo
{
  class bubbles_artist : public bitmap_artist_abc
  {
  public:

          bubbles_artist(int width, int height, int line_step, ggo::pixel_type pixel_type, ggo::lines_order memory_lines_order);

  private:

    void  render_bitmap(void * buffer) const override;

    template <ggo::pixel_type pixel_type, ggo::lines_order memory_lines_order>
    void  render_bitmap_t(void * buffer) const;
  };
}

#endif
