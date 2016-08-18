#include "ggo_entabeni.h"
#include <ggo_shapes3d.h>
#include <ggo_paint.h>

//////////////////////////////////////////////////////////////
ggo::array<float, 2> ggo_entabeni::create_grid(bool loop_x, bool loop_y)
{
  int dim_x = (1 << 8) + 1;
  int dim_y = (1 << 14) + 1;

  auto add_noise_to = [](float & v, float amplitude)
  {
    float inf = std::max(-1.f, v - amplitude);
    float sup = std::min( 1.f, v + amplitude);
    v = ggo::rand_float(inf, sup);
  };

  ggo::array<float, 2> grid(dim_x, dim_y, 0.f);
  float amplitude = 0.75f;

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
ggo::cubic_curve<float, ggo::color> ggo_entabeni::create_color_map()
{
  ggo::cubic_curve<float, ggo::color> color_map;

  float hue_inf = ggo::rand_float();
  float hue_sup = hue_inf + 0.2f;
  float altitude = 0.f;
  float sat = ggo::rand_float(0.f, 0.1f);
  float val = 1.25f;
  for (int i = 0; i < 10; ++i)
  {
    color_map.push_point(altitude, ggo::color::from_hsv(ggo::rand_float(hue_inf, hue_sup), sat, val));
    altitude += ggo::rand_float(0.25f, 0.5f);
    sat += ggo::rand_float(0.2f, 0.4f);
    val -= ggo::rand_float(0.1f, 0.2f);
  }

  return color_map;
}

//////////////////////////////////////////////////////////////
void ggo_entabeni::render_bitmap(ggo::rgb_image_abc & image,
                                 const ggo::array<float, 2> & grid,
                                 const ggo::cubic_curve<float, ggo::color> & color_map,
                                 float z,
                                 float angle)
{
  ggo::basis3d_float basis;
  basis.move(0.f, 0.f, z);

  const float far = 100.0f;

  auto project = [&](const ggo::pos3f & v)
  {
    float radius = 1.1f - v.get<2>();
    float x_3d = radius * std::cos(angle + v.get<0>());
    float y_3d = radius * std::sin(angle + v.get<0>());
    float z_3d = v.get<1>();

    ggo::pos3f pos3d(x_3d, y_3d, z_3d);
    ggo::pos2f proj = basis.project(pos3d, 0.25f, image.get_width(), image.get_height());

    return std::make_tuple(pos3d, proj);
  };

  std::vector<ggo::rgb_layer> layers;

  auto paint_triangle = [&](const ggo::pos3f & v1, const ggo::pos3f & v2, const ggo::pos3f & v3)
  {
    auto p1 = project(v1);
    auto p2 = project(v2);
    auto p3 = project(v3);
    if (std::get<0>(p1).get<2>() >= z || std::get<0>(p2).get<2>() >= z || std::get<0>(p3).get<2>() >= z)
    {
      return; // Triangle behind the camera.
    }

    ggo::pos3f center = (std::get<0>(p1) + std::get<0>(p2) + std::get<0>(p3)) / 3.f;
    ggo::vec3f diff = center - basis.pos();

    float dist = diff.get_length();
    if (dist > far)
    {
      return;
    }
    
    float altitude = ggo::hypot(center.get<0>(), center.get<1>());

    ggo::color triangle_color = ggo::map(dist, 0.f, far, color_map.evaluate(altitude), ggo::color::BLACK);
    auto triangle = std::make_shared<ggo::triangle2d_float>(std::get<1>(p1), std::get<1>(p2), std::get<1>(p3));
    layers.emplace_back(triangle, triangle_color, 1.f);
  };

  const float delta = 2 * ggo::pi<float>() / (grid.get_size<0>() - 1);

  for (int y = grid.get_size<1>() - 2; y >= 0; --y) // Paint from the furthest.
  {
    for (int x = 0; x < grid.get_size<0>() - 1; ++x)
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

  image.fill(ggo::color::BLACK);
  ggo::paint(image, layers, ggo::pixel_sampler_8X8());
}


