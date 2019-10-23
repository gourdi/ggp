#include "ggo_metaballs_bitmap_artist.h"
#include "ggo_metaballs_artist.h"
#include <raytracer/cameras/ggo_point_camera.h>
#include <raytracer/renderers/ggo_global_sampling_renderer.h>
#include <raytracer/renderers/ggo_mono_sampling_renderer.h>

#define MONO_SAMPLING

//////////////////////////////////////////////////////////////
void ggo::metaballs_bitmap_artist::render_bitmap(void * buffer, int width, int height, int line_byte_step, ggo::pixel_type pixel_type, ggo::lines_order memory_lines_order) const
{
  const float ball_size = 3.f;

	ggo::metaballs_artist::params params;

  const float camera_aperture = 0.1f;
  const ggo::basis3d_f camera_basis({ 0.f, 0.f, 25.f });

#ifdef MONO_SAMPLING
  ggo::mono_sampling_point_camera camera(width, height, camera_basis, camera_aperture);
#else
  ggo::multi_sampling_point_camera camera(width, height, camera_basis, camera_aperture, 22.f, ggo::rand<float>(0.2f, 0.5f));
#endif

	while (params._centers.size() < 200)
	{
		ggo::pos3_f center(ggo::rand<float>(-ball_size, ball_size), ggo::rand<float>(-ball_size, ball_size), ggo::rand<float>(-ball_size, ball_size));
		if (ggo::length(center) < ball_size)
		{
			params._centers.push_back(center);
		}
	}

#ifdef MONO_SAMPLING
  ggo::mono_sampling_renderer renderer(camera);
#else
  ggo::global_sampling_renderer renderer(camera, 56);
#endif
	ggo::metaballs_artist::render_bitmap(buffer, width, height, line_byte_step, pixel_type, memory_lines_order, renderer, params);
}
