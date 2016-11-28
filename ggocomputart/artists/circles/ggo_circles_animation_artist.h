#ifndef __GGO_CIRCLES_ANIMATION_ARTIST__
#define __GGO_CIRCLES_ANIMATION_ARTIST__

#include <ggo_animation_artist_abc.h>
#include <ggo_array.h>
#include <ggo_animator.h>
#include <ggo_animate_abc.h>

namespace ggo
{
  class circles_animation_artist : public animation_artist_abc
  {
  public:

          circles_animation_artist(int width, int height, int line_step, ggo::pixel_buffer_format pbf, rendering_type rt);

  private:

    void	init_sub() override;
    bool	render_next_frame_sub(void * buffer, int frame_index) override;

  private:

    struct circle_animate : public ggo::position_animate_abc
    {
            circle_animate(const ggo::pos2f & pos, int start_offset) : ggo::position_animate_abc(pos, start_offset) {};

      bool  update(void * buffer, int width, int height, int line_step, ggo::pixel_buffer_format pbf, int counter, const ggo::pos2f & pos) override;

      float         _radius;
      ggo::color_8u _color;
      float         _attenuation_factor;
      float         _bounding_factor;
    };

    ggo::animator _animator;
    ggo::color_8u _bkgd_color;
  };
}

#endif
