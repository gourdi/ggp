#include "ggo_duffing_bitmap_artist.h"
#include <ggo_gaussian_blur2d.h>
#include <ggo_buffer_fill.h>
#include <ggo_buffer_paint.h>
#include <ggo_brush.h>
#include <ggo_blender.h>
#include <ggo_blit.h>

//////////////////////////////////////////////////////////////
ggo::duffing_bitmap_artist::duffing_bitmap_artist(int render_width, int render_height)
:
bitmap_artist_abc(render_width, render_height)
{
}

//////////////////////////////////////////////////////////////
void ggo::duffing_bitmap_artist::render_bitmap(void * buffer) const
{
  const int iterations_count = 5000000;

	// Compute the points.
	std::cout << "Computing points" << std::endl;
	
	std::vector<ggo::pos2f>	points;

	const float	A = ggo::rand<float>(0.245f, 0.255f);
	const float	B = ggo::rand<float>(0.295f, 0.305f);
  const float dt = 0.002f;
  
	ggo::pos2f	point(ggo::rand<float>(-1, 1), ggo::rand<float>(-1, 1));
	float angle_offset = ggo::rand<float>(0, 2 * ggo::pi<float>());
  float t = 0;
	
	for (int i = 0; i < iterations_count; ++i)
	{
		float	x	= point.x();
		float	y	= point.y();
		float	dx	= y;
		float	dy	= x - x * x * x - A * y + B * std::cos(t);

		point.x() += dx * dt;
		point.y() += dy * dt;

		float angle = atan2(point.y(), point.x()) + angle_offset;
		float dist = point.get_length();

		ggo::pos2f render_pt(dist * std::cos(angle), dist * std::sin(angle));
		render_pt = map_fit(render_pt, -1.7f, 1.7f);
		
		points.push_back(render_pt);
		
		t += dt;
	}

	float radius = get_render_min_size() / 1500.f;
	std::vector<float> buffer_float(3 * get_render_width() * get_render_height());

	// Render the background.
	std::cout << "Rendering background" << std::endl;
	
  ggo::color_32f color1 = ggo::from_hsv<ggo::color_32f>(0, 0, ggo::rand<float>(0.5, 1));
	ggo::color_32f color2 = ggo::from_hsv<ggo::color_32f>(0, 0, ggo::rand<float>(0.5, 1));
	ggo::color_32f color3 = ggo::from_hsv<ggo::color_32f>(0, 0, ggo::rand<float>(0.5, 1));
	ggo::color_32f color4 = ggo::from_hsv<ggo::color_32f>(0, 0, ggo::rand<float>(0.5, 1));
	
	ggo::fill_4_colors<ggo::rgb_32f_yu>(buffer_float.data(), get_render_width(), get_render_height(), 3 * sizeof(float) * get_render_width(),
    color1, color2, color3, color4);

	// Render the shadow points.
	std::cout << "Rendering the shadow" << std::endl;
	
  std::vector<float> shadow_buffer(get_render_width() * get_render_height(), 1.f);
	for (const auto & point : points)
	{
		// Offset the shadow.
		ggo::pos2f render_pt = point;
		render_pt.x() += 0.05f * get_render_min_size();
		render_pt.y() += 0.05f * get_render_min_size();

		ggo::paint_shape<ggo::y_32f_yu, ggo::sampling_2x2>(
      shadow_buffer.data(), get_render_width(), get_render_height(), sizeof(float) * get_render_width(),
      ggo::disc_float(render_pt, radius), ggo::make_solid_brush<float>(0.f), ggo::alpha_blender<float>(0.2f));
	}

	// Blur and blend the shadow.
	std::cout << "Blurring the shadow" << std::endl;
	
  ggo::gaussian_blur2d<ggo::y_32f_yu>(shadow_buffer.data(), get_render_width(),
    get_render_height(), sizeof(float) * get_render_width(), 0.4f * get_render_min_size());

	std::cout << "Blending the shadow" << std::endl;

	apply_shadow(buffer_float.data(), shadow_buffer.data());

	// Render the points.
	std::cout << "Rendering points" << std::endl;
	
	ggo::cubic_curve_float hue_curve;
	hue_curve.push_point(0, ggo::rand<float>());
	hue_curve.push_point(1, ggo::rand<float>());
	for (int i = 1; i <= 3; ++i)
	{
		hue_curve.push_point(i / 4.f, ggo::rand<float>());
	}
	
	for (int i = 0; i < points.size(); ++i)
	{
		float t = float(i) / points.size();
		float hue = hue_curve.evaluate(t);
		ggo::color_32f color = ggo::from_hsv<ggo::color_32f>(hue, 1, 1);

    auto disc = std::make_shared<const ggo::disc_float>(points[i], radius);

    ggo::paint_shape<ggo::rgb_32f_yu, ggo::sampling_4x4>(
      buffer_float.data(), get_render_width(), get_render_height(), 3 * sizeof(float) * get_render_width(),
      ggo::disc_float(points[i], radius), ggo::make_solid_brush<color_32f>(color), ggo::alpha_blender<color_32f>(0.02f));
	}

	// From float to uint8_t.
  ggo::blit<ggo::rgb_32f_yu, ggo::rgb_8u_yu>(
    buffer_float.data(), get_render_width(), get_render_height(), 3 * sizeof(float) * get_render_width(),
    buffer, get_render_width(), get_render_height(), 3 * get_render_width(), 0, 0);
}

//////////////////////////////////////////////////////////////
void ggo::duffing_bitmap_artist::apply_shadow(float * buffer, const float * shadow_buffer) const
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
