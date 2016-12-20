#include "ggo_rex_artist.h"
#include <ggo_delaunay_triangulation.h>
#include <ggo_link.h>
#include <ggo_array.h>
#include <ggo_buffer_paint.h>
#include <ggo_multi_shape_paint.h>
#include <ggo_color_triangle.h>
#include <ggo_blend.h>
#include <map>

namespace
{  
  using rex_edge = ggo::link<const ggo::pos2f *>;

  using opaque_blender_rgb8u = ggo::overwrite_blender<ggo::color_8u>;
  using color_triangle_rgb8u = ggo::color_triangle<opaque_blender_rgb8u, ggo::color_8u>;

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
  template <ggo::pixel_buffer_format pbf>
  void render_color_triangles(void * buffer, const ggo::bitmap_artist_abc & artist, const std::vector<color_triangle_rgb8u> & color_triangles)
  {
    static_assert(std::is_same<ggo::pixel_buffer_format_info<pbf>::color_t, ggo::color_8u>::value, "wrong pixel buffer type");

    ggo::paint_shapes<pbf, ggo::sampling_1>(buffer, artist.get_width(), artist.get_height(), artist.get_line_step(),
      color_triangles.begin(), color_triangles.end(), ggo::pixel_rect::from_width_height(artist.get_width(), artist.get_height()));
  }

  //////////////////////////////////////////////////////////////
  void render_patterns(void * buffer, const ggo::bitmap_artist_abc & artist, const std::vector<rex_pattern_triangle> & pattern_triangles)
  {
    for (const auto & pattern_triangle : pattern_triangles)
    {
      auto clip_triangle = std::make_shared<ggo::triangle2d_float>(pattern_triangle._v1, pattern_triangle._v2, pattern_triangle._v3);
      auto bounding_rect = clip_triangle->get_bounding_rect();

      const float delta = pattern_triangle._delta * artist.get_min_size();
      const float radius = pattern_triangle._radius * artist.get_min_size();
      for (float y = bounding_rect._pos.y(); y < bounding_rect._pos.y() + bounding_rect._height; y += delta)
      {
        for (float x = bounding_rect._pos.x(); x < bounding_rect._pos.x() + bounding_rect._width; x += delta)
        {
          ggo::multi_shape<float, ggo::boolean_mode::INTERSECTION> clipped_disc;
          clipped_disc.add_shape(std::make_shared<ggo::disc_float>(x, y, radius));
          clipped_disc.add_shape(clip_triangle);

          ggo::paint_shape<ggo::rgb_8u_yu, ggo::sampling_8x8>(
            buffer, artist.get_width(), artist.get_height(), artist.get_line_step(),
            clipped_disc, ggo::black_8u());
        }
      }
    }
  }

  //////////////////////////////////////////////////////////////
  void render_clipped_discs(void * buffer, const ggo::bitmap_artist_abc & artist,
    const std::vector<rex_disc_clip_triangle> & discs_clip_triangles,
    const std::vector<rex_opened_disc_data> & opened_discs)
  {
    // Create the circles shape.
    auto circles = std::make_shared<ggo::multi_shape<float, ggo::boolean_mode::UNION>>();

    for (const auto & opened_disc : opened_discs)
    {
      ggo::pos2f center = artist.map_fit(opened_disc._center, 0, 1);
      float radius = artist.map_fit(opened_disc._radius, 0, 1);
      float width = artist.map_fit(opened_disc._width, 0, 1);

      auto circle = std::make_shared<ggo::multi_shape<float, ggo::boolean_mode::DIFFERENCE>>();
      circle->add_shape(std::make_shared<ggo::disc_float>(center, radius + 0.5f * width));
      circle->add_shape(std::make_shared<ggo::disc_float>(center, radius - 0.5f * width));

      circles->add_shape(circle);
    }

    // Paint each triangle.
    for (const auto & disc_clip_triangle : discs_clip_triangles)
    {
      ggo::multi_shape<float, ggo::boolean_mode::INTERSECTION> clip_triangle;

      clip_triangle.add_shape(std::make_shared<ggo::triangle2d_float>(disc_clip_triangle._v1, disc_clip_triangle._v2, disc_clip_triangle._v3));
      clip_triangle.add_shape(circles);

      ggo::paint_shape<ggo::rgb_8u_yu, ggo::sampling_8x8>(
        buffer, artist.get_width(), artist.get_height(), artist.get_line_step(),
        clip_triangle, ggo::black_8u());
    }
  }

  //////////////////////////////////////////////////////////////
  void render_edges(void * buffer, const ggo::bitmap_artist_abc & artist, const std::vector<ggo::segment_float> & edges)
  {
    std::vector<ggo::solid_color_shape<ggo::extended_segment_float, ggo::color_8u>> shapes;

    for (const auto & edge : edges)
    {
      ggo::extended_segment_float segment(edge.p1(), edge.p2(), 0.001f * artist.get_min_size());
      shapes.emplace_back(segment, ggo::black_8u());
    }

    ggo::paint_shapes<ggo::rgb_8u_yu, ggo::sampling_8x8>(buffer, artist.get_width(), artist.get_height(), artist.get_line_step(),
      shapes.begin(), shapes.end(), ggo::pixel_rect::from_width_height(artist.get_width(), artist.get_height()));
  }
}

//////////////////////////////////////////////////////////////
ggo::rex_artist::rex_artist(int width, int height, int line_step, ggo::pixel_buffer_format pbf)
:
bitmap_artist_abc(width, height, line_step, pbf)
{
	
}

//////////////////////////////////////////////////////////////
void ggo::rex_artist::render_bitmap(void * buffer, const bool & qui) const
{
  std::vector<ggo::segment_float>		  edges;
  std::vector<color_triangle_rgb8u>		color_triangles;
  std::vector<rex_pattern_triangle>	  pattern_triangles;
  std::vector<rex_disc_clip_triangle> discs_clip_triangles;
  std::vector<rex_opened_disc_data>   opened_discs;

  const int edges_vertices_count = 50;
  const int triangles_count = 200;

  std::vector<ggo::pos2f> vertices;

  vertices.push_back(get_center());
  vertices.push_back({ 0.f, 0.f });
  vertices.push_back({ get_width() - 1.f, 0.f });
  vertices.push_back({ 0.f, get_height() - 1.f });
  vertices.push_back({ get_width() - 1.f, get_height() - 1.f });

  for (int i = 0; i < edges_vertices_count; ++i)
  {
    switch (ggo::rand<int>(0, 3))
    {
    case 0: // Left edge.
      vertices.push_back({ 0.f, ggo::rand<float>(0.f, get_height() - 1.f) });
      break;
    case 1: // Right edge.
      vertices.push_back({ get_width() - 1.f, ggo::rand<float>(0.f, get_height() - 1.f) });
      break;
    case 2: // Bottom edge.
      vertices.push_back({ ggo::rand<float>(0.f, get_width() - 1.f), 0.f });
      break;
    case 3: // Top edge.
      vertices.push_back({ ggo::rand<float>(0.f, get_width() - 1.f), get_height() - 1.f });
      break;
    }
  }

  for (int i = 0; i < triangles_count; ++i)
  {
    vertices.push_back(ggo::pos2f(ggo::rand<float>(0.f, get_width() - 1.f), ggo::rand<float>(0.f, get_height() - 1.f)));
  }

  // Perform the Delaunay triangulation.
  auto triangles = ggo::delaunay_triangulation(vertices);

  // Assign a colour to each vertex.
  float hue = ggo::rand<float>();

  std::map<const ggo::pos2f *, ggo::color_32f> color_map;
  for (const auto & v : vertices)
  {
    color_map[&v] = ggo::from_hsv<ggo::color_32f>(hue, 1, ggo::rand<float>());
  }

  // Fill the colour triangles and the edges.
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

    // Save the triangles.
    ggo::color_32f color1 = color_map[triangle._v1];
    ggo::color_32f color2 = color_map[triangle._v2];
    ggo::color_32f color3 = color_map[triangle._v3];

    float random = ggo::rand<float>();

    if (random < 0.1)
    {
      color1 = color2 = color3 = ggo::from_hsv<ggo::color_32f>(ggo::rand<float>(), 1, 1);
    }
    else if (random < 0.4)
    {
      color1 = color2 = color3 = ggo::from_hsv<ggo::color_32f>(hue, 1, 1);

      rex_pattern_triangle pattern_triangle;

      pattern_triangle._v1 = *triangle._v1;
      pattern_triangle._v2 = *triangle._v2;
      pattern_triangle._v3 = *triangle._v3;

      pattern_triangle._delta = ggo::rand<float>(0.004f, 0.008f);
      pattern_triangle._radius = ggo::rand<float>(0.2f, ggo::sqrt2<float>() / 2) * pattern_triangle._delta;

      pattern_triangles.push_back(pattern_triangle);
    }
    else if (random < 0.5)
    {
      color1 = color2 = color3 = ggo::from_hsv<ggo::color_32f>(hue, 1, 1);

      rex_disc_clip_triangle clip_triangle;

      clip_triangle._v1 = *triangle._v1;
      clip_triangle._v2 = *triangle._v2;
      clip_triangle._v3 = *triangle._v3;

      discs_clip_triangles.push_back(clip_triangle);
    }

    ggo::triangle2d_float triangle(*triangle._v1, *triangle._v2, *triangle._v3);
    color_triangles.emplace_back(triangle, color1, color2, color3, opaque_blender_rgb8u());
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

  switch (get_pixel_buffer_format())
  {
  case ggo::rgb_8u_yu:
    render_color_triangles<ggo::rgb_8u_yu>(buffer, *this, color_triangles);
    render_patterns(buffer, *this, pattern_triangles);
    render_clipped_discs(buffer, *this, discs_clip_triangles, opened_discs);
    render_edges(buffer, *this, edges);
    break;
  case ggo::bgra_8u_yd:
    render_color_triangles<bgra_8u_yd>(buffer, *this, color_triangles);
    render_patterns(buffer, *this, pattern_triangles);
    render_clipped_discs(buffer, *this, discs_clip_triangles, opened_discs);
    render_edges(buffer, *this, edges);
    break;
  }
  
}
