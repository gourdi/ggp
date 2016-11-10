#ifndef __GGO_NEON_ARTIST__
#define __GGO_NEON_ARTIST__

#include <ggo_animation_artist_abc.h>

namespace ggo
{
  class neon_artist : public accumulation_animation_artist_abc
  {
  public:

    neon_artist(int render_width, int render_height);

  private:

    void	init_sub() override;
    void	init_output_buffer(void * buffer) const override;
    bool	render_next_frame_sub(void * buffer, int frame_index) override;

    void	paint_point(void * buffer, const ggo::pos2f & point_pos, const ggo::color_8u & color) const;

  private:

    float	_angle;
    float	_radius_prv;
    float	_radius_cur;
    float	_radius_attractor;
    float	_hue_point;
  };
}

#endif
