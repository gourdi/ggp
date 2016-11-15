#include "ggo_vortex_artist.h"
#include <ggo_buffer_paint.h>
#include <ggo_buffer_fill.h>
#include <ggo_brush.h>
#include <ggo_blender.h>

//////////////////////////////////////////////////////////////
ggo::vortex_artist::vortex_artist(int width, int height, int line_step, ggo::pixel_buffer_format pbf)
:
bitmap_artist_abc(width, height, line_step, pbf)
{

}

//////////////////////////////////////////////////////////////
void ggo::vortex_artist::render_bitmap(void * buffer) const
{
  const int points_count = 75;

	ggo::fill_solid<ggo::rgb_8u_yu>(buffer, get_width(), get_height(), get_line_step(), ggo::black<ggo::color_8u>());

	float hue				      = ggo::rand<float>();
	float	sat				      = ggo::rand<float>();
	float	vortex_angle	  = ggo::rand<float>(0.2f, 0.5f);
	float	vortex_speed	  = ggo::rand<float>(0.04f, 0.08f);
	int		ellipse_count1	= ggo::rand<int>(50, 70);
	int		ellipse_count2	= ggo::rand<int>(20, 30);

	for (int i1 = 0; i1 < ellipse_count1; ++i1)
	{
		ggo::polygon2d_float	eclipse;

		float m		  = ggo::rand<float>(0.001f, 2.f);
		float n		  = ggo::rand<float>(0.001f, 2.f);
		float a		  = ggo::rand<float>(0.1f, 0.2f);
		float b		  = ggo::rand<float>(0.1f, 0.2f);
		float pos_x	= ggo::rand<float>(-1.5f, 1.5f);
		float pos_y	= ggo::rand<float>(-1.5f, 1.5f);

		for (int i = 0; i < points_count; ++i)
		{
			float angle	= 2 * ggo::pi<float>() * i / points_count;
			float x 	= pos_x + a * std::pow(std::abs(std::cos(angle)), m) * ggo::sign(std::cos(angle));
			float y 	= pos_y + b * std::pow(std::abs(std::sin(angle)), n) * ggo::sign(std::sin(angle));
			eclipse.add_point(x, y);
		}
		
		for (int i2 = 0; i2 < ellipse_count2; ++i2)
		{
			// Apply the vortex.
			for (int i = 0; i < eclipse.get_points_count(); ++i)
			{
				float x		  = eclipse.get_point(i).get<0>();
				float y		  = eclipse.get_point(i).get<1>();
				float angle	= std::atan2(y, x);
				float dist	= std::sqrt(x * x + y * y);

				eclipse.get_point(i).get<0>() = x + vortex_speed * std::pow(dist, 0.5f) * std::cos(angle + ggo::pi<float>() / 2 + vortex_angle);
				eclipse.get_point(i).get<1>() = y + vortex_speed * std::pow(dist, 0.5f) * std::sin(angle + ggo::pi<float>() / 2 + vortex_angle);
			}

			// Switch to render coordinates.
      ggo::polygon2d_float eclipse_render;
			for (int i = 0; i < eclipse.get_points_count(); ++i)
			{
				ggo::pos2f pt = map_fit(eclipse.get_point(i), -1, 1);
        
				eclipse_render.add_point(pt);
			}

			// Paint the shape (change a bit the color).
			float h = std::fmod(hue + ggo::rand<float>(-0.1f, 0.1f), 1.f); 
			if (h < 0) { h += 1; }
			
			float s = ggo::clamp(sat + ggo::rand<float>(-0.1f, 0.1f), 0.f, 1.f);
			
			ggo::paint_shape<ggo::rgb_8u_yu, ggo::sampling_4x4>(
        buffer, get_width(), get_height(), get_line_step(),
        eclipse_render, ggo::white_brush_8u(), ggo::alpha_blender_rgb8u(0.2f));
		}
	}
}
