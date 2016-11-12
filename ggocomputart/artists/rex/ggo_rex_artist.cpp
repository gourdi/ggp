#include "ggo_rex_artist.h"
#include <ggo_delaunay_triangulation.h>
#include <ggo_link.h>
#include <ggo_array.h>
#include <ggo_buffer_paint.h>
#include <map>

namespace
{  
  using rex_edge = ggo::link<const ggo::pos2f *>;

  struct rex_color_triangle
  {
    ggo::pos2f      _v1, _v2, _v3;
    ggo::color_32f  _color1, _color2, _color3;
  };

  struct rex_pattern_triangle
  {
    ggo::pos2f	_v1, _v2, _v3;
    float				_delta;
    float				_radius;
  };

  struct rex_disc_clip_triangle
  {
    ggo::pos2f	_v1, _v2, _v3;
  };

  struct rex_opened_disc_data
  {
    ggo::pos2f  _center;
    float       _radius;
    float       _width;
  };

  //////////////////////////////////////////////////////////////
  float area_of_triangle(const ggo::vec2f & v1, const ggo::vec2f & v2, const ggo::vec2f & v3)
  {
    ggo::vec2f u = v1 - v2;
    ggo::vec2f v = v1 - v3;

    return 0.5f * std::abs(u.x() * v.y() - u.y() * v.x());
  }

  //////////////////////////////////////////////////////////////
  void render_color_triangles(void * buffer, const ggo::artist & artist, const std::vector<rex_color_triangle> & color_triangles)
  {
    for (const auto & color_triangle : color_triangles)
    {
      ggo::pos2f v1 = artist.map_fit(color_triangle._v1, 0, 1);
      ggo::pos2f v2 = artist.map_fit(color_triangle._v2, 0, 1);
      ggo::pos2f v3 = artist.map_fit(color_triangle._v3, 0, 1);

      int x_min = int(std::min(v1.x(), std::min(v2.x(), v3.x())));
      int x_max = int(std::max(v1.x(), std::max(v2.x(), v3.x()))) + 1;
      int y_min = int(std::min(v1.y(), std::min(v2.y(), v3.y())));
      int y_max = int(std::max(v1.y(), std::max(v2.y(), v3.y()))) + 1;

      x_min = ggo::clamp(x_min, 0, artist.get_render_width());
      x_max = ggo::clamp(x_max, 0, artist.get_render_width());
      y_min = ggo::clamp(y_min, 0, artist.get_render_height());
      y_max = ggo::clamp(y_max, 0, artist.get_render_height());

      ggo::polygon2d_float mapped_triangle;
      mapped_triangle.add_point(v1);
      mapped_triangle.add_point(v2);
      mapped_triangle.add_point(v3);

      for (int y = y_min; y < y_max; ++y)
      {
        for (int x = x_min; x < x_max; ++x)
        {
          ggo::pos2f pt(static_cast<float>(x), static_cast<float>(y));

          if (mapped_triangle.is_point_inside(pt.x(), pt.y()) == true)
          {
            float w1 = area_of_triangle(pt, v2, v3);
            float w2 = area_of_triangle(pt, v1, v3);
            float w3 = area_of_triangle(pt, v1, v2);

            ggo::color_32f c_32f(ggo::black<ggo::color_32f>());
            c_32f += w1 * (color_triangle._color1);
            c_32f += w2 * (color_triangle._color2);
            c_32f += w3 * (color_triangle._color3);
            c_32f /= (w1 + w2 + w3);

            ggo::color_8u c_8u = ggo::convert_color_to<ggo::color_8u>(c_32f);

            uint8_t * ptr = static_cast<uint8_t *>(buffer) + 3 * (y * artist.get_render_width() + x);
            ptr[0] = c_8u.r();
            ptr[1] = c_8u.g();
            ptr[2] = c_8u.b();
          }
        }
      }
    }
  }

  //////////////////////////////////////////////////////////////
  void render_patterns(void * buffer, const ggo::artist & artist, const std::vector<rex_pattern_triangle> & pattern_triangles)
  {
    for (const auto & pattern_triangle : pattern_triangles)
    {
      ggo::pos2f v1 = artist.map_fit(pattern_triangle._v1, 0, 1);
      ggo::pos2f v2 = artist.map_fit(pattern_triangle._v2, 0, 1);
      ggo::pos2f v3 = artist.map_fit(pattern_triangle._v3, 0, 1);

      float min_x = std::min(v1.x(), std::min(v2.x(), v3.x()));
      float min_y = std::min(v1.y(), std::min(v2.y(), v3.y()));
      float max_x = std::max(v1.x(), std::max(v2.x(), v3.x()));
      float max_y = std::max(v1.y(), std::max(v2.y(), v3.y()));
      int size_x = int(max_x) - int(min_x) + 1;
      int size_y = int(max_y) - int(min_y) + 1;

      v1.x() -= int(min_x);
      v1.y() -= int(min_y);
      v2.x() -= int(min_x);
      v2.y() -= int(min_y);
      v3.x() -= int(min_x);
      v3.y() -= int(min_y);

      // Paint the mask.
      std::vector<uint8_t> mask_buffer(size_x * size_y, 0);

      ggo::polygon2d_float mapped_triangle;
      mapped_triangle.add_point(v1);
      mapped_triangle.add_point(v2);
      mapped_triangle.add_point(v3);
      ggo::paint_shape<ggo::y_8u_yu, ggo::sampling_1>(
        mask_buffer.data(), size_x, size_y, size_x, mapped_triangle, uint8_t(1));

      // Paint the pattern.
      std::vector<uint8_t> pattern_buffer(size_x * size_y, 0);

      float delta = pattern_triangle._delta * artist.get_render_min_size();
      float radius = pattern_triangle._radius * artist.get_render_min_size();
      for (float y = 0; y < size_y; y += delta)
      {
        for (float x = 0; x < size_x; x += delta)
        {
          ggo::paint_shape<ggo::y_8u_yu, ggo::sampling_16x16>(
            pattern_buffer.data(), size_x, size_y, size_x,
            ggo::disc_float(x, y, radius), uint8_t(1));
        }
      }

      // Merge pattern and mask in the render buffer.
      int pos_x = int(min_x);
      int pos_y = int(min_y);
      for (int y = 0; y < size_y; ++y)
      {
        int dst_y = pos_y + y;
        int v = 255 - (255 * dst_y / artist.get_render_height());
        if ((dst_y >= 0) && (dst_y < artist.get_render_height()))
        {
          for (int x = 0; x < size_x; ++x)
          {
            int dst_x = pos_x + x;
            if ((dst_x >= 0) && (dst_x < artist.get_render_width()))
            {
              // Opacity.
              int opacity = (mask_buffer.data()[y * size_x + x] * pattern_buffer.data()[y * size_x + x] + 128) >> 8;
              int inv_opacity = 255 - opacity;

              // Paint pixel.
              uint8_t * it = static_cast<uint8_t *>(buffer) + 3 * (dst_y * artist.get_render_width() + dst_x);
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
  void render_clipped_discs(void * buffer, const ggo::artist & artist,
    const std::vector<rex_disc_clip_triangle> & discs_clip_triangles,
    const std::vector<rex_opened_disc_data> & opened_discs)
  {
    // Paint clipped circle triangles.
    std::vector<uint8_t> mask1_buffer(artist.get_render_width() * artist.get_render_height(), 0);

    for (const auto & disc_clip_triangle : discs_clip_triangles)
    {
      ggo::pos2f v1 = artist.map_fit(disc_clip_triangle._v1, 0, 1);
      ggo::pos2f v2 = artist.map_fit(disc_clip_triangle._v2, 0, 1);
      ggo::pos2f v3 = artist.map_fit(disc_clip_triangle._v3, 0, 1);

      ggo::polygon2d_float mapped_triangle;
      mapped_triangle.add_point(v1);
      mapped_triangle.add_point(v2);
      mapped_triangle.add_point(v3);

      ggo::paint_shape<ggo::y_8u_yu, ggo::sampling_1>(
        mask1_buffer.data(), artist.get_render_width(), artist.get_render_height(), artist.get_render_width(),
        mapped_triangle, uint8_t(1));
    }

    // Paint circles.
    std::vector<uint8_t> mask2_buffer(artist.get_render_width() * artist.get_render_height(), 0);

    for (const auto & opened_disc : opened_discs)
    {
      ggo::pos2f center = artist.map_fit(opened_disc._center, 0, 1);
      float radius = artist.map_fit(opened_disc._radius, 0, 1);
      float width = artist.map_fit(opened_disc._width, 0, 1);

      auto disc1 = std::make_shared<ggo::disc_float>(center, radius + 0.5f * width);
      auto disc2 = std::make_shared<ggo::disc_float>(center, radius - 0.5f * width);

      ggo::multi_shape<float, ggo::boolean_mode::DIFFERENCE> multi_shape;
      multi_shape.add_shapes(disc1, disc2);

      ggo::paint_shape<ggo::y_8u_yu, ggo::sampling_16x16>(
        mask2_buffer.data(), artist.get_render_width(), artist.get_render_height(), artist.get_render_width(),
        multi_shape, uint8_t(1));
    }

    // Blend masks in the render buffer.
    uint8_t * ptr = static_cast<uint8_t *>(buffer);
    const uint8_t * ptr1 = mask1_buffer.data();
    const uint8_t * ptr2 = mask2_buffer.data();
    for (int i = 0; i < artist.get_render_height() * artist.get_render_width(); ++i)
    {
      int opacity = ((*ptr1) * (*ptr2) + 128) >> 8;
      int inv_opacity = 255 - opacity;

      ptr[0] = (inv_opacity * ptr[0] + 128) >> 8;
      ptr[1] = (inv_opacity * ptr[1] + 128) >> 8;
      ptr[2] = (inv_opacity * ptr[2] + 128) >> 8;

      ++ptr1;
      ++ptr2;
      ptr += 3;
    }
  }

  //////////////////////////////////////////////////////////////
  void render_edges(void * buffer, const ggo::artist & artist, const std::vector<ggo::segment_float> & edges)
  {
    ggo::multi_shape_float multi_shape;

    for (const auto & edge : edges)
    {
      ggo::pos2f v1 = artist.map_fit(edge.p1(), 0, 1);
      ggo::pos2f v2 = artist.map_fit(edge.p2(), 0, 1);

      auto segment = std::make_shared<ggo::extended_segment_float>(v1, v2, 0.001f * artist.get_render_min_size());

      multi_shape.add_shape(segment);
    }

    ggo::paint_shape<ggo::rgb_8u_yu, ggo::sampling_16x16>(
      buffer, artist.get_render_width(), artist.get_render_height(), 3 * artist.get_render_width(),
      multi_shape, ggo::black<ggo::color_8u>());
  }
}

//////////////////////////////////////////////////////////////
ggo::rex_artist::rex_artist(int render_width, int render_height)
:
bitmap_artist_abc(render_width, render_height)
{
	
}

//////////////////////////////////////////////////////////////
void ggo::rex_artist::render_bitmap(void * buffer) const
{
  std::vector<ggo::segment_float>		  edges;
  std::vector<rex_color_triangle>		  color_triangles;
  std::vector<rex_pattern_triangle>	  pattern_triangles;
  std::vector<rex_disc_clip_triangle> discs_clip_triangles;
  std::vector<rex_opened_disc_data>   opened_discs;

  const int edges_vertices_count = 15;
  const int triangles_count = edges_vertices_count * edges_vertices_count;

  std::vector<ggo::pos2f> vertices;

  std::cout << "Creating points" << std::endl;

  vertices.push_back(ggo::pos2f(0.5f, 0.5f));

  for (int i = 0; i < edges_vertices_count; ++i)
  {
    vertices.push_back(ggo::pos2f(float(i + 1) / edges_vertices_count, 0.f));
    vertices.push_back(ggo::pos2f(1.f, float(i + 1) / edges_vertices_count));
    vertices.push_back(ggo::pos2f(float(edges_vertices_count - i - 1) / edges_vertices_count, 1.f));
    vertices.push_back(ggo::pos2f(0.f, float(edges_vertices_count - i - 1) / edges_vertices_count));
  }

  for (int i = 0; i < triangles_count; ++i)
  {
    vertices.push_back(ggo::pos2f(ggo::rand<float>(), ggo::rand<float>()));
  }

  // Perform the Delaunay triangulation.
  std::cout << "Delaunay triangulation" << std::endl;

  auto triangles = ggo::delaunay_triangulation(vertices);

  // Assign a colour to each vertex.
  float hue = ggo::rand<float>();

  std::map<const ggo::pos2f *, ggo::color_32f> color_map;
  for (const auto & v : vertices)
  {
    color_map[&v] = ggo::from_hsv<ggo::color_32f>(hue, 1, ggo::rand<float>());
  }

  // Fill the colour triangles and the edges.
  std::cout << "Storing data" << std::endl;

  std::vector<rex_edge> built_edges;
  for (const auto & triangle : triangles)
  {
    rex_edge edge(nullptr, nullptr);

    // The 3 edges of the current triangle. Store them only once.
    edge = rex_edge(triangle._v1, triangle._v2);
    if (std::find(built_edges.begin(), built_edges.end(), edge) == built_edges.end())
    {
      built_edges.push_back(edge);
      edges.push_back(ggo::segment_float(*edge._v1, *edge._v2));
    }

    edge = rex_edge(triangle._v2, triangle._v3);
    if (std::find(built_edges.begin(), built_edges.end(), edge) == built_edges.end())
    {
      built_edges.push_back(edge);
      edges.push_back(ggo::segment_float(*edge._v1, *edge._v2));
    }

    edge = rex_edge(triangle._v3, triangle._v1);
    if (std::find(built_edges.begin(), built_edges.end(), edge) == built_edges.end())
    {
      built_edges.push_back(edge);
      edges.push_back(ggo::segment_float(*edge._v1, *edge._v2));
    }

    GGO_ASSERT_BTW(triangle._v1->x(), -0.0001f, 1.0001f);
    GGO_ASSERT_BTW(triangle._v1->y(), -0.0001f, 1.0001f);
    GGO_ASSERT_BTW(triangle._v2->x(), -0.0001f, 1.0001f);
    GGO_ASSERT_BTW(triangle._v2->y(), -0.0001f, 1.0001f);
    GGO_ASSERT_BTW(triangle._v3->x(), -0.0001f, 1.0001f);
    GGO_ASSERT_BTW(triangle._v3->y(), -0.0001f, 1.0001f);

    // Save the triangles.
    rex_color_triangle color_triangle;
    const ggo::pos2f * v1 = triangle._v1;
    const ggo::pos2f * v2 = triangle._v2;
    const ggo::pos2f * v3 = triangle._v3;

    color_triangle._v1 = *v1;
    color_triangle._v2 = *v2;
    color_triangle._v3 = *v3;

    color_triangle._color1 = color_map[v1];
    color_triangle._color2 = color_map[v2];
    color_triangle._color3 = color_map[v3];

    float random = ggo::rand<float>();

    if (random < 0.1)
    {
      color_triangle._color1 = color_triangle._color2 = color_triangle._color3 = ggo::from_hsv<ggo::color_32f>(ggo::rand<float>(), 1, 1);
    }
    else if (random < 0.4)
    {
      color_triangle._color1 = color_triangle._color2 = color_triangle._color3 = ggo::from_hsv<ggo::color_32f>(hue, 1, 1);

      rex_pattern_triangle pattern_triangle;

      pattern_triangle._v1 = *v1;
      pattern_triangle._v2 = *v2;
      pattern_triangle._v3 = *v3;

      pattern_triangle._delta = ggo::rand<float>(0.004f, 0.008f);
      pattern_triangle._radius = ggo::rand<float>(0.2f, ggo::sqrt2<float>() / 2) * pattern_triangle._delta;

      pattern_triangles.push_back(pattern_triangle);
    }
    else if (random < 0.5)
    {
      color_triangle._color1 = color_triangle._color2 = color_triangle._color3 = ggo::from_hsv<ggo::color_32f>(hue, 1, 1);

      rex_disc_clip_triangle clip_triangle;

      clip_triangle._v1 = *v1;
      clip_triangle._v2 = *v2;
      clip_triangle._v3 = *v3;

      discs_clip_triangles.push_back(clip_triangle);
    }

    color_triangles.push_back(color_triangle);
  }

  // Create circles.
  float outter_radius = ggo::sqrt2<float>();
  float inner_radius = outter_radius - ggo::rand<float>(0.004f, 0.008f);
  while (inner_radius > 0)
  {
    rex_opened_disc_data disc;

    disc._center = ggo::pos2f(0.5f, 0.5f);
    disc._radius = (outter_radius + inner_radius) / 2;
    disc._width = outter_radius - inner_radius;

    opened_discs.push_back(disc);

    outter_radius = inner_radius - ggo::rand<float>(0.004f, 0.008f);
    inner_radius = outter_radius - ggo::rand<float>(0.001f, 0.002f);
  }

	std::cout << "Rendering triangles" << std::endl;
	render_color_triangles(buffer, *this, color_triangles);

	std::cout << "Rendering patterns" << std::endl;
	render_patterns(buffer, *this, pattern_triangles);

	std::cout << "Rendering clipped circles" << std::endl;
	render_clipped_discs(buffer, *this, discs_clip_triangles, opened_discs);

	std::cout << "Rendering edges" << std::endl;
	render_edges(buffer, *this, edges);
}
