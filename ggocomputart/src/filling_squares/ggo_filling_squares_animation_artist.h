#ifndef __GGO_FILLING_SQUARES_ANIMATION_ARTIST__
#define __GGO_FILLING_SQUARES_ANIMATION_ARTIST__

#include "ggo_animation_artist_abc.h"
#include "ggo_filling_squares_artist.h"
#include <animation/ggo_animate_abc.h>
#include <animation/ggo_animator.h>

namespace ggo
{
  class filling_squares_animation_artist : public fixed_frames_count_animation_artist_abc
  {
  public:

    filling_squares_animation_artist(int width, int height, int line_step, ggo::image_format format);

  private:

    class animated_square : public ggo::position_animate_abc
    {
    public:

            animated_square(const ggo::pos2_f & pos, int start_offset, float angle, filling_squares_artist::colored_square colored_square);

      bool  update(int frame_index, const ggo::pos2_f & pos) override;
      void  render(void * buffer, int width, int height, int line_step, ggo::image_format format, const ggo::rect_int & clipping, int frame_index, const ggo::pos2_f & pos) const override;

    private:

      float											                  _angle;
      ggo::filling_squares_artist::colored_square _colored_square;
    };

    void  render_frame(void * buffer, int frame_index, float time_step) override;

  private:

    ggo::animator _animator;
    float         _hue = 0.f;
    ggo::rgb_8u   _bkgd_color;
  };
}

#endif
