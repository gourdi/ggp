#ifndef __GGO_JULIA_ANIMATION_ARTIST__
#define __GGO_JULIA_ANIMATION_ARTIST__

#include <ggo_animation_artist_abc.h>
#include "ggo_julia_artist.h"

namespace ggo
{
  class julia_animation_artist : public animation_artist_abc
  {
  public:

          julia_animation_artist(int width, int height, int line_step, ggo::pixel_buffer_format pbf, rendering_type rt);

  private:

    void  init() override;
    bool  update() override;
    void  render_frame(void * buffer, const ggo::pixel_rect & clipping) override;

  private:

    ggo::julia_artist	_artist;
    int               _frame_index;
    float				      _angle;
    float				      _radius;
  };
}

#endif
