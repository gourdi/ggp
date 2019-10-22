#include "ggo_buddhabrot_artist.h"
#include <2d/fill/ggo_fill.h>
#include <algorithm>
#include <iostream>
#include <cstring>

//////////////////////////////////////////////////////////////
ggo::buddhabrot_artist::buddhabrot_artist(int width, int height, int line_step, ggo::pixel_type pixel_type, ggo::lines_order memory_lines_order)
:
bitmap_artist_abc(width, height, line_step, pixel_type, memory_lines_order)
{	

}

//////////////////////////////////////////////////////////////
void ggo::buddhabrot_artist::setup_seed_area(int escape_threshold_r, int escape_threshold_g, int escape_threshold_b,
  std::complex<double> & center, double & range)
{
	int counter_max = 0;
	for (int j = 0; j < 100; ++j)
	{
		std::complex<double> center_cur = std::complex<double>(ggo::rand<float>(-2, 2), ggo::rand<float>(-2, 2));
		double range_cur = ggo::rand<float>(0.2f, 0.3f);
		
		int counter = 0;
		for (int i = 0; i < 100; ++i)
		{
			double real = center_cur.real() + ggo::rand<double>(-range_cur, range_cur);
			double imag = center_cur.imag() + ggo::rand<double>(-range_cur, range_cur);
			std::complex<double> p(real, imag);
			
			if (discard_point(p) == false)
			{
				if (iterate(p, escape_threshold_r) == true)
				{
					++counter;
				}
			
				if (iterate(p, escape_threshold_g) == true)
				{
					++counter;
				}
			
				if (iterate(p, escape_threshold_b) == true)
				{
					++counter;
				}
			}
		}
		
		if (counter > counter_max)
		{
			center = center_cur;
			range = range_cur;
		}
	}
}

//////////////////////////////////////////////////////////////
bool ggo::buddhabrot_artist::discard_point(const std::complex<double> & p)
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
void ggo::buddhabrot_artist::process(int escape_threshold, ggo::array<int, 2> & accumulation,
  const std::complex<double> & center, double range) const
{
	std::complex<double> z(0, 0);

	while (true)
	{
		double real = center.real() + ggo::rand<double>(-range, range);
		double imag = center.imag() + ggo::rand<double>(-range, range);
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
			ggo::pos2_f point(static_cast<float>(complex.real() - center.real()), static_cast<float>(complex.imag() - center.imag()));
	
			point = map_fit(point, -1, 1);

			int x = ggo::round_to<int>(point.x());
			int y = ggo::round_to<int>(point.y());
			if ((x >= 0) && (x < width()) &&
		    	(y >= 0) && (y < height()))
			{
				accumulation(x, y) += 1;
			}
		}
	}
}

//////////////////////////////////////////////////////////////
void ggo::buddhabrot_artist::render_bitmap(void * buffer) const
{
	fill_black(ggo::image_t<ggo::pixel_type::rgb_8u, ggo::lines_order::up>(buffer, size(), line_byte_step()));

  ggo::array<int, 2> accumulation_r(width(), height());
  ggo::array<int, 2> accumulation_g(width(), height());
  ggo::array<int, 2> accumulation_b(width(), height());

  accumulation_r.fill(0);
	accumulation_g.fill(0);
	accumulation_b.fill(0);
    
  // We want the sum of the 3 escape thresholds to be constant.
	int escape_thresholds[3];
	escape_thresholds[0] = ggo::rand<int>(20, 10000);
	escape_thresholds[1] = ggo::rand<int>(20, 10000 - escape_thresholds[0]);
	escape_thresholds[2] = 10000 - escape_thresholds[1] - escape_thresholds[0];
	
	int indexes[3] = {0, 1, 2};
  std::shuffle(indexes, indexes + 3, std::default_random_engine());
	
	int escape_threshold_r = escape_thresholds[indexes[0]];
	int escape_threshold_g = escape_thresholds[indexes[1]];
	int escape_threshold_b = escape_thresholds[indexes[2]];
	
	// Accumulate.
  std::complex<double> center;
  double range;
	setup_seed_area(escape_threshold_r, escape_threshold_g, escape_threshold_b, center, range);
	
	int counter_max = width() * height() / 1000;	
	
	for (int counter = 0; counter < counter_max; ++counter)
	{	
		if (counter == counter_max / 2)
		{
			setup_seed_area(escape_threshold_r, escape_threshold_g, escape_threshold_b, center, range);
		}

		for (int i = 0; i < 12000; ++i)
		{
			process(escape_threshold_r, accumulation_r, center, range);
			process(escape_threshold_g, accumulation_g, center, range);
			process(escape_threshold_b, accumulation_b, center, range);	
		}
	}
	
	uint8_t * ptr = static_cast<uint8_t *>(buffer);
	for (int y = 0; y < height(); ++y)
	{
		for (int x = 0; x < height(); ++x)
		{
			*ptr++ = std::min(accumulation_r(x, y), 255);
			*ptr++ = std::min(accumulation_g(x, y), 255);
			*ptr++ = std::min(accumulation_b(x, y), 255);
		}
	}
}

//////////////////////////////////////////////////////////////
bool ggo::buddhabrot_artist::iterate(const std::complex<double> & p, int escape_threshold)
{
	std::vector<std::complex<double> > points;
	
	return iterate(p, points, escape_threshold);
}

//////////////////////////////////////////////////////////////
bool ggo::buddhabrot_artist::iterate(const std::complex<double> & p, std::vector<std::complex<double> > & points, int escape_threshold)
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
