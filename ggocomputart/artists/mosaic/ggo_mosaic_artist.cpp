#include "ggo_mosaic_artist.h"
#include <ggo_paint.h>
#include <iostream>
#include <cstring>

//////////////////////////////////////////////////////////////
ggo_mosaic_artist::ggo_mosaic_artist(int render_width, int render_height)
:
ggo_bitmap_artist_abc(render_width, render_height)
{
}
						
//////////////////////////////////////////////////////////////
void ggo_mosaic_artist::render_bitmap(uint8_t * buffer)
{
	memset(buffer, 0, 3 * get_render_width() * get_render_height());
	
	// Create the seeds.
	ggo::polygon2d_float	seed1;
	int points_count = ggo::rand_int(5, 9);
	for (int i = 0; i < points_count; ++i)
	{
		float radius  = ggo::rand_float(0.3f, 1);
		float angle   = i * 2 * ggo::PI<float>() / points_count + ggo::rand_float(-0.5f / points_count, 0.5f / points_count);

		seed1.add_point(radius * std::cos(angle), radius * std::sin(angle));
	}
	ggo::polygon2d_float	seed2;
	points_count = ggo::rand_int(5, 9);
	for (int i = 0; i < points_count; ++i)
	{
		float radius = ggo::rand_float(0.3f, 1);
		float angle = i * 2 * ggo::PI<float>() / points_count + ggo::rand_float(-0.5f / points_count, 0.5f / points_count);

		seed2.add_point(radius * std::cos(angle), radius * std::sin(angle));
	}
	
	// Create the seed positions blocks.
	std::vector<ggo_seed_positions> seed_positions;
	
	const int POINTS_IN_BLOCK = 20;
	for (int y = 0; y < get_render_height() / POINTS_IN_BLOCK; ++y)
	{
		for (int x = 0; x < get_render_width() / POINTS_IN_BLOCK; ++x)
		{
			ggo_seed_positions cur_seed_positions;
			cur_seed_positions._left = static_cast<float>(POINTS_IN_BLOCK * x);
			cur_seed_positions._right = static_cast<float>(POINTS_IN_BLOCK * (x + 1));
			cur_seed_positions._bottom = static_cast<float>(POINTS_IN_BLOCK * y);
			cur_seed_positions._top = static_cast<float>(POINTS_IN_BLOCK * (y + 1));
			
			for (int y2 = 0; y2 < POINTS_IN_BLOCK; ++y2)
			{
				for (int x2 = 0; x2 < POINTS_IN_BLOCK; ++x2)
				{
					cur_seed_positions._points.push_back(ggo::point2d_float(cur_seed_positions._left + x2, cur_seed_positions._bottom + y2));
				}
			}
			
			seed_positions.push_back(cur_seed_positions);
		}
	}
	
	// Create the polygons.
	std::vector<std::shared_ptr<ggo::polygon2d_float>> polygons;

	float hue_offset1 = ggo::rand_float();
	float hue_offset2 = ggo::rand_float();
	float saturation1 = ggo::rand_float(0.75, 1);
	float saturation2 = ggo::rand_float(0.75, 1);
	float angle_amplitude = ggo::rand_float(0, 2 * ggo::PI<float>());
	
	int failures_count = 0;
	float scale = ggo::rand_float(0.1f * get_render_min_size(), 0.2f * get_render_min_size());
	int failures_max = 1;
	int counter = 0;
	while (scale > 4)
	{
		// Create current shape from seed.
		const std::vector<ggo::point2d_float> & points = seed_positions[ggo::rand_int(0, static_cast<int>(seed_positions.size()) - 1)]._points;
		ggo::vector2d_float center = points[ggo::rand_int(0, static_cast<int>(points.size()) - 1)];
		center.x() += ggo::rand_float(-0.5f, 0.5f);
		center.y() += ggo::rand_float(-0.5f, 0.5f);
		
		float angle = ggo::rand_float(0, angle_amplitude);

		auto polygon = std::make_shared<ggo::polygon2d_float>();
		const ggo::polygon2d_float & seed = counter % 2 ? seed1 : seed2;
		for (int i = 0; i < seed.get_points_count(); ++i)
		{
			ggo::point2d_float point = seed.get_point(i);
			point.rotate(angle);
			point *= scale;
			point += center;

			polygon->add_point(point);
		}

		// Check for intersection with previous shapes.
		bool intersection = false;
		for (const auto & polygon2 : polygons)
		{
			if (polygon_intersection(*polygon, *polygon2) ==true)
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
				ggo::rect_float bounding_rect = polygon->get_bounding_rect();
			
				for (int i1 = static_cast<int>(seed_positions.size()) - 1; i1 >= 0; --i1)
				{
					if ((bounding_rect.left() < seed_positions[i1]._right) &&
					    (seed_positions[i1]._left < bounding_rect.right()) &&
					    (bounding_rect.bottom() < seed_positions[i1]._top) && 
					    (seed_positions[i1]._bottom < bounding_rect.top()))
					{
						std::vector<ggo::point2d_float> & points = seed_positions[i1]._points;
						for (int i2 = static_cast<int>(points.size()) - 1; i2 >= 0; --i2)
						{
							if (polygon->is_point_inside(points[i2].x(), points[i2].y()) == true)
							{
								points.erase(points.begin() + i2);
							}
						}
					
						if (points.empty() == true)
						{
							seed_positions.erase(seed_positions.begin() + i1);
						}
					}
				}
				
				float hue = (counter % 2 ? hue_offset1 : hue_offset2) + 0.5f * scale / get_render_min_size();
				float saturation = (counter % 2 ? saturation1 : saturation2);
				float value = 0.5f + 0.5f * scale / get_render_min_size();

				polygons.push_back(polygon);
				ggo::paint(buffer, get_render_width(), get_render_height(),
                   polygon,
                   ggo::color::from_hsv(hue, saturation, value));
				
				++counter;
			}
			
			std::cout << "S " << scale << ' ' << failures_count << ' ' << failures_max << ' ' << seed_positions.size() << std::endl;
			
			failures_count = 0;
			failures_max = ggo::to<int>(2000.f * get_render_min_size() / scale);
			scale *= 0.999f;
		}
		else
		{
			failures_count += 1;
		}
		
		if (failures_count > failures_max)
		{
			std::cout << "F " << scale << ' ' << failures_count << ' ' << failures_max << ' ' << seed_positions.size() << std::endl;
			
			failures_count = 0;
			failures_max = ggo::to<int>(2000.f * get_render_min_size() / scale);
			scale *= 0.995f;
		}
	}

	polygons.clear();
	
	// Render the light.
	float variance = ggo::rand_float(0.1f, 0.2f) * get_render_min_size() * get_render_min_size();
	for (int y = 0; y < get_render_height(); ++y)
	{
		float dy = static_cast<float>(y - get_render_height()) / 2;
		
		for (int x = 0; x < get_render_width(); ++x)
		{
			float dx = static_cast<float>(x - get_render_width()) / 2;
			int val = ggo::to<uint8_t>(92 * std::exp(-(dx * dx + dy * dy ) / variance));

			buffer[0] = std::min(255, buffer[0] + val);
			buffer[1] = std::min(255, buffer[1] + val);
			buffer[2] = std::min(255, buffer[2] + val);
			
			buffer += 3;
		}
	}
}

//////////////////////////////////////////////////////////////
bool ggo_mosaic_artist::polygon_intersection(const ggo::polygon2d_float & polygon1, const ggo::polygon2d_float & polygon2)
{
	// Fast exclusion tests.
	ggo::rect_float bounding_rect1 = polygon1.get_bounding_rect();
  ggo::rect_float bounding_rect2 = polygon2.get_bounding_rect();

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
		ggo::segment_float s1(polygon1.get_point(i1), polygon1.get_point((i1 + 1) % polygon1.get_points_count()));
		
		for (int i2 = 0; i2 < polygon2.get_points_count(); ++i2)
		{
			ggo::segment_float s2(polygon2.get_point(i2), polygon2.get_point((i2 + 1) % polygon2.get_points_count()));
			
			ggo::point2d_float intersection;
			if (s1.intersect_segment(s2, intersection) == true)
			{
				return true;
			}
		}
	}
	
	// Check if a polygon is inside another.
	bool all_points_inside = true;
	for (int i = 0; i < polygon1.get_points_count(); ++i)
	{
		const ggo::point2d_float & p1 = polygon1.get_point(i);
		if (polygon2.is_point_inside(p1.x(), p1.y()) == false)
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
		const ggo::point2d_float & p2 = polygon2.get_point(i);
		if (polygon1.is_point_inside(p2.x(), p2.y()) == false)
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


