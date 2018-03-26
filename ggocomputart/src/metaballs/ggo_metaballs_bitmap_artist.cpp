#include "ggo_metaballs_bitmap_artist.h"
#include "ggo_metaballs_artist.h"
#include <ggo_point_camera.h>
#include <ggo_global_sampling_renderer.h>
#include <ggo_mono_sampling_renderer.h>

#define MONO_SAMPLING

//////////////////////////////////////////////////////////////
ggo::metaballs_bitmap_artist::metaballs_bitmap_artist(int width, int height, int line_step, ggo::pixel_buffer_format pbf)
:
bitmap_artist_abc(width, height, line_step, pbf)
{	
}

//////////////////////////////////////////////////////////////
void ggo::metaballs_bitmap_artist::render_bitmap(void * buffer) const
{
  const float ball_size = 3.f;

	ggo::metaballs_artist::params params;

  const float camera_aperture = 0.1f;
  const ggo::basis3d_float camera_basis({ 0.f, 0.f, 25.f });

#ifdef MONO_SAMPLING
  ggo::mono_sampling_point_camera camera(get_width(), get_height(), camera_basis, camera_aperture);
#else
  ggo::multi_sampling_point_camera camera(get_width(), get_height(), camera_basis, camera_aperture, 22.f, ggo::rand<float>(0.2f, 0.5f));
#endif

	while (params._centers.size() < 200)
	{
		ggo::pos3f center(ggo::rand<float>(-ball_size, ball_size), ggo::rand<float>(-ball_size, ball_size), ggo::rand<float>(-ball_size, ball_size));
		if (center.get_length() < ball_size)
		{
			params._centers.push_back(center);
		}
	}

#ifdef MONO_SAMPLING
  ggo::mono_sampling_renderer renderer(camera);
#else
  ggo::global_sampling_renderer renderer(camera, 56);
#endif
	ggo::metaballs_artist::render_bitmap(buffer, get_width(), get_height(), get_line_step(), get_pixel_buffer_format(), renderer, params);
}