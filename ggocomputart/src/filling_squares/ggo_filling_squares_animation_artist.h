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

    class animated_square : public ggo::position_animate_abc
    {
    public:

            animated_square(const ggo::pos2f & pos, int start_offset) : ggo::position_animate_abc(pos, start_offset) {};

      bool  update(int frame_index, const ggo::pos2f & pos) override;
      void  render(void * buffer, int width, int height, int line_step, ggo::pixel_buffer_format pbf, int frame_index, const ggo::pos2f & pos) const override;

      float											                  _angle;
      ggo::filling_squares_artist::colored_square _colored_square;
    };

    void  init_animation() override;
    bool  prepare_frame() override;
    void  render_frame(void * buffer, const ggo::rect_int & clipping) override;

  private:

    ggo::animator _animator;
    int           _frame_index;
    float         _hue;
    ggo::color_8u _bkgd_color;
  };
}

#endif
