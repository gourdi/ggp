#ifndef __GGO_CUMBIA_ANIMATION_ARTIST__
#define __GGO_CUMBIA_ANIMATION_ARTIST__

#include "ggo_animation_artist_abc.h"
#include "ggo_cumbia_artist.h"
#include <ggo_point_camera.h>

class ggo_cumbia_animation_artist : public ggo_animation_artist_abc
{
public:

			ggo_cumbia_animation_artist(int render_width, int render_height);

private:

	void	init_sub() override;
	bool	render_next_frame_sub(uint8_t * buffer, int frame_index) override;
	
private:
	
	ggo_cumbia_artist	              _artist;
	ggo::antialiasing_point_camera	_camera;
};

#endif
