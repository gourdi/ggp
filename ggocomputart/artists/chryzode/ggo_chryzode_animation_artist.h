#ifndef __GGO_CHRYZODE_ANIMATION_ARTIST__
#define __GGO_CHRYZODE_ANIMATION_ARTIST__

#include <ggo_animation_artist_abc.h>
#include "ggo_chryzode_artist.h"

namespace ggo
{
  class chryzode_animation_artist : public animation_artist_abc
  {
  public:

          chryzode_animation_artist(int width, int height, int line_step, ggo::pixel_buffer_format pbf);

  private:

    void  init_sub() override;
    bool  render_next_frame_sub(void * buffer, int frame_index) override;

  private:

    chryzode_params _params;
    float _hue_start = 0;
    float _hue_end = 0;
    float _multiplier1_start = 0;
    float _multiplier1_end = 0;
    float _multiplier2_start = 0;
    float _multiplier2_end = 0;
    float _offset1_start = 0;
    float _offset1_end = 0;
    float _offset2_start = 0;
    float _offset2_end = 0;
    int _modulo = 0;
    float _radius_start = 0;
    float _radius_end = 0;
  };
}

#endif
