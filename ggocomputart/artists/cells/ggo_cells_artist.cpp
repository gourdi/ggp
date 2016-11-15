#include "ggo_cells_artist.h"
#include <ggo_array.h>
#include <ggo_buffer_fill.h>
#include <ggo_interpolation1d.h>
#include <ggo_gaussian_blur2d.h>
#include <ggo_seed_paint.h>
#include <ggo_sampling_paint.h>

//////////////////////////////////////////////////////////////
ggo::cells_artist::cells_artist(int width, int height, int line_step, ggo::pixel_buffer_format pbf)
:
bitmap_artist_abc(width, height, line_step, pbf)
{

}

//////////////////////////////////////////////////////////////
void ggo::cells_artist::render_bitmap(void * buffer) const
{
  const int cells_count = 250;
  
	float hue1 = ggo::rand<float>();
	float sat1 = ggo::rand<float>();
	float hue2 = ggo::rand<float>();
	float sat2 = ggo::rand<float>();
	float hue3 = ggo::rand<float>();
	float sat3 = ggo::rand<float>();
	polynom	polynom1;
	polynom	polynom2;

	polynom1._deg2 = ggo::rand<float>(-2, 2);
	polynom1._deg1 = ggo::rand<float>(-2, 2);
	polynom1._deg0 = ggo::rand<float>(-2, 2);
	
	polynom2._deg2 = ggo::rand<float>(-2, 2);
	polynom2._deg1 = ggo::rand<float>(-2, 2);
	polynom2._deg0 = ggo::rand<float>(-2, 2);

	ggo::fill_solid<rgb_8u_yu>(buffer, get_width(), get_height(), get_line_step(),
    ggo::from_hsv<ggo::color_8u>(ggo::rand<float>(), ggo::rand<float>(), ggo::rand<float>()));
	
	for (int counter = 0; counter < cells_count; ++counter)
	{
		float blur_start = 0.005f * get_min_size();

		float pos_x = ggo::rand<float>();
		float pos_y = ggo::rand<float>();
		cell cell(pos_x - 0.02f, pos_x + 0.02f, pos_y + 0.02f, pos_y - 0.02f, get_width(), get_height());

		ggo::color_8u color;
		if (polynom1._deg2 * pos_x * pos_x + polynom1._deg1 * pos_x + polynom1._deg0 < pos_y)
		{
			color = ggo::from_hsv<color_8u>(hue1, sat1, ggo::rand<float>());
		}
		else
		if (polynom2._deg2 * pos_x * pos_x + polynom2._deg1 * pos_x + polynom2._deg0 < pos_y)
		{
			color = ggo::from_hsv<color_8u>(hue2, sat2, ggo::rand<float>());
		}
		else
		{
			color = ggo::from_hsv<color_8u>(hue3, sat3, ggo::rand<float>());
		}

    using sample_t = sampler<sampling_4x4>;

    auto paint_pixel = [&](int x, int y)
    {
      bool done = true;
      const color_8u bkgd_color = read_pixel<rgb_8u_yu>(buffer, x, y, get_height(), get_line_step());
      accumulator<color_8u> acc;

      auto sample_shape = [&](float x_f, float y_f)
      {
        if (cell.is_point_inside(x_f, y_f) == true)
        {
          acc.add(color);
          done = false;
        }
        else
        {
          acc.add(bkgd_color);
        }
      };

      sample_t::sample_pixel<float>(x, y, sample_shape);

      write_pixel<rgb_8u_yu>(buffer, x, y, get_height(), get_line_step(), acc.div<sample_t::samples_count>());

      return !done;
    };

		ggo::paint_seed_shape(get_width(), get_height(), cell, paint_pixel);
        
    // Blur.
    if (counter % 10 == 0)
    {
      float stddev = 0.0005f * get_min_size();
      
      gaussian_blur2d_mirror<rgb_8u_yu>(buffer, get_width(), get_height(), get_line_step(), stddev);
    }
	}
}

//////////////////////////////////////////////////////////////
ggo::cells_artist::cell::cell(float left, float right, float top, float bottom, int width, int height)
{
	_inv_width	= 1.f / width;
	_inv_height	= 1.f / height;
	
	_centers[0] = ggo::pos2f(ggo::rand<float>(left, right), ggo::rand<float>(bottom, top));
	_centers[1] = ggo::pos2f(ggo::rand<float>(left, right), ggo::rand<float>(bottom, top));
	_centers[2] = ggo::pos2f(ggo::rand<float>(left, right), ggo::rand<float>(bottom, top));
}

//////////////////////////////////////////////////////////////
ggo::rect_data<float> ggo::cells_artist::cell::get_seed_rect() const
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
	
	left	  /= _inv_width;
	right	  /= _inv_width;
	top		  /= _inv_height;
	bottom	/= _inv_height;
  
  return { {left, bottom}, right - left, top - bottom };
}

//////////////////////////////////////////////////////////////
bool ggo::cells_artist::cell::is_point_inside(float x, float y) const
{
	float potential = 0;
	float x_map = x * _inv_width;
	float y_map = y * _inv_height;

	potential += 100 / (1 + 100 * ggo::distance(_centers[0], ggo::pos2f(x_map, y_map)));
	potential += 100 / (1 + 100 * ggo::distance(_centers[1], ggo::pos2f(x_map, y_map)));
	potential += 100 / (1 + 100 * ggo::distance(_centers[2], ggo::pos2f(x_map, y_map)));

	return potential > 110 && potential < 120;
}

