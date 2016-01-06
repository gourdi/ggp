#include "ggo_vortex_artist.h"
#include <ggo_paint.h>
#include <ggo_fill.h>
#include <ggo_color_conv.h>
using namespace std;

#define GGO_POINTS_COUNT 75

//////////////////////////////////////////////////////////////
ggo_vortex_artist::ggo_vortex_artist(int render_width, int render_height)
:
ggo_bitmap_artist_abc(render_width, render_height)
{

}

//////////////////////////////////////////////////////////////
void ggo_vortex_artist::render_bitmap(uint8_t * buffer)
{
	ggo::fill_solid_rgb(buffer, get_render_width() * get_render_height(), ggo::color::BLACK);

	float hue				      = ggo::rand_float();
	float	sat				      = ggo::rand_float();
	float	vortex_angle	  = ggo::rand_float(0.2f, 0.5f);
	float	vortex_speed	  = ggo::rand_float(0.04f, 0.08f);
	int		ellipse_count1	= ggo::rand_int(50, 70);
	int		ellipse_count2	= ggo::rand_int(20, 30);

	for (int i1 = 0; i1 < ellipse_count1; ++i1)
	{
		ggo::polygon2d_float	eclipse;

		float m		= ggo::rand_float(0.001f, 2.f);
		float n		= ggo::rand_float(0.001f, 2.f);
		float a		= ggo::rand_float(0.1f, 0.2f);
		float b		= ggo::rand_float(0.1f, 0.2f);
		float pos_x	= ggo::rand_float(-1.5f, 1.5f);
		float pos_y	= ggo::rand_float(-1.5f, 1.5f);

		for (int i = 0; i < GGO_POINTS_COUNT; ++i)
		{
			float angle	= 2 * ggo::PI<float>() * i / GGO_POINTS_COUNT;
			float x 	= pos_x + a * std::pow(std::abs(std::cos(angle)), m) * ggo::sign(std::cos(angle));
			float y 	= pos_y + b * std::pow(std::abs(std::sin(angle)), n) * ggo::sign(std::sin(angle));
			eclipse.add_point(x, y);
		}
		
		for (int i2 = 0; i2 < ellipse_count2; ++i2)
		{
			// Apply the vortex.
			for (int i = 0; i < eclipse.get_points_count(); ++i)
			{
				float x		  = eclipse.get_point(i).x();
				float y		  = eclipse.get_point(i).y();
				float angle	= std::atan2(y, x);
				float dist	= std::sqrt(x * x + y * y);

				eclipse.get_point(i).x() = x + vortex_speed * std::pow(dist, 0.5f) * std::cos(angle + ggo::PI<float>() / 2 + vortex_angle);
				eclipse.get_point(i).y() = y + vortex_speed * std::pow(dist, 0.5f) * std::sin(angle + ggo::PI<float>() / 2 + vortex_angle);
			}

			// Switch to render coordinates.
			auto eclipse_render = std::make_shared<ggo::polygon2d_float>();
			for (int i = 0; i < eclipse.get_points_count(); ++i)
			{
				ggo::point2d_float pt = map_fit(eclipse.get_point(i), -1, 1);
        
				eclipse_render->add_point(pt);
			}

			// Paint the shape (change a bit the color).
			float h = std::fmod(hue + ggo::rand_float(-0.1f, 0.1f), 1.f); 
			if (h < 0) { h += 1; }
			
			float s = ggo::clamp(sat + ggo::rand_float(-0.1f, 0.1f), 0.f, 1.f);
			
			ggo::paint(buffer, get_render_width(), get_render_height(),
                 eclipse_render,
                 ggo::color::from_hsv(h, s, 1), 0.2f);
		}
	}
}
