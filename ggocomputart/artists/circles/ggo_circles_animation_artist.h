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

    circles_animation_artist(int render_width, int render_height);

  private:

    void	init_sub() override;
    bool	render_next_frame_sub(void * buffer, int frame_index) override;

  private:

    struct circle_animate : public ggo_position_animate_abc
    {
      circle_animate(const ggo::pos2f & pos, int start_offset) : ggo_position_animate_abc(pos, start_offset) {};

      bool  update(uint8_t * output_buffer, uint8_t * bkgd_buffer, int width, int height, int counter, const ggo::pos2f & pos) override;

      float         _radius;
      ggo::color_8u _color;
      float         _attenuation_factor;
      float         _bounding_factor;
      bool          _bkgd_rendering_allowed;
    };

    ggo_animator  _animator;
  };
}

#endif
