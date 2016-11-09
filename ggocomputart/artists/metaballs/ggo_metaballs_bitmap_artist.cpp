#include "ggo_metaballs_bitmap_artist.h"
#include "ggo_metaballs_artist.h"
#include <ggo_point_camera.h>
#include <ggo_global_sampling_renderer.h>
#include <ggo_mono_sampling_renderer.h>

#define MONO_SAMPLING

//////////////////////////////////////////////////////////////
ggo::metaballs_bitmap_artist::metaballs_bitmap_artist(int render_width, int render_height)
:
bitmap_artist_abc(render_width, render_height)
{	
}

//////////////////////////////////////////////////////////////
void ggo::metaballs_bitmap_artist::render_bitmap(void * buffer) const
{
  const float ball_size = 3.f;

	ggo::metaballs_artist::params params;

#ifdef MONO_SAMPLING
  ggo::mono_sampling_point_camera camera(get_render_width(), get_render_height());
#else
  ggo::multi_sampling_point_camera camera(get_render_width(), get_render_height());
  camera.set_depth_of_field_factor(ggo::rand<float>(0.2f, 0.5f));
  camera.set_depth_of_field(22);
#endif
	camera.basis().set_pos(0, 0, 25);
	camera.set_aperture(0.1f);

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
	ggo::metaballs_artist artist(get_render_width(), get_render_height());
	artist.render_bitmap(buffer, renderer, params);
}
