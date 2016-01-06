#include "ggo_ifs_bitmap_artist.h"
#include <ggo_fill.h>

//////////////////////////////////////////////////////////////
ggo_ifs_bitmap_artist::ggo_ifs_bitmap_artist(int render_width, int render_height)
:
ggo_bitmap_artist_abc(render_width, render_height),
_artist(render_width, render_height)
{
	
}

//////////////////////////////////////////////////////////////
void ggo_ifs_bitmap_artist::render_bitmap(uint8_t * buffer)
{
	float transform[4];
	transform[0] = ggo::rand_float(1, 2);
	transform[1] = ggo::rand_float(1, 2);
	transform[2] = ggo::rand_float(1, 2);
	transform[3] = ggo::rand_float(1, 2);
	
	float hue = ggo::rand_float();
	
  ggo::rgb_image_data_uint8 image_data(buffer, get_render_width(), get_render_height());
	ggo::fill_4_colors(image_data, 
                     ggo::color::from_hsv(hue, ggo::rand_float(), ggo::rand_float(0, 0.25)),
                     ggo::color::from_hsv(hue, ggo::rand_float(), ggo::rand_float(0.5, 0.75)),
                     ggo::color::from_hsv(hue, ggo::rand_float(), ggo::rand_float(0, 0.25)),
                     ggo::color::from_hsv(hue, ggo::rand_float(), ggo::rand_float(0.5, 0.75)));

	_artist.render(buffer, transform, hue, ggo::rand_float(0, 2 * ggo::PI<float>()), ggo::rand_float(0, 2 * ggo::PI<float>()));
}