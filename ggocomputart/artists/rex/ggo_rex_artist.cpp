#include "ggo_rex_artist.h"
#include <ggo_delaunay_triangulation.h>
#include <ggo_link.h>
#include <ggo_array.h>
#include <ggo_paint.h>
#include <ggo_fill.h>
#include <ggo_layer.h>
#include <algorithm>
#include <map>

namespace
{
  const int GGO_EDGES_VERTICES_COUNT = 15;
  const int GGO_TRIANGLES_COUNT 		 = GGO_EDGES_VERTICES_COUNT * GGO_EDGES_VERTICES_COUNT;
  
  using ggo_edge = ggo::link<const ggo::pos2f *>;
}

//////////////////////////////////////////////////////////////
static float area_of_triangle(const ggo::vec2f & v1, const ggo::vec2f & v2, const ggo::vec2f & v3)
{
	ggo::vec2f u = v1 - v2;
	ggo::vec2f v = v1 - v3;
	
	return 0.5f * std::abs(u.get<0>() * v.get<1>() - u.get<1>() * v.get<0>());
}

//////////////////////////////////////////////////////////////
ggo_rex_artist::ggo_rex_artist(int render_width, int render_height)
:
ggo_bitmap_artist_abc(render_width, render_height)
{
	
}

//////////////////////////////////////////////////////////////
void ggo_rex_artist::clear()
{
	_edges.clear();
	_color_triangles.clear();
	_pattern_triangles.clear();
	_discs_clip_triangles.clear();
	_opened_discs.clear();
}

//////////////////////////////////////////////////////////////
void ggo_rex_artist::init()
{
	clear();

	std::vector<ggo::pos2f> vertices;
	
	std::cout << "Creating points" << std::endl;

	vertices.push_back(ggo::pos2f(0.5f, 0.5f));
	
	for (int i = 0; i < GGO_EDGES_VERTICES_COUNT; ++i)
	{
		vertices.push_back(ggo::pos2f(float(i + 1) / GGO_EDGES_VERTICES_COUNT, 0.f));
		vertices.push_back(ggo::pos2f(1.f, float(i + 1) / GGO_EDGES_VERTICES_COUNT));
		vertices.push_back(ggo::pos2f(float(GGO_EDGES_VERTICES_COUNT - i - 1) / GGO_EDGES_VERTICES_COUNT, 1.f));
		vertices.push_back(ggo::pos2f(0.f, float(GGO_EDGES_VERTICES_COUNT - i - 1) / GGO_EDGES_VERTICES_COUNT));
	}

	for (int i = 0; i < GGO_TRIANGLES_COUNT; ++i)
	{
		vertices.push_back(ggo::pos2f(ggo::rand_float(), ggo::rand_float()));
	}
	
	// Perform the Delaunay triangulation.
	std::cout << "Delaunay triangulation" << std::endl;
	
	auto triangles = ggo::delaunay_triangulation(vertices);
	
	// Assign a colour to each vertex.
	float hue = ggo::rand_float();
	
	std::map<const ggo::pos2f *, ggo::color> color_map;
	for (const auto & v : vertices)
	{
		color_map[&v] = ggo::color::from_hsv(hue, 1, ggo::rand_float());
	}

	// Fill the colour triangles and the edges.
	std::cout << "Storing data" << std::endl;

	std::vector<ggo_edge> edges;
	for (const auto & triangle : triangles)
	{
		ggo_edge edge(nullptr, nullptr);

		// The 3 edges of the current triangle. Store them only once.
		edge = ggo_edge(triangle._v1, triangle._v2);
		if (std::find(edges.begin(), edges.end(), edge) == edges.end())
		{
			edges.push_back(edge);
			_edges.push_back(ggo::segment_float(*edge._v1, *edge._v2));
		}
		
		edge = ggo_edge(triangle._v2, triangle._v3);
		if (std::find(edges.begin(), edges.end(), edge) == edges.end())
		{
			edges.push_back(edge);
			_edges.push_back(ggo::segment_float(*edge._v1, *edge._v2));
		}
		
		edge = ggo_edge(triangle._v3, triangle._v1);
		if (std::find(edges.begin(), edges.end(), edge) == edges.end())
		{
			edges.push_back(edge);
			_edges.push_back(ggo::segment_float(*edge._v1, *edge._v2));
		}
		
		GGO_ASSERT_BTW(triangle._v1->get<0>(), -0.0001f, 1.0001f);
		GGO_ASSERT_BTW(triangle._v1->get<1>(), -0.0001f, 1.0001f);
		GGO_ASSERT_BTW(triangle._v2->get<0>(), -0.0001f, 1.0001f);
		GGO_ASSERT_BTW(triangle._v2->get<1>(), -0.0001f, 1.0001f);
		GGO_ASSERT_BTW(triangle._v3->get<0>(), -0.0001f, 1.0001f);
		GGO_ASSERT_BTW(triangle._v3->get<1>(), -0.0001f, 1.0001f);
		
		// Save the triangles.
		ggo_color_triangle color_triangle;
		const ggo::pos2f * v1 = triangle._v1;
		const ggo::pos2f * v2 = triangle._v2;
		const ggo::pos2f * v3 = triangle._v3;

		color_triangle._v1 = *v1;
		color_triangle._v2 = *v2;
		color_triangle._v3 = *v3;
		
		color_triangle._color1 = color_map[v1];
		color_triangle._color2 = color_map[v2];
		color_triangle._color3 = color_map[v3];
		
		float random = ggo::rand_float();
		
		if (random < 0.1)
		{
			color_triangle._color1 = color_triangle._color2 = color_triangle._color3 = ggo::color::from_hsv(ggo::rand_float(), 1, 1);
		}
		else if (random < 0.4)
		{
			color_triangle._color1 = color_triangle._color2 = color_triangle._color3 = ggo::color::from_hsv(hue, 1, 1);
			
			ggo_pattern_triangle pattern_triangle;
		
			pattern_triangle._v1 = *v1;
			pattern_triangle._v2 = *v2;
			pattern_triangle._v3 = *v3;
			
			pattern_triangle._delta = ggo::rand_float(0.004f, 0.008f);
			pattern_triangle._radius = ggo::rand_float(0.2f, ggo::sqrt2<float>() / 2) * pattern_triangle._delta;
			
			_pattern_triangles.push_back(pattern_triangle);
		}
		else if (random < 0.5)
		{
			color_triangle._color1 = color_triangle._color2 = color_triangle._color3 = ggo::color::from_hsv(hue, 1, 1);
			
			ggo_disc_clip_triangle clip_triangle;
		
			clip_triangle._v1 = *v1;
			clip_triangle._v2 = *v2;
			clip_triangle._v3 = *v3;
			
			_discs_clip_triangles.push_back(clip_triangle);
		}
		
		_color_triangles.push_back(color_triangle);
	}
	
	// Create circles.
	float outter_radius = ggo::sqrt2<float>();
	float inner_radius = outter_radius - ggo::rand_float(0.004f, 0.008f);
	while (inner_radius > 0)
	{
		ggo_opened_disc_data disc;
		
		disc._center = ggo::pos2f(0.5f, 0.5f);
		disc._radius = (outter_radius + inner_radius) / 2;
		disc._width = outter_radius - inner_radius;
		
		_opened_discs.push_back(disc);
		
		outter_radius = inner_radius - ggo::rand_float(0.004f, 0.008f);
		inner_radius = outter_radius - ggo::rand_float(0.001f, 0.002f);
	}
}

//////////////////////////////////////////////////////////////
void ggo_rex_artist::render_bitmap(uint8_t * buffer)
{
	init();
	
	std::cout << "Rendering triangles" << std::endl;
	render_color_triangles(buffer);

	std::cout << "Rendering patterns" << std::endl;
	render_patterns(buffer);

	std::cout << "Rendering clipped circles" << std::endl;
	render_clipped_discs(buffer);

	std::cout << "Rendering edges" << std::endl;
	render_edges(buffer);
}

//////////////////////////////////////////////////////////////
void ggo_rex_artist::render_color_triangles(uint8_t * buffer) const
{
	for (const auto & color_triangle : _color_triangles)
	{
		ggo::pos2f v1 = map_fit(color_triangle._v1, 0, 1);
		ggo::pos2f v2 = map_fit(color_triangle._v2, 0, 1);
		ggo::pos2f v3 = map_fit(color_triangle._v3, 0, 1);
		
		int x_min = int(std::min(v1.get<0>(), std::min(v2.get<0>(), v3.get<0>())));
		int x_max = int(std::max(v1.get<0>(), std::max(v2.get<0>(), v3.get<0>()))) + 1;
		int y_min = int(std::min(v1.get<1>(), std::min(v2.get<1>(), v3.get<1>())));
		int y_max = int(std::max(v1.get<1>(), std::max(v2.get<1>(), v3.get<1>()))) + 1;
		
		x_min = ggo::clamp(x_min, 0, get_render_width());
		x_max = ggo::clamp(x_max, 0, get_render_width());
		y_min = ggo::clamp(y_min, 0, get_render_height());
		y_max = ggo::clamp(y_max, 0, get_render_height());

		ggo::polygon2d_float mapped_triangle;
		mapped_triangle.add_point(v1);
		mapped_triangle.add_point(v2);
		mapped_triangle.add_point(v3);
		
		for (int y = y_min; y < y_max; ++y)
		{
			for (int x = x_min; x < x_max; ++x)
			{
				ggo::pos2f pt(static_cast<float>(x), static_cast<float>(y));
			
				if (mapped_triangle.is_point_inside(pt.get<0>(), pt.get<1>()) == true)
				{
					float w1 = area_of_triangle(pt, v2, v3);
					float w2 = area_of_triangle(pt, v1, v3);
					float w3 = area_of_triangle(pt, v1, v2);

					ggo::color color(ggo::color::BLACK);
					color += w1 * (color_triangle._color1);
					color += w2 * (color_triangle._color2);
					color += w3 * (color_triangle._color3);
					color /= (w1 + w2 + w3);
					
					uint8_t * ptr = buffer + 3 * (y * get_render_width() + x);
					ptr[0] = color.r8();
					ptr[1] = color.g8();
					ptr[2] = color.b8();
				}
			}
		}
	}
}

//////////////////////////////////////////////////////////////
void ggo_rex_artist::render_patterns(uint8_t * buffer) const
{
	for (const auto & pattern_triangle : _pattern_triangles)
	{
		ggo::pos2f v1 = map_fit(pattern_triangle._v1, 0, 1);
		ggo::pos2f v2 = map_fit(pattern_triangle._v2, 0, 1);
		ggo::pos2f v3 = map_fit(pattern_triangle._v3, 0, 1);

		float min_x = std::min(v1.get<0>(), std::min(v2.get<0>(), v3.get<0>()));
		float min_y = std::min(v1.get<1>(), std::min(v2.get<1>(), v3.get<1>()));
		float max_x = std::max(v1.get<0>(), std::max(v2.get<0>(), v3.get<0>()));
		float max_y = std::max(v1.get<1>(), std::max(v2.get<1>(), v3.get<1>()));
		int size_x = int(max_x) - int(min_x) + 1;
		int size_y = int(max_y) - int(min_y) + 1;

		v1.get<0>() -= int(min_x);
		v1.get<1>() -= int(min_y);
		v2.get<0>() -= int(min_x);
		v2.get<1>() -= int(min_y);
		v3.get<0>() -= int(min_x);
		v3.get<1>() -= int(min_y);

		// Paint the mask.
		ggo::gray_image_buffer_uint8 mask_image(size_x, size_y);
    mask_image.fill(0);
    
		auto mapped_triangle = std::make_shared<ggo::polygon2d_float>();
		mapped_triangle->add_point(v1);
		mapped_triangle->add_point(v2);
		mapped_triangle->add_point(v3);
		ggo::paint(mask_image,
               mapped_triangle,
               1, 1, std::make_shared<ggo::gray_alpha_blender>(),
               ggo::pixel_sampler_1());

		// Paint the pattern.
		ggo::gray_image_buffer_uint8 pattern_image(size_x, size_y);
    mask_image.fill(0);
    
		float delta = pattern_triangle._delta * get_render_min_size();
		float radius = pattern_triangle._radius * get_render_min_size();
		for (float y = 0; y < size_y; y += delta)
		{
			for (float x = 0; x < size_x; x += delta)
			{
				ggo::paint(pattern_image,
                   std::make_shared<ggo::disc_float>(x, y, radius),
                   1, 1, std::make_shared<ggo::gray_alpha_blender>(),
                   ggo::pixel_sampler_16X16());
			}
		}

		// Merge pattern and mask in the render buffer.
		int pos_x = int(min_x);
		int pos_y = int(min_y);
		for (int y = 0; y < size_y; ++y)
		{
			int dst_y = pos_y + y;
			int v = 255 - (255 * dst_y / get_render_height());
			if ((dst_y >= 0) && (dst_y < get_render_height()))
			{
				for (int x = 0; x < size_x; ++x)
				{
					int dst_x = pos_x + x;
					if ((dst_x >= 0) && (dst_x < get_render_width()))
					{
						// Opacity.
						int opacity = (mask_image.data()[y * size_x + x] * pattern_image.data()[y * size_x + x] + 128) >> 8;
						int inv_opacity = 255 - opacity;

						// Paint pixel.
						uint8_t * it = buffer + 3 * (dst_y * get_render_width() + dst_x);
						it[0] = (inv_opacity * it[0] + 128) >> 8;
						it[1] = (inv_opacity * it[1] + 128) >> 8;
						it[2] = (inv_opacity * it[2] + 128) >> 8;			
					}
				}
			}
		}
	}
}

//////////////////////////////////////////////////////////////
void ggo_rex_artist::render_clipped_discs(uint8_t * buffer) const
{
  // Paint clipped circle triangles.
	ggo::gray_image_buffer_uint8 mask1_image(get_render_width(), get_render_height());
  mask1_image.fill(0);
  
	for (const auto & disc_clip_triangle : _discs_clip_triangles)
	{
		ggo::pos2f v1 = map_fit(disc_clip_triangle._v1, 0, 1);
		ggo::pos2f v2 = map_fit(disc_clip_triangle._v2, 0, 1);
		ggo::pos2f v3 = map_fit(disc_clip_triangle._v3, 0, 1);

		auto mapped_triangle = std::make_shared<ggo::polygon2d_float>();
		mapped_triangle->add_point(v1);
		mapped_triangle->add_point(v2);
		mapped_triangle->add_point(v3);
    
		ggo::paint(mask1_image, mapped_triangle, 1, 1, std::make_shared<ggo::gray_alpha_blender>(), ggo::pixel_sampler_1());
	}

	// Paint circles.
	ggo::gray_image_buffer_uint8 mask2_image(get_render_width(), get_render_height());
  mask2_image.fill(0);
  
	for (const auto & opened_disc : _opened_discs)
	{	
    ggo::pos2f center = map_fit(opened_disc._center, 0, 1);
    float radius = map_fit(opened_disc._radius, 0, 1);
    float width = map_fit(opened_disc._width, 0, 1);

    auto disc1 = std::make_shared<ggo::disc_float>(center, radius + 0.5f * width);
    auto disc2 = std::make_shared<ggo::disc_float>(center, radius - 0.5f * width);

    auto multi_shape = std::make_shared<ggo::multi_shape<float, ggo::boolean_mode::DIFFERENCE>>();
    multi_shape->add_shapes(disc1, disc2);

		ggo::paint(mask2_image, multi_shape, 1, 1, std::make_shared<ggo::gray_alpha_blender>(), ggo::pixel_sampler_16X16());
	}

	// Blend masks in the render buffer.
	uint8_t * it1 = mask1_image.data();
	uint8_t * it2 = mask2_image.data();
	for (int i = 0; i < get_render_height() * get_render_width(); ++i)
	{
		int opacity = ((*it1) * (*it2) + 128) >> 8;
		int inv_opacity = 255 - opacity;

		buffer[0] = (inv_opacity * buffer[0] + 128) >> 8;
		buffer[1] = (inv_opacity * buffer[1] + 128) >> 8;
		buffer[2] = (inv_opacity * buffer[2] + 128) >> 8;

		++it1;
		++it2;
		buffer += 3;
	}
}

//////////////////////////////////////////////////////////////
void ggo_rex_artist::render_edges(uint8_t * buffer) const
{
  auto multi_shape = std::make_shared<ggo::multi_shape_float>();
	
	for (const auto & edge : _edges)
	{
		ggo::pos2f v1 = map_fit(edge.p1(), 0, 1);
		ggo::pos2f v2 = map_fit(edge.p2(), 0, 1);
		
    auto segment = std::make_shared<ggo::extended_segment_float>(v1, v2, 0.001f * get_render_min_size());
    
    multi_shape->add_shape(segment);
	}
    
  ggo::paint(buffer, get_render_width(), get_render_height(),
             multi_shape,
             ggo::color::BLACK, 1, std::make_shared<ggo::rgb_alpha_blender>(),
             ggo::pixel_sampler_16X16());
}

