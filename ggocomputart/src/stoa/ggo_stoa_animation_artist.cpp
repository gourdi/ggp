#include "ggo_stoa_animation_artist.h"
#include <raytracer/cameras/ggo_point_camera.h>
#include <raytracer/renderers/ggo_mono_sampling_renderer.h>
#include <raytracer/renderers/ggo_antialiasing_renderer.h>

//////////////////////////////////////////////////////////////
ggo::stoa_animation_artist::stoa_animation_artist(int width, int height, int line_byte_step, ggo::pixel_type pixel_type, ggo::lines_order memory_lines_order)
:
fixed_frames_count_animation_artist_abc(width, height, line_byte_step, pixel_type, memory_lines_order, 150)
{
  _artist.reset(new ggo::stoa_artist(256));

  _hue = ggo::rand<float>();

  _light1_angle_start = ggo::rand<float>(0, ggo::pi<float>());
  _light1_angle_end = ggo::rand<float>(0, ggo::pi<float>());
  _light2_angle_start = ggo::rand<float>(0, ggo::pi<float>());
  _light2_angle_end = ggo::rand<float>(0, ggo::pi<float>());
}

//////////////////////////////////////////////////////////////
void ggo::stoa_animation_artist::render_frame(void * buffer, int frame_index, float time_step)
{
  // The camera.
  const float camera_aperture = 0.1f;

  ggo::basis3d_f camera_basis({ 0.f, 0.f, 40.f });
  float range = ggo::pi<float>() / 6.f;
  camera_basis.rotate(ggo::ray3d_f::O_Y(), ggo::ease_inout(frame_index, frames_count(), -range, range));

#ifdef MONO_SAMPLING
  ggo::mono_sampling_point_camera camera(width(), height(), camera_basis, camera_aperture);
#else
  ggo::antialiasing_point_camera camera(width(), height(), camera_basis, camera_aperture);
#endif

  // Lights.
  float angle1 = ggo::ease_inout(frame_index, frames_count(), _light1_angle_start, _light1_angle_end);
  float angle2 = ggo::ease_inout(frame_index, frames_count(), _light2_angle_start, _light2_angle_end);
  const ggo::pos3_f light_pos1(30.f * std::cos(angle1), 30.f * std::sin(angle1), 30.f);
  const ggo::pos3_f light_pos2(30.f * std::cos(angle2), 30.f * std::sin(angle2), 30.f);

  // Rendering.
#ifdef MONO_SAMPLING
  ggo::mono_sampling_renderer renderer(camera);
#else
  ggo::antialiasing_renderer renderer(camera);
#endif

  _artist->render(buffer, width(), height(), line_byte_step(), pixel_type(), memory_lines_order(), _hue, light_pos1, light_pos2, renderer);
}


