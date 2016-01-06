#ifndef __GGO_PLASTIC_ANIMATION_ARTIST__
#define __GGO_PLASTIC_ANIMATION_ARTIST__

#include "ggo_animation_artist_abc.h"
#include "ggo_plastic_artist.h"

class ggo_plastic_animation_artist : public ggo_animation_artist_abc
{
public:

			ggo_plastic_animation_artist(int render_width, int render_height);

private:

	void	init_sub() override;
	bool	render_next_frame_sub(uint8_t * buffer, int frame_index) override;

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

	ggo::color  _color;
	float 		  _altitude_factor;
};

#endif
