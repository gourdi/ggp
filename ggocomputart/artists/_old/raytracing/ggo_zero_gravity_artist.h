#ifndef __GGO_ZERO_GRAVITY_ARTIST__
#define __GGO_ZERO_GRAVITY_ARTIST__

#include "ggo_raytracing_artist_abc.h"

class ggo_zero_gravity_artist : public ggo_raytracing_artist_abc
{
public:

							ggo_zero_gravity_artist(int render_width, int render_height, ggo_rendering_mode rendering_mode);

private:
	
	virtual ggo_artist_id  	get_id() const { return GGO_ARTIST_ZERO_GRAVITY; };
	virtual std::string		get_name() const { return "Zero Gravity"; };
};

#endif


