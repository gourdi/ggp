#include "ggo_julia_animation_artist.h"

#define GGO_JULIA_FRAMES_COUNT 500

//////////////////////////////////////////////////////////////
ggo_julia_animation_artist::ggo_julia_animation_artist(int render_width, int render_height)
:
ggo_animation_artist_abc(render_width, render_height),
_artist(render_width, render_height)
{
	
}

//////////////////////////////////////////////////////////////
void ggo_julia_animation_artist::init_sub()
{
	_angle = ggo::rand_float(0, 2 * ggo::PI<float>());
	_radius = ggo::rand_float(0.254f, 0.256f);
}

//////////////////////////////////////////////////////////////
bool ggo_julia_animation_artist::render_next_frame_sub(uint8_t * buffer, int frame_index)
{
	if (frame_index > GGO_JULIA_FRAMES_COUNT)
	{
		return false;
	}

	float angle = ggo::ease_inout(frame_index, GGO_JULIA_FRAMES_COUNT, _angle, _angle + ggo::PI<float>() / 8);

	float x = -1 + _radius * std::cos(angle);
	float y = _radius * std::sin(angle);

	_artist.render_bitmap(buffer, std::complex<float>(x, y), 1);
	
	return true;
}
