#ifndef __GGO_CHRYZODE_BITMAP_ARTIST__
#define __GGO_CHRYZODE_BITMAP_ARTIST__

#include <ggo_bitmap_artist_abc.h>

namespace ggo
{
  class chryzode_bitmap_artist : public bitmap_artist_abc
  {
  public:

          chryzode_bitmap_artist(int width, int height, int line_step, ggo::image_format format);

  private:

    void  render_bitmap(void * buffer) const override;
  };
}

#endif
