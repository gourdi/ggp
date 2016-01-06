#include "ggo_metaballs_bitmap_artist.h"
#include "ggo_metaballs_artist.h"
#include <ggo_point_camera.h>
#include <ggo_global_sampling_renderer.h>

#define GGO_BALL_SIZE 3

//////////////////////////////////////////////////////////////
ggo_metaballs_bitmap_artist::ggo_metaballs_bitmap_artist(int render_width, int render_height)
:
ggo_bitmap_artist_abc(render_width, render_height)
{	
}

//////////////////////////////////////////////////////////////
void ggo_metaballs_bitmap_artist::render_bitmap(uint8_t * buffer)
{
	ggo_metaballs_artist::ggo_metaballs_params params;

	ggo::multi_sampling_point_camera camera(get_render_width(), get_render_height());
	camera.basis().set_pos(0, 0, 25);
	camera.set_aperture(0.1f);
	camera.set_depth_of_field_factor(ggo::rand_float(0.2f, 0.5f));
	camera.set_depth_of_field(22);

	while (params._centers.size() < 200)
	{
		ggo::point3d_float center(ggo::rand_float(-GGO_BALL_SIZE, GGO_BALL_SIZE), ggo::rand_float(-GGO_BALL_SIZE, GGO_BALL_SIZE), ggo::rand_float(-GGO_BALL_SIZE, GGO_BALL_SIZE));
		if (center.get_length() < GGO_BALL_SIZE)
		{
			params._centers.push_back(center);
		}
	}

  ggo::global_sampling_renderer renderer(camera, 56);
	ggo_metaballs_artist artist(get_render_width(), get_render_height());
	artist.render_bitmap(buffer, renderer, params);
}
