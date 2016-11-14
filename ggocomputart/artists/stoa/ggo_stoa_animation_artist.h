#ifndef __GGO_STOA_ANIMATION_ARTIST__
#define __GGO_STOA_ANIMATION_ARTIST__

#include <ggo_animation_artist_abc.h>
#include "ggo_stoa_artist.h"

namespace ggo
{
  class stoa_animation_artist : public animation_artist_abc
  {
  public:

    stoa_animation_artist(int render_width, int render_height);

  private:

    void init_sub() override;

    bool render_next_frame_sub(void * buffer, int frame_index) override;

  private:

    std::unique_ptr<ggo::stoa_artist> _artist;
    float                             _hue;
    float                             _light1_angle_start;
    float                             _light1_angle_end;
    float                             _light2_angle_start;
    float                             _light2_angle_end;
  };
}

#endif