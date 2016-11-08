#ifndef __GGO_FILLING_SQUARES_ANIMATION_ARTIST__
#define __GGO_FILLING_SQUARES_ANIMATION_ARTIST__

#include "ggo_animation_artist_abc.h"
#include "ggo_filling_squares_artist.h"
#include <ggo_animate_abc.h>
#include <ggo_animator.h>

namespace ggo
{
  class filling_squares_animation_artist : public animation_artist_abc
  {
  public:

    filling_squares_animation_artist(int render_width, int render_height);

  private:

    class animated_square : public ggo_animate_abc
    {
    public:

      animated_square(int start_offset) : ggo_animate_abc(start_offset) {};

      bool update(void * buffer, int width, int height, int counter) override;

      ggo::pos2f  						                    _pos;
      float											                  _angle;
      ggo::filling_squares_artist::colored_square _colored_square;

      static  bool bkgd_rendering_allowed;
    };

    void	init_sub() override;
    bool	render_next_frame_sub(void * buffer, int frame_index) override;

  private:

    ggo_animator  _animator;
    float         _hue;
    ggo::color_8u _bkgd_color;
  };
}

#endif
