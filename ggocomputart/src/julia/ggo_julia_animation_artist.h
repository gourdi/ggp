#ifndef __GGO_JULIA_ANIMATION_ARTIST__
#define __GGO_JULIA_ANIMATION_ARTIST__

#include <ggo_animation_artist_abc.h>
#include "ggo_julia_artist.h"

namespace ggo
{
  class julia_animation_artist : public animation_artist_abc
  {
  public:

          julia_animation_artist(int width, int height, int line_step, ggo::image_format format, rendering_type rt);

  private:

    void  init_animation() override;
    bool  prepare_frame() override;
    void  render_frame(void * buffer, const ggo::rect_int & clipping) override;

  private:

    ggo::julia_artist	_artist;
    int               _frame_index;
    float				      _angle;
    float				      _radius;
  };
}

#endif
