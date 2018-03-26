#include "ggo_ifs_animation_artist.h"
#include <ggo_buffer_fill.h>

namespace
{
  const int frames_count = 300;
}

//////////////////////////////////////////////////////////////
ggo::ifs_animation_artist::ifs_animation_artist(int width, int height, int line_step, ggo::pixel_buffer_format pbf, rendering_type rt)
:
animation_artist_abc(width, height, line_step, pbf, rt),
_artist(width, height)
{
	
}

//////////////////////////////////////////////////////////////
void ggo::ifs_animation_artist::init_animation()
{
  _frame_index = -1;

	_transform_start[0] = ggo::rand<float>(1, 2);
	_transform_start[1] = ggo::rand<float>(1, 2);
	_transform_start[2] = ggo::rand<float>(1, 2);
	_transform_start[3] = ggo::rand<float>(1, 2);
	
	_transform_end[0] = _transform_start[0] + ggo::rand<float>(-0.3f, 0.3f);
	_transform_end[1] = _transform_start[1] + ggo::rand<float>(-0.3f, 0.3f);
	_transform_end[2] = _transform_start[2] + ggo::rand<float>(-0.3f, 0.3f);
	_transform_end[3] = _transform_start[3] + ggo::rand<float>(-0.3f, 0.3f);
	
	_angle1_start = ggo::rand<float>(0, 2 * ggo::pi<float>());
	_angle1_end = ggo::rand<float>(0, 2 * ggo::pi<float>());
	_angle2_start = ggo::rand<float>(0, 2 * ggo::pi<float>());
	_angle2_end = ggo::rand<float>(0, 2 * ggo::pi<float>());
	
	_hue = ggo::rand<float>();
	
	_bkgd_colors[0] = ggo::from_hsv<ggo::color_8u>(_hue, ggo::rand<float>(), ggo::rand<float>(0, 0.25));
	_bkgd_colors[1] = ggo::from_hsv<ggo::color_8u>(_hue, ggo::rand<float>(), ggo::rand<float>(0.5, 0.75));
	_bkgd_colors[2] = ggo::from_hsv<ggo::color_8u>(_hue, ggo::rand<float>(), ggo::rand<float>(0, 0.25));
	_bkgd_colors[3] = ggo::from_hsv<ggo::color_8u>(_hue, ggo::rand<float>(), ggo::rand<float>(0.5, 0.75));
}

//////////////////////////////////////////////////////////////
bool ggo::ifs_animation_artist::prepare_frame()
{
  ++_frame_index;

  if (_frame_index >= frames_count)
  {
    return false;
  }

  return true;
}

//////////////////////////////////////////////////////////////
void ggo::ifs_animation_artist::render_frame(void * buffer, const ggo::rect_int & clipping)
{
  ggo::fill_4_colors<ggo::rgb_8u_yu>(
    buffer, get_width(), get_height(), get_line_step(), 
    _bkgd_colors[0], _bkgd_colors[1], _bkgd_colors[2], _bkgd_colors[3], clipping);
	
	float t =  ggo::ease_inout_to<float>(_frame_index, frames_count);
	
	float angle1 = ggo::map<float>(t, 0, 1, _angle1_start, _angle1_end);
	float angle2 = ggo::map<float>(t, 0, 1, _angle2_start, _angle2_end);
	
	float transform[4];
	transform[0] = ggo::map<float>(t, 0, 1, _transform_start[0], _transform_end[0]);
	transform[1] = ggo::map<float>(t, 0, 1, _transform_start[1], _transform_end[1]);
	transform[2] = ggo::map<float>(t, 0, 1, _transform_start[2], _transform_end[2]);
	transform[3] = ggo::map<float>(t, 0, 1, _transform_start[3], _transform_end[3]);
	
	_artist.render(buffer, get_line_step(), get_pixel_buffer_format(), transform, _hue, angle1, angle2);
}