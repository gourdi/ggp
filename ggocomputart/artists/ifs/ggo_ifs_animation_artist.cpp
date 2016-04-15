#include "ggo_ifs_animation_artist.h"
#include <ggo_fill.h>

namespace
{
  const int FRAMES_COUNT = 300;
}

//////////////////////////////////////////////////////////////
ggo_ifs_animation_artist::ggo_ifs_animation_artist(int render_width, int render_height)
:
ggo_animation_artist_abc(render_width, render_height),
_artist(render_width, render_height)
{
	
}

//////////////////////////////////////////////////////////////
void ggo_ifs_animation_artist::init_sub()
{
	_transform_start[0] = ggo::rand_float(1, 2);
	_transform_start[1] = ggo::rand_float(1, 2);
	_transform_start[2] = ggo::rand_float(1, 2);
	_transform_start[3] = ggo::rand_float(1, 2);
	
	_transform_end[0] = _transform_start[0] + ggo::rand_float(-0.3f, 0.3f);
	_transform_end[1] = _transform_start[1] + ggo::rand_float(-0.3f, 0.3f);
	_transform_end[2] = _transform_start[2] + ggo::rand_float(-0.3f, 0.3f);
	_transform_end[3] = _transform_start[3] + ggo::rand_float(-0.3f, 0.3f);
	
	_angle1_start = ggo::rand_float(0, 2 * ggo::PI<float>());
	_angle1_end = ggo::rand_float(0, 2 * ggo::PI<float>());
	_angle2_start = ggo::rand_float(0, 2 * ggo::PI<float>());
	_angle2_end = ggo::rand_float(0, 2 * ggo::PI<float>());
	
	_hue = ggo::rand_float();
	
	_bkgd_colors[0] = ggo::color::from_hsv(_hue, ggo::rand_float(), ggo::rand_float(0, 0.25));
	_bkgd_colors[1] = ggo::color::from_hsv(_hue, ggo::rand_float(), ggo::rand_float(0.5, 0.75));
	_bkgd_colors[2] = ggo::color::from_hsv(_hue, ggo::rand_float(), ggo::rand_float(0, 0.25));
	_bkgd_colors[3] = ggo::color::from_hsv(_hue, ggo::rand_float(), ggo::rand_float(0.5, 0.75));
}

//////////////////////////////////////////////////////////////
bool ggo_ifs_animation_artist::render_next_frame_sub(uint8_t * buffer, int frame_index)
{
	ggo::fill_4_colors(make_image_buffer(buffer), _bkgd_colors[0], _bkgd_colors[1], _bkgd_colors[2], _bkgd_colors[3]);
	
	float t =  ggo::ease_inout(frame_index, FRAMES_COUNT);
	
	float angle1 = ggo::map<float>(t, 0, 1, _angle1_start, _angle1_end);
	float angle2 = ggo::map<float>(t, 0, 1, _angle2_start, _angle2_end);
	
	float transform[4];
	transform[0] = ggo::map<float>(t, 0, 1, _transform_start[0], _transform_end[0]);
	transform[1] = ggo::map<float>(t, 0, 1, _transform_start[1], _transform_end[1]);
	transform[2] = ggo::map<float>(t, 0, 1, _transform_start[2], _transform_end[2]);
	transform[3] = ggo::map<float>(t, 0, 1, _transform_start[3], _transform_end[3]);
	
	_artist.render(buffer, transform, _hue, angle1, angle2);
	
	return frame_index < FRAMES_COUNT;
}