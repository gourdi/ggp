#include "ggo_flower_artist.h"
#include <ggo_array.h>
#include <ggo_dct.h>
#include <ggo_paint.h>
#include <ggo_fill.h>
#include <ggo_color.h>
#include <iostream>

#define GGO_HORZ_COUNT 	3000
#define GGO_VERT_COUNT	1500

//////////////////////////////////////////////////////////////
ggo_flower_artist::ggo_flower_artist(int render_width, int render_height)
:
ggo_bitmap_artist_abc(render_width, render_height)
{
}

//////////////////////////////////////////////////////////////
void ggo_flower_artist::render_bitmap(uint8_t * buffer)
{
	float	hue				    = ggo::rand_float();
	int		petals_count	= ggo::rand_int(8, 12);

	ggo::cubic_curve_float flower_shape_curve;
	flower_shape_curve.push_point(0, ggo::rand_float(0.25f, 0.75f));
	flower_shape_curve.push_point(1, ggo::rand_float(0.01f, 0.1f));
	flower_shape_curve.push_point(ggo::rand_float(0.1f, 0.5f), ggo::rand_float(0.5f, 0.9f));
	flower_shape_curve.push_point(ggo::rand_float(0.5f, 0.9f), ggo::rand_float(0.1f, 0.5f));

	ggo::cubic_curve_float petal_shape_curve;
	petal_shape_curve.push_point(0, 0);
	petal_shape_curve.push_point(1, 1);
	petal_shape_curve.push_point(ggo::rand_float(0.1f, 0.5f), ggo::rand_float(0.1f, 0.5f));
	petal_shape_curve.push_point(ggo::rand_float(0.5f, 0.9f), ggo::rand_float(0.5f, 0.9f));
		
  ggo::rgb_image_buffer_float render_buffer(get_render_width(), get_render_height());
	render_buffer.fill(ggo::color::BLACK);

	for (int counter = 0; counter < petals_count; ++counter)
	{
		std::cout << "Rendering petal " << (counter + 1) << " out of " << petals_count << std::endl; 
		
		ggo::pos2f center(0.5f * get_render_width(), 0.9f * get_render_height());

		float ratio 		    = counter / float(petals_count);
		float petal_height	= 0.8f * get_render_height() * counter / petals_count;
		float dy 			      = petal_height / GGO_VERT_COUNT;
		float radius_coef 	= flower_shape_curve.evaluate(ratio);

		ggo::array_float freq(GGO_HORZ_COUNT / 2, 0.f);
		ggo::array_float spat(GGO_HORZ_COUNT / 2, 0.f);
		for (int i = 1; i < 10; ++i)
		{
			freq(i) = ggo::rand_float(-1, 1);
		}
		ggo::dct(freq.data(), spat.data(), GGO_HORZ_COUNT/2);

		for (int i = 0; i < GGO_VERT_COUNT; ++i)
		{
			float 		  grow 	= i / float(GGO_VERT_COUNT);
			ggo::color 	color = ggo::color::from_hsv(hue, grow, 1);
			float 		  s		  = 0.5f * radius_coef * petal_shape_curve.evaluate(grow) * get_render_min_size();

			for (int j = 0; j < GGO_HORZ_COUNT; ++j) 
			{
				int   k = j < GGO_HORZ_COUNT/2 ? j : GGO_HORZ_COUNT-j-1;
				float t = 2 * ggo::PI<float>() * j / GGO_HORZ_COUNT;
				float x = center.get<0>() + s * cos(t);
				float y = center.get<1>() + s * sin(t) / 3 + grow * 0.5f * get_render_height() * spat(k);

				ggo::paint(render_buffer, 
                   std::make_shared<ggo::disc_float>(x, y, 0.001f * get_render_min_size()),
                   color, 0.0025f, std::make_shared<ggo::rgb_additive_blender>());
			}

			center.get<1>() -= dy;
		}
	}
  
  auto image_buffer = make_image_buffer(buffer);
	image_buffer.from(render_buffer);
}

