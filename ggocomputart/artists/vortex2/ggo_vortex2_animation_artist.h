#ifndef __GGO_VORTEX2_ANIM_ARTIST__
#define __GGO_VORTEX2_ANIM_ARTIST__

#include "ggo_animation_artist_abc.h"
#include "ggo_vortex2_artist.h"
#include <map>

class ggo_vortex2_animation_artist : public ggo_animation_artist_abc
{
public:

        ggo_vortex2_animation_artist(int render_width, int render_height);

private:
	
	void	init_sub() override;
	bool	render_next_frame_sub(uint8_t * buffer, int frame_index) override;
	
private:
	
	struct ggo_vortex_path
	{
		ggo::point2d_float	_start_pos;
		ggo::point2d_float	_end_pos;
	};
	
	std::map<ggo_vortex2_artist::ggo_vortex *, ggo_vortex_path>	_vortices_paths;
	ggo_vortex2_artist::ggo_vortex2_params 						          _params;
};

#endif
