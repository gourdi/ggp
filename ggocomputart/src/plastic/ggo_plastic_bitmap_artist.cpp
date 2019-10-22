#include "ggo_plastic_bitmap_artist.h"

//////////////////////////////////////////////////////////////
ggo::plastic_bitmap_artist::plastic_bitmap_artist(int width, int height, int line_byte_step, ggo::pixel_type pixel_type, ggo::lines_order memory_lines_order)
:
bitmap_artist_abc(width, height, line_byte_step, pixel_type, memory_lines_order)
{

}

//////////////////////////////////////////////////////////////
void ggo::plastic_bitmap_artist::render_bitmap(void * buffer) const
{
	std::vector<ggo::plastic_artist::params> params;
	for (int i = 0; i < 50; ++i)
	{
    ggo::plastic_artist::params param;
		
		param._pos_x = ggo::rand<float>(-2, 2);
		param._pos_y = ggo::rand<float>(-2, 2);
		param._den = ggo::rand<float>(0.5, 2);
		param._mult = ggo::rand<float>(0.5, 2);
		param._power = ggo::rand<float>(5, 10);
		
		params.push_back(param);
	}
	
	ggo::rgb_32f color = ggo::from_hsv<ggo::rgb_32f>(ggo::rand<float>(), ggo::rand<float>(), 1);
	float altitude_factor = ggo::rand<float>(0.04f, 0.08f);
	
  ggo::plastic_artist::render(buffer, width(), height(), line_byte_step(), pixel_type(), memory_lines_order(),
    params, color, altitude_factor);
}
