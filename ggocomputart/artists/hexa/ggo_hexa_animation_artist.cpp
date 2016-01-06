#include "ggo_hexa_animation_artist.h"
#include <ggo_antialiasing_renderer.h>
#include <ggo_mono_sampling_renderer.h>

#define GGO_HEXA_ANTIALIASING 1

namespace
{
  const int FRAMES_COUNT = 300;
}

//////////////////////////////////////////////////////////////
ggo_hexa_animation_artist::ggo_hexa_animation_artist(int render_width, int render_height)
:
ggo_animation_artist_abc(render_width, render_height)
{
} 

//////////////////////////////////////////////////////////////
void ggo_hexa_animation_artist::init_sub()
{
  // Hexa infos and thier delays.
  _hexa_infos = ggo_hexa_artist::generate_hexa_infos();
  
  // Colors.
  auto colors = ggo_hexa_artist::generate_colors();

  _color_top = colors.first;
  _color_side = colors.second;
  
  // The camera.
  _camera_rotation_start = ggo::rand_float(-0.2f, 0.2f);
  _camera_rotation_end = ggo::rand_float(-0.2f, 0.2f);
  
  // Lights.
  _lights_pos = ggo_hexa_artist::generate_light_positions();
  
  // Fog.
  _fog_color = ggo::color(ggo::rand_float(0, 0.25));
}
  
//////////////////////////////////////////////////////////////
bool ggo_hexa_animation_artist::render_next_frame_sub(uint8_t * buffer, int frame_index)
{
  if (frame_index > FRAMES_COUNT)
  {
    return false;
  }
  // Update camera.
#ifdef GGO_HEXA_ANTIALIASING
  ggo::antialiasing_point_camera camera(get_render_width(), get_render_height());
#else
  ggo::mono_sampling_point_camera camera(get_render_width(), get_render_height());
#endif
  
  camera.basis().set_pos(0, 0, ggo::ease_inout(frame_index, FRAMES_COUNT, 1, 350));
  camera.basis().rotate(ggo::ray3d_float::O_X(), ggo::ease_inout(frame_index, FRAMES_COUNT, 0.2f, 1.2f));
  camera.basis().rotate(ggo::ray3d_float::O_Z(), ggo::ease_inout(frame_index, FRAMES_COUNT, _camera_rotation_start, _camera_rotation_end));
  camera.set_aperture(0.15f);

  // Rendering.
#ifdef GGO_HEXA_ANTIALIASING
  ggo::antialiasing_renderer renderer(camera);
#else
  ggo::mono_sampling_renderer renderer(camera);
#endif

  std::vector<ggo_hexa_artist::hexa_info> hexa_infos = _hexa_infos;
  for (auto & hexa_info : hexa_infos)
  {
    hexa_info._height = ggo::ease_inout(frame_index, FRAMES_COUNT, 0, hexa_info._height);
  }

  ggo_hexa_artist::render(buffer, get_render_width(), get_render_height(),
                          hexa_infos,
                          _color_top, _color_side,
                          _lights_pos, _fog_color, renderer);
  
  return true;
}
