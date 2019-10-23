#include "ggo_filling_squares_bitmap_artist.h"
#include "ggo_filling_squares_artist.h"
#include <2d/fill/ggo_fill.h>
#include <2d/paint/ggo_paint.h>

//////////////////////////////////////////////////////////////
void ggo::filling_squares_bitmap_artist::render_bitmap(void * buffer, int width, int height, int line_byte_step, ggo::pixel_type pixel_type, ggo::lines_order memory_lines_order) const
{
  ggo::image_t<ggo::pixel_type::rgb_8u, ggo::lines_order::up> view(buffer, { width, height }, line_byte_step);

	float hue;
	
  auto multi_squares = ggo::filling_squares_artist::build_squares(width, height, hue);
	
	ggo::fill_solid(view, ggo::from_hsv<ggo::rgb_8u>(hue, ggo::rand<float>(), ggo::rand<float>()));

	for (const auto & multi_square : multi_squares)
	{
		for (const auto & colored_square : multi_square._squares)
		{
			float half_size = colored_square._size / 2;
      float left      = multi_square._pos.x() - half_size;
      float right     = multi_square._pos.x() + half_size;
      float bottom    = multi_square._pos.y() - half_size;
      float top       = multi_square._pos.y() + half_size;
      
      ggo::polygon2d_f square;
      square.add_point(left, bottom);
      square.add_point(right, bottom);
      square.add_point(right, top);
      square.add_point(left, top);

			ggo::paint<ggo::sampling_4x4>(view, square, colored_square._color);
		}
	}
}
