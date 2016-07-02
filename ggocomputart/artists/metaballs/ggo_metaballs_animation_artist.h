#ifndef __GGO_METABALLS_ANIMATION_ARTIST__
#define __GGO_METABALLS_ANIMATION_ARTIST__

#include "ggo_animation_artist_abc.h"
#include "ggo_metaballs_artist.h"
#include <ggo_point_camera.h>

class ggo_metaballs_animation_artist : public ggo_animation_artist_abc
{
public:

			ggo_metaballs_animation_artist(int render_width, int render_height);

private:
	
	struct ggo_moving_center
	{
		ggo::pos3f  _center;
		float				_radius;
		float				_start_angle1;
		float				_start_angle2;
		float				_end_angle1;
		float				_end_angle2;
	};
	
private:
		
	void	init_sub() override;
	bool	render_next_frame_sub(uint8_t * buffer, int frame_index) override;
	
private:
	
	ggo_metaballs_artist::ggo_metaballs_params 	_params;
	std::vector<ggo_moving_center>				      _centers;
	ggo::antialiasing_point_camera							 _camera;
};

#endif
