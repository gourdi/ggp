#include "ggo_ifs_bitmap_artist.h"
#include "ggo_ifs_artist.h"
#include <ggo_buffer_fill.h>

//////////////////////////////////////////////////////////////
ggo::ifs_bitmap_artist::ifs_bitmap_artist(int render_width, int render_height)
:
bitmap_artist_abc(render_width, render_height)
{
	
}

//////////////////////////////////////////////////////////////
void ggo::ifs_bitmap_artist::render_bitmap(void * buffer) const
{
  ggo::ifs_artist artist(get_render_width(), get_render_height());

	float transform[4];
	transform[0] = ggo::rand<float>(1, 2);
	transform[1] = ggo::rand<float>(1, 2);
	transform[2] = ggo::rand<float>(1, 2);
	transform[3] = ggo::rand<float>(1, 2);
	
	float hue = ggo::rand<float>();
	
  ggo::fill_4_colors<ggo::rgb_8u_yu>(
    buffer, get_render_width(), get_render_height(), 3 * get_render_width(),
    ggo::from_hsv<ggo::color_8u>(hue, ggo::rand<float>(), ggo::rand<float>(0, 0.25)),
    ggo::from_hsv<ggo::color_8u>(hue, ggo::rand<float>(), ggo::rand<float>(0.5, 0.75)),
    ggo::from_hsv<ggo::color_8u>(hue, ggo::rand<float>(), ggo::rand<float>(0, 0.25)),
    ggo::from_hsv<ggo::color_8u>(hue, ggo::rand<float>(), ggo::rand<float>(0.5, 0.75)));

	artist.render(buffer, transform, hue, ggo::rand<float>(0, 2 * ggo::pi<float>()), ggo::rand<float>(0, 2 * ggo::pi<float>()));
}
