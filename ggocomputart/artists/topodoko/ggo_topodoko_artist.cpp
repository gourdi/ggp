#include "ggo_topodoko_artist.h"
#include <ggo_gaussian_blur.h>
#include <ggo_array.h>
#include <ggo_paint.h>
#include <ggo_fill.h>

//////////////////////////////////////////////////////////////
ggo_topodoko_artist::ggo_topodoko_artist(int render_width, int render_height)
:
ggo_bitmap_artist_abc(render_width, render_height)
{
	
}

//////////////////////////////////////////////////////////////
void ggo_topodoko_artist::render_bitmap(uint8_t * buffer)
{
	float hue1 = ggo::rand_float();
	float hue2 = std::fmod(hue1 + ggo::rand_float(0.25f, 0.75f), 1.f);

	ggo::color bkgd_color1 = ggo::color::from_hsv(ggo::rand_bool() ? hue1 : hue2, 0.5f, 0.5f);
	ggo::color bkgd_color2 = ggo::color::from_hsv(ggo::rand_bool() ? hue1 : hue2, 0.5f, 0.5f);
	ggo::color bkgd_color3 = ggo::color::from_hsv(ggo::rand_bool() ? hue1 : hue2, 0.5f, 0.5f);
	ggo::color bkgd_color4 = ggo::color::from_hsv(ggo::rand_bool() ? hue1 : hue2, 0.5f, 0.5f);
	
  auto image_buffer = make_image_buffer(buffer);
  
	ggo::fill_4_colors(image_buffer, _bkgd_color1, _bkgd_color2, _bkgd_color3, _bkgd_color4);
	
	std::vector<ggo_color_square>	color_squares;

	const int BLOCKS = ggo::rand_int(4, 6);
	const int SUB_BLOCKS = ggo::rand_int(4, 6);
	const float SQUARE_DIST = ggo::rand_float(0.8f, 0.9f) / (BLOCKS * SUB_BLOCKS);
	const float SQUARE_SIZE = ggo::rand_float(0.8f, 0.9f) * SQUARE_DIST;

	for (int block_y = 0; block_y < BLOCKS; ++block_y)
	{
		for (int block_x = 0; block_x < BLOCKS; ++block_x)
		{
			ggo::point2d_float block_center((1 + 2 * block_x) / (2.f * BLOCKS), (1 + 2 * block_y) / (2.f * BLOCKS));
			
			ggo::color color = ggo::color::from_hsv(ggo::rand_bool() ? hue1 : hue2, 0.75, 1);
			
			for (int y = 0; y < SUB_BLOCKS; ++y)
			{
				for (int x = 0; x < SUB_BLOCKS; ++x)
				{
					ggo_color_square color_square;
					
					ggo::point2d_float square_center(block_center);
					square_center.x() += (x - 0.5f * SUB_BLOCKS + 0.5f) * SQUARE_DIST + (SQUARE_DIST - SQUARE_SIZE) * ggo::rand_float(-1, 1);
					square_center.y() += (y - 0.5f * SUB_BLOCKS + 0.5f) * SQUARE_DIST + (SQUARE_DIST - SQUARE_SIZE) * ggo::rand_float(-1, 1);
					
					float dangle = ggo::rand_float(-0.1f, 0.1f);
					color_square._square.add_point(square_center + ggo::point2d_float::from_polar(    ggo::PI<float>() / 4 + dangle, 0.5f * SQUARE_SIZE * ggo::SQRT2<float>()));
					color_square._square.add_point(square_center + ggo::point2d_float::from_polar(3 * ggo::PI<float>() / 4 + dangle, 0.5f * SQUARE_SIZE * ggo::SQRT2<float>()));
					color_square._square.add_point(square_center + ggo::point2d_float::from_polar(5 * ggo::PI<float>() / 4 + dangle, 0.5f * SQUARE_SIZE * ggo::SQRT2<float>()));
					color_square._square.add_point(square_center + ggo::point2d_float::from_polar(7 * ggo::PI<float>() / 4 + dangle, 0.5f * SQUARE_SIZE * ggo::SQRT2<float>()));

					if (ggo::rand_int(0, 8) == 0)
					{
						color_square._color = ggo::color::from_hsv(ggo::rand_bool() ? hue1 : hue2, 0.75, 1);
					}
					else
					{
						color_square._color = color;
					}
					
					color_square._color.r() = ggo::clamp(color_square._color.r() + ggo::rand_float(-0.15f, 0.15f), 0.f, 1.f);
					color_square._color.g() = ggo::clamp(color_square._color.g() + ggo::rand_float(-0.15f, 0.15f), 0.f, 1.f);
					color_square._color.b() = ggo::clamp(color_square._color.b() + ggo::rand_float(-0.15f, 0.15f), 0.f, 1.f);
					
					if (ggo::rand_int(0, 20) != 0)
					{
						color_squares.push_back(color_square);
					}
				}
			}
		}
	}

	std::cout << "Rendering shadow" << std::endl;
	
	// Render the shadow.
	ggo::gray_image_buffer_uint8 shadow_image(get_render_width(), get_render_height());
  shadow_image.fill(1.f);
	
	float shadow_offset_scalar = 0.25f * SQUARE_SIZE;
	ggo::point2d_float shadow_offset(shadow_offset_scalar, shadow_offset_scalar);
	
	for (const auto & color_square : _color_squares)
	{
		auto square = std::make_shared<ggo::polygon2d_float>(4);
		for (int i = 0; i < 4; ++i)
		{
			ggo::point2d_float point = map_fit(color_square._square.get_point(i) + shadow_offset, 0, 1);
      
			square->add_point(point);
		}

		ggo::paint(shadow_image, square, 0x40 / 255.f);
	}

	ggo::gaussian_blur_2d_mirror(shadow_image.data(),
                               shadow_image.data(),
                               get_render_width(),
                               get_render_height(),
                               0.05f * get_render_min_size(), 0.001f);

  image_buffer.from(shadow_image);
	
	std::cout << "Rendering squares" << std::endl;
	
	// Render the coloured squares.
	for (const auto & color_square : _color_squares)
	{
		auto square = std::make_shared<ggo::polygon2d_float>(4);
		for (int i = 0; i < 4; ++i)
		{
			ggo::point2d_float point = color_square._square.get_point(i);
			map_fit(point, 0, 1);
			square->add_point(point);
		}
		
		ggo::paint(image_buffer, square, color_square._color, 1, std::make_shared<ggo::rgb_alpha_blender>(), ggo::pixel_sampler_16X16());
	}
}

