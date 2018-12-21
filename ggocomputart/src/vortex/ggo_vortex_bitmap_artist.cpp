#include "ggo_vortex_bitmap_artist.h"
#include "ggo_vortex_artist.h"

//////////////////////////////////////////////////////////////
ggo::vortex_bitmap_artist::vortex_bitmap_artist(int width, int height, int line_step, ggo::image_format format)
:
bitmap_artist_abc(width, height, line_step, format)
{
}

//////////////////////////////////////////////////////////////
void ggo::vortex_bitmap_artist::render_bitmap(void * buffer) const
{
  const int vortices_count = 20;

	ggo::vortex_artist::params params;
	params._vortices = ggo::array<vortex_artist::vortex, 1>(vortices_count);
	params._color1 = ggo::from_hsv<ggo::rgb_32f>(ggo::rand<float>(), ggo::rand<float>(0.4f, 0.6f), 1);
	params._color2 = ggo::from_hsv<ggo::rgb_32f>(ggo::rand<float>(), ggo::rand<float>(0.4f, 0.6f), 1);
	params._color3 = ggo::from_hsv<ggo::rgb_32f>(ggo::rand<float>(), ggo::rand<float>(0.4f, 0.6f), 1);
	params._color4 = ggo::from_hsv<ggo::rgb_32f>(ggo::rand<float>(), ggo::rand<float>(0.4f, 0.6f), 1);
	params._split_horz = ggo::rand<float>(0.4f * width(),  0.6f * width());
	params._split_vert = ggo::rand<float>(0.4f * height(), 0.6f * height());

	for (auto & vortex : params._vortices)
	{
		vortex._pos.x() = ggo::rand<float>() * width();
		vortex._pos.y() = ggo::rand<float>() * height();
		vortex._angle = ggo::rand<float>(ggo::pi<float>() / 2, ggo::pi<float>() / 2 + 0.5f);
		vortex._speed = ggo::rand<float>(0.004f, 0.008f) * max_size();
		vortex._power = ggo::rand<float>(6.f, 8.f);
	}
	
	ggo::vortex_artist::render(buffer, width(), height(), line_step(), ggo::rgb_8u_yu, params);
}



