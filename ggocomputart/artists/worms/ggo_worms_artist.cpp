#include "ggo_worms_artist.h"
#include <ggo_buffer_fill.h>
#include <ggo_buffer_paint.h>
#include <ggo_gaussian_blur2d.h>

//////////////////////////////////////////////////////////////
ggo::worms_artist::worms_artist(int render_width, int render_height)
:
bitmap_artist_abc(render_width, render_height)
{
}

//////////////////////////////////////////////////////////////
void ggo::worms_artist::render_bitmap(void * buffer) const
{
  const int counter_max = 1500;

	ggo::fill_solid<ggo::rgb_8u_yu>(buffer, get_render_width(), get_render_height(), 3 * get_render_width(), ggo::white<ggo::color_8u>());

	float hue1 = ggo::rand<float>();
	float hue2 = ggo::rand<float>();

	float variance_from = 0.01f * get_render_min_size();
	float variance_to = 0;

	for (int counter = 0; counter < counter_max; ++counter)
	{
		std::cout << "\rRendering spline " << counter << " out of " << counter_max << std::endl;

		ggo::spline<float> spline;
        
		// Build the spline.
		for (int i = 0; i < 10; ++i)
		{
      int sprite_size = ggo::to<int>(0.1f * get_render_min_size());
      int pos_x = ggo::rand<int>(0, get_render_width());
      int pos_y = ggo::rand<int>(0, get_render_height());
        
			ggo::pos2f pt;
			pt.get<0>() = pos_x + ggo::rand<float>(0, 0.1f * get_render_min_size());
			pt.get<1>() = pos_y + ggo::rand<float>(0, 0.1f * get_render_min_size());
			spline.add_control_point(pt);
		}
		
		// Build the extended segments.
		int segments_count = 5 + 10 * counter / counter_max;
		auto points = spline.evaluate(segments_count);
    ggo::multi_shape_float extended_segments_inside;
    ggo::multi_shape_float extended_segments_border;

		for (int i = 1; i < points.size(); ++i)
		{
			const ggo::pos2f & p1 = points[i - 1];
			const ggo::pos2f & p2 = points[i];

			float inside_width = 0.003f * get_render_min_size();
      float border_width = 0.005f * get_render_min_size();
        
      auto inside_segment = std::make_shared<ggo::extended_segment_float>(p1, p2, inside_width);
      auto border_segment = std::make_shared<ggo::extended_segment_float>(p1, p2, border_width);
			
			extended_segments_inside.add_shape(inside_segment);
			extended_segments_border.add_shape(border_segment);
		}
			
		// Paint the spline.
    const ggo::color_8u color = ggo::from_hsv<ggo::color_8u>(ggo::rand_bool() ? hue1 : hue2, ggo::rand<float>(), 1);
    ggo::paint_shape<ggo::rgb_8u_yu, ggo::sampling_4x4>(
      buffer, get_render_width(), get_render_height(), 3 * get_render_width(),
      extended_segments_border, ggo::black<ggo::color_8u>());

		ggo::paint_shape<ggo::rgb_8u_yu, ggo::sampling_4x4>(
      buffer, get_render_width(), get_render_height(), 3 * get_render_width(),
      extended_segments_inside, color);

    // Blur the render buffer.
		float stddev = 0.02f * get_render_min_size();
    ggo::gaussian_blur2d_mirror<ggo::rgb_8u_yu>(buffer, get_render_width(), get_render_height(), 3 * get_render_width(), stddev);
	}
}
