#include "ggo_filling_squares_bitmap_artist.h"
#include "ggo_filling_squares_artist.h"
#include <ggo_buffer_fill.h>
#include <ggo_buffer_paint.h>

//////////////////////////////////////////////////////////////
ggo::filling_squares_bitmap_artist::filling_squares_bitmap_artist(int render_width, int render_height)
:
bitmap_artist_abc(render_width, render_height)
{
	
}

//////////////////////////////////////////////////////////////
void ggo::filling_squares_bitmap_artist::render_bitmap(void * buffer) const
{
	float hue;
	
  auto multi_squares = ggo::filling_squares_artist::build_squares(get_render_width(), get_render_height(), hue);
	
	ggo::fill_solid<ggo::rgb_8u_yu>(buffer, get_render_width(), get_render_height(), 3 * get_render_width(), ggo::from_hsv<ggo::color_8u>(hue, ggo::rand<float>(), ggo::rand<float>()));

	for (const auto & multi_square : multi_squares)
	{
		for (const auto & colored_square : multi_square._squares)
		{
			float half_size = colored_square._size / 2;
      float left      = multi_square._pos.get<0>() - half_size;
      float right     = multi_square._pos.get<0>() + half_size;
      float bottom    = multi_square._pos.get<1>() - half_size;
      float top       = multi_square._pos.get<1>() + half_size;
      
      ggo::polygon2d_float square;
      square.add_point(left, bottom);
      square.add_point(right, bottom);
      square.add_point(right, top);
      square.add_point(left, top);

			ggo::paint_shape<ggo::rgb_8u_yu, ggo::sampling_4x4>
        (buffer, get_render_width(), get_render_height(), 3 * get_render_width(), square, colored_square._color);
		}
	}
}
