#ifndef __GGO_ARTIST_FLOWER__
#define __GGO_ARTIST_FLOWER__

#include <ggo_bitmap_artist_abc.h>

namespace ggo
{
  class flower_artist : public bitmap_artist_abc
  {
  public:

          flower_artist(int width, int height, int line_step, ggo::pixel_buffer_format pbf);

  private:

    void  render_bitmap(void * buffer, const bool & quit) const override;
  };
}

#endif


