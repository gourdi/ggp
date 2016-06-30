#include "ggo_entabeni_bitmap_artist.h"
#include <ggo_array.h>
#include <ggo_shapes2d.h>
#include <ggo_shapes3d.h>
#include <ggo_rle_rgb8_image.h>
#include <ggo_paint.h>

namespace ggo
{
  std::tuple<int, int, int> sum(const std::tuple<int, int, int> & t)
  {
    return t;
  }

  template <typename... args>
  std::tuple<int, int, int> sum(const std::tuple<int, int, int> & t1, args... a)
  {
    auto t2 = sum(a...);
    return std::make_tuple(std::get<0>(t1) + std::get<0>(t2), std::get<1>(t1) + std::get<1>(t2), std::get<2>(t1) + std::get<2>(t2));
  }

  template <typename... args>
  std::tuple<int, int, int> average(const std::tuple<int, int, int> & t, args... a)
  {
    const auto s = sum(t, a...);
    const int offset = (sizeof...(a) + 1) / 2;
    const int div = sizeof...(a) + 1;
    return std::make_tuple((std::get<0>(s) + offset) / div, (std::get<1>(s) + offset) / div, (std::get<2>(s) + offset) / div);
  }
}

#include <ggo_downscaling.h>

namespace
{
  //////////////////////////////////////////////////////////////
  ggo::array<float, 2> create_grid(bool loop_x, bool loop_y)
  {
    int dim_x = (1 << 8) + 1;
    int dim_y = (1 << 12) + 1;

    ggo::array<float, 2> grid(dim_x, dim_y, 0.f);
    float amplitude = 1.f;

    int cell_size = std::min(dim_x, dim_y);

    while (cell_size > 2)
    {
      for (int cell_start_y = 0; cell_start_y < grid.get_size<1>() - 1; cell_start_y += cell_size - 1)
      {
        for (int cell_start_x = 0; cell_start_x < grid.get_size<0>() - 1; cell_start_x += cell_size - 1)
        {
          int cell_end_x = cell_start_x + cell_size - 1;
          int cell_end_y = cell_start_y + cell_size - 1;

          int cell_middle_x = (cell_start_x + cell_end_x) / 2;
          int cell_middle_y = (cell_start_y + cell_end_y) / 2;

          // Diamond step: update the middle point of the cell.
          grid(cell_middle_x, cell_middle_y) = (grid(cell_start_x, cell_start_y) +
                                                grid(cell_start_x, cell_end_y) +
                                                grid(cell_end_x, cell_start_y) +
                                                grid(cell_end_x, cell_end_y)) / 4;
          grid(cell_middle_x, cell_middle_y) += ggo::rand_float(-amplitude, amplitude);

          // Square step: set the edges midpoints.
          grid(cell_middle_x, cell_start_y) = (grid(cell_start_x, cell_start_y) + grid(cell_end_x, cell_start_y)) / 2 + ggo::rand_float(-amplitude, amplitude);
          grid(cell_start_x, cell_middle_y) = (grid(cell_start_x, cell_start_y) + grid(cell_start_x, cell_end_y)) / 2 + ggo::rand_float(-amplitude, amplitude);

          if (loop_y == true && cell_end_y == dim_y - 1)
          {
            grid(cell_middle_x, cell_end_y) = grid(cell_middle_x, 0);
          }
          else
          {
            grid(cell_middle_x, cell_end_y) = (grid(cell_start_x, cell_end_y) + grid(cell_end_x, cell_end_y)) / 2 + ggo::rand_float(-amplitude, amplitude);
          }

          if (loop_x == true && cell_end_x == dim_x - 1)
          {
            grid(cell_end_x, cell_middle_y) = grid(0, cell_middle_y);
          }
          else
          {
            grid(cell_end_x, cell_middle_y) = (grid(cell_end_x, cell_start_y) + grid(cell_end_x, cell_end_y)) / 2 + ggo::rand_float(-amplitude, amplitude);
          }
        }
      }

      cell_size = cell_size / 2 + 1;
      amplitude *= 0.5f;
    }

    return grid;
  }
}

//////////////////////////////////////////////////////////////
ggo_entabeni_bitmap_artist::ggo_entabeni_bitmap_artist(int render_width, int render_height)
:
ggo_bitmap_artist_abc(render_width, render_height)
{
}

//////////////////////////////////////////////////////////////
void ggo_entabeni_bitmap_artist::render_bitmap(uint8_t * buffer)
{
  auto grid = create_grid(true, false);

  ggo::basis3d_float basis;
  basis.move(0.f, 0.f, 1.f);

  const float hue = ggo::rand_float();

  auto project = [&](const ggo::point3d_float & v)
  {
    float radius = 1 + v.z();
    float x_3d = radius * std::cos(v.x());
    float y_3d = radius * std::sin(v.x());
    float z_3d = v.y();

    ggo::point3d_float pos3d(x_3d, y_3d, z_3d);
    ggo::pos2f proj = basis.project(pos3d, 0.25f, get_render_width(), get_render_height());

    return std::make_tuple(pos3d, proj);
  };

  std::vector<ggo::rgb_layer> layers;

  auto paint_triangle = [&](const ggo::point3d_float & v1, const ggo::point3d_float & v2, const ggo::point3d_float & v3)
  {
    auto p1 = project(v1);
    auto p2 = project(v2);
    auto p3 = project(v3);

    ggo::point3d_float center = (std::get<0>(p1) + std::get<0>(p2) + std::get<0>(p3)) / 3.f;
    ggo::vector3d_float diff = center - basis.pos();

    float dist = diff.get_length();
    float altitude = std::sqrt(ggo::square(center.x()) + ggo::square(center.y()));

    float val = ggo::clamp(1.25f - 0.85f * altitude, 0.f, 1.f);
    ggo::color triangle_color = ggo::color::from_hsv(0.25f, 1.f, val) / (1.f + 0.15f * dist);
    auto triangle = std::make_shared<ggo::triangle2d_float>(std::get<1>(p1), std::get<1>(p2), std::get<1>(p3));
    layers.emplace_back(triangle, triangle_color, 1.f);

    const float width = 0.001f * get_render_min_size() / (1.f + dist);
    ggo::color edge_color = 0.95f * triangle_color;

    auto create_edge_layer = [&](const ggo::pos2f & p1, const ggo::pos2f & p2)
    {
      auto segment = std::make_shared<ggo::extended_segment_float>(p1, p2, width);
      layers.emplace_back(segment, edge_color);
    };

    create_edge_layer(std::get<1>(p1), std::get<1>(p2));
    create_edge_layer(std::get<1>(p2), std::get<1>(p3));
    create_edge_layer(std::get<1>(p1), std::get<1>(p3));
  };

  const float delta = 2 * ggo::PI<float>() / (grid.get_size<0>() - 1);

  for (int y = grid.get_size<1>() - 2; y >= 0; --y) // Paint from the furthest.
  {
    for (int x = 0; x < grid.get_size<0>() - 1; ++x)
    {
      ggo::point3d_float v1(delta * x, -delta * y, grid(x, y));
      ggo::point3d_float v2(delta * x, -delta * (y + 1), grid(x, y + 1));
      ggo::point3d_float v3(delta * (x + 1), -delta * y, grid(x + 1, y));
      ggo::point3d_float v4(delta * (x + 1), -delta * (y + 1), grid(x + 1, y + 1));
      ggo::point3d_float v5(delta * (x + 0.5f), -delta * (y + 0.5f), 0.25f * (grid(x, y) + grid(x, y + 1) + grid(x + 1, y) + grid(x + 1, y + 1)));

      paint_triangle(v5, v2, v4);
      paint_triangle(v5, v1, v2);
      paint_triangle(v5, v3, v4);
      paint_triangle(v5, v1, v3);
    }
  }

  auto image = make_image_buffer(buffer);

  ggo::paint(image, layers, ggo::pixel_sampler_4X4());
}

