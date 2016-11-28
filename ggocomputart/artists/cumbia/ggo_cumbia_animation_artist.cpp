#include "ggo_cumbia_animation_artist.h"
#include "ggo_antialiasing_renderer.h"

//////////////////////////////////////////////////////////////
ggo::cumbia_animation_artist::cumbia_animation_artist(int width, int height, int line_step, ggo::pixel_buffer_format pbf, rendering_type rt)
:
animation_artist_abc(width, height, line_step, pbf, rt),
_camera(width, height)
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
  
	_artist.render_bitmap(buffer, get_width(), get_height(), get_line_step(), get_pixel_buffer_format(), renderer);
	
	_camera.basis().rotate(ggo::ray3d_float::O_Z(), 2 * ggo::pi<float>() / frames_count);
	
	return frame_index < frames_count;
}
