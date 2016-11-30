#include "ggo_cumbia_animation_artist.h"
#include "ggo_antialiasing_renderer.h"

namespace
{
  const int frames_count = 200;
}

//////////////////////////////////////////////////////////////
ggo::cumbia_animation_artist::cumbia_animation_artist(int width, int height, int line_step, ggo::pixel_buffer_format pbf, rendering_type rt)
:
animation_artist_abc(width, height, line_step, pbf, rt),
_camera(width, height)
{
	
}

//////////////////////////////////////////////////////////////
void ggo::cumbia_animation_artist::init()
{
  _frame_index = -1;
	_artist.init(_camera, 1 << 15);
}

//////////////////////////////////////////////////////////////
bool ggo::cumbia_animation_artist::update()
{
  ++_frame_index;

  if (_frame_index >= frames_count)
  {
    return false;
  }

  _camera.basis().rotate(ggo::ray3d_float::O_Z(), 2 * ggo::pi<float>() / frames_count);

  return true;
}

//////////////////////////////////////////////////////////////
void ggo::cumbia_animation_artist::render_frame(void * buffer, const ggo::pixel_rect & clipping) const
{
  ggo::antialiasing_renderer renderer(_camera);

  _artist.render_bitmap(buffer, get_width(), get_height(), get_line_step(), get_pixel_buffer_format(), renderer);
}
