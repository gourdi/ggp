#include "ggo_ifs_bitmap_artist.h"
#include "ggo_ifs_artist.h"
#include <2d/fill/ggo_fill.h>

//////////////////////////////////////////////////////////////
ggo::ifs_bitmap_artist::ifs_bitmap_artist(int width, int height, int line_step, ggo::image_format format)
:
bitmap_artist_abc(width, height, line_step, format)
{
	
}

//////////////////////////////////////////////////////////////
void ggo::ifs_bitmap_artist::render_bitmap(void * buffer) const
{
  ggo::ifs_artist artist(width(), height(), line_step(), format());

	float transform[4];
	transform[0] = ggo::rand<float>(1, 2);
	transform[1] = ggo::rand<float>(1, 2);
	transform[2] = ggo::rand<float>(1, 2);
	transform[3] = ggo::rand<float>(1, 2);
	
	float hue = ggo::rand<float>();
	
  ggo::fill_4_colors<ggo::rgb_8u_yu>(
    buffer, width(), height(), line_step(),
    ggo::from_hsv<ggo::color_8u>(hue, ggo::rand<float>(), ggo::rand<float>(0, 0.25)),
    ggo::from_hsv<ggo::color_8u>(hue, ggo::rand<float>(), ggo::rand<float>(0.5, 0.75)),
    ggo::from_hsv<ggo::color_8u>(hue, ggo::rand<float>(), ggo::rand<float>(0, 0.25)),
    ggo::from_hsv<ggo::color_8u>(hue, ggo::rand<float>(), ggo::rand<float>(0.5, 0.75)),
    ggo::rect_int::from_size(size()));

	artist.render(buffer, transform, hue, ggo::rand<float>(0, 2 * ggo::pi<float>()), ggo::rand<float>(0, 2 * ggo::pi<float>()));
}
