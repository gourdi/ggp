#include "ggo_hexa_animation_artist.h"
#include <ggo_global_sampling_renderer.h>
#include <ggo_mono_sampling_renderer.h>

//#define GGO_PREVIEW

namespace
{
  constexpr int frames_count = 300;
  constexpr int samples_count = 56;
}

//////////////////////////////////////////////////////////////
ggo::hexa_animation_artist::hexa_animation_artist(int width, int height, int line_step, ggo::pixel_buffer_format pbf, rendering_type rt)
:
animation_artist_abc(width, height, line_step, pbf, rt)
{
} 

//////////////////////////////////////////////////////////////
void ggo::hexa_animation_artist::init()
{
#ifdef GGO_PREVIEW
  _artist.reset(new ggo::hexa_artist(false));
#else
  _artist.reset(new ggo::hexa_artist(true));
#endif
}

//////////////////////////////////////////////////////////////
bool ggo::hexa_animation_artist::prepare_frame()
{
  ++_frame_index;

  if (_frame_index > frames_count)
  {
    return false;
  }

  return true;
}

//////////////////////////////////////////////////////////////
void ggo::hexa_animation_artist::process_frame(void * buffer, const ggo::rect_int & clipping)
{
  float progress = static_cast<float>(_frame_index) / frames_count;
  progress = ggo::ease_inout(progress);

  // The camera.
  const ggo::basis3d_float camera_basis = _artist->generate_camera_basis(progress);
  const float camera_aperture = 0.15f;

#ifdef GGO_PREVIEW
  ggo::mono_sampling_point_camera camera(get_width(), get_height(), camera_basis, camera_aperture);
#else
  float depth_of_field = 0.92f * camera_basis.pos().get_length();
  float depth_of_field_factor = 0.05f * camera_basis.pos().get_length();
  ggo::multi_sampling_point_camera camera(get_width(), get_height(), camera_basis, camera_aperture, depth_of_field, depth_of_field_factor);
#endif
  
  // Rendering.
#ifdef GGO_PREVIEW
  ggo::mono_sampling_renderer renderer(camera);
#else
  ggo::global_sampling_renderer renderer(camera, samples_count);
#endif
  _artist->render(buffer, get_width(), get_height(), get_line_step(), get_pixel_buffer_format(), progress, renderer);
}
