#include "ggo_cumbia_animation_artist.h"
#include "ggo_antialiasing_renderer.h"

namespace
{
  const int frames_count = 200;
}

//////////////////////////////////////////////////////////////
ggo::cumbia_animation_artist::cumbia_animation_artist(int width, int height, int line_step, ggo::pixel_buffer_format pbf, rendering_type rt)
:
animation_artist_abc(width, height, line_step, pbf, rt)
{
	
}

//////////////////////////////////////////////////////////////
void ggo::cumbia_animation_artist::init()
{
  _frame_index = -1;
	_artist.init(_camera_basis, _camera_aperture, 1 << 15);
}

//////////////////////////////////////////////////////////////
bool ggo::cumbia_animation_artist::update()
{
  ++_frame_index;

  if (_frame_index >= frames_count)
  {
    return false;
  }

  _camera_basis.rotate(ggo::ray3d_float::O_Z(), 2 * ggo::pi<float>() / frames_count);

  return true;
}

//////////////////////////////////////////////////////////////
void ggo::cumbia_animation_artist::render_frame(void * buffer, const ggo::rect_int & clipping)
{
  ggo::antialiasing_point_camera	camera(get_width(), get_height(), _camera_basis, _camera_aperture);

  ggo::antialiasing_renderer renderer(camera);

  _artist.render_bitmap(buffer, get_width(), get_height(), get_line_step(), get_pixel_buffer_format(), renderer);
}
