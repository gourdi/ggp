#include "ggo_crystal_bitmap_artist.h"

//////////////////////////////////////////////////////////////
ggo_crystal_bitmap_artist::ggo_crystal_bitmap_artist(int render_width, int render_height)
:
ggo_bitmap_artist_abc(render_width, render_height)
{
	
}

//////////////////////////////////////////////////////////////
void ggo_crystal_bitmap_artist::render_bitmap(uint8_t * buffer)
{
	ggo_crystal_artist artist(get_render_width(), get_render_height());
	
	ggo::color bkgd_colors[4];
	bkgd_colors[0] = ggo::color::from_hsv(ggo::rand_float(0, 1), ggo::rand_float(0, 1), ggo::rand_float(0.1f, 0.3f));
	bkgd_colors[1] = ggo::color::from_hsv(ggo::rand_float(0, 1), ggo::rand_float(0, 1), ggo::rand_float(0.1f, 0.3f));
	bkgd_colors[2] = ggo::color::from_hsv(ggo::rand_float(0, 1), ggo::rand_float(0, 1), ggo::rand_float(0.1f, 0.3f));
	bkgd_colors[3] = ggo::color::from_hsv(ggo::rand_float(0, 1), ggo::rand_float(0, 1), ggo::rand_float(0.1f, 0.3f));

	ggo_crystal_artist::ggo_params params;
	ggo_crystal_artist::randomize_params(params);
	
	artist.render_bitmap(buffer, params, bkgd_colors);
}
