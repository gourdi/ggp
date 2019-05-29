#ifndef __GGO_PLASTIC_ANIMATION_ARTIST__
#define __GGO_PLASTIC_ANIMATION_ARTIST__

#include <ggo_animation_artist_abc.h>
#include "ggo_plastic_artist.h"

namespace ggo
{
  class plastic_animation_artist : public fixed_frames_count_animation_artist_abc
  {
  public:

          plastic_animation_artist(int width, int height, int line_step, ggo::image_format format);

  private:

    void  render_frame(void * buffer, int frame_index, float time_step) override;

  private:

    struct anim_plastic_params
    {
      float	_den;
      float	_mult;
      float _power;
      float	_center_x;
      float	_center_y;
      float	_angle;
      float	_radius;
      float	_dangle;
    };
    std::vector<anim_plastic_params> _params;

    ggo::rgb_32f  _color;
    float 		    _altitude_factor;
  };
}

#endif
