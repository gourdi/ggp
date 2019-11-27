#include "ggo_cumbia_animation_artist.h"
#include <raytracer/renderers/ggo_antialiasing_renderer.h>

//////////////////////////////////////////////////////////////
ggo::cumbia_animation_artist::cumbia_animation_artist(
  int width, int height, int line_byte_step,
  ggo::pixel_type pixel_type, ggo::lines_order memory_lines_order,
  ggo::ratio fps)
:
progress_animation_artist(width, height, line_byte_step, pixel_type, memory_lines_order, 8, fps)
{
  _artist.init(_camera_basis, _camera_aperture, 1 << 15);
}

//////////////////////////////////////////////////////////////
void ggo::cumbia_animation_artist::render_frame(void * buffer, float progress)
{
  _camera_basis.rotate(ggo::ray3d_f::O_Z(), progress * 2 * ggo::pi<float>());

  ggo::antialiasing_point_camera	camera(width(), height(), _camera_basis, _camera_aperture);

  ggo::antialiasing_renderer renderer(camera);

  _artist.render_bitmap(buffer, width(), height(), line_byte_step(), pixel_type(), memory_lines_order(), renderer);
}
