#ifndef __GGO_IFS_ANIMATION_ARTIST__
#define __GGO_IFS_ANIMATION_ARTIST__

#include "ggo_animation_artist_abc.h"
#include "ggo_ifs_artist.h"
#include <ggo_2d.h>

class ggo_ifs_animation_artist : public ggo_animation_artist_abc
{
public:

			ggo_ifs_animation_artist(int render_width, int render_height);

private:

	void	init_sub() override;
	bool	render_next_frame_sub(uint8_t * buffer, int frame_index) override;

private:
	
	ggo_ifs_artist	_artist;
	ggo::color	    _bkgd_colors[4];
	float			      _hue;
	float 		      _transform_start[4];
	float 		      _transform_end[4];
	float			      _angle1_start;
	float			      _angle1_end;
	float			      _angle2_start;
	float			      _angle2_end;
};

#endif


