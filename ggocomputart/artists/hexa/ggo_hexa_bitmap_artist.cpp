#include "ggo_hexa_bitmap_artist.h"
#include "ggo_hexa_artist.h"
#include <ggo_point_camera.h>
#include <ggo_global_sampling_renderer.h>
#include <ggo_mono_sampling_renderer.h>

#define GGO_PREVIEW

namespace 
{
  const int samples_count = 56;
}

//////////////////////////////////////////////////////////////
ggo::hexa_bitmap_artist::hexa_bitmap_artist(int width, int height, int line_step, ggo::pixel_buffer_format pbf)
:
bitmap_artist_abc(width, height, line_step, pbf)
{
}

//////////////////////////////////////////////////////////////
void ggo::hexa_bitmap_artist::render_bitmap(void * buffer, const bool & quit) const
{
  ggo::basis3d_float camera_basis({ 0.f, 0.f, 500.f });
  camera_basis.rotate(ggo::ray3d_float::O_X(), 1.1f);
  camera_basis.rotate(ggo::ray3d_float::O_Z(), ggo::rand<float>(-0.2f, 0.2f));
  const float camera_aperture = 0.15f;

  // The camera.
#ifdef GGO_PREVIEW
  ggo::mono_sampling_point_camera camera(get_width(), get_height(), camera_basis, camera_aperture);
#else
  ggo::multi_sampling_point_camera camera(get_width(), get_height(), camera_basis, camera_aperture, 375.f, 3.f);
#endif

  // Lights.
  auto lights_pos = ggo::hexa_artist::generate_light_positions();

  // Hexa infos.
  auto hexa_infos = ggo::hexa_artist::generate_hexa_infos();
  auto colors = ggo::hexa_artist::generate_colors();
  
  // Rendering.
#ifdef GGO_PREVIEW
  ggo::mono_sampling_renderer renderer(camera);
#else
  ggo::global_sampling_renderer renderer(camera, samples_count);
#endif
  ggo::hexa_artist::render(buffer, get_width(), get_height(), get_line_step(), get_pixel_buffer_format(),
                           hexa_infos, colors.first, colors.second, lights_pos, ggo::color_32f(ggo::rand<float>(0, 0.25)), renderer);
}
