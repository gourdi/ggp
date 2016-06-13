#include "ggo_entabeni_bitmap_artist.h"
#include <ggo_array.h>
#include <ggo_shapes2d.h>
#include <ggo_shapes3d.h>
#include <ggo_paint.h>

namespace
{
  //////////////////////////////////////////////////////////////
  ggo::array<float, 2> create_grid()
  {
    int cell_size = (1 << 8) + 1;
    int cells_count = 1;
    ggo::array<float, 2> grid(cell_size, cell_size, 0.f);
    float amplitude = 0.5f;
    int depth = 0;

    while (cell_size > 2)
    {
      int cell_start_y = 0;
      for (int cell_y = 0; cell_y < cells_count; ++cell_y)
      {
        int cell_start_x = 0;
        for (int cell_x = 0; cell_x < cells_count; ++cell_x)
        {
          int cell_end_x = cell_start_x + cell_size - 1;
          int cell_end_y = cell_start_y + cell_size - 1;

          int cell_middle_x = (cell_start_x + cell_end_x) / 2;
          int cell_middle_y = (cell_start_y + cell_end_y) / 2;

          if (depth > 2)
          {
            // Diamond step: update the middle point of the cell.
            grid(cell_middle_x, cell_middle_y) = (grid(cell_start_x, cell_start_y) +
              grid(cell_start_x, cell_end_y) +
              grid(cell_end_x, cell_start_y) +
              grid(cell_end_x, cell_end_y)) / 4;
            grid(cell_middle_x, cell_middle_y) += ggo::rand_float(-amplitude, amplitude);

            // Square step: set the edges midpoints.
            grid(cell_middle_x, cell_start_y) = (grid(cell_start_x, cell_start_y) + grid(cell_end_x, cell_start_y)) / 2 + ggo::rand_float(-amplitude, amplitude);
            grid(cell_middle_x, cell_end_y) = (grid(cell_start_x, cell_end_y) + grid(cell_end_x, cell_end_y)) / 2 + ggo::rand_float(-amplitude, amplitude);
            grid(cell_start_x, cell_middle_y) = (grid(cell_start_x, cell_start_y) + grid(cell_start_x, cell_end_y)) / 2 + ggo::rand_float(-amplitude, amplitude);
            grid(cell_end_x, cell_middle_y) = (grid(cell_end_x, cell_start_y) + grid(cell_end_x, cell_end_y)) / 2 + ggo::rand_float(-amplitude, amplitude);
          }

          cell_start_x += cell_size - 1;
        }

        cell_start_y += cell_size - 1;
      }

      cell_size = cell_size / 2 + 1;
      cells_count *= 2;
      amplitude *= 0.5f;
      ++depth;
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
  auto grid = create_grid();

  ggo::basis3d_float basis;
  basis.move(0.f, 0.f, 2.f);
  basis.rotate(ggo::ray3d_float::O_X(), 7 * ggo::PI<float>() / 16);
  basis.rotate(ggo::ray3d_float::O_Z(), ggo::PI<float>() / 8);

  auto project = [&](int x, int y)
  {
    float x_3d = ggo::map(static_cast<float>(x), 0.f, static_cast<float>(grid.get_size<0>() - 1), -1.f, 1.f);
    float y_3d = ggo::map(static_cast<float>(y), 0.f, static_cast<float>(grid.get_size<1>() - 1), -1.f, 1.f);
    float z_3d = grid(x, y);
    ggo::point3d_float p(x_3d, y_3d, z_3d);

    ggo::point2d_float proj = basis.project(p, 0.1f, get_render_width(), get_render_height());

    float dist = ggo::distance(basis.pos(), p);
    ggo::color color = ggo::map(dist, 1.5f, 2.0f, ggo::color::WHITE, ggo::color::BLACK);
  
    return std::make_tuple(proj, color);
  };

  std::vector<ggo::rgb_layer> layers;
  for (int y = 0; y < grid.get_size<1>() - 1; ++y)
  {
    for (int x = 0; x < grid.get_size<0>() - 1; ++x)
    {
      auto proj1 = project(x, y);
      auto proj2 = project(x + 1, y);
      auto proj3 = project(x, y + 1);

      auto segment1 = std::make_shared<ggo::extended_segment_float>(std::get<0>(proj1), std::get<0>(proj2), 0.5f);
      ggo::color color1 = 0.5f * (std::get<1>(proj1) + std::get<1>(proj2));
      if (color1.is_black() == false)
      {
        layers.emplace_back(segment1, color1);
      }
      
      auto segment2 = std::make_shared<ggo::extended_segment_float>(std::get<0>(proj1), std::get<0>(proj3), 0.5f);
      ggo::color color2 = 0.5f * (std::get<1>(proj1) + std::get<1>(proj3));
      if (color2.is_black() == false)
      {
        layers.emplace_back(segment2, color2);
      }
    }
  }

  ggo::paint(buffer, get_render_width(), get_render_height(), layers);
}
