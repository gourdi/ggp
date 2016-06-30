#include "ggo_filling_squares_bitmap_artist.h"
#include "ggo_filling_squares_artist.h"
#include "ggo_fill.h"
#include "ggo_paint.h"

//////////////////////////////////////////////////////////////
ggo_filling_squares_bitmap_artist::ggo_filling_squares_bitmap_artist(int render_width, int render_height)
:
ggo_bitmap_artist_abc(render_width, render_height)
{
	
}

//////////////////////////////////////////////////////////////
void ggo_filling_squares_bitmap_artist::render_bitmap(uint8_t * buffer)
{
	float hue;
	std::vector<ggo_filling_squares_artist::ggo_multi_square> multi_squares;

	ggo_filling_squares_artist::build_squares(get_render_width(), get_render_height(), hue, multi_squares);
	
	ggo::fill_solid_rgb(buffer, get_render_width() * get_render_height(), ggo::color::from_hsv(hue, ggo::rand_float(), ggo::rand_float()));

	for (const auto & multi_square : multi_squares)
	{
		for (const auto & colored_square : multi_square._squares)
		{
			float half_size = colored_square._size / 2;
      float left      = multi_square._pos.get<0>() - half_size;
      float right     = multi_square._pos.get<0>() + half_size;
      float bottom    = multi_square._pos.get<1>() - half_size;
      float top       = multi_square._pos.get<1>() + half_size;
      
      auto square = std::make_shared<ggo::polygon2d_float>();
      square->add_point(left, bottom);
      square->add_point(right, bottom);
      square->add_point(right, top);
      square->add_point(left, top);

			ggo::paint(buffer, get_render_width(), get_render_height(),
                 square,
                 colored_square._color);
		}
	}
}
