#include "ggo_cumbia_animation_artist.h"
#include "ggo_antialiasing_renderer.h"

//////////////////////////////////////////////////////////////
ggo::cumbia_animation_artist::cumbia_animation_artist(int render_width, int render_height)
:
animation_artist_abc(render_width, render_height),
_camera(render_width, render_height)
{
	
}

//////////////////////////////////////////////////////////////
void ggo::cumbia_animation_artist::init_sub()
{
	_artist.init(_camera, 1 << 15);
}
	
//////////////////////////////////////////////////////////////
bool ggo::cumbia_animation_artist::render_next_frame_sub(void * buffer, int frame_index)
{
	const int frames_count = 200;

  ggo::antialiasing_renderer renderer(_camera);
  
	_artist.render_bitmap(buffer, get_render_width(), get_render_height(), renderer);
	
	_camera.basis().rotate(ggo::ray3d_float::O_Z(), 2 * ggo::pi<float>() / frames_count);
	
	return frame_index < frames_count;
}
