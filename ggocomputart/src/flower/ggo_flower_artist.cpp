#include "ggo_flower_artist.h"
#include <kernel/memory/ggo_array.h>
#include <kernel/math/interpolation/ggo_curve.h>
#include <kernel/math/signal_proc/ggo_dct.h>
#include <2d/ggo_color.h>
#include <2d/paint/ggo_paint.h>
#include <2d/paint/ggo_brush.h>
#include <2d/paint/ggo_blend.h>
#include <2d/ggo_blit.h>
#include <iostream>

//////////////////////////////////////////////////////////////
ggo::flower_artist::flower_artist(int width, int height, int line_step, ggo::image_format format)
:
bitmap_artist_abc(width, height, line_step, format)
{
}

//////////////////////////////////////////////////////////////
void ggo::flower_artist::render_bitmap(void * buffer) const
{
  const int horz_count = 3000;
  const int vert_count = 1500;

	float	hue				    = ggo::rand<float>();
	int		petals_count	= ggo::rand<int>(8, 12);

	ggo::cubic_curve_float flower_shape_curve;
	flower_shape_curve.push_point(0, ggo::rand<float>(0.25f, 0.75f));
	flower_shape_curve.push_point(1, ggo::rand<float>(0.01f, 0.1f));
	flower_shape_curve.push_point(ggo::rand<float>(0.1f, 0.5f), ggo::rand<float>(0.5f, 0.9f));
	flower_shape_curve.push_point(ggo::rand<float>(0.5f, 0.9f), ggo::rand<float>(0.1f, 0.5f));

	ggo::cubic_curve_float petal_shape_curve;
	petal_shape_curve.push_point(0, 0);
	petal_shape_curve.push_point(1, 1);
	petal_shape_curve.push_point(ggo::rand<float>(0.1f, 0.5f), ggo::rand<float>(0.1f, 0.5f));
	petal_shape_curve.push_point(ggo::rand<float>(0.5f, 0.9f), ggo::rand<float>(0.5f, 0.9f));
		
  std::vector<float> render_buffer(3 * width() * height(), 0.f);

	for (int counter = 0; counter < petals_count; ++counter)
	{
		std::cout << "Rendering petal " << (counter + 1) << " out of " << petals_count << std::endl; 
		
		ggo::pos2f center(0.5f * width(), 0.9f * height());

		float ratio 		    = counter / float(petals_count);
		float petal_height	= 0.8f * height() * counter / petals_count;
		float dy 			      = petal_height / vert_count;
		float radius_coef 	= flower_shape_curve.evaluate(ratio);

		ggo::array_32f freq(horz_count / 2, 0.f);
		ggo::array_32f spat(horz_count / 2, 0.f);
		for (int i = 1; i < 10; ++i)
		{
			freq(i) = ggo::rand<float>(-1, 1);
		}
		ggo::dct(freq.data(), spat.data(), horz_count/2);

		for (int i = 0; i < vert_count; ++i)
		{
			float 		      grow 	= i / float(vert_count);
			ggo::color_32f 	color = 0.0025f * ggo::from_hsv<ggo::color_32f>(hue, grow, 1);
			float 		      s		  = 0.5f * radius_coef * petal_shape_curve.evaluate(grow) * min_size();

			for (int j = 0; j < horz_count; ++j) 
			{
				int   k = j < horz_count/2 ? j : horz_count-j-1;
				float t = 2 * ggo::pi<float>() * j / horz_count;
				float x = center.x() + s * std::cos(t);
				float y = center.y() + s * std::sin(t) / 3 + grow * 0.5f * height() * spat(k);

				ggo::paint_shape<ggo::rgb_32f_yu, ggo::sampling_4x4>(
          render_buffer.data(), width(), height(), 3 * sizeof(float) * width(),
          ggo::disc_float({ x, y }, 0.001f * min_size()),
          ggo::make_solid_brush(color), ggo::add_blender<ggo::color_32f>());
			}

			center.y() -= dy;
		}
	}

  ggo::blit<ggo::rgb_32f_yu, ggo::rgb_8u_yu>(
    render_buffer.data(), width(), height(), 3 * sizeof(float) * width(),
    buffer, width(), height(), line_step(), 0, 0);
}

