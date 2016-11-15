#include "ggo_hexa_animation_artist.h"
#include <ggo_antialiasing_renderer.h>
#include <ggo_mono_sampling_renderer.h>

#define GGO_HEXA_ANTIALIASING 1

//////////////////////////////////////////////////////////////
ggo::hexa_animation_artist::hexa_animation_artist(int width, int height, int line_step, ggo::pixel_buffer_format pbf)
:
animation_artist_abc(width, height, line_step, pbf)
{
} 

//////////////////////////////////////////////////////////////
void ggo::hexa_animation_artist::init_sub()
{
  // Hexa infos and thier delays.
  _hexa_infos = ggo::hexa_artist::generate_hexa_infos();
  
  // Colors.
  auto colors = ggo::hexa_artist::generate_colors();

  _color_top = colors.first;
  _color_side = colors.second;
  
  // The camera.
  _camera_rotation_start = ggo::rand<float>(-0.2f, 0.2f);
  _camera_rotation_end = ggo::rand<float>(-0.2f, 0.2f);
  
  // Lights.
  _lights_pos = ggo::hexa_artist::generate_light_positions();
  
  // Fog.
  _fog_color = ggo::color_32f(ggo::rand<float>(0, 0.25));
}
  
//////////////////////////////////////////////////////////////
bool ggo::hexa_animation_artist::render_next_frame_sub(void * buffer, int frame_index)
{
  const int frames_count = 300;

  if (frame_index > frames_count)
  {
    return false;
  }
  // Update camera.
#ifdef GGO_HEXA_ANTIALIASING
  ggo::antialiasing_point_camera camera(get_width(), get_height());
#else
  ggo::mono_sampling_point_camera camera(get_width(), get_height());
#endif
  
  camera.basis().set_pos(0.f, 0.f, ggo::ease_inout(frame_index, frames_count, 1.f, 350.f));
  camera.basis().rotate(ggo::ray3d_float::O_X(), ggo::ease_inout(frame_index, frames_count, 0.2f, 1.2f));
  camera.basis().rotate(ggo::ray3d_float::O_Z(), ggo::ease_inout(frame_index, frames_count, _camera_rotation_start, _camera_rotation_end));
  camera.set_aperture(0.15f);

  // Rendering.
#ifdef GGO_HEXA_ANTIALIASING
  ggo::antialiasing_renderer renderer(camera);
#else
  ggo::mono_sampling_renderer renderer(camera);
#endif

  std::vector<ggo::hexa_artist::hexa_info> hexa_infos = _hexa_infos;
  for (auto & hexa_info : hexa_infos)
  {
    hexa_info._height = ggo::ease_inout(frame_index, frames_count, 0.f, hexa_info._height);
  }

  ggo::hexa_artist::render(buffer, get_width(), get_height(), get_line_step(), get_pixel_buffer_format(),
                           hexa_infos, _color_top, _color_side, _lights_pos, _fog_color, renderer);
  
  return true;
}
