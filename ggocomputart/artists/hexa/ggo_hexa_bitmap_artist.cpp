#include "ggo_hexa_bitmap_artist.h"
#include "ggo_hexa_artist.h"
#include <ggo_point_camera.h>
#include <ggo_global_sampling_renderer.h>
#include <ggo_mono_sampling_renderer.h>

#define GGO_PREVIEW

namespace 
{
  const int GGO_HEXA_SAMPLES_COUNT = 56;
}

//////////////////////////////////////////////////////////////
ggo_hexa_bitmap_artist::ggo_hexa_bitmap_artist(int render_width, int render_height)
:
ggo_bitmap_artist_abc(render_width, render_height)
{
}

//////////////////////////////////////////////////////////////
void ggo_hexa_bitmap_artist::render_bitmap(uint8_t * buffer)
{
  // The camera.
#ifdef GGO_PREVIEW
  ggo::mono_sampling_point_camera camera(get_render_width(), get_render_height());
#else
  ggo::multi_sampling_point_camera camera(get_render_width(), get_render_height());
  camera.set_depth_of_field_factor(3);
  camera.set_depth_of_field(375);
#endif
  camera.basis().set_pos(0, 0, 500);
  camera.basis().rotate(ggo::ray3d_float::O_X(), 1.1f);
  camera.basis().rotate(ggo::ray3d_float::O_Z(), ggo::rand_float(-0.2f, 0.2f));
  camera.set_aperture(0.15f);

  // Lights.
  auto lights_pos = ggo_hexa_artist::generate_light_positions();

  // Hexa infos.
  auto hexa_infos = ggo_hexa_artist::generate_hexa_infos();
  auto colors = ggo_hexa_artist::generate_colors();
  
  // Rendering.
#ifdef GGO_PREVIEW
  ggo::mono_sampling_renderer renderer(camera);
#else
  ggo::global_sampling_renderer renderer(camera, GGO_HEXA_SAMPLES_COUNT);
#endif
  ggo_hexa_artist::render(buffer, get_render_width(), get_render_height(),
                          hexa_infos,
                          colors.first, colors.second,
                          lights_pos, ggo::color(ggo::rand_float(0, 0.25)), renderer);
}
