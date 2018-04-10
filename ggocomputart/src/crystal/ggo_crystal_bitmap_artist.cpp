#include "ggo_crystal_bitmap_artist.h"
#include <ggo_pbf_fill.h>

//////////////////////////////////////////////////////////////
ggo::crystal_bitmap_artist::crystal_bitmap_artist(int width, int height, int line_step, ggo::pixel_buffer_format pbf)
:
bitmap_artist_abc(width, height, line_step, pbf)
{
	
}

//////////////////////////////////////////////////////////////
void ggo::crystal_bitmap_artist::render_bitmap(void * buffer) const
{
  ggo::fill_4_colors<ggo::rgb_8u_yu>(buffer, get_width(), get_height(), get_line_step(),
    ggo::from_hsv<ggo::color_8u>(ggo::rand<float>(), ggo::rand<float>(), ggo::rand<float>(0.1f, 0.3f)),
    ggo::from_hsv<ggo::color_8u>(ggo::rand<float>(), ggo::rand<float>(), ggo::rand<float>(0.1f, 0.3f)),
    ggo::from_hsv<ggo::color_8u>(ggo::rand<float>(), ggo::rand<float>(), ggo::rand<float>(0.1f, 0.3f)),
    ggo::from_hsv<ggo::color_8u>(ggo::rand<float>(), ggo::rand<float>(), ggo::rand<float>(0.1f, 0.3f)),
    ggo::rect_int::from_width_height(get_width(), get_height()));

	ggo::crystal_artist::params params;
	ggo::crystal_artist::randomize_params(params);
  ggo::crystal_artist::render_bitmap(buffer, get_width(), get_height(), get_line_step(), get_pixel_buffer_format(), params);
}
