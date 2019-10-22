#include "ggo_hexa_bitmap_artist.h"
#include "ggo_hexa_artist.h"
#include <raytracer/cameras/ggo_point_camera.h>
#include <raytracer/renderers/ggo_global_sampling_renderer.h>
#include <raytracer/renderers/ggo_mono_sampling_renderer.h>

//#define GGO_PREVIEW

namespace 
{
  const int samples_count = 56;
}

//////////////////////////////////////////////////////////////
ggo::hexa_bitmap_artist::hexa_bitmap_artist(int width, int height, int line_byte_step, ggo::pixel_type pixel_type, ggo::lines_order memory_lines_order)
:
bitmap_artist_abc(width, height, line_byte_step, pixel_type, memory_lines_order)
{
}

//////////////////////////////////////////////////////////////
void ggo::hexa_bitmap_artist::render_bitmap(void * buffer) const
{
#ifdef GGO_PREVIEW
  ggo::hexa_artist artist(false);
#else
  ggo::hexa_artist artist(true);
#endif

  // The camera.
  const ggo::basis3d_f camera_basis = artist.generate_camera_basis(1.f);
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
  artist.render(buffer, width(), height(), line_byte_step(), pixel_type(), memory_lines_order(), 1.f, renderer);
}
