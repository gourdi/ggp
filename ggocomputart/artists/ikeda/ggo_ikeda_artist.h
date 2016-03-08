#ifndef __GGO_IKEDA_ARTIST__
#define __GGO_IKEDA_ARTIST__

#include "ggo_animation_artist_abc.h"
#include <ggo_harmonic_curve.h>

class ggo_ikeda_artist : public ggo_animation_artist_abc
{
public:

        ggo_ikeda_artist(int render_width, int render_height);

  void	init_sub() override;
	bool	render_next_frame_sub(uint8_t * buffer, int frame_index) override;

private:

	struct ggo_particle
	{
		ggo::point2d_float	_pos;
		ggo::color		      _color;
		float				        _radius;
	};

private:

	ggo_particle	create_seed() const;
	
private:
	
	ggo::color				          _bkgd_colors[4];
	ggo::harmonic_curve<float> 	_u0;
	ggo::harmonic_curve<float> 	_u1;
	ggo::harmonic_curve<float> 	_u2;
	float 						          _hue1;
	float 						          _hue2;
	float 						          _range;
	std::vector<ggo_particle> 	_seeds;
};

#endif
