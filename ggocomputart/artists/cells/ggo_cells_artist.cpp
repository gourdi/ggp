#include "ggo_cells_artist.h"
#include <cstring>
#include <ggo_array.h>
#include <ggo_fill.h>
#include <ggo_interpolation1d.h>
#include <ggo_gaussian_blur.h>
#include <ggo_paint.h>

//////////////////////////////////////////////////////////////
ggo_cells_artist::ggo_cells_artist(int render_width, int render_height)
:
ggo_bitmap_artist_abc(render_width, render_height)
{

}

//////////////////////////////////////////////////////////////
void ggo_cells_artist::render_bitmap(uint8_t * buffer)
{
  const int CELLS_COUNT = 250;
  
	float				hue1 = ggo::rand_float();
	float				sat1 = ggo::rand_float();
	float				hue2 = ggo::rand_float();
	float				sat2 = ggo::rand_float();
	float				hue3 = ggo::rand_float();
	float				sat3 = ggo::rand_float();
	ggo_polynom	polynom1;
	ggo_polynom	polynom2;

	polynom1._deg2 = ggo::rand_float(-2, 2);
	polynom1._deg1 = ggo::rand_float(-2, 2);
	polynom1._deg0 = ggo::rand_float(-2, 2);
	
	polynom2._deg2 = ggo::rand_float(-2, 2);
	polynom2._deg1 = ggo::rand_float(-2, 2);
	polynom2._deg0 = ggo::rand_float(-2, 2);
  
  auto image = make_image_buffer(buffer);
	
	ggo::fill_solid(image, ggo::color::from_hsv(ggo::rand_float(), ggo::rand_float(), ggo::rand_float()));
	
	for (int counter = 0; counter < CELLS_COUNT; ++counter)
	{
		float blur_start = 0.005f * get_render_min_size();

		float pos_x = ggo::rand_float();
		float pos_y = ggo::rand_float();
		ggo_cell cell(pos_x - 0.02f, pos_x + 0.02f, pos_y + 0.02f, pos_y - 0.02f, get_render_width(), get_render_height());

		ggo::color color;
		if (polynom1._deg2 * pos_x * pos_x + polynom1._deg1 * pos_x + polynom1._deg0 < pos_y)
		{
			color = ggo::color::from_hsv(hue1, sat1, ggo::rand_float());
		}
		else
		if (polynom2._deg2 * pos_x * pos_x + polynom2._deg1 * pos_x + polynom2._deg0 < pos_y)
		{
			color = ggo::color::from_hsv(hue2, sat2, ggo::rand_float());
		}
		else
		{
			color = ggo::color::from_hsv(hue3, sat3, ggo::rand_float());
		}

		ggo::paint_seed_shape(image, cell, color);
        
    // Blur.
    if (counter % 10 == 0)
    {
      float variance = 0.0005f * get_render_min_size();
      
      ggo::array_uint8 tmp_buffer(3 * get_render_width() * get_render_height());

      ggo::gaussian_blur_2d_mirror<3, 3>(buffer + 0, tmp_buffer.data() + 0, get_render_width(), get_render_height(), variance, 0.001f);
      ggo::gaussian_blur_2d_mirror<3, 3>(buffer + 1, tmp_buffer.data() + 1, get_render_width(), get_render_height(), variance, 0.001f);
      ggo::gaussian_blur_2d_mirror<3, 3>(buffer + 2, tmp_buffer.data() + 2, get_render_width(), get_render_height(), variance, 0.001f);
      
      memcpy(buffer, tmp_buffer.data(), tmp_buffer.get_count());
    }
	}
}

//////////////////////////////////////////////////////////////
ggo_cells_artist::ggo_cell::ggo_cell(float left, float right, float top, float bottom, int render_width, int render_height)
{
	_inv_render_width	= 1.f / render_width;
	_inv_render_height	= 1.f / render_height;
	
	_centers[0] = ggo::pos2f(ggo::rand_float(left, right), ggo::rand_float(bottom, top));
	_centers[1] = ggo::pos2f(ggo::rand_float(left, right), ggo::rand_float(bottom, top));
	_centers[2] = ggo::pos2f(ggo::rand_float(left, right), ggo::rand_float(bottom, top));
}

//////////////////////////////////////////////////////////////
ggo::rect_data<float> ggo_cells_artist::ggo_cell::get_seed_rect() const
{
	float left	  = _centers[0].get<0>();
	float right	  = _centers[0].get<0>();
	float top		  = _centers[0].get<1>();
	float bottom	= _centers[0].get<1>();
	
	left	  = std::min(_centers[1].get<0>(), left);
	right	  = std::max(_centers[1].get<0>(), right);
	top		  = std::max(_centers[1].get<1>(), top);
	bottom	= std::min(_centers[1].get<1>(), bottom);

	left	  = std::min(_centers[2].get<0>(), left);
	right	  = std::max(_centers[2].get<0>(), right);
	top		  = std::max(_centers[2].get<1>(), top);
	bottom	= std::min(_centers[2].get<1>(), bottom);
	
	left	  /= _inv_render_width;
	right	  /= _inv_render_width;
	top		  /= _inv_render_height;
	bottom	/= _inv_render_height;
  
  return { {left, bottom}, right - left, top - bottom };
}

//////////////////////////////////////////////////////////////
bool ggo_cells_artist::ggo_cell::is_point_inside(float x, float y) const
{
	float potential = 0;
	float x_map = x * _inv_render_width;
	float y_map = y * _inv_render_height;

	potential += 100 / (1 + 100 * ggo::distance(_centers[0], ggo::pos2f(x_map, y_map)));
	potential += 100 / (1 + 100 * ggo::distance(_centers[1], ggo::pos2f(x_map, y_map)));
	potential += 100 / (1 + 100 * ggo::distance(_centers[2], ggo::pos2f(x_map, y_map)));

	return potential > 110 && potential < 120;
}

