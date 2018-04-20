#ifndef __GGO_TOPODOKO_BITMAP_ARTIST__
#define __GGO_TOPODOKO_BITMAP_ARTIST__

#include <ggo_bitmap_artist_abc.h>

namespace ggo
{
  class topodoko_artist : public bitmap_artist_abc
  {
  public:

          topodoko_artist(int width, int height, int line_step, ggo::image_format format);

    void	render_bitmap(void * buffer) const override;
  };
}

#endif
