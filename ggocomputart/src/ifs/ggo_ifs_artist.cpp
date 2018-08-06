#include "ggo_ifs_artist.h"
#include <2d/blur/ggo_gaussian_blur.h>
#include <2d/paint/ggo_blend.h>
#include <iostream>

//////////////////////////////////////////////////////////////
ggo::ifs_artist::ifs_artist(int width, int height, int line_step, ggo::image_format format)
:
artist(width, height, line_step, format)
{
}

//////////////////////////////////////////////////////////////
void ggo::ifs_artist::update(ggo::pos3f & point, const float transform[4]) const
{
	float x_fp	= point.x();
	float y_fp	= point.y();
	float z_fp	= point.z();
	
	point.x()	=      std::sin(transform[0]*y_fp) - z_fp*std::cos(transform[1]*x_fp);
	point.y()	= z_fp*std::sin(transform[2]*x_fp) -      std::cos(transform[3]*y_fp);
	point.z()	=      std::sin(x_fp);
}

//////////////////////////////////////////////////////////////
void ggo::ifs_artist::render(void * buffer, float transform[4], float hue, float angle1, float angle2) const
{
	std::cout << "Computing points" << std::endl;

	ggo::array_32f accumul_buffer(width() * height());
	accumul_buffer.fill(0);

	float cos1 = std::cos(angle1);
	float sin1 = std::sin(angle1);
	float cos2 = std::cos(angle2);
	float sin2 = std::sin(angle2);

	int counter_max = min_size() * min_size();
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

			int x_i = ggo::round_to<int>(render_point.x());
			int y_i = ggo::round_to<int>(render_point.y());
			int index = y_i * width() + x_i;

			if ((x_i >= 0) && (x_i < width()) &&
				(y_i >= 0) && (y_i < height()))
			{
				accumul_buffer(index) = std::min(1.f, accumul_buffer(index) + 0.00015f);
			}
		}
	}

	// Render shadow.
	std::cout << "Rendering shadow" << std::endl;
	
	ggo::array_32f shadow_buffer(accumul_buffer);
	ggo::gaussian_blur2d<ggo::y_32f_yu>(shadow_buffer.data(), sizeof(float) * width(), size(), 0.4f * min_size());
	paint_buffer(buffer, 255, shadow_buffer);

	// Render the shape.
	std::cout << "Rendering shape" << std::endl;
	
	paint_buffer(buffer, 255, accumul_buffer);
}

//////////////////////////////////////////////////////////////
void ggo::ifs_artist::paint_buffer(void * buffer, uint8_t color, const ggo::array_32f & accumul_buffer) const
{
  const float * ptr_src = accumul_buffer.data();

	for (int y = 0; y < height(); ++y)
	{
    void * ptr_dst = ggo::get_line_ptr<ggo::memory_lines_order::bottom_up>(buffer, y, height(), line_step());

    for (int x = 0; x < width(); ++x)
    {
      ggo::alpha_blender_rgb8u blender(*ptr_src);
      ggo::rgb_8u c_8u = blender(x, y, ggo::read_pixel<ggo::rgb_8u_yu>(ptr_dst), ggo::rgb_8u(color, color, color));
      ggo::write_pixel<ggo::rgb_8u_yu>(ptr_dst, c_8u);

      ptr_src++;
      ptr_dst = ggo::ptr_offset<ggo::image_format_traits<ggo::rgb_8u_yu>::pixel_byte_size>(ptr_dst);
    }
	}
}

