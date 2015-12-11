#ifndef __GGO_CRYSTAL_ANIMATION_ARTIST__
#define __GGO_CRYSTAL_ANIMATION_ARTIST__

#include "ggo_animation_artist_abc.h"
#include "ggo_crystal_artist.h"

class ggo_crystal_animation_artist : public ggo_animation_artist_abc
{
public:

        ggo_crystal_animation_artist(int render_width, int render_height);

private:

	void	init_sub() override;
	bool	render_next_frame_sub(uint8_t * buffer, int frame_index) override;

private:

	ggo_crystal_artist				      _artist;
	ggo::color						          _bkgd_colors[4];
	ggo_crystal_artist::ggo_params	_start_params;
	ggo_crystal_artist::ggo_params	_end_params;
};

#endif
