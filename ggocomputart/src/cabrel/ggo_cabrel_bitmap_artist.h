#ifndef __GGO_CABREL_BITMAP_ARTIST__
#define __GGO_CABREL_BITMAP_ARTIST__

#include <ggo_bitmap_artist_abc.h>

namespace ggo
{
  class cabrel_bitmap_artist : public bitmap_artist_abc
  {
  public:

          cabrel_bitmap_artist(int width, int height, int line_step, ggo::pixel_type pixel_type, ggo::lines_order memory_lines_order);

  private:

    void  render_bitmap(void * buffer) const override;

    template <ggo::pixel_type pixel_type, ggo::lines_order memory_lines_order>
    void render_bitmap_t(void * buffer) const;
  };
}

#endif
