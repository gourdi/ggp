#include "ggo_ifs_bitmap_artist.h"
#include "ggo_ifs_artist.h"
#include <ggo_buffer_fill.h>

//////////////////////////////////////////////////////////////
ggo::ifs_bitmap_artist::ifs_bitmap_artist(int width, int height, int line_step, ggo::pixel_buffer_format pbf)
:
bitmap_artist_abc(width, height, line_step, pbf)
{
	
}

//////////////////////////////////////////////////////////////
void ggo::ifs_bitmap_artist::render_bitmap(void * buffer) const
{
  ggo::ifs_artist artist(get_width(), get_height());

	float transform[4];
	transform[0] = ggo::rand<float>(1, 2);
	transform[1] = ggo::rand<float>(1, 2);
	transform[2] = ggo::rand<float>(1, 2);
	transform[3] = ggo::rand<float>(1, 2);
	
	float hue = ggo::rand<float>();
	
  ggo::fill_4_colors<ggo::rgb_8u_yu>(
    buffer, get_width(), get_height(), get_line_step(),
    ggo::from_hsv<ggo::color_8u>(hue, ggo::rand<float>(), ggo::rand<float>(0, 0.25)),
    ggo::from_hsv<ggo::color_8u>(hue, ggo::rand<float>(), ggo::rand<float>(0.5, 0.75)),
    ggo::from_hsv<ggo::color_8u>(hue, ggo::rand<float>(), ggo::rand<float>(0, 0.25)),
    ggo::from_hsv<ggo::color_8u>(hue, ggo::rand<float>(), ggo::rand<float>(0.5, 0.75)));

	artist.render(buffer, get_line_step(), get_pixel_buffer_format(), transform, hue, ggo::rand<float>(0, 2 * ggo::pi<float>()), ggo::rand<float>(0, 2 * ggo::pi<float>()));
}
