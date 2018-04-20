#ifndef __GGO_PLASTIC_ANIMATION_ARTIST__
#define __GGO_PLASTIC_ANIMATION_ARTIST__

#include <ggo_animation_artist_abc.h>
#include "ggo_plastic_artist.h"

namespace ggo
{
  class plastic_animation_artist : public animation_artist_abc
  {
  public:

          plastic_animation_artist(int width, int height, int line_step, ggo::image_format format, rendering_type rt);

  private:

    void  init_animation() override;
    bool  prepare_frame() override;
    void  render_frame(void * buffer, const ggo::rect_int & clipping) override;

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

    int             _frame_index;
    ggo::color_32f  _color;
    float 		      _altitude_factor;
  };
}

#endif
