#include "ggo_crystal_artist.h"
#include <ggo_color.h>
#include <ggo_buffer_fill.h>

//////////////////////////////////////////////////////////////
ggo::crystal_artist::crystal_artist(int render_width, int render_height)
:
artist(render_width, render_height),
_render_buffer(3 * get_render_width() * get_render_height())
{

}

//////////////////////////////////////////////////////////////
void ggo::crystal_artist::render_bitmap(uint8_t * buffer, const params & params, const ggo::color_8u bkgd_colors[4])
{
	ggo::fill_4_colors<ggo::rgb_8u_yu>(buffer, get_render_width(), get_render_height(), 3 * get_render_width(),
    bkgd_colors[0], bkgd_colors[1], bkgd_colors[2], bkgd_colors[3]);

	for (int j = 0; j < 16 * get_render_width() * get_render_height(); ++j)
	{
		process_transform(params);
	}

	for (int i = 0; i < 3 * get_render_width() * get_render_height(); ++i)
	{
		buffer[i] = ggo::clamp(ggo::to<int>(255 * _render_buffer(i)), 0, 255);
	}
}

//////////////////////////////////////////////////////////////
void ggo::crystal_artist::process_transform(const params & params)
{
	ggo::pos2f pt;
	
	pt.x() = ggo::rand<float>(-1, 1);
	pt.y() = ggo::rand<float>(-1, 1);

	for (int points_count = 0; points_count < 4; ++points_count)
	{
		// Apply transform.
		int i = ggo::rand<int>(0, transforms_cout);
	
		pt.x() = params._coefs[i][0]*pt.x() + params._coefs[i][1]*pt.y() + params._coefs[i][2];
		pt.y() = params._coefs[i][3]*pt.x() + params._coefs[i][4]*pt.y() + params._coefs[i][5];

		// Render point.
		ggo::pos2f render_pt = map_fit(pt, -5, 5);
		int x = ggo::to<int>(render_pt.x());
		int y = ggo::to<int>(render_pt.y());

		if ((x >= 0) && (x < get_render_width()) &&
			  (y >= 0) && (y < get_render_height()))
		{
			float * ptr = _render_buffer.data() + 3 * (y * get_render_width() + x);

			float diff = 4 * (ptr[0] + ptr[1] + ptr[2]);
			if (x > 0)
			{
				float * it_diff = _render_buffer.data() + 3 * (y * get_render_width() + x - 1);
				diff -= it_diff[0] + it_diff[1] + it_diff[2];
			}
			if (x < get_render_width() - 1)
			{
				float * it_diff = _render_buffer.data() + 3 * (y * get_render_width() + x + 1);
				diff -= it_diff[0]+it_diff[1]+it_diff[2];
			}
			if (y > 0)
			{
				float * it_diff = _render_buffer.data() + 3 * ((y - 1) * get_render_width() + x);
				diff -= it_diff[0] + it_diff[1] + it_diff[2];
			}
			if (y < get_render_height() - 1)
			{
				float * it_diff = _render_buffer.data() + 3 * ((y + 1) * get_render_width() + x);
				diff -= it_diff[0] + it_diff[1] + it_diff[2];
			}

			if (diff < 0.25)
			{
				ptr[0] += params._dr[i];
				ptr[1] += params._dg[i];
				ptr[2] += params._db[i];
			}
		}
	}
}

//////////////////////////////////////////////////////////////
void ggo::crystal_artist::randomize_params(params & params)
{		
	for (int i = 0; i < transforms_cout; ++i)
	{
		params._coefs[i][0] = ggo::rand<float>(-1.f, 1.5f);
		params._coefs[i][1] = ggo::rand<float>(-1.f, 1.5f);
		params._coefs[i][2] = ggo::rand<float>(-1.f, 1.5f);
		params._coefs[i][3] = ggo::rand<float>(-1.f, 1.5f);
		params._coefs[i][4] = ggo::rand<float>(-1.f, 1.5f);
		params._coefs[i][5] = ggo::rand<float>(-1.f, 1.5f);
		
		params._dr[i] = ggo::rand<float>(0.0005f, 0.001f);
		params._dg[i] = ggo::rand<float>(0.0005f, 0.001f);
		params._db[i] = ggo::rand<float>(0.0005f, 0.001f);
	}
}

