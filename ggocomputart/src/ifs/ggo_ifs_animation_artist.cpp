#include "ggo_ifs_animation_artist.h"
#include "ggo_ifs_artist.h"
#include <2d/fill/ggo_fill.h>

namespace
{
  const int frames_count = 300;
}

//////////////////////////////////////////////////////////////
ggo::ifs_animation_artist::ifs_animation_artist(
  int width, int height, int line_byte_step,
  ggo::pixel_type pixel_type, ggo::lines_order memory_lines_order,
  ggo::ratio fps)
:
progress_animation_artist(width, height, line_byte_step, pixel_type, memory_lines_order, 14, fps)
{
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

  _bkgd_colors[0] = ggo::from_hsv<ggo::rgb_8u>(_hue, ggo::rand<float>(), ggo::rand<float>(0, 0.25));
  _bkgd_colors[1] = ggo::from_hsv<ggo::rgb_8u>(_hue, ggo::rand<float>(), ggo::rand<float>(0.5, 0.75));
  _bkgd_colors[2] = ggo::from_hsv<ggo::rgb_8u>(_hue, ggo::rand<float>(), ggo::rand<float>(0, 0.25));
  _bkgd_colors[3] = ggo::from_hsv<ggo::rgb_8u>(_hue, ggo::rand<float>(), ggo::rand<float>(0.5, 0.75));
}

//////////////////////////////////////////////////////////////
void ggo::ifs_animation_artist::render_frame(void * buffer, float progress)
{
  ggo::image_t<ggo::pixel_type::rgb_8u, ggo::lines_order::up> img(buffer, size(), line_byte_step());

  ggo::fill_4_colors(img, _bkgd_colors[0], _bkgd_colors[1], _bkgd_colors[2], _bkgd_colors[3]);
	
  progress =  ggo::ease_inout(progress);
	
	float angle1 = ggo::linerp(_angle1_start, _angle1_end, progress);
	float angle2 = ggo::linerp(_angle2_start, _angle2_end, progress);
	
	float transform[4];
	transform[0] = ggo::linerp(_transform_start[0], _transform_end[0], progress);
	transform[1] = ggo::linerp(_transform_start[1], _transform_end[1], progress);
	transform[2] = ggo::linerp(_transform_start[2], _transform_end[2], progress);
	transform[3] = ggo::linerp(_transform_start[3], _transform_end[3], progress);
	
  ggo::ifs_artist	artist(width(), height());
  artist.render(buffer, line_byte_step(), pixel_type(), memory_lines_order(), transform, _hue, angle1, angle2);
}
