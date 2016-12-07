#include "ggo_stoa_animation_artist.h"
#include <ggo_point_camera.h>
#include <ggo_mono_sampling_renderer.h>
#include <ggo_antialiasing_renderer.h>

namespace
{
  const int frames_count = 150;
}

//////////////////////////////////////////////////////////////
ggo::stoa_animation_artist::stoa_animation_artist(int width, int height, int line_step, ggo::pixel_buffer_format pbf, rendering_type rt)
:
animation_artist_abc(width, height, line_step, pbf, rt)
{
}

//////////////////////////////////////////////////////////////
void ggo::stoa_animation_artist::init()
{
  _artist.reset(new ggo::stoa_artist(256));

  _hue = ggo::rand<float>();

  _light1_angle_start = ggo::rand<float>(0, ggo::pi<float>());
  _light1_angle_end = ggo::rand<float>(0, ggo::pi<float>());
  _light2_angle_start = ggo::rand<float>(0, ggo::pi<float>());
  _light2_angle_end = ggo::rand<float>(0, ggo::pi<float>());
}

//////////////////////////////////////////////////////////////
bool ggo::stoa_animation_artist::update()
{
  ++_frame_index;

  if (_frame_index > frames_count)
  {
    return false;
  }

  return true;
}

//////////////////////////////////////////////////////////////
void ggo::stoa_animation_artist::render_frame(void * buffer, const ggo::pixel_rect & clipping)
{
  // The camera.
#ifdef MONO_SAMPLING
  ggo::mono_sampling_point_camera camera(get_width(), get_height());
#else
  ggo::antialiasing_point_camera camera(get_width(), get_height());
#endif
  camera.basis().set_pos(0, 0, 40);
  camera.set_aperture(0.1f);

  float range = ggo::pi<float>() / 6.f;
  camera.basis().rotate(ggo::ray3d_float::O_Y(), ggo::ease_inout(_frame_index, frames_count, -range, range));

  // Lights.
  float angle1 = ggo::ease_inout(_frame_index, frames_count, _light1_angle_start, _light1_angle_end);
  float angle2 = ggo::ease_inout(_frame_index, frames_count, _light2_angle_start, _light2_angle_end);
  const ggo::pos3f light_pos1(30.f * std::cos(angle1), 30.f * std::sin(angle1), 30.f);
  const ggo::pos3f light_pos2(30.f * std::cos(angle2), 30.f * std::sin(angle2), 30.f);

  // Rendering.
#ifdef MONO_SAMPLING
  ggo::mono_sampling_renderer renderer(camera);
#else
  ggo::antialiasing_renderer renderer(camera);
#endif

  _artist->render(buffer, get_width(), get_height(), get_line_step(), get_pixel_buffer_format(), _hue, light_pos1, light_pos2, renderer);
}


