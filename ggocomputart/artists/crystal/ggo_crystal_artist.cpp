#include "ggo_crystal_artist.h"
#include <ggo_coordinates_conversions.h>
#include <ggo_color.h>
#include <ggo_array.h>
#include <ggo_blend.h>

//////////////////////////////////////////////////////////////
void ggo::crystal_artist::render_bitmap(void * buffer, int width, int height, int line_step, ggo::pixel_buffer_format pbf, const params & params)
{
  ggo::array<float, 1> float_buffer(3 * width * height, 0.f);

	for (int j = 0; j < 16 * width * height; ++j)
	{
		render_transform(float_buffer.data(), width, height, params);
	}

	for (int y = 0; y < height; ++y)
	{
    const float * ptr_in = float_buffer.data() + 3 * y * width;
    void * ptr_out = ggo::get_line_ptr<ggo::y_8u_yu>(buffer, y, height, line_step);

    for (int x = 0; x < width; ++x)
    {
      const ggo::color_32f c_32f(ptr_in[0], ptr_in[1], ptr_in[2]);
      const ggo::color_8u c_8u(additive_blend(read_pixel<ggo::rgb_8u_yu>(buffer), ggo::convert_color_to<ggo::color_8u>(c_32f)));
      ggo::write_pixel<ggo::rgb_8u_yu>(ptr_out, c_8u);

      ptr_in += 3;
      ptr_out = ggo::ptr_offset<ggo::pixel_buffer_format_info<ggo::rgb_8u_yu>::pixel_byte_size>(ptr_out);
    }
	}
}

//////////////////////////////////////////////////////////////
void ggo::crystal_artist::render_transform(float * buffer, int width, int height, const params & params)
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
		ggo::pos2f render_pt = ggo::map_fit(pt, -5.f, 5.f, width, height);
		int x = ggo::round_to<int>(render_pt.x());
		int y = ggo::round_to<int>(render_pt.y());

		if ((x >= 0) && (x < width) &&
			  (y >= 0) && (y < height))
		{
			float * ptr = buffer + 3 * (y * width + x);

			float diff = 4 * (ptr[0] + ptr[1] + ptr[2]);
			if (x > 0)
			{
				float * it_diff = buffer + 3 * (y * width + x - 1);
				diff -= it_diff[0] + it_diff[1] + it_diff[2];
			}
			if (x < width - 1)
			{
				float * it_diff = buffer + 3 * (y * width + x + 1);
				diff -= it_diff[0]+it_diff[1]+it_diff[2];
			}
			if (y > 0)
			{
				float * it_diff = buffer + 3 * ((y - 1) * width + x);
				diff -= it_diff[0] + it_diff[1] + it_diff[2];
			}
			if (y < height - 1)
			{
				float * it_diff = buffer + 3 * ((y + 1) * width + x);
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

