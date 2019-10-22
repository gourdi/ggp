#include "ggo_crystal_bitmap_artist.h"
#include <2d/fill/ggo_fill.h>

//////////////////////////////////////////////////////////////
ggo::crystal_bitmap_artist::crystal_bitmap_artist(int width, int height, int line_step, ggo::pixel_type pixel_type, ggo::lines_order memory_lines_order)
:
bitmap_artist_abc(width, height, line_step, pixel_type, memory_lines_order)
{
	
}

//////////////////////////////////////////////////////////////
void ggo::crystal_bitmap_artist::render_bitmap(void * buffer) const
{
  ggo::image_t<ggo::pixel_type::rgb_8u, ggo::lines_order::up> img(buffer, size(), line_byte_step());

  ggo::fill_4_colors(img,
    ggo::from_hsv<ggo::rgb_8u>(ggo::rand<float>(), ggo::rand<float>(), ggo::rand<float>(0.1f, 0.3f)),
    ggo::from_hsv<ggo::rgb_8u>(ggo::rand<float>(), ggo::rand<float>(), ggo::rand<float>(0.1f, 0.3f)),
    ggo::from_hsv<ggo::rgb_8u>(ggo::rand<float>(), ggo::rand<float>(), ggo::rand<float>(0.1f, 0.3f)),
    ggo::from_hsv<ggo::rgb_8u>(ggo::rand<float>(), ggo::rand<float>(), ggo::rand<float>(0.1f, 0.3f)),
    ggo::rect_int::from_width_height(width(), height()));

	ggo::crystal_artist::params params;
	ggo::crystal_artist::randomize_params(params);
  ggo::crystal_artist::render_bitmap(img, params);
}
