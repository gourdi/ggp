#include "ggo_duffing_bitmap_artist.h"
#include <ggo_gaussian_blur.h>
#include <ggo_gray_image_data.h>
#include <ggo_rgb_image_data.h>
#include <ggo_paint.h>
#include <ggo_fill.h>

const int GGO_ITERATIONS_COUNT = 5000000;

//////////////////////////////////////////////////////////////
ggo_duffing_bitmap_artist::ggo_duffing_bitmap_artist(int render_width, int render_height)
:
ggo_bitmap_artist_abc(render_width, render_height)
{
}

//////////////////////////////////////////////////////////////
void ggo_duffing_bitmap_artist::render_bitmap(uint8_t * buffer)
{
	// Compute the points.
	std::cout << "Computing points" << std::endl;
	
	std::vector<ggo::point2d_float>	points;

	const float	A = ggo::rand_float(0.245f, 0.255f);
	const float	B = ggo::rand_float(0.295f, 0.305f);
  const float dt = 0.002f;
  
	ggo::point2d_float	point(ggo::rand_float(-1, 1), ggo::rand_float(-1, 1));
	float angle_offset = ggo::rand_float(0, 2 * ggo::PI<float>());
  float t = 0;
	
	for (int i = 0; i < GGO_ITERATIONS_COUNT; ++i)
	{
		float	x	= point.x();
		float	y	= point.y();
		float	dx	= y;
		float	dy	= x - x * x * x - A * y + B * std::cos(t);

		point.x() += dx * dt;
		point.y() += dy * dt;

		float angle = atan2(point.y(), point.x()) + angle_offset;
		float dist = point.get_length();

		ggo::point2d_float render_pt(dist * std::cos(angle), dist * std::sin(angle));
		render_pt = map_fit(render_pt, -1.7f, 1.7f);
		
		points.push_back(render_pt);
		
		t += dt;
	}

	float radius = get_render_min_size() / 1500.f;
	ggo::rgb_image_data_float image_data_float(get_render_width(), get_render_height());

	// Render the background.
	std::cout << "Rendering background" << std::endl;
	
  ggo::color color1 = ggo::color::from_hsv(0, 0, ggo::rand_float(0.5, 1));
	ggo::color color2 = ggo::color::from_hsv(0, 0, ggo::rand_float(0.5, 1));
	ggo::color color3 = ggo::color::from_hsv(0, 0, ggo::rand_float(0.5, 1));
	ggo::color color4 = ggo::color::from_hsv(0, 0, ggo::rand_float(0.5, 1));
	
	ggo::fill_4_colors(image_data_float, color1, color2, color3, color4);

	// Render the shadow points.
	std::cout << "Rendering the shadow" << std::endl;
	
	ggo::gray_image_data_float shadow_image_data(get_render_width(), get_render_height());
	shadow_image_data.fill(1);
	for (int i = 0; i < points.size(); ++i)
	{
		// Offset the shadow.
		ggo::point2d_float render_pt = points[i];
		render_pt.x() += 0.05f * get_render_min_size();
		render_pt.y() += 0.05f * get_render_min_size();

    auto disc = std::make_shared<const ggo::disc_float>(render_pt, radius);
    
		ggo::paint(shadow_image_data, disc, 0, 0.2f);
	}

	// Blur and blend the shadow.
	std::cout << "Blurring the shadow" << std::endl;
	
	ggo::gaussian_blur_2d(shadow_image_data.get_buffer(),
                        shadow_image_data.get_buffer(),
                        get_render_width(),
                        get_render_height(),
                        0.4f * get_render_min_size());
	
	std::cout << "Blending the shadow" << std::endl;

	apply_shadow(image_data_float.get_buffer(), shadow_image_data.get_buffer());

	// Render the points.
	std::cout << "Rendering points" << std::endl;
	
	ggo::cubic_curve_float hue_curve;
	hue_curve.push_point(0, ggo::rand_float());
	hue_curve.push_point(1, ggo::rand_float());
	for (int i = 1; i <= 3; ++i)
	{
		hue_curve.push_point(i / 4.f, ggo::rand_float());
	}
	
	for (int i = 0; i < points.size(); ++i)
	{
		float t = float(i) / points.size();
		float hue = hue_curve.evaluate(t);
		ggo::color color = ggo::color::from_hsv(hue, 1, 1);

    auto disc = std::make_shared<const ggo::disc_float>(points[i], radius);
    
		ggo::paint(image_data_float, disc, color, 0.02f);
	}

	// From float to uint8_t.
  ggo::rgb_image_data_uint8 image_data(buffer, get_render_width(), get_render_height());
	image_data.copy(image_data_float);
}

//////////////////////////////////////////////////////////////
void ggo_duffing_bitmap_artist::apply_shadow(float * buffer, const float * shadow_buffer) const
{
	int count = get_render_width() * get_render_height();
	
	for (int i = 0; i < count; ++i)
	{
		buffer[0] = shadow_buffer[0] * buffer[0];
		buffer[1] = shadow_buffer[0] * buffer[1];
		buffer[2] = shadow_buffer[0] * buffer[2];
		
		buffer += 3;
		shadow_buffer += 1;
	}
}
