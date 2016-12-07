#include "ggo_crystal_animation_artist.h"
#include <ggo_buffer_fill.h>

namespace
{
  const int frames_count = 100;
}

//////////////////////////////////////////////////////////////
ggo::crystal_animation_artist::crystal_animation_artist(int width, int height, int line_step, ggo::pixel_buffer_format pbf, rendering_type rt)
:
animation_artist_abc(width, height, line_step, pbf, rt)
{
	
}

//////////////////////////////////////////////////////////////
void ggo::crystal_animation_artist::init()
{
  _frame_index = -1;

	_bkgd_colors[0] = ggo::from_hsv<ggo::color_8u>(ggo::rand<float>(), ggo::rand<float>(), ggo::rand<float>(0.1f, 0.3f));
	_bkgd_colors[1] = ggo::from_hsv<ggo::color_8u>(ggo::rand<float>(), ggo::rand<float>(), ggo::rand<float>(0.1f, 0.3f));
	_bkgd_colors[2] = ggo::from_hsv<ggo::color_8u>(ggo::rand<float>(), ggo::rand<float>(), ggo::rand<float>(0.1f, 0.3f));
	_bkgd_colors[3] = ggo::from_hsv<ggo::color_8u>(ggo::rand<float>(), ggo::rand<float>(), ggo::rand<float>(0.1f, 0.3f));

	ggo::crystal_artist::randomize_params(_start_params);
	ggo::crystal_artist::randomize_params(_end_params);
}

//////////////////////////////////////////////////////////////
bool ggo::crystal_animation_artist::update()
{
  ++_frame_index;

  if (_frame_index >= frames_count)
  {
    return false;
  }

  return true;
}

//////////////////////////////////////////////////////////////
void ggo::crystal_animation_artist::render_frame(void * buffer, const ggo::pixel_rect & clipping)
{
  ggo::fill_4_colors<ggo::rgb_8u_yu>(buffer, get_width(), get_height(), get_line_step(),
    _bkgd_colors[0], _bkgd_colors[1], _bkgd_colors[2], _bkgd_colors[3], clipping);

	ggo::crystal_artist::params params;
	
	for (int i = 0; i < ggo::crystal_artist::transforms_cout; ++i)
	{
		params._coefs[i][0] = ggo::ease_inout(_frame_index, frames_count, _start_params._coefs[i][0], _end_params._coefs[i][0]);
		params._coefs[i][1] = ggo::ease_inout(_frame_index, frames_count, _start_params._coefs[i][1], _end_params._coefs[i][1]);
		params._coefs[i][2] = ggo::ease_inout(_frame_index, frames_count, _start_params._coefs[i][2], _end_params._coefs[i][2]);
		params._coefs[i][3] = ggo::ease_inout(_frame_index, frames_count, _start_params._coefs[i][3], _end_params._coefs[i][3]);
		params._coefs[i][4] = ggo::ease_inout(_frame_index, frames_count, _start_params._coefs[i][4], _end_params._coefs[i][4]);
		params._coefs[i][5] = ggo::ease_inout(_frame_index, frames_count, _start_params._coefs[i][5], _end_params._coefs[i][5]);
		
		params._dr[i] = ggo::ease_inout(_frame_index, frames_count, _start_params._dr[i], _end_params._dr[i]);
		params._dg[i] = ggo::ease_inout(_frame_index, frames_count, _start_params._dg[i], _end_params._dg[i]);
		params._db[i] = ggo::ease_inout(_frame_index, frames_count, _start_params._db[i], _end_params._db[i]);
	}
	
	_artist.render_bitmap(buffer, get_width(), get_height(), get_line_step(), get_pixel_buffer_format(), params);
}