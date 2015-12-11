#include "ggo_filling_squares_artist.h"
#include <ggo_color.h>

//////////////////////////////////////////////////////////////
void ggo_filling_squares_artist::build_squares(int render_width, int render_height, float & hue, std::vector<ggo_multi_square> & result)
{
	constexpr int COUNT = 1250;
	
	int render_min_size = std::min(render_width, render_height);
	float range = render_min_size * 0.075f;
	
	hue = ggo::rand_float();
	
	ggo::point2d_float center(static_cast<float>(render_width) / 2.f, static_cast<float>(render_height) / 2.f);
	std::vector<ggo::point2d_float> centers;
	centers.push_back(center);
	
	for (int i = 0; i < COUNT; ++i)
	{
		ggo_multi_square multi_square;
	
		// Center.
		int index = ggo::rand_int(0, static_cast<int>(centers.size()) - 1);
		multi_square._pos = centers[index];

		ggo::point2d_float new_center(centers[index]);
		new_center.x() += ggo::rand_float(-range, range);
		new_center.y() += ggo::rand_float(-range, range);
		
		if (new_center.x() >= 0 && new_center.x() < render_width &&
			  new_center.y() >= 0 && new_center.y() < render_height)
		{
			centers.push_back(new_center);

			if (centers.size() > 25)
			{
				centers.erase(centers.begin() + ggo::rand_int(0, static_cast<int>(centers.size()) - 1));
			}
		}
		
		// The size of the cube.
		float delta = render_min_size * ggo::rand_float(0.005f, 0.008f);

		float size = std::pow(ggo::rand_float(), 10.f);
		size = render_min_size * (0.6f + 0.5f * size) / 10;

		// Create squares.
		ggo_colored_square colored_square;
		colored_square._size = delta;
		do
		{
			colored_square._color = ggo::color::from_hsv(hue, ggo::rand_float(), ggo::rand_float());

			// Store from largest to smallest square.
			multi_square._squares.insert(multi_square._squares.begin(), colored_square);

			colored_square._size += delta;				

		} while (colored_square._size <= size);

		result.push_back(multi_square);
	}
}

