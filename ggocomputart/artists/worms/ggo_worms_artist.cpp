#include "ggo_worms_artist.h"
#include <ggo_paint.h>
#include <ggo_fill.h>
#include <ggo_layer.h>
#include <ggo_color.h>
#include <ggo_gaussian_blur.h>
#include <ggo_paint.h>

#define GGO_COUNTER_MAX 1500

//////////////////////////////////////////////////////////////
ggo_worms_artist::ggo_worms_artist(int render_width, int render_height)
:
ggo_bitmap_artist_abc(render_width, render_height)
{
}

//////////////////////////////////////////////////////////////
void ggo_worms_artist::render_bitmap(uint8_t * buffer)
{
	ggo::fill_solid_rgb(buffer, get_render_width() * get_render_height(), ggo::color::WHITE);

	float hue1 = ggo::rand_float();
	float hue2 = ggo::rand_float();

	float variance_from = 0.01f * get_render_min_size();
	float variance_to = 0;

	for (int counter = 0; counter < GGO_COUNTER_MAX; ++counter)
	{
		std::cout << "\rRendering spline " << counter << " out of " << GGO_COUNTER_MAX << std::endl;

		ggo::spline<float> spline;
        
		// Build the spline.
		for (int i = 0; i < 10; ++i)
		{
      int sprite_size = ggo::to<int>(0.1f * get_render_min_size());
      int pos_x = ggo::rand_int(0, get_render_width());
      int pos_y = ggo::rand_int(0, get_render_height());
        
			ggo::point2d_float pt;
			pt.x() = pos_x + ggo::rand_float(0, 0.1f * get_render_min_size());
			pt.y() = pos_y + ggo::rand_float(0, 0.1f * get_render_min_size());
			spline.add_control_point(pt);
		}
		
		// Build the extended segments.
		int segments_count = 5 + 10 * counter / GGO_COUNTER_MAX; 
		auto points = spline.evaluate(segments_count);
    auto extended_segments_inside = std::make_shared<ggo::multi_shape_float>();
		auto extended_segments_border = std::make_shared<ggo::multi_shape_float>();

		for (int i = 1; i < points.size(); ++i)
		{
			const ggo::point2d_float & p1 = points[i - 1];
			const ggo::point2d_float & p2 = points[i];

			float inside_width = 0.003f * get_render_min_size();
      float border_width = 0.005f * get_render_min_size();
        
      auto inside_segment = std::make_shared<ggo::extended_segment_float>(p1, p2, inside_width);
      auto border_segment = std::make_shared<ggo::extended_segment_float>(p1, p2, border_width);
			
			extended_segments_inside->add_shape(inside_segment);
			extended_segments_border->add_shape(border_segment);
		}
			
		// Paint the spline.
    ggo::color color = ggo::color::from_hsv(ggo::rand_bool() ? hue1 : hue2, ggo::rand_float(), 1);
    ggo::paint(buffer, get_render_width(), get_render_height(), extended_segments_border, ggo::color::BLACK);
		ggo::paint(buffer, get_render_width(), get_render_height(), extended_segments_inside, color);

    // Blur the render buffer.
		float variance = 0.02f * get_render_min_size();
    
    ggo::gaussian_parameters<uint8_t> params;
    params._stride_in = 3;
    params._stride_out = 3;
		
    ggo::gaussian_blur_2d(buffer + 0, buffer + 0, get_render_width(), get_render_height(), variance, params);
    ggo::gaussian_blur_2d(buffer + 1, buffer + 1, get_render_width(), get_render_height(), variance, params);
    ggo::gaussian_blur_2d(buffer + 2, buffer + 2, get_render_width(), get_render_height(), variance, params);
	}
}
