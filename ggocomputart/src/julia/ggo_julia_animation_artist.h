#ifndef __GGO_JULIA_ANIMATION_ARTIST__
#define __GGO_JULIA_ANIMATION_ARTIST__

#include <ggo_animation_artist_abc.h>
#include "ggo_julia_artist.h"

namespace ggo
{
  class julia_animation_artist : public fixed_frames_count_animation_artist_abc
  {
  public:

          julia_animation_artist(int width, int height, int line_step, ggo::image_format format);

  private:

    void  render_frame(void * buffer, int frame_index) override;
    int   frames_count() const override { return 500; }

  private:

    ggo::julia_artist	_artist;
    float				      _angle;
    float				      _radius;
  };
}

#endif
