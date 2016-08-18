#include "ggo_cumbia_animation_artist.h"
#include "ggo_antialiasing_renderer.h"

//////////////////////////////////////////////////////////////
ggo_cumbia_animation_artist::ggo_cumbia_animation_artist(int render_width, int render_height)
:
ggo_animation_artist_abc(render_width, render_height),
_camera(render_width, render_height)
{
	
}

//////////////////////////////////////////////////////////////
void ggo_cumbia_animation_artist::init_sub()
{
	_artist.init(_camera, 1 << 15);
}
	
//////////////////////////////////////////////////////////////
bool ggo_cumbia_animation_artist::render_next_frame_sub(uint8_t * buffer, int frame_index)
{
	const int FRAMES_COUNT = 200;

  ggo::antialiasing_renderer renderer(_camera);
  
	_artist.render_bitmap(buffer, get_render_width(), get_render_height(), renderer);
	
	_camera.basis().rotate(ggo::ray3d_float::O_Z(), 2 * ggo::pi<float>() / FRAMES_COUNT);
	
	return frame_index < FRAMES_COUNT;
}
