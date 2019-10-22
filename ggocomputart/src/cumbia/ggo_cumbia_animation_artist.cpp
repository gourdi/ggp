#include "ggo_cumbia_animation_artist.h"
#include <raytracer/renderers/ggo_antialiasing_renderer.h>

//////////////////////////////////////////////////////////////
ggo::cumbia_animation_artist::cumbia_animation_artist(int width, int height, int line_byte_step, ggo::pixel_type pixel_type, ggo::lines_order memory_lines_order)
:
fixed_frames_count_animation_artist_abc(width, height, line_byte_step, pixel_type, memory_lines_order, 200)
{
  _artist.init(_camera_basis, _camera_aperture, 1 << 15);
}

//////////////////////////////////////////////////////////////
void ggo::cumbia_animation_artist::render_frame(void * buffer, int frame_index, float time_step)
{
  _camera_basis.rotate(ggo::ray3d_f::O_Z(), 2 * ggo::pi<float>() / frames_count());

  ggo::antialiasing_point_camera	camera(width(), height(), _camera_basis, _camera_aperture);

  ggo::antialiasing_renderer renderer(camera);

  _artist.render_bitmap(buffer, width(), height(), line_byte_step(), pixel_type(), memory_lines_order(), renderer);
}
