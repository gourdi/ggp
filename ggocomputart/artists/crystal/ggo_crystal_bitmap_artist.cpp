#include "ggo_crystal_bitmap_artist.h"

//////////////////////////////////////////////////////////////
ggo::crystal_bitmap_artist::crystal_bitmap_artist(int render_width, int render_height)
:
bitmap_artist_abc(render_width, render_height)
{
	
}

//////////////////////////////////////////////////////////////
void ggo::crystal_bitmap_artist::render_bitmap(void * buffer) const
{
	ggo::crystal_artist artist(get_render_width(), get_render_height());
	
	ggo::color_8u bkgd_colors[4];
	bkgd_colors[0] = ggo::from_hsv<color_8u>(ggo::rand<float>(), ggo::rand<float>(), ggo::rand<float>(0.1f, 0.3f));
	bkgd_colors[1] = ggo::from_hsv<color_8u>(ggo::rand<float>(), ggo::rand<float>(), ggo::rand<float>(0.1f, 0.3f));
	bkgd_colors[2] = ggo::from_hsv<color_8u>(ggo::rand<float>(), ggo::rand<float>(), ggo::rand<float>(0.1f, 0.3f));
	bkgd_colors[3] = ggo::from_hsv<color_8u>(ggo::rand<float>(), ggo::rand<float>(), ggo::rand<float>(0.1f, 0.3f));

	ggo::crystal_artist::params params;
	ggo::crystal_artist::randomize_params(params);
	
	artist.render_bitmap(static_cast<uint8_t *>(buffer), params, bkgd_colors);
}
