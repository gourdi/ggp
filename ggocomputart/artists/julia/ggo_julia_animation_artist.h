#ifndef __GGO_JULIA_ANIMATION_ARTIST__
#define __GGO_JULIA_ANIMATION_ARTIST__

#include "ggo_animation_artist_abc.h"
#include "ggo_julia_artist.h"

class ggo_julia_animation_artist : public ggo_animation_artist_abc
{
public:

			ggo_julia_animation_artist(int render_width, int render_height);

private:

	void	init_sub() override;
	bool	render_next_frame_sub(uint8_t * buffer, int frame_index) override;
	
private:
	
	ggo_julia_artist	_artist;
	float				      _angle;
	float				      _radius;
};

#endif
