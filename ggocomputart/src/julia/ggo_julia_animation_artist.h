#ifndef __GGO_JULIA_ANIMATION_ARTIST__
#define __GGO_JULIA_ANIMATION_ARTIST__

#include <ggo_animation_artist.h>
#include "ggo_julia_artist.h"

namespace ggo
{
  class julia_animation_artist : public progress_animation_artist
  {
  public:

          julia_animation_artist(
            int width, int height, int line_byte_step,
            ggo::pixel_type pixel_type, ggo::lines_order memory_lines_order,
            ggo::ratio fps);

  private:

    void  render_frame(void * buffer, float progress) override;

  private:

    ggo::julia_artist	_artist;
    float				      _angle;
    float				      _radius;
  };
}

#endif
