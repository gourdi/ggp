#ifndef __GGO_NEON_ARTIST__
#define __GGO_NEON_ARTIST__

#include "ggo_animation_artist_abc.h"

class ggo_neon_artist : public ggo_accumulation_animation_artist_abc
{
public:

        ggo_neon_artist(int render_width, int render_height);

private:

	void	init_sub() override;
  void	init_output_buffer(uint8_t * buffer) override;
	bool	render_next_frame_sub(uint8_t * buffer, int frame_index) override;

	void	paint_point(uint8_t * buffer, const ggo::point2d_float & point_pos, const ggo::color & color) const;

private:

	float	_angle;
	float	_radius_prv;
	float	_radius_cur;
	float	_radius_attractor;
	float	_hue_point;
};

#endif
