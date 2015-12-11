#ifndef __GGO_PLASTIC_ARTIST__
#define __GGO_PLASTIC_ARTIST__

#include "ggo_bitmap_artist_abc.h"
#include <ggo_color.h>
#include <vector>

class ggo_plastic_artist : public ggo_artist_abc
{
public:
	
	struct ggo_plastic_params
	{
		float _pos_x;
		float _pos_y;
		float _den;
		float _mult;
		float _power;
	};
	
public:
	
        ggo_plastic_artist(int render_width, int render_height);
						
	void	render(uint8_t * buffer, const std::vector<ggo_plastic_params> & params, const ggo::color & color, float altitude_factor) const;
	
private:

	float	evaluate(float x, float y, const std::vector<ggo_plastic_params> & params) const;
};

#endif