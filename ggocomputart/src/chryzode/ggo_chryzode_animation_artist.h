#ifndef __GGO_CHRYZODE_ANIMATION_ARTIST__
#define __GGO_CHRYZODE_ANIMATION_ARTIST__

#include <ggo_animation_artist_abc.h>
#include "ggo_chryzode_artist.h"

namespace ggo
{
  class chryzode_animation_artist : public fixed_frames_count_animation_artist_abc
  {
  public:

          chryzode_animation_artist(int width, int height, int line_step, ggo::image_format format);

  private:

    void  render_frame(void * buffer, int frame_index) override;
    int   frames_count() const override { return 300; }

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
