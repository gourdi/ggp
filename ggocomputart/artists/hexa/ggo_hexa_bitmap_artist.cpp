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
void ggo::hexa_bitmap_artist::render_bitmap(void * buffer) const
{
  // The camera.
#ifdef GGO_PREVIEW
  ggo::mono_sampling_point_camera camera(get_width(), get_height());
#else
  ggo::multi_sampling_point_camera camera(get_width(), get_height());
  camera.set_depth_of_field_factor(3);
  camera.set_depth_of_field(375);
#endif
  camera.basis().set_pos(0, 0, 500);
  camera.basis().rotate(ggo::ray3d_float::O_X(), 1.1f);
  camera.basis().rotate(ggo::ray3d_float::O_Z(), ggo::rand<float>(-0.2f, 0.2f));
  camera.set_aperture(0.15f);

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
