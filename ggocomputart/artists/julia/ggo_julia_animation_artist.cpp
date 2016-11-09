#include "ggo_julia_animation_artist.h"

//////////////////////////////////////////////////////////////
ggo::julia_animation_artist::julia_animation_artist(int render_width, int render_height)
:
animation_artist_abc(render_width, render_height),
_artist(render_width, render_height)
{
	
}

//////////////////////////////////////////////////////////////
void ggo::julia_animation_artist::init_sub()
{
	_angle = ggo::rand<float>(0, 2 * ggo::pi<float>());
	_radius = ggo::rand<float>(0.254f, 0.256f);
}

//////////////////////////////////////////////////////////////
bool ggo::julia_animation_artist::render_next_frame_sub(void * buffer, int frame_index)
{
  const int frames_count = 500;


	if (frame_index > frames_count)
	{
		return false;
	}

	float angle = ggo::ease_inout(frame_index, frames_count, _angle, _angle + ggo::pi<float>() / 8);

	float x = -1 + _radius * std::cos(angle);
	float y = _radius * std::sin(angle);

	_artist.render_bitmap(buffer, std::complex<float>(x, y), 1);
	
	return true;
}
