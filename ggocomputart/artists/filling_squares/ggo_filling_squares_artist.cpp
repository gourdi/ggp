#include "ggo_filling_squares_artist.h"

//////////////////////////////////////////////////////////////
std::vector<ggo::filling_squares_artist::multi_square> ggo::filling_squares_artist::build_squares(int width, int height, float & hue)
{
	constexpr int count = 1250;

  std::vector<ggo::filling_squares_artist::multi_square> result;
	
	int min_size = std::min(width, height);
	float range = min_size * 0.075f;
	
	hue = ggo::rand<float>();
	
	ggo::pos2f center(static_cast<float>(width) / 2.f, static_cast<float>(height) / 2.f);
	std::vector<ggo::pos2f> centers;
	centers.push_back(center);
	
	for (int i = 0; i < count; ++i)
	{
		multi_square multi_square;
	
		// Center.
		int index = ggo::rand<int>(0, static_cast<int>(centers.size()) - 1);
		multi_square._pos = centers[index];

		ggo::pos2f new_center(centers[index]);
		new_center.get<0>() += ggo::rand<float>(-range, range);
		new_center.get<1>() += ggo::rand<float>(-range, range);
		
		if (new_center.get<0>() >= 0 && new_center.get<0>() < width &&
			  new_center.get<1>() >= 0 && new_center.get<1>() < height)
		{
			centers.push_back(new_center);

			if (centers.size() > 25)
			{
				centers.erase(centers.begin() + ggo::rand<int>(0, static_cast<int>(centers.size()) - 1));
			}
		}
		
		// The size of the cube.
		float delta = min_size * ggo::rand<float>(0.005f, 0.008f);

		float size = std::pow(ggo::rand<float>(), 10.f);
		size = min_size * (0.6f + 0.5f * size) / 10;

		// Create squares.
		colored_square colored_square;
		colored_square._size = delta;
		do
		{
			colored_square._color = ggo::from_hsv<ggo::color_8u>(hue, ggo::rand<float>(), ggo::rand<float>());

			// Store from largest to smallest square.
			multi_square._squares.insert(multi_square._squares.begin(), colored_square);

			colored_square._size += delta;				

		} while (colored_square._size <= size);

		result.push_back(multi_square);
	}

  return result;
}

