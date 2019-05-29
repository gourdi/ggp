#ifndef __GGO_POUPETTE_ANIMATION_ARTIST__
#define __GGO_POUPETTE_ANIMATION_ARTIST__

#include <ggo_animation_artist_abc.h>
#include "ggo_poupette_artist.h"

namespace ggo
{
  class poupette_animation_artist : public fixed_frames_count_animation_artist_abc
  {
  public:

    poupette_animation_artist(int width, int height, int line_step, ggo::image_format format);

  private:
    
    void  render_frame(void * buffer, int frame_index, float time_step) override;

  private:

    ggo::poupette_artist _artist;
  };
}

#endif