#include "ggo_duffing_animation_artist.h"
#include <ggo_gaussian_blur.h>
#include <ggo_paint.h>
#include <ggo_color_conv.h>
#include <ggo_fill.h>

const int GGO_ITERATIONS_COUNT = 200000;
const int GGO_POINTS_PER_FRAME = 500;
const int GGO_VISIBLE_POINTS_COUNT_POINTS = 12000;

//////////////////////////////////////////////////////////////
ggo_duffing_animation_artist::ggo_duffing_animation_artist(int render_width, int render_height)
:
ggo_animation_artist_abc(render_width, render_height)
{
}

//////////////////////////////////////////////////////////////
ggo::point2d_float ggo_duffing_animation_artist::apply_duffing(float t, float dt, float angle_offset, ggo::point2d_float & point) const
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
	
	ggo::point2d_float render_pt(dist * std::cos(angle), dist * std::sin(angle));
	render_pt = map_fit(render_pt, -1.7f, 1.7f);
	
	return render_pt;
}

//////////////////////////////////////////////////////////////
void ggo_duffing_animation_artist::init_sub()
{
	// Compute points.
	float 				      t = 0;
	float 				      dt = 0.002f;
	ggo::point2d_float	point(ggo::rand_float(-1, 1), ggo::rand_float(-1, 1));
	float				        angle_offset = ggo::rand_float(0, 2 * ggo::PI<float>());

	_points.clear();
	_points.reserve(GGO_ITERATIONS_COUNT);

	for (int i = 0; i < GGO_ITERATIONS_COUNT; ++i)
	{
		ggo::point2d_float render_pt = apply_duffing(t, dt, angle_offset, point);
		
		_points.push_back(render_pt);
		
		angle_offset = std::fmod(angle_offset + 0.00005f, 2 * ggo::PI<float>());
		t += dt;
	}
	
	// Init color mappings.
	_hue_curve.reset();
	_hue_curve.push_point(0, ggo::rand_float());
	_hue_curve.push_point(1, ggo::rand_float());
	_hue_curve.push_point(0.5, ggo::rand_float());
	
	_sat_curve.reset();
	_sat_curve.push_point(0, ggo::rand_float(0.5, 1));
	_sat_curve.push_point(1, ggo::rand_float(0.5, 1));
	_sat_curve.push_point(0.5, ggo::rand_float(0.5, 1));

	_val_curve1.reset();
	_val_curve1.push_point(0, ggo::rand_float());
	_val_curve1.push_point(1, ggo::rand_float());
	_val_curve1.push_point(0.25, ggo::rand_float());
	_val_curve1.push_point(0.5, ggo::rand_float());
	_val_curve1.push_point(0.75, ggo::rand_float());
	
	_val_curve2.reset();
	_val_curve2.push_point(0, ggo::rand_float());
	_val_curve2.push_point(1, ggo::rand_float());
	_val_curve2.push_point(0.25, ggo::rand_float());
	_val_curve2.push_point(0.5, ggo::rand_float());
	_val_curve2.push_point(0.75, ggo::rand_float());
	
	_val_curve3.reset();
	_val_curve3.push_point(0, ggo::rand_float());
	_val_curve3.push_point(1, ggo::rand_float());
	_val_curve3.push_point(0.25, ggo::rand_float());
	_val_curve3.push_point(0.5, ggo::rand_float());
	_val_curve3.push_point(0.75, ggo::rand_float());
	
	_val_curve4.reset();
	_val_curve4.push_point(0, ggo::rand_float());
	_val_curve4.push_point(1, ggo::rand_float());
	_val_curve4.push_point(0.25, ggo::rand_float());
	_val_curve4.push_point(0.5, ggo::rand_float());
	_val_curve4.push_point(0.75, ggo::rand_float());
}

//////////////////////////////////////////////////////////////				
bool ggo_duffing_animation_artist::render_next_frame_sub(uint8_t * buffer, int frame_index)
{
	int last_point = std::max(1, frame_index * GGO_POINTS_PER_FRAME);
	int first_point = last_point - GGO_VISIBLE_POINTS_COUNT_POINTS;
	
	if (first_point > GGO_ITERATIONS_COUNT)
	{
		return false;
	}
	
	float radius = get_render_min_size() / 100.f;
	ggo::rgb_image_data_float image_data_float(get_render_width(), get_render_height());

	// Render the background.
	float t = float(GGO_POINTS_PER_FRAME) * frame_index / _points.size();
	
	float hue = _hue_curve.evaluate(t);
	ggo::color color1 = ggo::color::from_hsv(hue, 0.2f, _val_curve1.evaluate(t));
	ggo::color color2 = ggo::color::from_hsv(hue, 0.2f, _val_curve2.evaluate(t));
	ggo::color color3 = ggo::color::from_hsv(hue, 0.2f, _val_curve3.evaluate(t));
	ggo::color color4 = ggo::color::from_hsv(hue, 0.2f, _val_curve4.evaluate(t));
	
	ggo::fill_4_colors(image_data_float, color1, color2, color3, color4);

	// Render the shadow points.
	ggo::gray_image_data_float shadow_image_data(get_render_width(), get_render_height());
	shadow_image_data.fill(1);
	for (int i = first_point; i <= last_point; ++i)
	{
		if (i >= 0 && i < _points.size())
		{
			float opacity = 0.02f * (i - first_point) / GGO_VISIBLE_POINTS_COUNT_POINTS;
			
			// Offset the shadow.
			ggo::point2d_float render_pt = _points[i];
			render_pt.x() += 0.05f * get_render_min_size();
			render_pt.y() += 0.05f * get_render_min_size();

      auto disc = std::make_shared<const ggo::disc_float>(render_pt, radius);
      
      ggo::paint(shadow_image_data, disc, 0, opacity);
		}
	}

	// Blur and blend the shadow.
	ggo::gaussian_blur_2d_mirror(shadow_image_data.get_buffer(),
                               shadow_image_data.get_buffer(),
                               get_render_width(),
                               get_render_height(),
                               0.4f * get_render_min_size(), 1, 1, 0.001f);

	apply_shadow(image_data_float.get_buffer(), shadow_image_data.get_buffer());

	// Render the points.
	float point_opacity = 0.02f;
	for (int i = first_point; i <= last_point; ++i)
	{
		if (i >= 0 && i < _points.size())
		{
			float opacity = 0.02f * (i - first_point) / GGO_VISIBLE_POINTS_COUNT_POINTS;
			
			float t = float(i) / _points.size();
			float hue = _hue_curve.evaluate(t);
			float sat = _sat_curve.evaluate(t);
			ggo::color color = ggo::color::from_hsv(hue, sat, 1);

      auto disc = std::make_shared<const ggo::disc_float>(_points[i], radius);
      
      ggo::paint(image_data_float, disc, color, opacity);
		}
	}

	// From float to uint8_t.
  ggo::rgb_image_data_uint8 image_data(buffer, get_render_width(), get_render_height());
	image_data.copy(image_data_float);

	return true;
}

//////////////////////////////////////////////////////////////
void ggo_duffing_animation_artist::apply_shadow(float * buffer, const float * shadow_buffer) const
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
