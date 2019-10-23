#include "ggo_ifs_artist.h"
#include <2d/fill/ggo_fill.h>
#include <2d/processing/ggo_gaussian_blur.h>
#include <2d/processing/ggo_blit.h>
#include <2d/paint/ggo_blend.h>
#include <iostream>

//////////////////////////////////////////////////////////////
ggo::ifs_artist::ifs_artist(int width, int height)
:
artist(width, height)
{
}

//////////////////////////////////////////////////////////////
void ggo::ifs_artist::update(ggo::pos3_f & point, const float transform[4]) const
{
	float x_fp	= point.x();
	float y_fp	= point.y();
	float z_fp	= point.z();
	
	point.x()	=      std::sin(transform[0]*y_fp) - z_fp*std::cos(transform[1]*x_fp);
	point.y()	= z_fp*std::sin(transform[2]*x_fp) -      std::cos(transform[3]*y_fp);
	point.z()	=      std::sin(x_fp);
}

//////////////////////////////////////////////////////////////
void ggo::ifs_artist::render(void * buffer, int line_byte_step, ggo::pixel_type pixel_type, ggo::lines_order memory_lines_order, float transform[4], float hue, float angle1, float angle2) const
{
	std::cout << "Computing points" << std::endl;

	ggo::image_t<ggo::pixel_type::y_32f, ggo::lines_order::up> img_accumul(size());
	fill_black(img_accumul);

	float cos1 = std::cos(angle1);
	float sin1 = std::sin(angle1);
	float cos2 = std::cos(angle2);
	float sin2 = std::sin(angle2);

	int counter_max = min_size() * min_size();
	for (int counter= 0; counter < counter_max; ++counter)
	{
		// Get a starting point
		ggo::pos3_f point;
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
			
			ggo::pos2_f render_point(cos2 * point.x() - sin2 * point.z(), sin2 * point.x() + cos2 * point.z());
			render_point = map_fit(render_point, -2, 2);

			int x_i = ggo::round_to<int>(render_point.x());
			int y_i = ggo::round_to<int>(render_point.y());

			if ((x_i >= 0) && (x_i < width()) &&
				  (y_i >= 0) && (y_i < height()))
			{
        float * ptr = static_cast<float *>(img_accumul.pixel_ptr(x_i, y_i));
        *ptr = std::min(1.f, *ptr + 0.00015f);
			}
		}
	}

	// Render shadow.
	std::cout << "Rendering shadow" << std::endl;
	
	ggo::image_t<ggo::pixel_type::y_32f, ggo::lines_order::up> img_shadow(size());
  ggo::blit(img_accumul, img_shadow);
	ggo::gaussian_blur(img_shadow, 0.4f * min_size());
	paint_buffer(buffer, line_byte_step, pixel_type, memory_lines_order, 255, img_shadow);

	// Render the shape.
	std::cout << "Rendering shape" << std::endl;
	
	paint_buffer(buffer, line_byte_step, pixel_type, memory_lines_order, 255, img_accumul);
}

//////////////////////////////////////////////////////////////
void ggo::ifs_artist::paint_buffer(void * buffer, int line_byte_step, ggo::pixel_type pixel_type, ggo::lines_order memory_lines_order,
  uint8_t color, const ggo::image_t<ggo::pixel_type::y_32f, ggo::lines_order::up> & accumul_buffer) const
{
  image_t<ggo::pixel_type::rgb_8u, ggo::lines_order::up> img(buffer, size(), line_byte_step);

  for_each_pixel(img, [&](int x, int y)
  {
    ggo::alpha_blender_rgb8u blender(accumul_buffer.read_pixel(x, y));
    ggo::rgb_8u c_8u = blender(x, y, img.read_pixel(x, y), { color, color, color });
    img.write_pixel(x, y, c_8u);
  });
}

