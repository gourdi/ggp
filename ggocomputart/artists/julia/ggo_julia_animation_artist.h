#ifndef __GGO_JULIA_ANIMATION_ARTIST__
#define __GGO_JULIA_ANIMATION_ARTIST__

#include <ggo_animation_artist_abc.h>
#include "ggo_julia_artist.h"

namespace ggo
{
  class julia_animation_artist : public animation_artist_abc
  {
  public:

          julia_animation_artist(int width, int height, int line_step, ggo::pixel_buffer_format pbf);

  private:

    void	init_sub() override;
    bool	render_next_frame_sub(void * buffer, int frame_index) override;

  private:

    ggo::julia_artist	_artist;
    float				      _angle;
    float				      _radius;
  };
}

#endif
