#include "ggo_vortex2_animation_artist.h"
#include <ggo_interpolation1d.h>

#define GGO_VORTEX2_VORTICES_COUNT 20

//////////////////////////////////////////////////////////////
ggo_vortex2_animation_artist::ggo_vortex2_animation_artist(int render_width, int render_height)
:
ggo_animation_artist_abc(render_width, render_height)
{
	
}

//////////////////////////////////////////////////////////////
void ggo_vortex2_animation_artist::init_sub()
{
	_vortices_paths.clear();

	_params._vortices.set_size(GGO_VORTEX2_VORTICES_COUNT);
	_params._color1 = ggo::color::from_hsv(ggo::rand_float(), ggo::rand_float(0.4f, 0.6f), 1);
	_params._color2 = ggo::color::from_hsv(ggo::rand_float(), ggo::rand_float(0.4f, 0.6f), 1);
	_params._color3 = ggo::color::from_hsv(ggo::rand_float(), ggo::rand_float(0.4f, 0.6f), 1);
	_params._color4 = ggo::color::from_hsv(ggo::rand_float(), ggo::rand_float(0.4f, 0.6f), 1);
	_params._split_horz = ggo::rand_float(0.4f * get_render_width(),  0.6f * get_render_width());
	_params._split_vert = ggo::rand_float(0.4f * get_render_height(), 0.6f * get_render_height());

	for (int i = 0; i < _params._vortices.get_size(); ++i)
	{
		_params._vortices[i]._angle = ggo::rand_float(ggo::PI<float>() / 2, ggo::PI<float>() / 2 + 0.5f);
		_params._vortices[i]._speed = ggo::rand_float(0.004f, 0.008f) * get_render_max_size();
		_params._vortices[i]._power = ggo::rand_float(6, 8);
		
		ggo_vortex_path vortex_path;
		vortex_path._start_pos.x() = ggo::rand_float() * get_render_width();
		vortex_path._start_pos.y() = ggo::rand_float() * get_render_height();
		vortex_path._end_pos.x() = ggo::rand_float() * get_render_width();
		vortex_path._end_pos.y() = ggo::rand_float() * get_render_height();
		
		_vortices_paths[&_params._vortices[i]] = vortex_path;
	}
}

//////////////////////////////////////////////////////////////
bool ggo_vortex2_animation_artist::render_next_frame_sub(uint8_t * buffer, int frame_index)
{
	const int FRAMES_COUNT = 250;
	
	if (frame_index >= FRAMES_COUNT)
	{
		return false;
	}
	
	// Interpolate vortices position.
	float interp = ggo::ease_inout(frame_index, FRAMES_COUNT);
	for (int i = 0; i < _params._vortices.get_size(); ++i)
	{
		const ggo::point2d_float & start_pos = _vortices_paths[&_params._vortices[i]]._start_pos;
		const ggo::point2d_float & end_pos = _vortices_paths[&_params._vortices[i]]._end_pos;
		
		_params._vortices[i]._pos = ggo::linear_interpolation(0.f, start_pos, 1.f, end_pos, interp);
	}
	
	ggo_vortex2_artist::render(buffer, get_render_width(), get_render_height(), _params);
	
	return true;
}