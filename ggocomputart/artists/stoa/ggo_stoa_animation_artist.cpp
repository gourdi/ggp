#include "ggo_stoa_animation_artist.h"
#include <ggo_point_camera.h>
#include <ggo_mono_sampling_renderer.h>
#include <ggo_antialiasing_renderer.h>

//////////////////////////////////////////////////////////////
ggo::stoa_animation_artist::stoa_animation_artist(int render_width, int render_height)
:
animation_artist_abc(render_width, render_height)
{
}

//////////////////////////////////////////////////////////////
void ggo::stoa_animation_artist::init_sub()
{
  _artist.reset(new ggo::stoa_artist(256));

  _hue = ggo::rand<float>();

  _light1_angle_start = ggo::rand<float>(0, ggo::pi<float>());
  _light1_angle_end   = ggo::rand<float>(0, ggo::pi<float>());
  _light2_angle_start = ggo::rand<float>(0, ggo::pi<float>());
  _light2_angle_end   = ggo::rand<float>(0, ggo::pi<float>());
}

//////////////////////////////////////////////////////////////
bool ggo::stoa_animation_artist::render_next_frame_sub(void * buffer, int frame_index)
{
  const int frames_count = 150;

  if (frame_index > frames_count)
  {
    return false;
  }

  // The camera.
#ifdef MONO_SAMPLING
  ggo::mono_sampling_point_camera camera(get_render_width(), get_render_height());
#else
  ggo::antialiasing_point_camera camera(get_render_width(), get_render_height());
#endif
  camera.basis().set_pos(0, 0, 40);
  camera.set_aperture(0.1f);

  float range = ggo::pi<float>() / 6.f;
  camera.basis().rotate(ggo::ray3d_float::O_Y(), ggo::ease_inout(frame_index, frames_count, -range, range));

  // Lights.
  float angle1 = ggo::ease_inout(frame_index, frames_count, _light1_angle_start, _light1_angle_end);
  float angle2 = ggo::ease_inout(frame_index, frames_count, _light2_angle_start, _light2_angle_end);
  const ggo::pos3f light_pos1(30.f * std::cos(angle1), 30.f * std::sin(angle1), 30.f);
  const ggo::pos3f light_pos2(30.f * std::cos(angle2), 30.f * std::sin(angle2), 30.f);

  // Rendering.
#ifdef MONO_SAMPLING
  ggo::mono_sampling_renderer renderer(camera);
#else
  ggo::antialiasing_renderer renderer(camera);
#endif

  _artist->render(buffer, get_render_width(), get_render_height(), _hue, light_pos1, light_pos2, renderer);

  return true;
}


