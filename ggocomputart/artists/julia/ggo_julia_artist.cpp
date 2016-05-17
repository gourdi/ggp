#include "ggo_julia_artist.h"
#include <ggo_curve.h>
#include <ggo_color.h>

//////////////////////////////////////////////////////////////
ggo_julia_artist::ggo_julia_artist(int render_width, int render_height)
:
ggo_artist_abc(render_width, render_height),
_palette(1000)
{
	// Build the palette.
	float hue = ggo::rand_float();
	
	ggo::linear_curve_float sat_curve;
	sat_curve.push_point(0, ggo::rand_float(0, 0.25));
	sat_curve.push_point(1, ggo::rand_float());
	
	ggo::linear_curve_float val_curve;
	val_curve.push_point(0, ggo::rand_float(0.75, 1));
	val_curve.push_point(1, ggo::rand_float(0, 0.25));
	
	for (int i = 0; i < _palette.get_count(); ++i)
	{
		float x = float(i) / _palette.get_count();
		
		float sat = sat_curve.evaluate(x);
		float val = val_curve.evaluate(x);
		
		ggo::color color = ggo::color::from_hsv(hue, sat, val);
		
		_palette(i)._r = color.r8();
		_palette(i)._b = color.g8();
		_palette(i)._g = color.b8();
	}
}
	
//////////////////////////////////////////////////////////////
std::complex<float> ggo_julia_artist::pickup_seed()
{
	if (ggo::rand_bool())
	{	
		// Pick the seek near the main cardioid of the Mandelbrot set.
		float angle = ggo::rand_float(0, 2 * ggo::PI<float>());
		float radius = (1 - cos(angle)) / 2 + ggo::rand_float(0, 0.01f);
	
		return std::complex<float>(0.25f + radius * std::cos(angle), radius * std::sin(angle));
	}
	else
	{
		// Pick the seek near the circle on the left of the main cardioid.
		float angle = ggo::rand_float(0, 2 * ggo::PI<float>());
		float radius = 0.25f + ggo::rand_float(0, 0.01f);

		return std::complex<float>(-1 + radius * std::cos(angle), radius * std::sin(angle));	
	}
}
	
//////////////////////////////////////////////////////////////
void ggo_julia_artist::render_bitmap(uint8_t * buffer, const std::complex<float> & seed, float range)
{
	for (int y = 0; y < get_render_height(); ++y)
	{
		float range_x = get_render_width() > get_render_height() ? range * get_render_width() / get_render_height() : range;
		float range_y = get_render_width() > get_render_height() ? range : range * get_render_height() / get_render_width();

		float y1 = ggo::map(y - 3 / 8.f, 0.f, static_cast<float>(get_render_height()), -range_y, range_y);
		float y2 = ggo::map(y - 1 / 8.f, 0.f, static_cast<float>(get_render_height()), -range_y, range_y);
		float y3 = ggo::map(y + 1 / 8.f, 0.f, static_cast<float>(get_render_height()), -range_y, range_y);
		float y4 = ggo::map(y + 3 / 8.f, 0.f, static_cast<float>(get_render_height()), -range_y, range_y);
		int iterations[16];
	
		for (int x = 0; x < get_render_width(); ++x)
		{
			// Iterate and sample.
			float x1 = ggo::map(x - 3 / 8.f, 0.f, static_cast<float>(get_render_width()), -range_x, range_x);
			float x2 = ggo::map(x - 1 / 8.f, 0.f, static_cast<float>(get_render_width()), -range_x, range_x);
			float x3 = ggo::map(x + 1 / 8.f, 0.f, static_cast<float>(get_render_width()), -range_x, range_x);
			float x4 = ggo::map(x + 3 / 8.f, 0.f, static_cast<float>(get_render_width()), -range_x, range_x);
		
			iterations[0] = iterate(x1, y1, seed);
			iterations[1] = iterate(x1, y4, seed);
			iterations[2] = iterate(x4, y1, seed);
			iterations[3] = iterate(x4, y4, seed);
		
			int index = 0;
			if ((iterations[0] == iterations[1]) && (iterations[1] == iterations[2]) && (iterations[2] == iterations[3]))
			{
				index = iterations[0];
			}
			else
			{
				iterations[4] = iterate(x1, y2, seed);
				iterations[5] = iterate(x1, y3, seed);
			
				iterations[6] = iterate(x2, y1, seed);
				iterations[7] = iterate(x2, y2, seed);
				iterations[8] = iterate(x2, y3, seed);
				iterations[9] = iterate(x2, y4, seed);
			
				iterations[10] = iterate(x3, y1, seed);
				iterations[11] = iterate(x3, y2, seed);
				iterations[12] = iterate(x3, y3, seed);
				iterations[13] = iterate(x3, y4, seed);
			
				iterations[14] = iterate(x4, y2, seed);
				iterations[15] = iterate(x4, y3, seed);
			
				index = iterations[0] + iterations[1]+ iterations[2] + iterations[3];
				index += iterations[4] + iterations[5]+ iterations[6] + iterations[7];
				index += iterations[8] + iterations[9]+ iterations[10] + iterations[11];
				index += iterations[12] + iterations[13]+ iterations[14] + iterations[15];
				index = (index + 8) / 16;
			}
		
			// Set the proper pixel color.
			index = std::min(_palette.get_count() - 1, index);
			buffer[0] = _palette(index)._r;
			buffer[1] = _palette(index)._g;
			buffer[2] = _palette(index)._b;
			buffer += 3;
		}
	}
}

//////////////////////////////////////////////////////////////
int ggo_julia_artist::iterate(float x, float y, const std::complex<float> & seed) const
{
	std::complex<float> z(x, y);
	
	int i;
	
	for (i = 0; i < _palette.get_count(); ++i)
	{
		z = z * z + seed;

		if (z.real() * z.real() + z.imag() * z.imag() > 10)
		{
			break;
		}
	}
	
	return i;
}

