#include "ggo_duffing_bitmap_artist.h"
#include "ggo_duffing.h"
#include <2d/ggo_blit.h>
#include <2d/blur/ggo_gaussian_blur.h>
#include <2d/fill/ggo_fill.h>
#include <2d/paint/ggo_paint.h>
#include <2d/paint/ggo_brush.h>
#include <2d/paint/ggo_blend.h>

//////////////////////////////////////////////////////////////
ggo::duffing_bitmap_artist::duffing_bitmap_artist(int width, int height, int line_step, ggo::image_format format)
:
bitmap_artist_abc(width, height, line_step, format)
{
}

//////////////////////////////////////////////////////////////
void ggo::duffing_bitmap_artist::render_bitmap(void * buffer) const
{
  const int iterations_count = 5000000;

	// Compute the points.
	std::cout << "Computing points" << std::endl;
	
	std::vector<ggo::pos2_f>	points;

  ggo::duffing duffing;

	const float angle_offset = ggo::rand<float>(0, 2 * ggo::pi<float>());
  
	for (int i = 0; i < iterations_count; ++i)
	{
    ggo::pos2_f point = duffing.update(0.002f);

		float angle = atan2(point.y(), point.x()) + angle_offset;
		float dist = ggo::length(point);

    point = dist * ggo::vec2_f::from_angle(angle);
    point = map_fit(point, -1.7f, 1.7f);
		
		points.push_back(point);
	}

	float radius = min_size() / 1500.f;
	std::vector<float> buffer_float(3 * width() * height());

	// Render the background.
	std::cout << "Rendering background" << std::endl;
	
  const ggo::rgb_32f color1 = ggo::from_hsv<ggo::rgb_32f>(0, 0, ggo::rand<float>(0.5, 1));
	const ggo::rgb_32f color2 = ggo::from_hsv<ggo::rgb_32f>(0, 0, ggo::rand<float>(0.5, 1));
	const ggo::rgb_32f color3 = ggo::from_hsv<ggo::rgb_32f>(0, 0, ggo::rand<float>(0.5, 1));
	const ggo::rgb_32f color4 = ggo::from_hsv<ggo::rgb_32f>(0, 0, ggo::rand<float>(0.5, 1));
	
	ggo::fill_4_colors<ggo::rgb_32f_yu>(buffer_float.data(), width(), height(), 3 * sizeof(float) * width(),
    color1, color2, color3, color4, ggo::rect_int::from_width_height(width(), height()));

	// Render the shadow points.
	std::cout << "Rendering the shadow" << std::endl;
	
  std::vector<float> shadow_buffer(width() * height(), 1.f);
	for (const auto & point : points)
	{
		// Offset the shadow.
		ggo::pos2_f render_pt = point;
		render_pt.x() += 0.05f * min_size();
		render_pt.y() += 0.05f * min_size();

		ggo::paint<ggo::y_32f_yu, ggo::sampling_2x2>(
      shadow_buffer.data(), width(), height(), sizeof(float) * width(),
      ggo::disc_f(render_pt, radius), ggo::make_solid_brush(0.f), ggo::alpha_blender_y32f(0.2f));
	}

	// Blur and blend the shadow.
	std::cout << "Blurring the shadow" << std::endl;
	
  ggo::gaussian_blur2d<ggo::y_32f_yu>(shadow_buffer.data(), sizeof(float) * width(), size(), 0.4f * min_size());

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
		ggo::rgb_32f color = ggo::from_hsv<ggo::rgb_32f>(hue, 1, 1);

    ggo::paint<ggo::rgb_32f_yu, ggo::sampling_4x4>(
      buffer_float.data(), width(), height(), 3 * sizeof(float) * width(),
      ggo::disc_f(points[i], radius), ggo::make_solid_brush<rgb_32f>(color), ggo::alpha_blender_rgb32f(0.02f));
	}

	// From float to uint8_t.
  ggo::blit<ggo::rgb_32f_yu, ggo::rgb_8u_yu>(
    buffer_float.data(), width(), height(), 3 * sizeof(float) * width(),
    buffer, width(), height(), line_step(), 0, 0);
}

//////////////////////////////////////////////////////////////
void ggo::duffing_bitmap_artist::apply_shadow(float * buffer, const float * shadow_buffer) const
{
	int count = width() * height();
	
	for (int i = 0; i < count; ++i)
	{
		buffer[0] = shadow_buffer[0] * buffer[0];
		buffer[1] = shadow_buffer[0] * buffer[1];
		buffer[2] = shadow_buffer[0] * buffer[2];
		
		buffer += 3;
		shadow_buffer += 1;
	}
}
