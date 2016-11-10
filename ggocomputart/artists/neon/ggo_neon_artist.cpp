#include "ggo_neon_artist.h"
#include <ggo_color.h>
#include <ggo_buffer_paint.h>
#include <ggo_buffer_fill.h>
#include <ggo_brush.h>
#include <ggo_blender.h>

//////////////////////////////////////////////////////////////
ggo::neon_artist::neon_artist(int render_width, int render_height)
:
accumulation_animation_artist_abc(render_width, render_height)
{

}

//////////////////////////////////////////////////////////////
void ggo::neon_artist::init_sub()
{
	_angle 				    = 0;
	_radius_prv			  = ggo::rand<float>();
	_radius_cur			  = _radius_prv;
	_radius_attractor	= ggo::rand<float>();
	_angle				    = ggo::rand<float>(0, 2 * ggo::pi<float>());
	_hue_point			  = ggo::rand<float>();
}

//////////////////////////////////////////////////////////////
void ggo::neon_artist::init_output_buffer(void * buffer) const
{
  ggo::fill_solid<ggo::rgb_8u_yu>(buffer, get_render_width(), get_render_height(), 3 * get_render_width(), ggo::black<ggo::color_8u>());
}

//////////////////////////////////////////////////////////////
bool ggo::neon_artist::render_next_frame_sub(void * buffer, int frame_index)
{
  const int frames_count = 20000;

	if (frame_index >= frames_count)
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
		_radius_attractor	= ggo::rand<float>(0.2f, 1);
		_hue_point			  = ggo::rand<float>();
	}
	
	ggo::pos2f pos;
	
	pos.x() = _radius_cur * cos(_angle);
	pos.y() = _radius_cur * sin(_angle);
	paint_point(buffer, pos, ggo::white<ggo::color_8u>());

	pos.x() = _radius_attractor * std::cos(_angle);
	pos.y() = _radius_attractor * std::sin(_angle);
	paint_point(buffer, pos, ggo::from_hsv<ggo::color_8u>(_hue_point, 1, 1));
	
	return true;
}

//////////////////////////////////////////////////////////////
void ggo::neon_artist::paint_point(void * buffer, const ggo::pos2f & point_pos, const ggo::color_8u & color) const
{
	const float radius = 0.01f * get_render_min_size();
  const ggo::color_8u paint_color(
    uint8_t(ggo::fixed_point_div<8>(uint32_t(2 * color.r()))),
    uint8_t(ggo::fixed_point_div<8>(uint32_t(2 * color.g()))),
    uint8_t(ggo::fixed_point_div<8>(uint32_t(2 * color.b()))));

	ggo::pos2f pos = map_fit(0.8f * point_pos, -1, 1);

  ggo::disc_float disc1(pos, radius);
	ggo::paint_shape<ggo::rgb_8u_yu, ggo::sampling_4x4>(
    buffer, get_render_width(), get_render_height(), 3 * get_render_width(),
    disc1, ggo::make_solid_brush(paint_color), ggo::additive_blender<ggo::color_8u>());

	pos.x()	= get_render_width() - pos.x() - 1;

  ggo::disc_float disc2(pos, radius);
  ggo::paint_shape<ggo::rgb_8u_yu, ggo::sampling_4x4>(
    buffer, get_render_width(), get_render_height(), 3 * get_render_width(),
    disc2, ggo::make_solid_brush(paint_color), ggo::additive_blender<ggo::color_8u>());
}

