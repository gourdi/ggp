#include "ggo_cabrel_bitmap_artist.h"
#include <kernel/math/sampling/shape_sampling/ggo_shape_sampling.h>
#include <2d/fill/ggo_fill.h>
#include <2d/paint/ggo_paint.h>
#include <2d/paint/ggo_color_triangle.h>
#include <2d/blur/ggo_gaussian_blur.h>

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
    const float angle1 = ggo::angle(v2 - v1, v3 - v1);
    const float angle2 = ggo::angle(v1 - v2, v3 - v2);
    const float angle3 = ggo::angle(v1 - v3, v2 - v3);

    float angle_score = std::abs(angle1 - ggo::pi<float>() / 3.f) + std::abs(angle2 - ggo::pi<float>() / 3.f) + std::abs(angle3 - ggo::pi<float>() / 3.f);

    return 10.f * area_score + angle_score;
  }

  //////////////////////////////////////////////////////////////
  std::vector<ggo::triangle2d_float> compute_triangles()
  {
    std::vector<ggo::pos2f> points;
    std::vector<std::tuple<int, int>> available_edges;
    std::vector<std::tuple<int, int, int>> triangles;

    points.emplace_back(ggo::rotate<ggo::pos2f>({ 0.f, 1.f }, 0.f));
    points.emplace_back(ggo::rotate<ggo::pos2f>({ 0.f, 1.f }, 2.f * ggo::pi<float>() / 3.f));
    points.emplace_back(ggo::rotate<ggo::pos2f>({ 0.f, 1.f }, 4.f * ggo::pi<float>() / 3.f));

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
        available_edges.erase(available_edges.begin() + ggo::rand<size_t>(0, available_edges.size() - 1));
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

  //////////////////////////////////////////////////////////////
  template <ggo::image_format format>
  void render_bitmap_t(void * buffer, const ggo::cabrel_bitmap_artist & artist)
  {
    ggo::fill_solid<format>(buffer, artist.width(), artist.height(), artist.line_step(),
      ggo::white_8u(),
      ggo::rect_int::from_width_height(artist.width(), artist.height()));

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
    const float ratio = 0.75f * artist.min_size() / (std::max(bounding_box.width(), bounding_box.height()));
    const ggo::vec2f offset(0.5f * artist.width(), 0.5f * artist.height());
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
    std::vector<ggo::static_paint_shape<ggo::triangle2d_float, ggo::rgb_8u>> shadows;
    const ggo::vec2f shadow_offset(0.01f * artist.min_size(), -0.01f * artist.min_size());
    for (auto triangle : triangles)
    {
      ggo::triangle2d_float shadow_triangle(triangle);
      shadow_triangle.v1() += shadow_offset;
      shadow_triangle.v2() += shadow_offset;
      shadow_triangle.v3() += shadow_offset;

      shadows.emplace_back(shadow_triangle, ggo::black_8u());
    }
    ggo::paint_shapes<format, ggo::sampling_4x4>(buffer, artist.width(), artist.height(), artist.line_step(), shadows);

    float stddev = 0.01f * artist.min_size();
    ggo::gaussian_blur2d<format>(buffer, artist.line_step(), artist.size(), stddev);

    // Paint the triangles.
    using paint_shape_t = ggo::paint_shape_abc<float, ggo::rgb_8u, ggo::rgb_8u>;
    std::vector<std::unique_ptr<paint_shape_t>> shapes;

    float border_size = 0.00025f * artist.min_size();

    for (const auto & triangle : triangles)
    {
      using paint_triangle_t = ggo::static_paint_shape<ggo::triangle2d_float, ggo::rgb_8u>;
      auto paint_triangle = std::make_unique<paint_triangle_t>(triangle, ggo::rgb_8u(ggo::rand<uint8_t>(), ggo::rand<uint8_t>(), ggo::rand<uint8_t>()));
      shapes.push_back(std::move(paint_triangle));

      auto create_segment = [&](const ggo::pos2f & p1, const ggo::pos2f & p2)
      {
        using paint_extended_segment_t = ggo::static_paint_shape<ggo::capsule_float, ggo::rgb_8u>;
        auto paint_extented_segment = std::make_unique<paint_extended_segment_t>(ggo::capsule_float(p1, p2, border_size), ggo::black_8u());
        shapes.push_back(std::move(paint_extented_segment));
      };

      create_segment(triangle.v1(), triangle.v2());
      create_segment(triangle.v2(), triangle.v3());
      create_segment(triangle.v3(), triangle.v1());
    }

    ggo::paint<format, ggo::sampling_4x4>(
      buffer, artist.width(), artist.height(), artist.line_step(),
      ggo::make_adaptor(shapes, [](const auto & paint_shape) { return paint_shape.get(); }),
      ggo::rect_int::from_width_height(artist.width(), artist.height()));
  }
}

//////////////////////////////////////////////////////////////
ggo::cabrel_bitmap_artist::cabrel_bitmap_artist(int width, int height, int line_step, ggo::image_format format)
:
bitmap_artist_abc(width, height, line_step, format)
{
}

//////////////////////////////////////////////////////////////
void ggo::cabrel_bitmap_artist::render_bitmap(void * buffer) const
{
  switch (format())
  {
  case ggo::rgb_8u_yu:
    render_bitmap_t<ggo::rgb_8u_yu>(buffer, *this);
    break;
  case ggo::bgrx_8u_yd:
    render_bitmap_t<ggo::bgrx_8u_yd>(buffer, *this);
    break;
  default:
    GGO_FAIL();
    break;
  }
}
