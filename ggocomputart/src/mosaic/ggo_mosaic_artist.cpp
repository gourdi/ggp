#include "ggo_mosaic_artist.h"
#include <2d/paint/ggo_paint.h>
#include <2d/fill/ggo_fill.h>

namespace
{
  struct seed_positions
  {
    float _left, _right, _top, _bottom;
    std::vector<ggo::pos2_f> _points;
  };

  //////////////////////////////////////////////////////////////
  bool polygon_intersection(const ggo::polygon2d_f & polygon1, const ggo::polygon2d_f & polygon2)
  {
    // Fast exclusion tests.
    ggo::rect_f bounding_rect1 = polygon1.get_bounding_rect();
    ggo::rect_f bounding_rect2 = polygon2.get_bounding_rect();

    if ((bounding_rect1.left() > bounding_rect2.right()) ||
      (bounding_rect1.right() < bounding_rect2.left()) ||
      (bounding_rect1.bottom() > bounding_rect2.top()) ||
      (bounding_rect1.top() < bounding_rect2.bottom()))
    {
      return false;
    }

    // Check for edges intersection.
    for (int i1 = 0; i1 < polygon1.get_points_count(); ++i1)
    {
      ggo::segment_f s1(polygon1.get_point(i1), polygon1.get_point((i1 + 1) % polygon1.get_points_count()));

      for (int i2 = 0; i2 < polygon2.get_points_count(); ++i2)
      {
        ggo::segment_f s2(polygon2.get_point(i2), polygon2.get_point((i2 + 1) % polygon2.get_points_count()));

        auto intersection = s1.intersect(s2);
        if (intersection)
        {
          return true;
        }
      }
    }

    // Check if a polygon is inside another.
    bool all_points_inside = true;
    for (int i = 0; i < polygon1.get_points_count(); ++i)
    {
      const ggo::pos2_f & p1 = polygon1.get_point(i);
      if (polygon2.is_point_inside(p1) == false)
      {
        all_points_inside = false;
        break;
      }
    }
    if (all_points_inside == true)
    {
      return true;
    }

    all_points_inside = true;
    for (int i = 0; i < polygon2.get_points_count(); ++i)
    {
      const ggo::pos2_f & p2 = polygon2.get_point(i);
      if (polygon1.is_point_inside(p2) == false)
      {
        all_points_inside = false;
        break;
      }
    }
    if (all_points_inside == true)
    {
      return true;
    }

    // We know for sure there is no intersection.
    return false;
  }
}

//////////////////////////////////////////////////////////////
ggo::mosaic_artist::mosaic_artist(int width, int height, int line_byte_step, ggo::pixel_type pixel_type, ggo::lines_order memory_lines_order)
:
bitmap_artist_abc(width, height, line_byte_step, pixel_type, memory_lines_order)
{
}
						
//////////////////////////////////////////////////////////////
void ggo::mosaic_artist::render_bitmap(void * buffer) const
{
  image_t<ggo::pixel_type::rgb_8u, ggo::lines_order::up> img(buffer, size(), line_byte_step());

  ggo::fill_black(img, ggo::rect_int::from_width_height(width(), height()));
	
	// Create the seeds.
	ggo::polygon2d_f	seed1;
	int points_count = ggo::rand<int>(5, 9);
	for (int i = 0; i < points_count; ++i)
	{
		float radius  = ggo::rand<float>(0.3f, 1);
		float angle   = i * 2 * ggo::pi<float>() / points_count + ggo::rand<float>(-0.5f / points_count, 0.5f / points_count);

		seed1.add_point(radius * std::cos(angle), radius * std::sin(angle));
	}
	ggo::polygon2d_f	seed2;
	points_count = ggo::rand<int>(5, 9);
	for (int i = 0; i < points_count; ++i)
	{
		float radius = ggo::rand<float>(0.3f, 1);
		float angle = i * 2 * ggo::pi<float>() / points_count + ggo::rand<float>(-0.5f / points_count, 0.5f / points_count);

		seed2.add_point(radius * std::cos(angle), radius * std::sin(angle));
	}
	
	// Create the seed positions blocks.
	std::vector<seed_positions> seeds;
	
	const int points_in_block = 20;
	for (int y = 0; y < height() / points_in_block; ++y)
	{
		for (int x = 0; x < width() / points_in_block; ++x)
		{
			seed_positions cur_seed_positions;
			cur_seed_positions._left = static_cast<float>(points_in_block * x);
			cur_seed_positions._right = static_cast<float>(points_in_block * (x + 1));
			cur_seed_positions._bottom = static_cast<float>(points_in_block * y);
			cur_seed_positions._top = static_cast<float>(points_in_block * (y + 1));
			
			for (int y2 = 0; y2 < points_in_block; ++y2)
			{
				for (int x2 = 0; x2 < points_in_block; ++x2)
				{
					cur_seed_positions._points.push_back(ggo::pos2_f(cur_seed_positions._left + x2, cur_seed_positions._bottom + y2));
				}
			}
			
      seeds.push_back(cur_seed_positions);
		}
	}
	
	// Create the polygons.
	std::vector<ggo::polygon2d_f> polygons;

	float hue_offset1 = ggo::rand<float>();
	float hue_offset2 = ggo::rand<float>();
	float saturation1 = ggo::rand<float>(0.75, 1);
	float saturation2 = ggo::rand<float>(0.75, 1);
	float angle_amplitude = ggo::rand<float>(0, 2 * ggo::pi<float>());
	
	int failures_count = 0;
	float scale = ggo::rand<float>(0.1f * min_size(), 0.2f * min_size());
	int failures_max = 1;
	int counter = 0;
	while (scale > 4)
	{
		// Create current shape from seed.
		const std::vector<ggo::pos2_f> & points = seeds[ggo::rand<int>(0, static_cast<int>(seeds.size()) - 1)]._points;
		ggo::pos2_f center = points[ggo::rand<int>(0, static_cast<int>(points.size()) - 1)];
		center.x() += ggo::rand<float>(-0.5f, 0.5f);
		center.y() += ggo::rand<float>(-0.5f, 0.5f);
		
		float angle = ggo::rand<float>(0, angle_amplitude);

    ggo::polygon2d_f polygon;
		const ggo::polygon2d_f & seed = counter % 2 ? seed1 : seed2;
		for (int i = 0; i < seed.get_points_count(); ++i)
		{
			ggo::pos2_f point = seed.get_point(i);
			point = ggo::rotate(point, angle);
			point *= scale;
			point += center;

			polygon.add_point(point);
		}

		// Check for intersection with previous shapes.
		bool intersection = false;
		for (const auto & polygon2 : polygons)
		{
			if (polygon_intersection(polygon, polygon2) ==true)
			{
				intersection = true;
				break;
			}
		}
		
		// Paint the shape.
		if (intersection == false)
		{
			if (intersection == false)
			{
				// Remove dead seed positions.
				ggo::rect_f bounding_rect = polygon.get_bounding_rect();
			
				for (int i1 = static_cast<int>(seeds.size()) - 1; i1 >= 0; --i1)
				{
					if ((bounding_rect.left() < seeds[i1]._right) &&
					    (seeds[i1]._left < bounding_rect.right()) &&
					    (bounding_rect.bottom() < seeds[i1]._top) &&
					    (seeds[i1]._bottom < bounding_rect.top()))
					{
						std::vector<ggo::pos2_f> & points = seeds[i1]._points;
						for (int i2 = static_cast<int>(points.size()) - 1; i2 >= 0; --i2)
						{
							if (polygon.is_point_inside(points[i2]) == true)
							{
								points.erase(points.begin() + i2);
							}
						}
					
						if (points.empty() == true)
						{
              seeds.erase(seeds.begin() + i1);
						}
					}
				}
				
				float hue = (counter % 2 ? hue_offset1 : hue_offset2) + 0.5f * scale / min_size();
				float saturation = (counter % 2 ? saturation1 : saturation2);
				float value = 0.5f + 0.5f * scale / min_size();

				polygons.push_back(polygon);
				ggo::paint<ggo::sampling_4x4>(img, polygon, ggo::from_hsv<ggo::rgb_8u>(hue, saturation, value));
				
				++counter;
			}
			
			std::cout << "S " << scale << ' ' << failures_count << ' ' << failures_max << ' ' << seeds.size() << std::endl;
			
			failures_count = 0;
			failures_max = ggo::round_to<int>(2000.f * min_size() / scale);
			scale *= 0.999f;
		}
		else
		{
			failures_count += 1;
		}
		
		if (failures_count > failures_max)
		{
			std::cout << "F " << scale << ' ' << failures_count << ' ' << failures_max << ' ' << seeds.size() << std::endl;
			
			failures_count = 0;
			failures_max = ggo::round_to<int>(2000.f * min_size() / scale);
			scale *= 0.995f;
		}
	}

	polygons.clear();
	
	// Render the light.
	float variance = ggo::rand<float>(0.1f, 0.2f) * min_size() * min_size();
	for (int y = 0; y < height(); ++y)
	{
		float dy = static_cast<float>(y - height()) / 2;
		
		for (int x = 0; x < width(); ++x)
		{
			float dx = static_cast<float>(x - width()) / 2;
			int val = ggo::round_to<uint8_t>(92 * std::exp(-(dx * dx + dy * dy ) / variance));

      ggo::rgb_8u c_8u = img.read_pixel(x, y);

			c_8u.r() = uint8_t(std::min(255, c_8u.r() + val));
			c_8u.g() = uint8_t(std::min(255, c_8u.g() + val));
			c_8u.b() = uint8_t(std::min(255, c_8u.b() + val));
			
      img.write_pixel(x, y, c_8u);
    }
	}
}


