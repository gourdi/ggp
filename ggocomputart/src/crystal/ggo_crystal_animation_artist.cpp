#include "ggo_crystal_animation_artist.h"
#include <2d/fill/ggo_fill.h>

//////////////////////////////////////////////////////////////
ggo::crystal_animation_artist::crystal_animation_artist(
  int width, int height, int line_byte_step,
  ggo::pixel_type pixel_type, ggo::lines_order memory_lines_order,
  ggo::ratio fps)
:
progress_animation_artist(width, height, line_byte_step, pixel_type, memory_lines_order, 4, fps)
{
  _bkgd_colors[0] = ggo::from_hsv<ggo::rgb_8u>(ggo::rand<float>(), ggo::rand<float>(), ggo::rand<float>(0.1f, 0.3f));
  _bkgd_colors[1] = ggo::from_hsv<ggo::rgb_8u>(ggo::rand<float>(), ggo::rand<float>(), ggo::rand<float>(0.1f, 0.3f));
  _bkgd_colors[2] = ggo::from_hsv<ggo::rgb_8u>(ggo::rand<float>(), ggo::rand<float>(), ggo::rand<float>(0.1f, 0.3f));
  _bkgd_colors[3] = ggo::from_hsv<ggo::rgb_8u>(ggo::rand<float>(), ggo::rand<float>(), ggo::rand<float>(0.1f, 0.3f));

  ggo::crystal_artist::randomize_params(_start_params);
  ggo::crystal_artist::randomize_params(_end_params);
}

//////////////////////////////////////////////////////////////
void ggo::crystal_animation_artist::render_frame(void * buffer, float progress)
{
  progress = ggo::ease_inout(progress);

  image_t<ggo::pixel_type::rgb_8u, ggo::lines_order::up> img(buffer, size(), line_byte_step());

  ggo::fill_4_colors(img, _bkgd_colors[0], _bkgd_colors[1], _bkgd_colors[2], _bkgd_colors[3]);

	ggo::crystal_artist::params params;
	
	for (int i = 0; i < ggo::crystal_artist::transforms_cout; ++i)
	{
		params._coefs[i][0] = ggo::linerp(_start_params._coefs[i][0], _end_params._coefs[i][0], progress);
		params._coefs[i][1] = ggo::linerp(_start_params._coefs[i][1], _end_params._coefs[i][1], progress);
		params._coefs[i][2] = ggo::linerp(_start_params._coefs[i][2], _end_params._coefs[i][2], progress);
		params._coefs[i][3] = ggo::linerp(_start_params._coefs[i][3], _end_params._coefs[i][3], progress);
		params._coefs[i][4] = ggo::linerp(_start_params._coefs[i][4], _end_params._coefs[i][4], progress);
		params._coefs[i][5] = ggo::linerp(_start_params._coefs[i][5], _end_params._coefs[i][5], progress);
		
		params._dr[i] = ggo::linerp(_start_params._dr[i], _end_params._dr[i], progress);
		params._dg[i] = ggo::linerp(_start_params._dg[i], _end_params._dg[i], progress);
		params._db[i] = ggo::linerp(_start_params._db[i], _end_params._db[i], progress);
	}
	
  ggo::crystal_artist::render_bitmap(img, params);
}