#include "ggo_cabrel_bitmap_artist.h"
#include <ggo_paint.h>

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
}

//////////////////////////////////////////////////////////////
ggo_cabrel_bitmap_artist::ggo_cabrel_bitmap_artist(int render_width, int render_height)
:
ggo_bitmap_artist_abc(render_width, render_height)
{
}

//////////////////////////////////////////////////////////////
void ggo_cabrel_bitmap_artist::render_bitmap(uint8_t * buffer)
{
  std::vector<ggo::pos2f> points;
  std::vector<std::tuple<int, int>> available_edges;
  std::vector<std::tuple<int, int, int>> triangles;

  points.emplace_back(ggo::rand_float(0.45f, 0.55f) * get_render_width(), ggo::rand_float(0.45f, 0.55f) * get_render_height());
  points.emplace_back(ggo::rand_float(0.45f, 0.55f) * get_render_width(), ggo::rand_float(0.45f, 0.55f) * get_render_height());
  points.emplace_back(ggo::rand_float(0.45f, 0.55f) * get_render_width(), ggo::rand_float(0.45f, 0.55f) * get_render_height());

  available_edges.push_back(std::make_tuple(0, 1));
  available_edges.push_back(std::make_tuple(1, 2));
  available_edges.push_back(std::make_tuple(2, 0));

  triangles.push_back(std::make_tuple(0, 1, 2));

  for (int i = 0; i < 10; ++i)
  {
    int index = ggo::rand_int(0, static_cast<int>(available_edges.size()) - 1);
    auto edge = available_edges[index];

    // Remove selected edge from available edges list.
    available_edges.erase(available_edges.begin() + index);

    while (true)
    {
      ggo::triangle2d_float triangle(get_random_point(), points[std::get<0>(edge)], points[std::get<1>(edge)]);
      while (triangle.a)

      // Check if the newly created triangle intersect other triangles.
      bool intersection = false;
      for (const auto & triangle_tuple : triangles)
      {
        // Handle special case whe the current triangle holds the selected edge. 
        int v3 = -1;
        if (is_edge_from_triangle(edge, triangle_tuple, v3) == true)
        {
          ggo::vec2f diff(points[std::get<0>(edge)] - points[std::get<1>(edge)]);

          if (ggo::ortho_dot(diff, points[v3]) * ggo::ortho_dot(diff, triangle.v1()) < 0.f)
          {
            intersection = true;
            break;
          }
        }
        else
        {
          ggo::triangle2d_float other_triangle(points[std::get<0>(triangle_tuple)], points[std::get<1>(triangle_tuple)], points[std::get<2>(triangle_tuple)]);

          if (ggo::get_triangle_intersection(triangle, other_triangle) != ggo::triangle_intersection::disjoints)
          {
            intersection = true;
            break;
          }
        }
      }

      // We can the new triangle siince it does not intersect with another one.
      if (intersection == false)
      {
        // Add point.
        points.push_back(triangle.v1());

        // Add edges.
        available_edges.push_back(std::make_tuple(static_cast<int>(points.size()) - 1, std::get<0>(edge)));
        available_edges.push_back(std::make_tuple(static_cast<int>(points.size()) - 1, std::get<1>(edge)));

        // Add triangle.
        triangles.push_back(std::make_tuple(std::get<0>(edge), std::get<1>(edge), static_cast<int>(points.size()) - 1));

        // We have a new triangle, break the 'while (true)' loop.
        break;
      }
    }
  }

  // Paint the triangles.
  for (const auto & triangle_tuple : triangles)
  {
    auto triangle = std::make_shared<ggo::triangle2d_float>(points[std::get<0>(triangle_tuple)], points[std::get<1>(triangle_tuple)], points[std::get<2>(triangle_tuple)]);

    ggo::paint(buffer, get_render_width(), get_render_height(), triangle, ggo::color::get_random());
  }
}
