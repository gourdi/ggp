#include "ggo_ifs_bitmap_artist.h"
#include "ggo_ifs_artist.h"
#include <2d/fill/ggo_fill.h>

//////////////////////////////////////////////////////////////
ggo::ifs_bitmap_artist::ifs_bitmap_artist(int width, int height, int line_byte_step, ggo::pixel_type pixel_type, ggo::lines_order memory_lines_order)
:
bitmap_artist_abc(width, height, line_byte_step, pixel_type, memory_lines_order)
{
	
}

//////////////////////////////////////////////////////////////
void ggo::ifs_bitmap_artist::render_bitmap(void * buffer) const
{
	float transform[4];
	transform[0] = ggo::rand<float>(1, 2);
	transform[1] = ggo::rand<float>(1, 2);
	transform[2] = ggo::rand<float>(1, 2);
	transform[3] = ggo::rand<float>(1, 2);
	
	float hue = ggo::rand<float>();
	
  ggo::image_t<ggo::pixel_type::rgb_8u, ggo::lines_order::up> img(buffer, size(), line_byte_step());

  ggo::fill_4_colors(img,
    ggo::from_hsv<ggo::rgb_8u>(hue, ggo::rand<float>(), ggo::rand<float>(0, 0.25)),
    ggo::from_hsv<ggo::rgb_8u>(hue, ggo::rand<float>(), ggo::rand<float>(0.5, 0.75)),
    ggo::from_hsv<ggo::rgb_8u>(hue, ggo::rand<float>(), ggo::rand<float>(0, 0.25)),
    ggo::from_hsv<ggo::rgb_8u>(hue, ggo::rand<float>(), ggo::rand<float>(0.5, 0.75)),
    ggo::rect_int::from_size(size()));

  ggo::ifs_artist artist(width(), height());
	artist.render(buffer, line_byte_step(), pixel_type(), memory_lines_order(),
    transform, hue, ggo::rand<float>(0, 2 * ggo::pi<float>()), ggo::rand<float>(0, 2 * ggo::pi<float>()));
}
