#include "ggo_julia_animation_artist.h"

//////////////////////////////////////////////////////////////
ggo::julia_animation_artist::julia_animation_artist(int width, int height, int line_step, ggo::pixel_buffer_format pbf, rendering_type rt)
:
animation_artist_abc(width, height, line_step, pbf, rt),
_artist(width, height, line_step, pbf)
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
