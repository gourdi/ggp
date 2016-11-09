#include "ggo_duffing_animation_artist.h"
#include <ggo_gaussian_blur2d.h>
#include <ggo_buffer_paint.h>
#include <ggo_buffer_fill.h>
#include <ggo_brush.h>
#include <ggo_blender.h>

namespace
{
  const int iterations_count = 200000;
}

//////////////////////////////////////////////////////////////
ggo::duffing_animation_artist::duffing_animation_artist(int render_width, int render_height)
:
animation_artist_abc(render_width, render_height)
{
}

//////////////////////////////////////////////////////////////
ggo::pos2f ggo::duffing_animation_artist::apply_duffing(float t, float dt, float angle_offset, ggo::pos2f & point) const
{
	const float A = 0.25f;
	const float B = 0.3f;
	float	x		    = point.x();
	float	y		    = point.y();
	float	dx		  = y;
	float	dy		  = x - x * x * x - A * y + B * std::cos(t);

	point.x() = x + dx * dt;
	point.y() = y + dy * dt;
	
	float angle = atan2(point.y(), point.x()) + angle_offset;
	float dist = point.get_length();
	
	ggo::pos2f render_pt(dist * std::cos(angle), dist * std::sin(angle));
	render_pt = map_fit(render_pt, -1.7f, 1.7f);
	
	return render_pt;
}

//////////////////////////////////////////////////////////////
void ggo::duffing_animation_artist::init_sub()
{
	// Compute points.
	float 			t = 0;
	float 			dt = 0.002f;
	ggo::pos2f	point(ggo::rand<float>(-1, 1), ggo::rand<float>(-1, 1));
	float				angle_offset = ggo::rand<float>(0, 2 * ggo::pi<float>());

	_points.clear();
	_points.reserve(iterations_count);

	for (int i = 0; i < iterations_count; ++i)
	{
		ggo::pos2f render_pt = apply_duffing(t, dt, angle_offset, point);
		
		_points.push_back(render_pt);
		
		angle_offset = std::fmod(angle_offset + 0.00005f, 2 * ggo::pi<float>());
		t += dt;
	}
	
	// Init color mappings.
	_hue_curve.reset();
	_hue_curve.push_point(0, ggo::rand<float>());
	_hue_curve.push_point(1, ggo::rand<float>());
	_hue_curve.push_point(0.5, ggo::rand<float>());
	
	_sat_curve.reset();
	_sat_curve.push_point(0, ggo::rand<float>(0.5, 1));
	_sat_curve.push_point(1, ggo::rand<float>(0.5, 1));
	_sat_curve.push_point(0.5, ggo::rand<float>(0.5, 1));

	_val_curve1.reset();
	_val_curve1.push_point(0, ggo::rand<float>());
	_val_curve1.push_point(1, ggo::rand<float>());
	_val_curve1.push_point(0.25, ggo::rand<float>());
	_val_curve1.push_point(0.5, ggo::rand<float>());
	_val_curve1.push_point(0.75, ggo::rand<float>());
	
	_val_curve2.reset();
	_val_curve2.push_point(0, ggo::rand<float>());
	_val_curve2.push_point(1, ggo::rand<float>());
	_val_curve2.push_point(0.25, ggo::rand<float>());
	_val_curve2.push_point(0.5, ggo::rand<float>());
	_val_curve2.push_point(0.75, ggo::rand<float>());
	
	_val_curve3.reset();
	_val_curve3.push_point(0, ggo::rand<float>());
	_val_curve3.push_point(1, ggo::rand<float>());
	_val_curve3.push_point(0.25, ggo::rand<float>());
	_val_curve3.push_point(0.5, ggo::rand<float>());
	_val_curve3.push_point(0.75, ggo::rand<float>());
	
	_val_curve4.reset();
	_val_curve4.push_point(0, ggo::rand<float>());
	_val_curve4.push_point(1, ggo::rand<float>());
	_val_curve4.push_point(0.25, ggo::rand<float>());
	_val_curve4.push_point(0.5, ggo::rand<float>());
	_val_curve4.push_point(0.75, ggo::rand<float>());
}

//////////////////////////////////////////////////////////////				
bool ggo::duffing_animation_artist::render_next_frame_sub(void * buffer, int frame_index)
{
  const int points_per_frame = 500;
  const int visible_points_count = 12000;

	int last_point = std::max(1, frame_index * points_per_frame);
	int first_point = last_point - visible_points_count;
	
	if (first_point > iterations_count)
	{
		return false;
	}
	
	float radius = get_render_min_size() / 100.f;
  std::vector<float> buffer_float(3 * get_render_width() * get_render_height());

	// Render the background.
	float t = float(points_per_frame) * frame_index / _points.size();
	
	float hue = _hue_curve.evaluate(t);
  ggo::color_32f color1 = ggo::from_hsv<ggo::color_32f>(hue, 0.2f, _val_curve1.evaluate(t));
  ggo::color_32f color2 = ggo::from_hsv<ggo::color_32f>(hue, 0.2f, _val_curve2.evaluate(t));
  ggo::color_32f color3 = ggo::from_hsv<ggo::color_32f>(hue, 0.2f, _val_curve3.evaluate(t));
  ggo::color_32f color4 = ggo::from_hsv<ggo::color_32f>(hue, 0.2f, _val_curve4.evaluate(t));
	
  ggo::fill_4_colors<ggo::rgb_32f_yu>(buffer_float.data(), get_render_width(), get_render_height(), 3 * sizeof(float) * get_render_width(),
    color1, color2, color3, color4);

	// Render the shadow points.
  std::vector<float> shadow_buffer(get_render_width() * get_render_height(), 1.f);
  for (int i = first_point; i <= last_point; ++i)
	{
		if (i >= 0 && i < _points.size())
		{
			float opacity = 0.02f * (i - first_point) / visible_points_count;
			
			// Offset the shadow.
			ggo::pos2f render_pt = _points[i];
			render_pt.x() += 0.05f * get_render_min_size();
			render_pt.y() += 0.05f * get_render_min_size();

      ggo::paint_shape<ggo::y_32f_yu, ggo::sampling_2x2>(
        shadow_buffer.data(), get_render_width(), get_render_height(), sizeof(float) * get_render_width(),
        ggo::disc_float(render_pt, radius), ggo::make_solid_brush(0.f), ggo::alpha_blender_y32f(opacity));
		}
	}

	// Blur and blend the shadow.
  ggo::gaussian_blur2d<ggo::y_32f_yu>(shadow_buffer.data(), get_render_width(),
    get_render_height(), sizeof(float) * get_render_width(), 0.4f * get_render_min_size());

	apply_shadow(buffer_float.data(), shadow_buffer.data());

	// Render the points.
	float point_opacity = 0.02f;
	for (int i = first_point; i <= last_point; ++i)
	{
		if (i >= 0 && i < _points.size())
		{
			float opacity = 0.02f * (i - first_point) / visible_points_count;
			
			float t = float(i) / _points.size();
			float hue = _hue_curve.evaluate(t);
			float sat = _sat_curve.evaluate(t);
			ggo::color_32f color = ggo::from_hsv<ggo::color_32f>(hue, sat, 1);

      ggo::paint_shape<ggo::rgb_32f_yu, ggo::sampling_4x4>(
        buffer_float.data(), get_render_width(), get_render_height(), 3 * sizeof(float) * get_render_width(),
        ggo::disc_float(_points[i], radius), ggo::make_solid_brush(color), ggo::alpha_blender_rgb32f(opacity));
		}
	}

	// From float to uint8_t.
  for (int y = 0; y < get_render_height(); ++y)
  {
    for (int x = 0; x < get_render_width(); ++x)
    {
      auto c_32f = ggo::read_pixel<ggo::rgb_32f_yu>(buffer_float.data(), x, y, get_render_height(), 3 * sizeof(float) * get_render_height());
      auto c_8u = ggo::convert_color_to<ggo::color_8u>(c_32f);
      ggo::write_pixel<ggo::rgb_8u_yu>(buffer, x, y, get_render_height(), 3 * get_render_height(), c_8u);
    }
  }

	return true;
}

//////////////////////////////////////////////////////////////
void ggo::duffing_animation_artist::apply_shadow(float * buffer, const float * shadow_buffer) const
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
