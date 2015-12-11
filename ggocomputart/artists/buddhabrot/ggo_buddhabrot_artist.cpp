#include "ggo_buddhabrot_artist.h"
#include <ggo_color.h>
#include <ggo_paint.h>
#include <algorithm>
#include <iostream>
#include <cstring>

//////////////////////////////////////////////////////////////
ggo_buddhabrot_artist::ggo_buddhabrot_artist(int render_width, int render_height)
:
ggo_bitmap_artist_abc(render_width, render_height),
_accumulation_r(render_width, render_height),
_accumulation_g(render_width, render_height),
_accumulation_b(render_width, render_height)
{	

}

//////////////////////////////////////////////////////////////
void ggo_buddhabrot_artist::setup_seed_area()
{
	int counter_max = 0;
	for (int j = 0; j < 100; ++j)
	{
		std::complex<double> center = std::complex<double>(ggo::rand_float(-2, 2), ggo::rand_float(-2, 2));
		double range = ggo::rand_float(0.2f, 0.3f);
		
		int counter = 0;
		for (int i = 0; i < 100; ++i)
		{
			double real = center.real() + ggo::rand_double(-range, range);
			double imag = center.imag() + ggo::rand_double(-range, range);
			std::complex<double> p(real, imag);
			
			if (discard_point(p) == false)
			{
				if (iterate(p, _escape_threshold_r) == true)
				{
					++counter;
				}
			
				if (iterate(p, _escape_threshold_g) == true)
				{
					++counter;
				}
			
				if (iterate(p, _escape_threshold_b) == true)
				{
					++counter;
				}
			}
		}
		
		if (counter > counter_max)
		{
			_center = center;
			_range = range;
		}
	}
}

//////////////////////////////////////////////////////////////
bool ggo_buddhabrot_artist::discard_point(const std::complex<double> & p)
{
	double x = p.real();
	double y = p.real();
	
	// The main cardioid.
	double q = (x - 0.25);
	q = q * q + y * y;
	if (q * (q + (x - 0.25)) < 0.25 * y * y)
	{
		return true;
	}
	
	// First circle.
	q = x + 1;
	if (q * q + y * y < 0.0625)
	{
		return true;
	}
	
	return false;
}

//////////////////////////////////////////////////////////////
void ggo_buddhabrot_artist::process(int escape_threshold, ggo::array2d<int> & accumulation) const
{
	std::complex<double> z(0, 0);

	while (true)
	{
		double real = _center.real() + ggo::rand_double(-_range, _range);
		double imag = _center.imag() + ggo::rand_double(-_range, _range);
		z = std::complex<double>(real, imag);
		
		if (discard_point(z) == false)
		{
			break;
		}
	}

	std::vector<std::complex<double> > points;
	points.reserve(escape_threshold);

	if (iterate(z, points, escape_threshold) == true)
	{	
		for (const auto & complex : points)
		{
			ggo::point2d_float point(static_cast<float>(complex.real() - _center.real()), static_cast<float>(complex.imag() - _center.imag()));
	
			point = map_fit(point, -1, 1);

			int x = ggo::to<int>(point.x());
			int y = ggo::to<int>(point.y());
			if ((x >= 0) && (x < get_render_width()) &&
		    	(y >= 0) && (y < get_render_height()))
			{
				accumulation(x, y) += 1;
			}
		}
	}
}

//////////////////////////////////////////////////////////////
void ggo_buddhabrot_artist::render_bitmap(uint8_t * buffer)
{
	memset(buffer, 0, 3 * get_render_width() * get_render_height());
  _accumulation_r.fill(0);
	_accumulation_g.fill(0);
	_accumulation_b.fill(0);
    
    // We want the sum of the 3 escape thresholds to be constant.
	int escape_thresholds[3];
	escape_thresholds[0] = ggo::rand_int(20, 10000);
	escape_thresholds[1] = ggo::rand_int(20, 10000 - escape_thresholds[0]);
	escape_thresholds[2] = 10000 - escape_thresholds[1] - escape_thresholds[0];
	
	int indexes[3] = {0, 1, 2};
	std::random_shuffle(indexes, indexes + 3);
	
	_escape_threshold_r = escape_thresholds[indexes[0]];
	_escape_threshold_g = escape_thresholds[indexes[1]];
	_escape_threshold_b = escape_thresholds[indexes[2]];
	
	// Accumulate.
	setup_seed_area();
	
	int counter_max = get_render_width() * get_render_height() / 1000;	
	
	for (int counter = 0; counter < counter_max; ++counter)
	{	
		if (counter == counter_max / 2)
		{
			setup_seed_area();
		}

		for (int i = 0; i < 12000; ++i)
		{
			process(_escape_threshold_r, _accumulation_r);
			process(_escape_threshold_g, _accumulation_g);
			process(_escape_threshold_b, _accumulation_b);	
		}
	}
	
	uint8_t * it = buffer;
	for (int y = 0; y < get_render_height(); ++y)
	{
		for (int x = 0; x < get_render_height(); ++x)
		{
			*it++ = std::min(_accumulation_r(x, y), 255);
			*it++ = std::min(_accumulation_g(x, y), 255);
			*it++ = std::min(_accumulation_b(x, y), 255);
		}
	}
}

//////////////////////////////////////////////////////////////
bool ggo_buddhabrot_artist::iterate(const std::complex<double> & p, int escape_threshold)
{
	std::vector<std::complex<double> > points;
	
	return iterate(p, points, escape_threshold);
}

//////////////////////////////////////////////////////////////
bool ggo_buddhabrot_artist::iterate(const std::complex<double> & p, std::vector<std::complex<double> > & points, int escape_threshold)
{
	points.clear();

	std::complex<double> z(p), c(z);

	for (int i = 0; i < escape_threshold; ++i)
	{
		points.push_back(z);

		z = z * z + c;	

		if (z.real() * z.real() + z.imag() * z.imag() > 4)
		{
			// Output will look better if we paint only points that don't escape too fast.
			return points.size() > escape_threshold / 5;
		}
	}
	
	return false;
}
