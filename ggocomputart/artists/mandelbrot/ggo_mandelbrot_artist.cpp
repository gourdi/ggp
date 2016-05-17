#include "ggo_mandelbrot_artist.h"
#include <ggo_curve.h>
#include <ggo_color.h>
#include <algorithm>
#include <cmath>

#define GGO_BLOCK_COUNT 5

struct ggo_block
{
	std::complex<double> _center;
	double _range;
	double _deviation;
};

static bool sort_func(const ggo_block & v1, const ggo_block & v2)
{
	return v1._deviation > v2._deviation;
}

//////////////////////////////////////////////////////////////
ggo_mandelbrot_artist::ggo_mandelbrot_artist(int render_width, int render_height)
:
ggo_bitmap_artist_abc(render_width, render_height),
_palette(1000)
{
}
	
//////////////////////////////////////////////////////////////
void ggo_mandelbrot_artist::render_bitmap(uint8_t * buffer)
{
	setup_palette();
	setup_block();

	for (int y = 0; y < get_render_height(); ++y)
	{
		double range_x = get_render_width() > get_render_height() ? _range * get_render_width() / get_render_height() : _range;
		double range_y = get_render_width() > get_render_height() ? _range : _range * get_render_height() / get_render_width();

		double y1 = ggo::map<double>(y - 3 / 8., 0, get_render_height(), _center.imag() - range_y, _center.imag() + range_y);
		double y2 = ggo::map<double>(y - 1 / 8., 0, get_render_height(), _center.imag() - range_y, _center.imag() + range_y);
		double y3 = ggo::map<double>(y + 1 / 8., 0, get_render_height(), _center.imag() - range_y, _center.imag() + range_y);
		double y4 = ggo::map<double>(y + 3 / 8., 0, get_render_height(), _center.imag() - range_y, _center.imag() + range_y);
	
		int iterations[16];

		for (int x = 0; x < get_render_width(); ++x)
		{
			// Iterate and sample.
			double x1 = ggo::map<double>(x - 3 / 8., 0, get_render_width(), _center.real() - range_x, _center.real() + range_x);
			double x2 = ggo::map<double>(x - 1 / 8., 0, get_render_width(), _center.real() - range_x, _center.real() + range_x);
			double x3 = ggo::map<double>(x + 1 / 8., 0, get_render_width(), _center.real() - range_x, _center.real() + range_x);
			double x4 = ggo::map<double>(x + 3 / 8., 0, get_render_width(), _center.real() - range_x, _center.real() + range_x);
		
			iterations[0] = iterate(x1, y1);
			iterations[1] = iterate(x1, y4);
			iterations[2] = iterate(x4, y1);
			iterations[3] = iterate(x4, y4);
		
			int index = 0;
			if ((iterations[0] == iterations[1]) && (iterations[1] == iterations[2]) && (iterations[2] == iterations[3]))
			{
				int index = std::min(_palette.get_count() - 1, iterations[0]);
				buffer[0] = _palette(index)._r;
				buffer[1] = _palette(index)._g;
				buffer[2] = _palette(index)._b;
			}
			else
			{
				iterations[4] = iterate(x1, y2);
				iterations[5] = iterate(x1, y3);
			
				iterations[6] = iterate(x2, y1);
				iterations[7] = iterate(x2, y2);
				iterations[8] = iterate(x2, y3);
				iterations[9] = iterate(x2, y4);
			
				iterations[10] = iterate(x3, y1);
				iterations[11] = iterate(x3, y2);
				iterations[12] = iterate(x3, y3);
				iterations[13] = iterate(x3, y4);
			
				iterations[14] = iterate(x4, y2);
				iterations[15] = iterate(x4, y3);
			
				int r = 0;
				int g = 0;
				int b = 0;
				for (int i = 0; i < 16; ++i)
				{
					int index = std::min(_palette.get_count() - 1, iterations[i]);
					r += _palette(index)._r;
					g += _palette(index)._g;
					b += _palette(index)._b;
				}
			
				buffer[0] = (r + 8) / 16;
				buffer[1] = (g + 8) / 16;
				buffer[2] = (b + 8) / 16;
			}

			buffer += 3;
		}
	}
}

//////////////////////////////////////////////////////////////
int ggo_mandelbrot_artist::iterate(double x, double y) const
{
	std::complex<double> z(x, y), c(x, y);
	
	int i;
	
	for (i = 0; i < _palette.get_count(); ++i)
	{
		z = z * z + c;

		if (z.real() * z.real() + z.imag() * z.imag() > 10)
		{
			break;
		}
	}
	
	return i;
}

//////////////////////////////////////////////////////////////
void ggo_mandelbrot_artist::setup_palette()
{
	const int PALETTE_SIZE = 1000;
	
	float hue_start = ggo::rand_float();
	float hue_end = hue_start + ggo::rand_float(0.5, 1);
	
	ggo::cubic_curve_float sat_curve;
	sat_curve.push_point(0, 0);
	sat_curve.push_point(ggo::rand_float(0, 0.5), 1);
	sat_curve.push_point(1, 0);
	
	ggo::linear_curve_float val_curve;
	val_curve.push_point(0, ggo::rand_float(0, 0.5));
	val_curve.push_point(1, 1);
	
	for (int i = 0; i < _palette.get_count(); ++i)
	{
		float x = i / float(PALETTE_SIZE);
		
		float hue = std::fmod(ggo::map(x, 0.f, 1.f, hue_start, hue_end), 1.f);
		float sat = sat_curve.evaluate(x);
		float val = val_curve.evaluate(x);
		
		ggo::color color = ggo::color::from_hsv(hue, sat, val);
		
		_palette(i)._r = color.r8();
		_palette(i)._b = color.g8();
		_palette(i)._g = color.b8();
	}
}

//////////////////////////////////////////////////////////////
void ggo_mandelbrot_artist::setup_block()
{
	int depth = 0;
	
	_center = std::complex<double>(0, 0);
	_range = 2;
	while (true)
	{
		// Compute iteration deviation for each of the sub blocks.
		std::vector<ggo_block> blocks;

		for (int block_y = -GGO_BLOCK_COUNT; block_y <= GGO_BLOCK_COUNT; ++block_y)
		{
			for (int block_x = -GGO_BLOCK_COUNT; block_x <= GGO_BLOCK_COUNT; ++block_x)
			{
				ggo_block block;
				
				// The current sub block position.
				block._range = 0.5 * _range / GGO_BLOCK_COUNT;
				double real = block._center.real() + block_x * _range / GGO_BLOCK_COUNT;
				double imag = block._center.imag() + block_y * _range / GGO_BLOCK_COUNT;
				
				// Shake it a bit.
				block._range *= ggo::rand_float(0.9f, 1.1f);
				real += ggo::rand_double(-block._range / 10, block._range / 10);
				imag += ggo::rand_double(-block._range / 10, block._range / 10);

				block._center = std::complex<double>(real, imag);
				
				// Iterations sampling.
				std::vector<int> iterations;
				for (int y = -GGO_BLOCK_COUNT; y <= GGO_BLOCK_COUNT; ++y)
				{
					double y_f = block._center.imag() + y * block._range / GGO_BLOCK_COUNT;
					
					for (int x = -GGO_BLOCK_COUNT; x <= GGO_BLOCK_COUNT; ++x)
					{
						double x_f = block._center.real() + x * block._range / GGO_BLOCK_COUNT;
						
						iterations.push_back(iterate(x_f, y_f));
					}
				}
				
				// Compute mean and deviation.
				double mean = 0;
				for (std::vector<int>::const_iterator it = iterations.begin(); it != iterations.end(); ++it)
				{
					mean += *it;
				}
				mean /= iterations.size();

				block._deviation = 0;
				for (std::vector<int>::const_iterator it = iterations.begin(); it != iterations.end(); ++it)
				{
					double delta = mean - *it;
					block._deviation += delta * delta;
				}
				
				blocks.push_back(block);
			}
		}
		
		std::sort(blocks.begin(), blocks.end(), sort_func);

		_center = blocks.front()._center;
		_range = blocks.front()._range;

		++depth;
		if (depth == 4)
		{
			break;
		}
	}
}

