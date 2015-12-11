#include "ggo_cumbia_bitmap_artist.h"
#include "ggo_cumbia_artist.h"
#include <ggo_point_camera.h>
#include <ggo_global_sampling_renderer.h>

//////////////////////////////////////////////////////////////
ggo_cumbia_bitmap_artist::ggo_cumbia_bitmap_artist(int render_width, int render_height)
:
ggo_bitmap_artist_abc(render_width, render_height)
{
}

//////////////////////////////////////////////////////////////
void ggo_cumbia_bitmap_artist::render_bitmap(uint8_t * buffer)
{
	ggo::multi_sampling_point_camera camera(get_render_width(), get_render_height());

	ggo_cumbia_artist artist;
	artist.init(camera, 1 << 17);
  
  ggo::global_sampling_renderer renderer(camera, 56);
	artist.render_bitmap(buffer, get_render_width(), get_render_height(), renderer);
}

