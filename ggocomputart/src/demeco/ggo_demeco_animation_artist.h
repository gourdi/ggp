#ifndef __GGO_DEMECO_ANIMATION_ARTIST__
#define __GGO_DEMECO_ANIMATION_ARTIST__

#include <ggo_animation_artist_abc.h>
#include "ggo_demeco_artist.h"
#include <2d/ggo_image.h>

namespace ggo
{
  class demeco_animation_artist : public animation_artist_abc
  {
  public:

    demeco_animation_artist(int width, int height, int line_step, ggo::image_format format);

  private:

    void  render_frame(void * buffer, int frame_index, float time_step, bool & finished) override;

  private:

    demeco_artist<ggo::rgba_8u_yd, ggo::sampling_8x8> _artist;
    ggo::image _render_image;
    ggo::image _shadow_image;
  };
}

#endif