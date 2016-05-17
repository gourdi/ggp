#include "ggo_vortex2_bitmap_artist.h"
#include "ggo_vortex2_artist.h"

#define GGO_VORTEX2_VORTICES_COUNT 20

//////////////////////////////////////////////////////////////
ggo_vortex2_bitmap_artist::ggo_vortex2_bitmap_artist(int render_width, int render_height)
:
ggo_bitmap_artist_abc(render_width, render_height)
{
}

//////////////////////////////////////////////////////////////
void ggo_vortex2_bitmap_artist::render_bitmap(uint8_t * buffer)
{
	ggo_vortex2_artist::ggo_vortex2_params params;
	params._vortices.resize(GGO_VORTEX2_VORTICES_COUNT);
	params._color1 = ggo::color::from_hsv(ggo::rand_float(), ggo::rand_float(0.4f, 0.6f), 1);
	params._color2 = ggo::color::from_hsv(ggo::rand_float(), ggo::rand_float(0.4f, 0.6f), 1);
	params._color3 = ggo::color::from_hsv(ggo::rand_float(), ggo::rand_float(0.4f, 0.6f), 1);
	params._color4 = ggo::color::from_hsv(ggo::rand_float(), ggo::rand_float(0.4f, 0.6f), 1);
	params._split_horz = ggo::rand_float(0.4f * get_render_width(),  0.6f * get_render_width());
	params._split_vert = ggo::rand_float(0.4f * get_render_height(), 0.6f * get_render_height());

	for (auto & vortex : params._vortices)
	{
		vortex._pos.x() = ggo::rand_float() * get_render_width();
		vortex._pos.y() = ggo::rand_float() * get_render_height();
		vortex._angle = ggo::rand_float(ggo::PI<float>() / 2, ggo::PI<float>() / 2 + 0.5f);
		vortex._speed = ggo::rand_float(0.004f, 0.008f) * get_render_max_size();
		vortex._power = ggo::rand_float(6.f, 8.f);
	}
	
	ggo_vortex2_artist::render(buffer, get_render_width(), get_render_height(), params);
}



