#include "ggo_cabrel_bitmap_artist.h"
#include <ggo_shape_sampling.h>
#include <ggo_multi_shape_paint.h>
#include <ggo_buffer_fill.h>
#include <ggo_gaussian_blur2d.h>

namespace
{
  //////////////////////////////////////////////////////////////
  bool is_edge_from_triangle(const std::tuple<int, int> & edge, const std::tuple<int, int, int> & triangle, int & v3)
  {
    if ((std::get<0>(edge) == std::get<0>(triangle) && std::get<1>(edge) == std::get<1>(triangle)) ||
        (std::get<0>(edge) == std::get<1>(triangle) && std::get<1>(edge) == std::get<0>(triangle)))
    {
      v3 = std::get<2>(triangle);
      return true;
    }

    if ((std::get<0>(edge) == std::get<1>(triangle) && std::get<1>(edge) == std::get<2>(triangle)) ||
        (std::get<0>(edge) == std::get<2>(triangle) && std::get<1>(edge) == std::get<1>(triangle)))
    {
      v3 = std::get<0>(triangle);
      return true;
    }

    if ((std::get<0>(edge) == std::get<0>(triangle) && std::get<1>(edge) == std::get<2>(triangle)) ||
        (std::get<0>(edge) == std::get<2>(triangle) && std::get<1>(edge) == std::get<0>(triangle)))
    {
      v3 = std::get<1>(triangle);
      return true;
    }

    return false;
  }

  //////////////////////////////////////////////////////////////
  bool triangle_intersect_other_triangles(const ggo::pos2f & new_vertex,
                                          const std::vector<ggo::pos2f> & points,
                                          const std::tuple<int, int> & edge,
                                          const std::vector<std::tuple<int, int, int>> & triangles)
  {
    // Check if the newly created triangle intersect other triangles.
    for (const auto & triangle_tuple : triangles)
    {
      // Handle special case when the current triangle holds the selected edge. 
      int v3 = -1;
      if (is_edge_from_triangle(edge, triangle_tuple, v3) == true)
      {
        ggo::vec2f edge_diff(points[std::get<1>(edge)] - points[std::get<0>(edge)]);
        ggo::vec2f diff1(points[v3] - points[std::get<0>(edge)]);
        ggo::vec2f diff2(new_vertex - points[std::get<0>(edge)]);

        if (ggo::ortho_dot(edge_diff, diff1) * ggo::ortho_dot(edge_diff, diff2) >= 0.f)
        {
          return true;
        }
      }
      else
      {
        ggo::triangle2d_float new_triangle(points[std::get<0>(edge)], points[std::get<1>(edge)], new_vertex);
        ggo::triangle2d_float cur_triangle(points[std::get<0>(triangle_tuple)], points[std::get<1>(triangle_tuple)], points[std::get<2>(triangle_tuple)]);

        if (ggo::get_triangle_intersection(new_triangle, cur_triangle) != ggo::triangle_intersection::disjoints)
        {
          return true;
        }
      }
    }

    return false;
  }

  //////////////////////////////////////////////////////////////
  float compute_score(const ggo::pos2f & v1, const ggo::pos2f & v2, const ggo::pos2f & v3, float reference_area)
  {
    // An area score of 0 implies that the candidate have the same area than the reference one.
    float area = ggo::triangle2d_float(v1, v2, v3).area();
    float area_score = area / reference_area;
    if (area_score < 1.f)
    {
      area_score = 1.f / area_score;
    }
    GGO_ASSERT(area_score >= 1.f);
    area_score -= 1.f; 

    // An angle score of 0 means that the candidate triangle is equilateral.
    const float angle1 = ggo::get_angle(v2 - v1, v3 - v1);
    const float angle2 = ggo::get_angle(v1 - v2, v3 - v2);
    const float angle3 = ggo::get_angle(v1 - v3, v2 - v3);

    float angle_score = std::abs(angle1 - ggo::pi<float>() / 3.f) + std::abs(angle2 - ggo::pi<float>() / 3.f) + std::abs(angle3 - ggo::pi<float>() / 3.f);

    return 10.f * area_score + angle_score;
  }

  //////////////////////////////////////////////////////////////
  std::vector<ggo::triangle2d_float> compute_triangles()
  {
    std::vector<ggo::pos2f> points;
    std::vector<std::tuple<int, int>> available_edges;
    std::vector<std::tuple<int, int, int>> triangles;

    points.emplace_back(ggo::rotate<float>({ 0.f, 1.f }, 0.f));
    points.emplace_back(ggo::rotate<float>({ 0.f, 1.f }, 2.f * ggo::pi<float>() / 3.f));
    points.emplace_back(ggo::rotate<float>({ 0.f, 1.f }, 4.f * ggo::pi<float>() / 3.f));

    available_edges.push_back(std::make_tuple(0, 1));
    available_edges.push_back(std::make_tuple(1, 2));
    available_edges.push_back(std::make_tuple(2, 0));

    float reference_area = ggo::triangle2d_float(points[0], points[1], points[2]).area();

    triangles.push_back(std::make_tuple(0, 1, 2));

    int triangles_count = ggo::rand<int>(192, 256);
    while (triangles.size() < triangles_count)
    {
      int index = ggo::rand<int>(0, static_cast<int>(available_edges.size()) - 1);
      auto edge = available_edges[index];

      // Find a triangle that does not intersect other triangles and which is the less flat as possible.
      float score = std::numeric_limits<float>::max();
      std::unique_ptr<ggo::triangle2d_float> triangle;
      for (int j = 0; j < 128; ++j)
      {
        ggo::pos2f edge_center = 0.5f * (points[std::get<0>(edge)] + points[std::get<1>(edge)]);
        float edge_length = ggo::distance(points[std::get<0>(edge)], points[std::get<1>(edge)]);

        ggo::pos2f new_vertex(ggo::disc_uniform_sampling(edge_center, 2.f * edge_length));

        // Check if the candidate triangle intersect other triangles.
        if (triangle_intersect_other_triangles(new_vertex, points, edge, triangles) == false)
        {
          float current_score = compute_score(new_vertex, points[std::get<0>(edge)], points[std::get<1>(edge)], reference_area);
          if (current_score < score)
          {
            score = current_score;
            triangle.reset(new ggo::triangle2d_float(new_vertex, points[std::get<0>(edge)], points[std::get<1>(edge)]));
          }
        }
      }

      if (triangle &&
          triangle->angle1() > ggo::pi<float>() / 6.0f &&
          triangle->angle2() > ggo::pi<float>() / 6.0f &&
          triangle->angle3() > ggo::pi<float>() / 6.0f)
      {
        // Remove selected edge from available edges list.
        available_edges.erase(available_edges.begin() + index);

        // Add point.
        points.push_back(triangle->v1());

        // Add edges.
        available_edges.push_back(std::make_tuple(static_cast<int>(points.size()) - 1, std::get<0>(edge)));
        available_edges.push_back(std::make_tuple(static_cast<int>(points.size()) - 1, std::get<1>(edge)));

        // Add triangle.
        triangles.push_back(std::make_tuple(std::get<0>(edge), std::get<1>(edge), static_cast<int>(points.size()) - 1));

        reference_area *= 0.99f;
      }

      // Randomly remove available edges.
      if (available_edges.size() > 10)
      {
        available_edges.erase(available_edges.begin() + ggo::rand<int>(0, available_edges.size() - 1));
      }
    }

    // Convert to ggo::triangle2f
    std::vector<ggo::triangle2d_float> result;
    for (const auto & triangle_tuple : triangles)
    {
      result.emplace_back(points[std::get<0>(triangle_tuple)], points[std::get<1>(triangle_tuple)], points[std::get<2>(triangle_tuple)]);
    }
    return result;
  }
}

//////////////////////////////////////////////////////////////
ggo::cabrel_bitmap_artist::cabrel_bitmap_artist(int render_width, int render_height)
:
bitmap_artist_abc(render_width, render_height)
{
}

//////////////////////////////////////////////////////////////
void ggo::cabrel_bitmap_artist::render_bitmap(void * buffer) const
{
  ggo::fill_solid<ggo::rgb_8u_yu>(buffer, get_render_width(), get_render_height(), 3 * get_render_width(), ggo::color_8u::white);

  auto triangles = compute_triangles();

  // Map triangles to the rendering dimensions.
  // Get triangles bounding box.
  ggo::rect_float bounding_box(0.f, 0.f, 0.f, 0.f);
  for (const auto & triangle : triangles)
  {
    bounding_box.extend(triangle.v1());
    bounding_box.extend(triangle.v2());
    bounding_box.extend(triangle.v3());
  }

  // Scale and move triangles.
  const float ratio = 0.75f * get_render_min_size() / (std::max(bounding_box.width(), bounding_box.height()));
  const ggo::vec2f offset(0.5f * get_render_width(), 0.5f * get_render_height());
  for (auto & triangle : triangles)
  {
    triangle.v1() -= bounding_box.center();
    triangle.v2() -= bounding_box.center();
    triangle.v3() -= bounding_box.center();

    triangle.v1() *= ratio;
    triangle.v2() *= ratio;
    triangle.v3() *= ratio;

    triangle.v1() += offset;
    triangle.v2() += offset;
    triangle.v3() += offset;
  }

  // Paint shadows.
  std::vector<solid_color_shape<triangle2d_float, color_8u>> shadows;
  const ggo::vec2f shadow_offset(0.01f * get_render_min_size(), -0.01f * get_render_min_size());
  for (auto triangle : triangles)
  {
    triangle2d_float shadow_triangle(triangle);
    shadow_triangle.v1() += shadow_offset;
    shadow_triangle.v2() += shadow_offset;
    shadow_triangle.v3() += shadow_offset;

    shadows.emplace_back(shadow_triangle, ggo::color_8u::black);
  }
  ggo::paint_shapes<rgb_8u_yu, sampling_4x4>(buffer, get_render_width(), get_render_height(), 3 * get_render_width(),
    shadows.begin(), shadows.end());

  float stddev = 0.01f * get_render_min_size();
  gaussian_blur2d<rgb_8u_yu>(buffer, get_render_width(), get_render_height(), 3 * get_render_width(), stddev);

  // Paint the triangles.
  std::vector<dyn_paint_shape<float, color_8u, color_8u>> shapes;

  auto black_brush = std::make_shared<solid_dyn_brush<color_8u>>(ggo::color_8u::black);
  auto blender = std::make_shared<overwrite_dyn_blender<color_8u, color_8u>>();

  for (const auto & triangle : triangles)
  {
    dyn_paint_shape<float, color_8u, color_8u> paint_triangle;
    paint_triangle._shape = std::make_shared<ggo::triangle2d_float>(triangle);
    paint_triangle._brush = std::make_shared<solid_dyn_brush<color_8u>>(color_8u(ggo::rand<uint8_t>(), ggo::rand<uint8_t>(), ggo::rand<uint8_t>()));
    paint_triangle._blender = blender;

    shapes.push_back(paint_triangle);

    auto create_segment = [&](const ggo::pos2f & p1, const ggo::pos2f & p2) {

      dyn_paint_shape<float, color_8u, color_8u> segment;
      segment._shape = std::make_shared<ggo::extended_segment_float>(p1, p2, 0.00025f * get_render_min_size());
      segment._brush = black_brush;
      segment._blender = blender;

      shapes.push_back(segment);
    };

    create_segment(triangle.v1(), triangle.v2());
    create_segment(triangle.v2(), triangle.v3());
    create_segment(triangle.v3(), triangle.v1());
  }

  ggo::paint_shapes<rgb_8u_yu, sampling_4x4>(buffer, get_render_width(), get_render_height(), 3 * get_render_width(),
    shapes.begin(), shapes.end());
}
