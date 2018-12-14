#include "ggo_filling_squares_bitmap_artist.h"
#include "ggo_filling_squares_artist.h"
#include <2d/fill/ggo_fill.h>
#include <2d/paint/ggo_paint.h>

//////////////////////////////////////////////////////////////
ggo::filling_squares_bitmap_artist::filling_squares_bitmap_artist(int width, int height, int line_step, ggo::image_format format)
:
bitmap_artist_abc(width, height, line_step, format)
{
	
}

//////////////////////////////////////////////////////////////
void ggo::filling_squares_bitmap_artist::render_bitmap(void * buffer) const
{
	float hue;
	
  auto multi_squares = ggo::filling_squares_artist::build_squares(width(), height(), hue);
	
	ggo::fill_solid<ggo::rgb_8u_yu>(buffer, width(), height(), line_step(),
    ggo::from_hsv<ggo::rgb_8u>(hue, ggo::rand<float>(), ggo::rand<float>()),
    ggo::rect_int::from_width_height(width(), height()));

	for (const auto & multi_square : multi_squares)
	{
		for (const auto & colored_square : multi_square._squares)
		{
			float half_size = colored_square._size / 2;
      float left      = multi_square._pos.x() - half_size;
      float right     = multi_square._pos.x() + half_size;
      float bottom    = multi_square._pos.y() - half_size;
      float top       = multi_square._pos.y() + half_size;
      
      ggo::polygon2d_float square;
      square.add_point(left, bottom);
      square.add_point(right, bottom);
      square.add_point(right, top);
      square.add_point(left, top);

			ggo::paint<ggo::rgb_8u_yu, ggo::sampling_4x4>
        (buffer, width(), height(), line_step(), square, colored_square._color);
		}
	}
}
