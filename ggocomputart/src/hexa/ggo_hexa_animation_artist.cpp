#include "ggo_hexa_animation_artist.h"
#include <raytracer/renderers/ggo_global_sampling_renderer.h>
#include <raytracer/renderers/ggo_mono_sampling_renderer.h>

//#define GGO_PREVIEW

namespace
{
  constexpr int frames_count = 300;
  constexpr int samples_count = 56;
}

//////////////////////////////////////////////////////////////
ggo::hexa_animation_artist::hexa_animation_artist(
  int width, int height, int line_byte_step,
  ggo::pixel_type pixel_type, ggo::lines_order memory_lines_order,
  ggo::ratio fps)
:
progress_animation_artist(width, height, line_byte_step, pixel_type, memory_lines_order, 6, fps)
{
#ifdef GGO_PREVIEW
  _artist.reset(new ggo::hexa_artist(false));
#else
  _artist.reset(new ggo::hexa_artist(true));
#endif
} 

//////////////////////////////////////////////////////////////
void ggo::hexa_animation_artist::render_frame(void * buffer, float progress)
{
  progress = ggo::ease_inout(progress);

  // The camera.
  const ggo::basis3d_f camera_basis = _artist->generate_camera_basis(progress);
  const float camera_aperture = 0.15f;

#ifdef GGO_PREVIEW
  ggo::mono_sampling_point_camera camera(width(), height(), camera_basis, camera_aperture);
#else
  float depth_of_field = 0.92f * ggo::length(camera_basis.pos());
  float depth_of_field_factor = 0.05f * ggo::length(camera_basis.pos());
  ggo::multi_sampling_point_camera camera(width(), height(), camera_basis, camera_aperture, depth_of_field, depth_of_field_factor);
#endif
  
  // Rendering.
#ifdef GGO_PREVIEW
  ggo::mono_sampling_renderer renderer(camera);
#else
  ggo::global_sampling_renderer renderer(camera, samples_count);
#endif
  _artist->render(buffer, width(), height(), line_byte_step(), pixel_type(), memory_lines_order(), progress, renderer);
}
