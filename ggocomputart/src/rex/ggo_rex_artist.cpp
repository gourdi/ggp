#include "ggo_rex_artist.h"
#include <kernel/memory/ggo_array.h>
#include <kernel/math/delaunay_triangulation/ggo_delaunay_triangulation.h>
#include <2d/paint/ggo_paint.h>
#include <2d/paint/ggo_blend.h>
#include <map>

//////////////////////////////////////////////////////////////
ggo::rex_artist::rex_artist(int width, int height, int line_byte_step, ggo::pixel_type pixel_type, ggo::lines_order memory_lines_order)
:
bitmap_artist_abc(width, height, line_byte_step, pixel_type, memory_lines_order)
{
	
}

//////////////////////////////////////////////////////////////
void ggo::rex_artist::render_bitmap(void * buffer) const
{
  std::vector<ggo::segment_f>		      edges;
  ggo::scene2d<ggo::rgb_8u>       		color_triangles;
  std::vector<rex_pattern_triangle>	  pattern_triangles;
  std::vector<rex_disc_clip_triangle> discs_clip_triangles;
  std::vector<rex_opened_disc_data>   opened_discs;

  const int edges_vertices_count = 50;
  const int triangles_count = 200;

  std::vector<ggo::pos2_f> vertices;

  vertices.push_back(center());
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
    vertices.push_back(ggo::pos2_f(ggo::rand<float>(0.f, width() - 1.f), ggo::rand<float>(0.f, height() - 1.f)));
  }

  // Perform the Delaunay triangulation.
  auto triangles = ggo::delaunay_triangulation(vertices);

  // Assign a colour to each vertex.
  float hue = ggo::rand<float>();

  std::map<const ggo::pos2_f *, ggo::rgb_32f> color_map;
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
      edges.push_back(ggo::segment_f(*edge._first, *edge._second));
    }

    edge = rex_edge(triangle._v2, triangle._v3);
    if (std::find(built_edges.begin(), built_edges.end(), edge) == built_edges.end())
    {
      built_edges.push_back(edge);
      edges.push_back(ggo::segment_f(*edge._first, *edge._second));
    }

    edge = rex_edge(triangle._v3, triangle._v1);
    if (std::find(built_edges.begin(), built_edges.end(), edge) == built_edges.end())
    {
      built_edges.push_back(edge);
      edges.push_back(ggo::segment_f(*edge._first, *edge._second));
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

    color_triangles.make<color_triangle_rgb8u>(ggo::triangle2d_f(*triangle._v1, *triangle._v2, *triangle._v3), color1, color2, color3);
  }

  // Create circles.
  const float diagonal = ggo::length(ggo::pos2_f(float(width()), float(height()))) / 2;
  float outter_radius = diagonal;
  float inner_radius = outter_radius - diagonal * ggo::rand<float>(0.002f, 0.004f);
  while (inner_radius > 0)
  {
    rex_opened_disc_data disc;

    disc._center = center();
    disc._outter_radius = outter_radius;
    disc._inner_radius = inner_radius;

    opened_discs.push_back(disc);

    outter_radius = inner_radius - diagonal * ggo::rand<float>(0.004f, 0.008f);
    inner_radius = outter_radius - diagonal * ggo::rand<float>(0.002f, 0.004f);
  }

  if (pixel_type() == ggo::pixel_type::rgb_8u && memory_lines_order() == ggo::lines_order::up)
  {
    render_color_triangles<ggo::pixel_type::rgb_8u, ggo::lines_order::up>(buffer, color_triangles);
    render_patterns<ggo::pixel_type::rgb_8u, ggo::lines_order::up>(buffer, pattern_triangles);
    render_clipped_discs<ggo::pixel_type::rgb_8u, ggo::lines_order::up>(buffer, discs_clip_triangles, opened_discs);
    render_edges<ggo::pixel_type::rgb_8u, ggo::lines_order::up>(buffer, edges);
  }
  else if (pixel_type() == ggo::pixel_type::bgrx_8u && memory_lines_order() == ggo::lines_order::down)
  {
    render_color_triangles<ggo::pixel_type::bgrx_8u, ggo::lines_order::down>(buffer, color_triangles);
    render_patterns<ggo::pixel_type::bgrx_8u, ggo::lines_order::down>(buffer, pattern_triangles);
    render_clipped_discs<ggo::pixel_type::bgrx_8u, ggo::lines_order::down>(buffer, discs_clip_triangles, opened_discs);
    render_edges<ggo::pixel_type::bgrx_8u, ggo::lines_order::down>(buffer, edges);
  }
  else
  {
    GGO_FAIL();
  }
}

//////////////////////////////////////////////////////////////
template <ggo::pixel_type pixel_type, ggo::lines_order memory_lines_order>
void ggo::rex_artist::render_color_triangles(void * buffer, const scene2d<ggo::rgb_8u> & color_triangles) const
{
  ggo::image_t<pixel_type, memory_lines_order> img(buffer, size(), line_byte_step());

  ggo::paint<ggo::sampling_1>(img, color_triangles);
}

//////////////////////////////////////////////////////////////
template <ggo::pixel_type pixel_type, ggo::lines_order memory_lines_order>
void ggo::rex_artist::render_patterns(void * buffer, const std::vector<rex_pattern_triangle> & pattern_triangles) const
{
  ggo::image_t<pixel_type, memory_lines_order> img(buffer, size(), line_byte_step());

  for (const auto & pattern_triangle : pattern_triangles)
  {
    auto clip_triangle = std::make_shared<ggo::triangle2d_f>(pattern_triangle._v1, pattern_triangle._v2, pattern_triangle._v3);
    auto bounding_rect = clip_triangle->get_bounding_rect();

    const float delta = pattern_triangle._delta * min_size();
    const float radius = pattern_triangle._radius * min_size();
    for (float y = bounding_rect._pos.y(); y < bounding_rect._pos.y() + bounding_rect._height; y += delta)
    {
      for (float x = bounding_rect._pos.x(); x < bounding_rect._pos.x() + bounding_rect._width; x += delta)
      {
        ggo::multi_shape<float, ggo::boolean_mode::INTERSECTION> clipped_disc;
        clipped_disc.add_shape(std::make_shared<ggo::disc_f>(ggo::pos2_f(x, y), radius));
        clipped_disc.add_shape(clip_triangle);

        ggo::paint<ggo::sampling_8x8>(img, clipped_disc, ggo::black_8u());
      }
    }
  }
}

//////////////////////////////////////////////////////////////
template <ggo::pixel_type pixel_type, ggo::lines_order memory_lines_order>
void ggo::rex_artist::render_clipped_discs(void * buffer,
  const std::vector<rex_disc_clip_triangle> & discs_clip_triangles,
  const std::vector<rex_opened_disc_data> & opened_discs) const
{
  ggo::image_t<pixel_type, memory_lines_order> img(buffer, size(), line_byte_step());

  // Create the circles shape.
  auto circles = std::make_shared<ggo::multi_shape<float, ggo::boolean_mode::UNION>>();

  for (const auto & opened_disc : opened_discs)
  {
    auto circle = std::make_shared<ggo::multi_shape<float, ggo::boolean_mode::DIFFERENCE>>();
    circle->add_shape(std::make_shared<ggo::disc_f>(opened_disc._center, opened_disc._outter_radius));
    circle->add_shape(std::make_shared<ggo::disc_f>(opened_disc._center, opened_disc._inner_radius));

    circles->add_shape(circle);
  }

  // Paint each triangle.
  for (const auto & disc_clip_triangle : discs_clip_triangles)
  {
    ggo::multi_shape<float, ggo::boolean_mode::INTERSECTION> clip_triangle;

    clip_triangle.add_shape(std::make_shared<ggo::triangle2d_f>(disc_clip_triangle._v1, disc_clip_triangle._v2, disc_clip_triangle._v3));
    clip_triangle.add_shape(circles);

    ggo::paint<ggo::sampling_8x8>(img, clip_triangle, ggo::black_8u());
  }
}

//////////////////////////////////////////////////////////////
template <ggo::pixel_type pixel_type, ggo::lines_order memory_lines_order>
void ggo::rex_artist::render_edges(void * buffer, const std::vector<ggo::segment_f> & edges) const
{
  ggo::image_t<pixel_type, memory_lines_order> img(buffer, size(), line_byte_step());

  scene2d<ggo::rgb_8u> scene;

  for (const auto & edge : edges)
  {
    scene.make_paint_shape(ggo::capsule_f(edge.p1(), edge.p2(), 0.001f * min_size()), ggo::black_8u());
  }

  ggo::paint<ggo::sampling_8x8>(img, scene);
}
