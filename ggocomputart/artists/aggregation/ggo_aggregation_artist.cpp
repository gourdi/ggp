#include "ggo_aggregation_artist.h"
#include <ggo_gaussian_blur.h>
#include <ggo_multi_shape_paint.h>
#include <ggo_buffer_fill.h>
#include <ggo_gaussian_blur2d.h>

namespace
{
  template <ggo::pixel_buffer_format pbf>
  void render_t(void * buffer, const ggo::aggregation_artist & artist)
  {
    ggo::fill_solid<pbf>(buffer, artist.get_width(), artist.get_height(), artist.get_line_step(), artist.get_background_color());

    {
      std::vector<ggo::solid_color_shape<ggo::disc_float, ggo::color_8u>> shapes;

      for (const auto & cell : artist.get_grid())
      {
        for (const auto & point : cell._points)
        {
          shapes.emplace_back(ggo::disc_float(point._pos, 2.f * artist.get_threshold_dist()), ggo::black<ggo::color_8u>());
        }
      }

      ggo::fill_solid<pbf>(buffer, artist.get_width(), artist.get_height(), artist.get_line_step(), artist.get_background_color());

      ggo::paint_shapes<pbf, ggo::sampling_4x4>(
        buffer, artist.get_width(), artist.get_height(), artist.get_line_step(), shapes.begin(), shapes.end());
    }

    float stddev = 0.001f * artist.get_min_size();
    ggo::gaussian_blur2d_mirror<pbf>(buffer, artist.get_width(), artist.get_height(), artist.get_line_step(), stddev);

    {
      std::vector<ggo::solid_color_shape<ggo::disc_float, ggo::color_8u>> shapes;

      for (const auto & cell : artist.get_grid())
      {
        for (const auto & point : cell._points)
        {
          float hue = point._hue + 0.25f / (1.f + 0.25f * point._counter);
          float sat = point._sat - 0.0015f * point._counter;
          ggo::color_8u c = ggo::from_hsv<ggo::color_8u>(hue, sat, point._val);
          shapes.emplace_back(ggo::disc_float(point._pos, artist.get_threshold_dist()), c);
        }
      }

      ggo::paint_shapes<pbf, ggo::sampling_4x4>(
        buffer, artist.get_width(), artist.get_height(), artist.get_line_step(), shapes.begin(), shapes.end());
    }
  }
}

//////////////////////////////////////////////////////////////
ggo::aggregation_artist::aggregation_artist(int width, int height, int line_step, ggo::pixel_buffer_format pbf)
:
artist(width, height, line_step, pbf)
{
  _threshold_dist = 0.00125f * std::min(width, height);
  _threshold_hypot = _threshold_dist * _threshold_dist;

  float hue = ggo::rand<float>();
  _background_color = from_hsv<ggo::color_8u>(hue, ggo::rand<float>(0.f, 0.25f), ggo::rand<float>(0.25f, 0.75f));

  // Create grid.
  const float cell_size = 0.1f * std::min(width, height);
  int grid_size_x = ggo::to<int>(width / cell_size);
  int grid_size_y = ggo::to<int>(height / cell_size);

  _grid.resize(grid_size_x, grid_size_y);

  for_each(_grid, [&](int x, int y)
  {
    float left = x * cell_size - _threshold_dist;
    float right = (x + 1) * cell_size + _threshold_dist;
    float bottom = y * cell_size - _threshold_dist;
    float top = (y + 1) * cell_size + _threshold_dist;
    _grid(x, y)._rect = ggo::rect_float::from_left_right_bottom_top(left, right, bottom, top);
  });

  // Create seeds.
  for (int i = 0; i < 5; ++i)
  {
    register_point(get_random_point(), hue, ggo::rand<float>(0.5f, 1.0f), ggo::rand<float>(0.9f, 1.0f));
  }
}

//////////////////////////////////////////////////////////////
void ggo::aggregation_artist::register_point(const ggo::pos2f & pos, float hue, float sat, float val)
{
  std::vector<ggo::pos2f> loop_pos{
    pos,
    { pos.get<0>() - get_width(), pos.get<1>() }, { pos.get<0>() + get_width(), pos.get<1>() },
    { pos.get<0>(), pos.get<1>() - get_height() },{ pos.get<0>(), pos.get<1>() + get_height() },
  };

  for (auto & cell : _grid)
  {
    for (const auto & p : loop_pos)
    {
      if (cell._rect.is_point_inside(p) == true)
      {
        cell._points.push_back({ p, hue, sat, val, 0 });
      }
    }
  }
}

//////////////////////////////////////////////////////////////
void ggo::aggregation_artist::update()
{
  ggo::pos2f p = get_random_point();
  int count = 0;

  while (true)
  {
    // Parse each cell.
    for (auto & cell : _grid)
    {
      // Check if the point is inside the cell.
      if (cell._rect.is_point_inside(p) == true)
      {
        // If so, check if the point is close to another point of the cell.
        for (const auto & point : cell._points)
        {
          if (ggo::hypot(p, point._pos) < _threshold_hypot)
          {
            if (count <= 5)
            {
              // We want a point to wander a little bit before being stuck.
              p = get_random_point();
              count = 0;
            }
            else
            {
              // If so, store the point. We must process all the cells
              // and not stop processing here because cells overlap.
              register_point(p, point._hue, point._sat, point._val);
              return;
            }
          }
        }
      }
    }

    // Move the point.
    ggo::vec2f disp = ggo::from_polar(ggo::rand<float>(0.f, 2.f * ggo::pi<float>()), _threshold_dist);
    p.move(disp.get<0>(), disp.get<1>());
    p.set<0>(ggo::pos_mod(p.get<0>(), static_cast<float>(get_width())));
    p.set<1>(ggo::pos_mod(p.get<1>(), static_cast<float>(get_height())));

    ++count;
  }
}

//////////////////////////////////////////////////////////////
void ggo::aggregation_artist::update(int points_count)
{
  for (auto & cell : _grid)
  {
    for (auto & point : cell._points)
    {
      point._counter += 1;
    }
  }

  for (int i = 0; i < points_count; ++i)
  {
    update();
  }
}

//////////////////////////////////////////////////////////////
void ggo::aggregation_artist::render(void * buffer) const
{
  switch (get_pixel_buffer_format())
  {
  case ggo::rgb_8u_yu:
    render_t<ggo::rgb_8u_yu>(buffer, *this);
    break;
  case ggo::bgra_8u_yd:
    render_t<ggo::bgra_8u_yd>(buffer, *this);
    break;
  }
}

//////////////////////////////////////////////////////////////
int ggo::aggregation_artist::get_final_points_count() const
{
  const float width = ggo::to<float>(get_width());
  const float height = ggo::to<float>(get_height());
  float ratio = std::max(width / height, height / width);
  return ggo::to<int>(150000.f * ratio);
}
