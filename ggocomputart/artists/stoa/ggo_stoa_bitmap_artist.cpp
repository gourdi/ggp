#include "ggo_stoa_bitmap_artist.h"
#include "ggo_stoa_artist.h"
#include <ggo_point_camera.h>
#include <ggo_mono_sampling_renderer.h>
#include <ggo_antialiasing_renderer.h>

//#define MONO_SAMPLING

//////////////////////////////////////////////////////////////
ggo_stoa_bitmap_artist::ggo_stoa_bitmap_artist(int render_width, int render_height)
  :
  ggo_bitmap_artist_abc(render_width, render_height)
{
}

//////////////////////////////////////////////////////////////
void ggo_stoa_bitmap_artist::render_bitmap(uint8_t * buffer)
{
  // The camera.
#ifdef MONO_SAMPLING
  ggo::mono_sampling_point_camera camera(get_render_width(), get_render_height());
#else
  ggo::antialiasing_point_camera camera(get_render_width(), get_render_height());
#endif
  camera.basis().set_pos(0, 0, 40);
  camera.set_aperture(0.1f);

  // Lights.
  float angle1 = ggo::rand_float(0, ggo::pi<float>());
  float angle2 = ggo::rand_float(0, ggo::pi<float>());
  const ggo::pos3f light_pos1(30.f * std::cos(angle1), 30.f * std::sin(angle1), 30.f);
  const ggo::pos3f light_pos2(30.f * std::cos(angle2), 30.f * std::sin(angle2), 30.f);

  // Rendering.
#ifdef MONO_SAMPLING
  ggo::mono_sampling_renderer renderer(camera);
#else
  ggo::antialiasing_renderer renderer(camera);
#endif

  float hue = ggo::rand_float();

  ggo_stoa_artist artist(384);
  artist.render(buffer, get_render_width(), get_render_height(), hue, light_pos1, light_pos2, renderer);
}
