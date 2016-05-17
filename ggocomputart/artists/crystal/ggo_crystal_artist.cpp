#include "ggo_crystal_artist.h"
#include <ggo_color.h>
#include <ggo_fill.h>

//////////////////////////////////////////////////////////////
ggo_crystal_artist::ggo_crystal_artist(int render_width, int render_height)
:
ggo_artist_abc(render_width, render_height),
_render_buffer(3 * get_render_width() * get_render_height())
{

}

//////////////////////////////////////////////////////////////
void ggo_crystal_artist::render_bitmap(uint8_t * buffer, const ggo_params & params, const ggo::color bkgd_colors[4])
{
  auto image = make_image_buffer(buffer);
	ggo::fill_4_colors(image, bkgd_colors[0], bkgd_colors[1], bkgd_colors[2], bkgd_colors[3]);

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
void ggo_crystal_artist::process_transform(const ggo_params & params)
{
	ggo::point2d_float pt;
	
	pt.x() = ggo::rand_float(-1, 1);
	pt.y() = ggo::rand_float(-1, 1);

	for (int points_count = 0; points_count < 4; ++points_count)
	{
		// Apply transform.
		int i = ggo::rand_int(0, GGO_TRANSFORMS_COUNT);
	
		pt.x() = params._coefs[i][0]*pt.x() + params._coefs[i][1]*pt.y() + params._coefs[i][2];
		pt.y() = params._coefs[i][3]*pt.x() + params._coefs[i][4]*pt.y() + params._coefs[i][5];

		// Render point.
		ggo::point2d_float render_pt = map_fit(pt, -5, 5);
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
void ggo_crystal_artist::randomize_params(ggo_params & params)
{		
	for (int i = 0; i < GGO_TRANSFORMS_COUNT; ++i)
	{
		params._coefs[i][0] = ggo::rand_float(-1.f, 1.5f);
		params._coefs[i][1] = ggo::rand_float(-1.f, 1.5f);
		params._coefs[i][2] = ggo::rand_float(-1.f, 1.5f);
		params._coefs[i][3] = ggo::rand_float(-1.f, 1.5f);
		params._coefs[i][4] = ggo::rand_float(-1.f, 1.5f);
		params._coefs[i][5] = ggo::rand_float(-1.f, 1.5f);
		
		params._dr[i] = ggo::rand_float(0.0005f, 0.001f);
		params._dg[i] = ggo::rand_float(0.0005f, 0.001f);
		params._db[i] = ggo::rand_float(0.0005f, 0.001f);
	}
}

