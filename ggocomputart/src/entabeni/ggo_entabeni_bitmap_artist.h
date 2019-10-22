#ifndef __GGO_ENTABENI_BITMAP_ARTIST__
#define __GGO_ENTABENI_BITMAP_ARTIST__

#include <ggo_bitmap_artist_abc.h>

namespace ggo
{
  class entabeni_bitmap_artist : public bitmap_artist_abc
  {
  public:

          entabeni_bitmap_artist(int width, int height, int line_byte_step, ggo::pixel_type pixel_type, ggo::lines_order memory_lines_order);

  private:

    void  render_bitmap(void * buffer) const override;
  };
}

#endif
