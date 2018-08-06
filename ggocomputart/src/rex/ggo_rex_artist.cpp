#include "ggo_rex_artist.h"
#include <kernel/ggo_link.h>
#include <kernel/memory/ggo_array.h>
#include <kernel/math/delaunay_triangulation/ggo_delaunay_triangulation.h>
#include <2d/paint/ggo_paint.h>
#include <2d/paint/ggo_color_triangle.h>
#include <2d/paint/ggo_blend.h>
#include <map>

namespace
{  
  using rex_edge = ggo::link<const ggo::pos2f *>;

  using color_triangle_rgb8u = ggo::solid_color_triangle<float, ggo::rgb_8u>;

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
    float       _outter_radius;
    float       _inner_radius;
  };

  //////////////////////////////////////////////////////////////
  template <ggo::image_format format>
  void render_color_triangles(void * buffer, const ggo::bitmap_artist_abc & artist, const std::vector<color_triangle_rgb8u> & color_triangles)
  {
    ggo::paint_shapes<format, ggo::sampling_1>(buffer, artist.width(), artist.height(), artist.line_step(), color_triangles);
  }

  //////////////////////////////////////////////////////////////
  template <ggo::image_format format>
  void render_patterns(void * buffer, const ggo::bitmap_artist_abc & artist, const std::vector<rex_pattern_triangle> & pattern_triangles)
  {
    for (const auto & pattern_triangle : pattern_triangles)
    {
      auto clip_triangle = std::make_shared<ggo::triangle2d_float>(pattern_triangle._v1, pattern_triangle._v2, pattern_triangle._v3);
      auto bounding_rect = clip_triangle->get_bounding_rect();

      const float delta = pattern_triangle._delta * artist.min_size();
      const float radius = pattern_triangle._radius * artist.min_size();
      for (float y = bounding_rect._pos.y(); y < bounding_rect._pos.y() + bounding_rect._height; y += delta)
      {
        for (float x = bounding_rect._pos.x(); x < bounding_rect._pos.x() + bounding_rect._width; x += delta)
        {
          ggo::multi_shape<float, ggo::boolean_mode::INTERSECTION> clipped_disc;
          clipped_disc.add_shape(std::make_shared<ggo::disc_float>(ggo::pos2f(x, y), radius));
          clipped_disc.add_shape(clip_triangle);

          ggo::paint<format, ggo::sampling_8x8>(buffer, artist.width(), artist.height(), artist.line_step(),
            clipped_disc, ggo::black_8u());
        }
      }
    }
  }

  //////////////////////////////////////////////////////////////
  template <ggo::image_format format>
  void render_clipped_discs(void * buffer, const ggo::bitmap_artist_abc & artist,
    const std::vector<rex_disc_clip_triangle> & discs_clip_triangles,
    const std::vector<rex_opened_disc_data> & opened_discs)
  {
    // Create the circles shape.
    auto circles = std::make_shared<ggo::multi_shape<float, ggo::boolean_mode::UNION>>();

    for (const auto & opened_disc : opened_discs)
    {
      auto circle = std::make_shared<ggo::multi_shape<float, ggo::boolean_mode::DIFFERENCE>>();
      circle->add_shape(std::make_shared<ggo::disc_float>(opened_disc._center, opened_disc._outter_radius));
      circle->add_shape(std::make_shared<ggo::disc_float>(opened_disc._center, opened_disc._inner_radius));

      circles->add_shape(circle);
    }

    // Paint each triangle.
    for (const auto & disc_clip_triangle : discs_clip_triangles)
    {
      ggo::multi_shape<float, ggo::boolean_mode::INTERSECTION> clip_triangle;

      clip_triangle.add_shape(std::make_shared<ggo::triangle2d_float>(disc_clip_triangle._v1, disc_clip_triangle._v2, disc_clip_triangle._v3));
      clip_triangle.add_shape(circles);

      ggo::paint<format, ggo::sampling_8x8>(buffer, artist.width(), artist.height(), artist.line_step(),
        clip_triangle, ggo::black_8u());
    }
  }

  //////////////////////////////////////////////////////////////
  template <ggo::image_format format>
  void render_edges(void * buffer, const ggo::bitmap_artist_abc & artist, const std::vector<ggo::segment_float> & edges)
  {
    std::vector<ggo::static_paint_shape<ggo::capsule_float, ggo::rgb_8u>> shapes;

    for (const auto & edge : edges)
    {
      ggo::capsule_float segment(edge.p1(), edge.p2(), 0.001f * artist.min_size());
      shapes.emplace_back(segment, ggo::black_8u());
    }

    ggo::paint_shapes<format, ggo::sampling_8x8>(buffer, artist.width(), artist.height(), artist.line_step(), shapes);
  }
}

//////////////////////////////////////////////////////////////
ggo::rex_artist::rex_artist(int width, int height, int line_step, ggo::image_format format)
:
bitmap_artist_abc(width, height, line_step, format)
{
	
}

//////////////////////////////////////////////////////////////
void ggo::rex_artist::render_bitmap(void * buffer) const
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
  vertices.push_back({ width() - 1.f, 0.f });
  vertices.push_back({ 0.f, height() - 1.f });
  vertices.push_back({ width() - 1.f, height() - 1.f });

  for (int i = 0; i < edges_vertices_count; ++i)
  {
    switch (ggo::rand<int>(0, 3))
    {
    case 0: // Left edge.
      vertices.push_back({ 0.f, ggo::rand<float>(0.f, height() - 1.f) });
      break;
    case 1: // Right edge.
      vertices.push_back({ width() - 1.f, ggo::rand<float>(0.f, height() - 1.f) });
      break;
    case 2: // Bottom edge.
      vertices.push_back({ ggo::rand<float>(0.f, width() - 1.f), 0.f });
      break;
    case 3: // Top edge.
      vertices.push_back({ ggo::rand<float>(0.f, width() - 1.f), height() - 1.f });
      break;
    }
  }

  for (int i = 0; i < triangles_count; ++i)
  {
    vertices.push_back(ggo::pos2f(ggo::rand<float>(0.f, width() - 1.f), ggo::rand<float>(0.f, height() - 1.f)));
  }

  // Perform the Delaunay triangulation.
  auto triangles = ggo::delaunay_triangulation(vertices);

  // Assign a colour to each vertex.
  float hue = ggo::rand<float>();

  std::map<const ggo::pos2f *, ggo::rgb_32f> color_map;
  for (const auto & v : vertices)
  {
    color_map[&v] = ggo::from_hsv<ggo::rgb_32f>(hue, 1, ggo::rand<float>());
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
    ggo::rgb_32f color1 = color_map[triangle._v1];
    ggo::rgb_32f color2 = color_map[triangle._v2];
    ggo::rgb_32f color3 = color_map[triangle._v3];

    float random = ggo::rand<float>();

    if (random < 0.1)
    {
      color1 = color2 = color3 = ggo::from_hsv<ggo::rgb_32f>(ggo::rand<float>(), 1, 1);
    }
    else if (random < 0.4)
    {
      color1 = color2 = color3 = ggo::from_hsv<ggo::rgb_32f>(hue, 1, 1);

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
      color1 = color2 = color3 = ggo::from_hsv<ggo::rgb_32f>(hue, 1, 1);

      rex_disc_clip_triangle clip_triangle;

      clip_triangle._v1 = *triangle._v1;
      clip_triangle._v2 = *triangle._v2;
      clip_triangle._v3 = *triangle._v3;

      discs_clip_triangles.push_back(clip_triangle);
    }

    color_triangles.emplace_back(
      ggo::triangle2d_float(*triangle._v1, *triangle._v2, *triangle._v3),
      color1, color2, color3);
  }

  // Create circles.
  const float diagonal = ggo::distance(float(width()), float(height())) / 2;
  float outter_radius = diagonal;
  float inner_radius = outter_radius - diagonal * ggo::rand<float>(0.002f, 0.004f);
  while (inner_radius > 0)
  {
    rex_opened_disc_data disc;

    disc._center = get_center();
    disc._outter_radius = outter_radius;
    disc._inner_radius = inner_radius;

    opened_discs.push_back(disc);

    outter_radius = inner_radius - diagonal * ggo::rand<float>(0.004f, 0.008f);
    inner_radius = outter_radius - diagonal * ggo::rand<float>(0.002f, 0.004f);
  }

  switch (format())
  {
  case ggo::rgb_8u_yu:
    render_color_triangles<ggo::rgb_8u_yu>(buffer, *this, color_triangles);
    render_patterns<ggo::rgb_8u_yu>(buffer, *this, pattern_triangles);
    render_clipped_discs<ggo::rgb_8u_yu>(buffer, *this, discs_clip_triangles, opened_discs);
    render_edges<ggo::rgb_8u_yu>(buffer, *this, edges);
    break;
  case ggo::bgrx_8u_yd:
    render_color_triangles<ggo::bgrx_8u_yd>(buffer, *this, color_triangles);
    render_patterns<ggo::bgrx_8u_yd>(buffer, *this, pattern_triangles);
    render_clipped_discs<ggo::bgrx_8u_yd>(buffer, *this, discs_clip_triangles, opened_discs);
    render_edges<ggo::bgrx_8u_yd>(buffer, *this, edges);
    break;
  default:
    GGO_FAIL();
    break;
  }
}
