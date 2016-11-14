#include "ggo_vortex2_bitmap_artist.h"
#include "ggo_vortex2_artist.h"

//////////////////////////////////////////////////////////////
ggo::vortex2_bitmap_artist::vortex2_bitmap_artist(int render_width, int render_height)
:
bitmap_artist_abc(render_width, render_height)
{
}

//////////////////////////////////////////////////////////////
void ggo::vortex2_bitmap_artist::render_bitmap(void * buffer) const
{
  const int vortices_count = 20;

	ggo::vortex2_artist::params params;
	params._vortices.resize(vortices_count);
	params._color1 = ggo::from_hsv<ggo::color_32f>(ggo::rand<float>(), ggo::rand<float>(0.4f, 0.6f), 1);
	params._color2 = ggo::from_hsv<ggo::color_32f>(ggo::rand<float>(), ggo::rand<float>(0.4f, 0.6f), 1);
	params._color3 = ggo::from_hsv<ggo::color_32f>(ggo::rand<float>(), ggo::rand<float>(0.4f, 0.6f), 1);
	params._color4 = ggo::from_hsv<ggo::color_32f>(ggo::rand<float>(), ggo::rand<float>(0.4f, 0.6f), 1);
	params._split_horz = ggo::rand<float>(0.4f * get_render_width(),  0.6f * get_render_width());
	params._split_vert = ggo::rand<float>(0.4f * get_render_height(), 0.6f * get_render_height());

	for (auto & vortex : params._vortices)
	{
		vortex._pos.get<0>() = ggo::rand<float>() * get_render_width();
		vortex._pos.get<1>() = ggo::rand<float>() * get_render_height();
		vortex._angle = ggo::rand<float>(ggo::pi<float>() / 2, ggo::pi<float>() / 2 + 0.5f);
		vortex._speed = ggo::rand<float>(0.004f, 0.008f) * get_render_max_size();
		vortex._power = ggo::rand<float>(6.f, 8.f);
	}
	
	ggo::vortex2_artist::render(buffer, get_render_width(), get_render_height(), params);
}



