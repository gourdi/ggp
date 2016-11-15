#include "ggo_stoa_bitmap_artist.h"
#include "ggo_stoa_artist.h"
#include <ggo_point_camera.h>
#include <ggo_mono_sampling_renderer.h>
#include <ggo_antialiasing_renderer.h>

//#define MONO_SAMPLING

//////////////////////////////////////////////////////////////
ggo::stoa_bitmap_artist::stoa_bitmap_artist(int width, int height, int line_step, ggo::pixel_buffer_format pbf)
:
bitmap_artist_abc(width, height, line_step, pbf)
{
}

//////////////////////////////////////////////////////////////
void ggo::stoa_bitmap_artist::render_bitmap(void * buffer) const
{
  // The camera.
#ifdef MONO_SAMPLING
  ggo::mono_sampling_point_camera camera(get_width(), get_height());
#else
  ggo::antialiasing_point_camera camera(get_width(), get_height());
#endif
  camera.basis().set_pos(0, 0, 40);
  camera.set_aperture(0.1f);

  // Lights.
  float angle1 = ggo::rand<float>(0, ggo::pi<float>());
  float angle2 = ggo::rand<float>(0, ggo::pi<float>());
  const ggo::pos3f light_pos1(30.f * std::cos(angle1), 30.f * std::sin(angle1), 30.f);
  const ggo::pos3f light_pos2(30.f * std::cos(angle2), 30.f * std::sin(angle2), 30.f);

  // Rendering.
#ifdef MONO_SAMPLING
  ggo::mono_sampling_renderer renderer(camera);
#else
  ggo::antialiasing_renderer renderer(camera);
#endif

  float hue = ggo::rand<float>();

  ggo::stoa_artist artist(384);
  artist.render(buffer, get_width(), get_height(), get_line_step(), get_pixel_buffer_format(), hue, light_pos1, light_pos2, renderer);
}
