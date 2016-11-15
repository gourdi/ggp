#include "ggo_crystal_artist.h"
#include <ggo_color.h>
#include <ggo_buffer_fill.h>

//////////////////////////////////////////////////////////////
ggo::crystal_artist::crystal_artist(int width, int height, int line_step, ggo::pixel_buffer_format pbf)
:
artist(width, height, line_step, pbf),
_render_buffer(width * line_step)
{

}

//////////////////////////////////////////////////////////////
void ggo::crystal_artist::render_bitmap(void * buffer, const params & params, const ggo::color_8u bkgd_colors[4])
{
	ggo::fill_4_colors<ggo::rgb_8u_yu>(buffer, get_width(), get_height(), get_line_step(),
    bkgd_colors[0], bkgd_colors[1], bkgd_colors[2], bkgd_colors[3]);

	for (int j = 0; j < 16 * get_width() * get_height(); ++j)
	{
		process_transform(params);
	}

	for (int y = 0; y < get_height(); ++y)
	{
    const float * ptr_in = _render_buffer.data() + 3 * y * get_width();
    void * ptr_out = ggo::get_line_ptr<ggo::y_up>(buffer, y, get_height(), get_line_step());

    for (int x = 0; x < get_width(); ++x)
    {
      ggo::color_32f c_32f(ptr_in[0], ptr_in[1], ptr_in[2]);
      ggo::write_pixel<ggo::rgb_8u_yu>(ptr_out, ggo::convert_color_to<ggo::color_8u>(c_32f));

      ptr_in += 3;
      ptr_out = ggo::ptr_offset(ptr_out, ggo::pixel_buffer_format_info<ggo::rgb_8u_yu>::pixel_byte_size);
    }
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

		if ((x >= 0) && (x < get_width()) &&
			  (y >= 0) && (y < get_height()))
		{
			float * ptr = _render_buffer.data() + 3 * (y * get_width() + x);

			float diff = 4 * (ptr[0] + ptr[1] + ptr[2]);
			if (x > 0)
			{
				float * it_diff = _render_buffer.data() + 3 * (y * get_width() + x - 1);
				diff -= it_diff[0] + it_diff[1] + it_diff[2];
			}
			if (x < get_width() - 1)
			{
				float * it_diff = _render_buffer.data() + 3 * (y * get_width() + x + 1);
				diff -= it_diff[0]+it_diff[1]+it_diff[2];
			}
			if (y > 0)
			{
				float * it_diff = _render_buffer.data() + 3 * ((y - 1) * get_width() + x);
				diff -= it_diff[0] + it_diff[1] + it_diff[2];
			}
			if (y < get_height() - 1)
			{
				float * it_diff = _render_buffer.data() + 3 * ((y + 1) * get_width() + x);
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

