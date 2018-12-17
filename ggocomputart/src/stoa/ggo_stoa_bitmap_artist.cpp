#include "ggo_stoa_bitmap_artist.h"
#include "ggo_stoa_artist.h"
#include <raytracer/cameras/ggo_point_camera.h>
#include <raytracer/renderers/ggo_mono_sampling_renderer.h>
#include <raytracer/renderers/ggo_antialiasing_renderer.h>

//#define MONO_SAMPLING

//////////////////////////////////////////////////////////////
ggo::stoa_bitmap_artist::stoa_bitmap_artist(int width, int height, int line_step, ggo::image_format format)
:
bitmap_artist_abc(width, height, line_step, format)
{
}

//////////////////////////////////////////////////////////////
void ggo::stoa_bitmap_artist::render_bitmap(void * buffer) const
{
  // The camera.
  const ggo::basis3d_f camera_basis({ 0.f, 0.f, 40.f });
  const float camera_aperture = 0.1f;
#ifdef MONO_SAMPLING
  ggo::mono_sampling_point_camera camera(width(), height(), camera_basis, camera_aperture);
#else
  ggo::antialiasing_point_camera camera(width(), height(), camera_basis, camera_aperture);
#endif

  // Lights.
  float angle1 = ggo::rand<float>(0, ggo::pi<float>());
  float angle2 = ggo::rand<float>(0, ggo::pi<float>());
  const ggo::pos3_f light_pos1(30.f * std::cos(angle1), 30.f * std::sin(angle1), 30.f);
  const ggo::pos3_f light_pos2(30.f * std::cos(angle2), 30.f * std::sin(angle2), 30.f);

  // Rendering.
#ifdef MONO_SAMPLING
  ggo::mono_sampling_renderer renderer(camera);
#else
  ggo::antialiasing_renderer renderer(camera);
#endif

  float hue = ggo::rand<float>();

  ggo::stoa_artist artist(384);
  artist.render(buffer, width(), height(), line_step(), format(), hue, light_pos1, light_pos2, renderer);
}
