#include "ggo_crystal_bitmap_artist.h"
#include <2d/fill/ggo_fill.h>

//////////////////////////////////////////////////////////////
ggo::crystal_bitmap_artist::crystal_bitmap_artist(int width, int height, int line_step, ggo::image_format format)
:
bitmap_artist_abc(width, height, line_step, format)
{
	
}

//////////////////////////////////////////////////////////////
void ggo::crystal_bitmap_artist::render_bitmap(void * buffer) const
{
  ggo::fill_4_colors<ggo::rgb_8u_yu>(buffer, width(), height(), line_step(),
    ggo::from_hsv<ggo::rgb_8u>(ggo::rand<float>(), ggo::rand<float>(), ggo::rand<float>(0.1f, 0.3f)),
    ggo::from_hsv<ggo::rgb_8u>(ggo::rand<float>(), ggo::rand<float>(), ggo::rand<float>(0.1f, 0.3f)),
    ggo::from_hsv<ggo::rgb_8u>(ggo::rand<float>(), ggo::rand<float>(), ggo::rand<float>(0.1f, 0.3f)),
    ggo::from_hsv<ggo::rgb_8u>(ggo::rand<float>(), ggo::rand<float>(), ggo::rand<float>(0.1f, 0.3f)),
    ggo::rect_int::from_width_height(width(), height()));

	ggo::crystal_artist::params params;
	ggo::crystal_artist::randomize_params(params);
  ggo::crystal_artist::render_bitmap(buffer, width(), height(), line_step(), format(), params);
}
