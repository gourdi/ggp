#include "ggo_crystal_animation_artist.h"

#define GGO_CRYSTAL_FRAMES_COUNT 100

//////////////////////////////////////////////////////////////
ggo_crystal_animation_artist::ggo_crystal_animation_artist(int render_width, int render_height)
:
ggo_animation_artist_abc(render_width, render_height),
_artist(render_width, render_height)
{
	
}

//////////////////////////////////////////////////////////////
void ggo_crystal_animation_artist::init_sub()
{
	_bkgd_colors[0] = ggo::color::from_hsv(ggo::rand_float(0, 1), ggo::rand_float(0, 1), ggo::rand_float(0.1f, 0.3f));
	_bkgd_colors[1] = ggo::color::from_hsv(ggo::rand_float(0, 1), ggo::rand_float(0, 1), ggo::rand_float(0.1f, 0.3f));
	_bkgd_colors[2] = ggo::color::from_hsv(ggo::rand_float(0, 1), ggo::rand_float(0, 1), ggo::rand_float(0.1f, 0.3f));
	_bkgd_colors[3] = ggo::color::from_hsv(ggo::rand_float(0, 1), ggo::rand_float(0, 1), ggo::rand_float(0.1f, 0.3f));

	ggo_crystal_artist::randomize_params(_start_params);
	ggo_crystal_artist::randomize_params(_end_params);
}

//////////////////////////////////////////////////////////////
bool ggo_crystal_animation_artist::render_next_frame_sub(uint8_t * buffer, int frame_index)
{
	if (frame_index >= GGO_CRYSTAL_FRAMES_COUNT) 
	{
		return false;
	}
	
	ggo_crystal_artist::ggo_params params;
	
	for (int i = 0; i < GGO_TRANSFORMS_COUNT; ++i)
	{
		params._coefs[i][0] = ggo::ease_inout(frame_index, GGO_CRYSTAL_FRAMES_COUNT, _start_params._coefs[i][0], _end_params._coefs[i][0]);
		params._coefs[i][1] = ggo::ease_inout(frame_index, GGO_CRYSTAL_FRAMES_COUNT, _start_params._coefs[i][1], _end_params._coefs[i][1]);
		params._coefs[i][2] = ggo::ease_inout(frame_index, GGO_CRYSTAL_FRAMES_COUNT, _start_params._coefs[i][2], _end_params._coefs[i][2]);
		params._coefs[i][3] = ggo::ease_inout(frame_index, GGO_CRYSTAL_FRAMES_COUNT, _start_params._coefs[i][3], _end_params._coefs[i][3]);
		params._coefs[i][4] = ggo::ease_inout(frame_index, GGO_CRYSTAL_FRAMES_COUNT, _start_params._coefs[i][4], _end_params._coefs[i][4]);
		params._coefs[i][5] = ggo::ease_inout(frame_index, GGO_CRYSTAL_FRAMES_COUNT, _start_params._coefs[i][5], _end_params._coefs[i][5]);
		
		params._dr[i] = ggo::ease_inout(frame_index, GGO_CRYSTAL_FRAMES_COUNT, _start_params._dr[i], _end_params._dr[i]);
		params._dg[i] = ggo::ease_inout(frame_index, GGO_CRYSTAL_FRAMES_COUNT, _start_params._dg[i], _end_params._dg[i]);
		params._db[i] = ggo::ease_inout(frame_index, GGO_CRYSTAL_FRAMES_COUNT, _start_params._db[i], _end_params._db[i]);
	}
	
	_artist.render_bitmap(buffer, params, _bkgd_colors);
	
	return true;
}