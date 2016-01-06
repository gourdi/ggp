#ifndef __GGO_DUFFING_ANIMATION_ARTIST__
#define __GGO_DUFFING_ANIMATION_ARTIST__

#include "ggo_animation_artist_abc.h"
#include <ggo_array.h>
#include <ggo_curve.h>

class ggo_duffing_animation_artist : public ggo_animation_artist_abc
{
public:

                      ggo_duffing_animation_artist(int render_width, int render_height);

private:

	void				        init_sub() override;
	bool				        render_next_frame_sub(uint8_t * buffer, int frame_index) override;

	void				        apply_shadow(float * buffer, const float * shadow_buffer) const;
	
	ggo::point2d_float	apply_duffing(float t, float dt, float angle_offset, ggo::point2d_float & point) const;

private:

	ggo::cubic_curve_float			    _hue_curve;
	ggo::cubic_curve_float			    _sat_curve;
	ggo::cubic_curve_float			    _val_curve1;
	ggo::cubic_curve_float			    _val_curve2;
	ggo::cubic_curve_float			    _val_curve3;
	ggo::cubic_curve_float			    _val_curve4;
	std::vector<ggo::point2d_float>	_points;
};

#endif
