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

    filling_squares_animation_artist(int width, int height, int line_step, ggo::pixel_buffer_format pbf, rendering_type rt);

  private:

    class animated_square : public ggo::animate_abc
    {
    public:

            animated_square(int start_offset) : ggo::animate_abc(start_offset) {};

      bool  update(void * buffer, int width, int height, int line_step, ggo::pixel_buffer_format pbf, int counter) override;

      ggo::pos2f  						                    _pos;
      float											                  _angle;
      ggo::filling_squares_artist::colored_square _colored_square;
    };

    void	init_sub() override;
    bool	render_next_frame_sub(void * buffer, int frame_index) override;

  private:

    ggo::animator _animator;
    float         _hue;
    ggo::color_8u _bkgd_color;
  };
}

#endif
