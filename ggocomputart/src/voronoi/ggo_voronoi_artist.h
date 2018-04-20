#ifndef __GGO_VORONOI_ARTIST__
#define __GGO_VORONOI_ARTIST__

#include <ggo_bitmap_artist_abc.h>

namespace ggo
{
  class voronoi_artist : public bitmap_artist_abc
  {
  public:

          voronoi_artist(int width, int height, int line_step, ggo::image_format format);

  private:

    void	render_bitmap(void * buffer) const override;
  };
}

#endif

