#include "ggo_mondrian_artist.h"
#include <ggo_color_conv.h>
#include <ggo_fill.h>
#include <ggo_paint.h>
#include <iostream>

//////////////////////////////////////////////////////////////
ggo_mondrian_artist::ggo_mondrian_artist(int render_width, int render_height)
:
ggo_bitmap_artist_abc(render_width, render_height)
{

}

//////////////////////////////////////////////////////////////
void ggo_mondrian_artist::render_bitmap(uint8_t * buffer)
{
	float	hue		= ggo::rand_float();
	float	range	= static_cast<float>(get_render_max_size());
	
	std::vector<ggo_area>	areas;
	for (int i = 0; i < 15; ++i)
	{
		ggo_area	area;
		
		float rnd = ggo::rand_float();
		if (rnd < 0.2)
		{
			area._color	= ggo::color::from_hsv(1 - hue, ggo::rand_float(), 1);
		}
		else
		if (rnd < 0.4)
		{
			area._color	= ggo::color(ggo::rand_float());
		}
		else
		{
			area._color	= ggo::color::from_hsv(hue, ggo::rand_float(), 1);
		}
		
		float x = ggo::rand_float(0, static_cast<float>(get_render_width()));
		float y = ggo::rand_float(0, static_cast<float>(get_render_height()));
		float range_x = ggo::rand_float(0, range);
		float range_y = ggo::rand_float(0, range);

		ggo::polygon2d_float::create_axis_aligned_box(x - range_x, x + range_x, y - range_y, y + range_y, area._rect);
		areas.push_back(area);
		
		range *= 0.9f;
	}

	float 		 size1 		  = ggo::rand_float(0.7f, 1 / 0.7f) * get_render_min_size() * 0.001f;
	float 		 size2 		  = ggo::rand_float(0.7f, 1 / 0.7f) * get_render_min_size() * 0.01f;
	ggo::color base_color	= ggo::color::from_hsv(hue, ggo::rand_float(), 1);
	
	ggo::fill_solid_rgb(buffer, get_render_width() * get_render_height(), base_color);
	
	for (int i = 0; i < 200000; ++i)
	{
		float				angle		  = ggo::rand_float(-0.2f, 0.2f);
		ggo::pos2f  center		= get_random_point();
		ggo::vec2f	direction	= ggo::vec2f(std::cos(angle), std::sin(angle));
	
		auto box = std::make_shared<ggo::polygon2d_float>();
		ggo::polygon2d_float::create_oriented_box(center, direction, size1, size2, *box);

		// Get a color depending of the area.
		ggo::color color = base_color;
		for (const auto & area : areas)
		{
			if (area._rect.is_point_inside(center) == true)
			{
				color = area._color;
			}
		}

		float factor = ggo::rand_float(0.9f, 1 / 0.9f);
		color.r() *= ggo::clamp<float>(color.r() * factor, 0, 1);
		color.g() *= ggo::clamp<float>(color.g() * factor, 0, 1);
		color.b() *= ggo::clamp<float>(color.b() * factor, 0, 1);

		// Render the current box.
		ggo::paint(buffer, get_render_width(), get_render_height(), box, color);
	}
}

