#include "ggo_ifs_artist.h"
#include <ggo_color_conv.h>
#include <ggo_gaussian_blur.h>
#include <iostream>

//////////////////////////////////////////////////////////////
ggo_ifs_artist::ggo_ifs_artist(int render_width, int render_height)
:
ggo_artist_abc(render_width, render_height)
{
}

//////////////////////////////////////////////////////////////
void ggo_ifs_artist::update(ggo::point3d_float & point, const float transform[4])
{
	float x_fp	= point.x();
	float y_fp	= point.y();
	float z_fp	= point.z();
	
	point.x()	=      std::sin(transform[0]*y_fp) - z_fp*std::cos(transform[1]*x_fp);
	point.y()	= z_fp*std::sin(transform[2]*x_fp) -      std::cos(transform[3]*y_fp);
	point.z()	=      std::sin(x_fp);
}

//////////////////////////////////////////////////////////////
void ggo_ifs_artist::render(uint8_t * buffer, float transform[4], float hue, float angle1, float angle2)
{
	std::cout << "Computing points" << std::endl;

	ggo::array_float accumul_buffer(get_render_width() * get_render_height());
	accumul_buffer.fill(0);

	float cos1 = std::cos(angle1);
	float sin1 = std::sin(angle1);
	float cos2 = std::cos(angle2);
	float sin2 = std::sin(angle2);

	int counter_max = get_render_min_size() * get_render_min_size();
	for (int counter= 0; counter < counter_max; ++counter)
	{
		// Get a starting point
		ggo::point3d_float point;
		point.x() = ggo::rand_float(-1, 1);
		point.y() = ggo::rand_float(-1, 1);
		point.z() = ggo::rand_float(-1, 1);

		// Don't render the first points
		for (int i = 0; i < 5; ++i)
		{
			update(point, transform);
		}

		for (int i = 0; i < 512; ++i)
		{
			update(point, transform);

			float x = cos1 * point.x() - sin1 * point.y();
			float y = sin1 * point.x() + cos1 * point.y();
			point.x() = x;
			point.y() = y;
			
			ggo::point2d_float render_point(cos2 * point.x() - sin2 * point.z(), sin2 * point.x() + cos2 * point.z());
			render_point = map_fit(render_point, -2, 2);

			int x_i = ggo::to<int>(render_point.x());
			int y_i = ggo::to<int>(render_point.y());
			int index = y_i * get_render_width() + x_i;

			if ((x_i >= 0) && (x_i < get_render_width()) &&
				(y_i >= 0) && (y_i < get_render_height()))
			{
				accumul_buffer[index] = std::min(1.f, accumul_buffer[index] + 0.00015f);
			}
		}
	}

	// Render shadow.
	std::cout << "Rendering shadow" << std::endl;
	
	ggo::array_float shadow_buffer(accumul_buffer);
	ggo::gaussian_blur_2d_float(shadow_buffer, shadow_buffer, get_render_width(), get_render_height(), 0.4f * get_render_min_size());
	paint_buffer(buffer, 255, shadow_buffer);

	// Render the shape.
	std::cout << "Rendering shape" << std::endl;
	
	paint_buffer(buffer, 255, accumul_buffer);
}

//////////////////////////////////////////////////////////////
void ggo_ifs_artist::paint_buffer(uint8_t * buffer, uint8_t color, const ggo::array_float & accumul_buffer) const
{
	for (int i = 0; i < get_render_width() * get_render_height(); ++i)
	{
		float alpha = accumul_buffer[i];

		float r = (1 - alpha) * buffer[0] + alpha * color;
		float g = (1 - alpha) * buffer[1] + alpha * color;
		float b = (1 - alpha) * buffer[2] + alpha * color;

		buffer[0] = ggo::to<uint8_t>(r);
		buffer[1] = ggo::to<uint8_t>(g);
		buffer[2] = ggo::to<uint8_t>(b);
	
		buffer += 3;
	}
}

