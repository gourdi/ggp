#include "ggo_entabeni.h"
#include <kernel/math/shapes_3d/ggo_shapes3d.h>
#include <2d/paint/ggo_paint.h>
#include <2d/paint/ggo_color_triangle.h>
#include <2d/fill/ggo_fill.h>

//////////////////////////////////////////////////////////////
ggo::array<float, 2> ggo::entabeni::create_grid(bool loop_x, bool loop_y)
{
  int dim_x = (1 << 8) + 1;
  int dim_y = (1 << 14) + 1;

  auto add_noise_to = [](float & v, float amplitude)
  {
    float inf = std::max(-1.f, v - amplitude);
    float sup = std::min( 1.f, v + amplitude);
    v = ggo::rand<float>(inf, sup);
  };

  ggo::array<float, 2> grid(dim_x, dim_y, 0.f);
  float amplitude = 0.75f;

  int cell_size = std::min(dim_x, dim_y);

  while (cell_size > 2)
  {
    for (int cell_start_y = 0; cell_start_y < grid.height() - 1; cell_start_y += cell_size - 1)
    {
      for (int cell_start_x = 0; cell_start_x < grid.width() - 1; cell_start_x += cell_size - 1)
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
        add_noise_to(grid(cell_middle_x, cell_middle_y), amplitude);

        // Square step: set the edges midpoints.
        grid(cell_middle_x, cell_start_y) = (grid(cell_start_x, cell_start_y) + grid(cell_end_x, cell_start_y)) / 2;
        grid(cell_start_x, cell_middle_y) = (grid(cell_start_x, cell_start_y) + grid(cell_start_x, cell_end_y)) / 2;
        add_noise_to(grid(cell_middle_x, cell_start_y), amplitude);
        add_noise_to(grid(cell_start_x, cell_middle_y), amplitude);

        if (loop_y == true && cell_end_y == dim_y - 1)
        {
          grid(cell_middle_x, cell_end_y) = grid(cell_middle_x, 0);
        }
        else
        {
          grid(cell_middle_x, cell_end_y) = (grid(cell_start_x, cell_end_y) + grid(cell_end_x, cell_end_y)) / 2;
          add_noise_to(grid(cell_middle_x, cell_end_y), amplitude);
        }

        if (loop_x == true && cell_end_x == dim_x - 1)
        {
          grid(cell_end_x, cell_middle_y) = grid(0, cell_middle_y);
        }
        else
        {
          grid(cell_end_x, cell_middle_y) = (grid(cell_end_x, cell_start_y) + grid(cell_end_x, cell_end_y)) / 2;
          add_noise_to(grid(cell_end_x, cell_middle_y), amplitude);
        }
      }
    }

    cell_size = cell_size / 2 + 1;
    amplitude *= 0.5f;
  }

  return grid;
}

//////////////////////////////////////////////////////////////
ggo::cubic_curve<float, ggo::rgb_32f> ggo::entabeni::create_color_map()
{
  ggo::cubic_curve<float, ggo::rgb_32f> color_map;

  float hue_inf = ggo::rand<float>();
  float hue_sup = hue_inf + 0.2f;
  float altitude = 0.f;
  float sat = ggo::rand<float>(0.f, 0.1f);
  float val = 1.25f;
  for (int i = 0; i < 10; ++i)
  {
    color_map.push_point(altitude, ggo::from_hsv<ggo::rgb_32f>(ggo::rand<float>(hue_inf, hue_sup), sat, val));
    altitude += ggo::rand<float>(0.25f, 0.5f);
    sat += ggo::rand<float>(0.2f, 0.4f);
    val -= ggo::rand<float>(0.1f, 0.2f);
  }

  return color_map;
}

//////////////////////////////////////////////////////////////
void ggo::entabeni::render_bitmap(void * buffer, int width, int height, int line_step, ggo::image_format format,
                                  const ggo::array<float, 2> & grid,
                                  const ggo::cubic_curve<float, ggo::rgb_32f> & color_map,
                                  float z,
                                  float angle)
{
  ggo::basis3d_float basis;
  basis.move(0.f, 0.f, z);

  const float far = 100.0f;

  auto project = [&](const ggo::pos3f & v)
  {
    float radius = 1.1f - v.z();
    float x_3d = radius * std::cos(angle + v.x());
    float y_3d = radius * std::sin(angle + v.x());
    float z_3d = v.y();

    ggo::pos3f pos3d(x_3d, y_3d, z_3d);
    ggo::pos2f proj = basis.project(pos3d, 0.25f, width, height);

    return std::make_tuple(pos3d, proj);
  };

  std::vector<ggo::static_paint_shape<ggo::triangle2d_float, ggo::rgb_8u>> shapes;

  auto paint_triangle = [&](const ggo::pos3f & v1, const ggo::pos3f & v2, const ggo::pos3f & v3)
  {
    auto p1 = project(v1);
    auto p2 = project(v2);
    auto p3 = project(v3);
    if (std::get<0>(p1).z() >= z || std::get<0>(p2).z() >= z || std::get<0>(p3).z() >= z)
    {
      return; // Triangle behind the camera.
    }

    ggo::pos3f center = (std::get<0>(p1) + std::get<0>(p2) + std::get<0>(p3)) / 3.f;
    ggo::vec3f diff = center - basis.pos();

    float dist = ggo::length(diff);
    if (dist > far)
    {
      return;
    }
    
    float altitude = ggo::hypot(ggo::pos2f(center.x(), center.y()));

    ggo::rgb_8u triangle_color = ggo::convert_color_to<ggo::rgb_8u>(ggo::map(dist, 0.f, far, color_map.evaluate(altitude), ggo::black_32f()));
    ggo::triangle2d_float triangle(std::get<1>(p1), std::get<1>(p2), std::get<1>(p3));
    shapes.emplace_back(triangle, triangle_color);
  };

  const float delta = 2 * ggo::pi<float>() / (grid.width() - 1);

  for (int y = grid.height() - 2; y >= 0; --y) // Paint from the furthest.
  {
    for (int x = 0; x < grid.width() - 1; ++x)
    {
      ggo::pos3f v1(delta * x, -delta * y, grid(x, y));
      ggo::pos3f v2(delta * x, -delta * (y + 1), grid(x, y + 1));
      ggo::pos3f v3(delta * (x + 1), -delta * y, grid(x + 1, y));
      ggo::pos3f v4(delta * (x + 1), -delta * (y + 1), grid(x + 1, y + 1));
      ggo::pos3f v5(delta * (x + 0.5f), -delta * (y + 0.5f), 0.25f * (grid(x, y) + grid(x, y + 1) + grid(x + 1, y) + grid(x + 1, y + 1)));

      paint_triangle(v5, v2, v4);
      paint_triangle(v5, v1, v2);
      paint_triangle(v5, v3, v4);
      paint_triangle(v5, v1, v3);
    }
  }

  ggo::fill_solid<ggo::rgb_8u_yu>(buffer, width, height, line_step, ggo::black_8u());
  ggo::paint_shapes<ggo::rgb_8u_yu, ggo::sampling_4x4>(buffer, width, height, line_step, shapes);
}


