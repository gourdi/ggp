#include "ggo_ifs_artist.h"
#include <ggo_gaussian_blur2d.h>
#include <iostream>

//////////////////////////////////////////////////////////////
ggo::ifs_artist::ifs_artist(int render_width, int render_height)
:
artist(render_width, render_height)
{
}

//////////////////////////////////////////////////////////////
void ggo::ifs_artist::update(ggo::pos3f & point, const float transform[4])
{
	float x_fp	= point.x();
	float y_fp	= point.y();
	float z_fp	= point.z();
	
	point.x()	=      std::sin(transform[0]*y_fp) - z_fp*std::cos(transform[1]*x_fp);
	point.y()	= z_fp*std::sin(transform[2]*x_fp) -      std::cos(transform[3]*y_fp);
	point.z()	=      std::sin(x_fp);
}

//////////////////////////////////////////////////////////////
void ggo::ifs_artist::render(void * buffer, float transform[4], float hue, float angle1, float angle2)
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
		ggo::pos3f point;
		point.x() = ggo::rand<float>(-1, 1);
		point.y() = ggo::rand<float>(-1, 1);
		point.z() = ggo::rand<float>(-1, 1);

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
			
			ggo::pos2f render_point(cos2 * point.x() - sin2 * point.z(), sin2 * point.x() + cos2 * point.z());
			render_point = map_fit(render_point, -2, 2);

			int x_i = ggo::to<int>(render_point.x());
			int y_i = ggo::to<int>(render_point.y());
			int index = y_i * get_render_width() + x_i;

			if ((x_i >= 0) && (x_i < get_render_width()) &&
				(y_i >= 0) && (y_i < get_render_height()))
			{
				accumul_buffer(index) = std::min(1.f, accumul_buffer(index) + 0.00015f);
			}
		}
	}

	// Render shadow.
	std::cout << "Rendering shadow" << std::endl;
	
	ggo::array_float shadow_buffer(accumul_buffer);
	ggo::gaussian_blur2d<ggo::y_32f_yu>(
    shadow_buffer.data(), get_render_width(), get_render_height(), sizeof(float) * get_render_width(), 0.4f * get_render_min_size());
	paint_buffer(buffer, 255, shadow_buffer);

	// Render the shape.
	std::cout << "Rendering shape" << std::endl;
	
	paint_buffer(buffer, 255, accumul_buffer);
}

//////////////////////////////////////////////////////////////
void ggo::ifs_artist::paint_buffer(void * buffer, uint8_t color, const ggo::array_float & accumul_buffer) const
{
  uint8_t * ptr = static_cast<uint8_t *>(buffer);

	for (int i = 0; i < get_render_width() * get_render_height(); ++i)
	{
		float alpha = accumul_buffer(i);

		float r = (1 - alpha) * ptr[0] + alpha * color;
		float g = (1 - alpha) * ptr[1] + alpha * color;
		float b = (1 - alpha) * ptr[2] + alpha * color;

    ptr[0] = ggo::to<uint8_t>(r);
    ptr[1] = ggo::to<uint8_t>(g);
    ptr[2] = ggo::to<uint8_t>(b);
	
    ptr += 3;
	}
}

