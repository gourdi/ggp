#include "ggo_neon_artist.h"
#include <ggo_color.h>
#include <ggo_paint.h>
#include <ggo_fill.h>

const int GGO_COUNTER_MAX = 20000;

//////////////////////////////////////////////////////////////
ggo_neon_artist::ggo_neon_artist(int render_width, int render_height)
:
ggo_accumulation_animation_artist_abc(render_width, render_height)
{

}

//////////////////////////////////////////////////////////////
void ggo_neon_artist::init_sub()
{
	_angle 				    = 0;
	_radius_prv			  = ggo::rand_float();
	_radius_cur			  = _radius_prv;
	_radius_attractor	= ggo::rand_float();
	_angle				    = ggo::rand_float(0, 2 * ggo::PI<float>());
	_hue_point			  = ggo::rand_float();
}

//////////////////////////////////////////////////////////////
void ggo_neon_artist::init_output_buffer(uint8_t * buffer)
{
  ggo::fill_solid_rgb(buffer, get_render_width()*get_render_height(), ggo::color::BLACK);
}

//////////////////////////////////////////////////////////////
bool ggo_neon_artist::render_next_frame_sub(uint8_t * buffer, int frame_index)
{
	if (frame_index >= GGO_COUNTER_MAX)
	{
		return false;
	}
	
	float force = -0.000001f;
	if ( _radius_attractor > _radius_cur ) { force *= -1; }
	float move = ggo::clamp(force + 0.9999f*(_radius_cur-_radius_prv), -0.005f, 0.005f);
	float tmp = _radius_cur;
	_radius_cur	+= move;
	_radius_prv = tmp;

	_angle += 0.0005f;
	if ((frame_index % 2000) == 0)
	{
		_radius_attractor	= ggo::rand_float(0.2f, 1);
		_hue_point			  = ggo::rand_float();
	}
	
	ggo::point2d_float pos;
	
	pos.x() = _radius_cur * cos(_angle);
	pos.y() = _radius_cur * sin(_angle);
	paint_point(buffer, pos, ggo::color::WHITE);

	pos.x() = _radius_attractor * std::cos(_angle);
	pos.y() = _radius_attractor * std::sin(_angle);
	paint_point(buffer, pos, ggo::color::from_hsv(_hue_point, 1, 1));
	
	return true;
}

//////////////////////////////////////////////////////////////
void ggo_neon_artist::paint_point(uint8_t * buffer, const ggo::point2d_float & point_pos, const ggo::color & color) const
{
  float opacity = 2.0f / 255;
	float radius = 0.01f * get_render_min_size();

	ggo::point2d_float pos = map_fit(0.8f * point_pos, -1, 1);

	auto disc1 = std::make_shared<ggo::disc_float>(pos, radius);
	ggo::paint(buffer, get_render_width(), get_render_height(), disc1, color, opacity, std::make_shared<ggo::rgb_additive_blender>());

	pos.x()	= get_render_width() - pos.x() - 1;

	auto disc2 = std::make_shared<ggo::disc_float>(pos, radius);
	ggo::paint(buffer, get_render_width(), get_render_height(), disc2, color, opacity, std::make_shared<ggo::rgb_additive_blender>());
}

