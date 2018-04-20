#ifndef __GGO_JULIA_BITMAP_ARTIST__
#define __GGO_JULIA_BITMAP_ARTIST__

#include <ggo_bitmap_artist_abc.h>

namespace ggo
{
  class julia_bitmap_artist : public bitmap_artist_abc
  {
  public:

          julia_bitmap_artist(int width, int height, int line_step, ggo::image_format format);

  private:

    void	render_bitmap(void * buffer) const override;
  };
}

#endif