#include "ggo_aggregation_artist.h"
#include <ggo_gaussian_blur.h>
#include <ggo_paint.h>
#include <ggo_fill.h>

namespace ggo
{
  //////////////////////////////////////////////////////////////
  aggregation_artist::aggregation_artist(int width, int height)
  :
  ggo_artist_abc(width, height)
  {
    _threshold_dist = 0.00125f * std::min(width, height);
    _threshold_hypot = _threshold_dist * _threshold_dist;

    float hue = ggo::rand_float();
    _background_color = ggo::color::from_hsv(hue, ggo::rand_float(0.f, 0.25f), ggo::rand_float(0.25f, 0.75f));

    // Create grid.
    const float cell_size = 0.1f * std::min(width, height);
    int grid_size_x = ggo::to<int>(width / cell_size);
    int grid_size_y = ggo::to<int>(height / cell_size);

    _grid.resize(grid_size_x, grid_size_y);

    for_each(_grid, [&](int x, int y)
    {
      float left    = x * cell_size - _threshold_dist;
      float right   = (x + 1) * cell_size + _threshold_dist;
      float bottom  = y * cell_size - _threshold_dist;
      float top     = (y + 1) * cell_size + _threshold_dist;
      _grid(x, y)._rect = ggo::rect_float::from_left_right_bottom_top(left, right, bottom, top);
    });

    // Create seeds.
    for (int i = 0; i < 5; ++i)
    {
      register_point(get_random_point(), hue, ggo::rand_float(0.5f, 1.0f), ggo::rand_float(0.9f, 1.0f));
    }
  }

  //////////////////////////////////////////////////////////////
  void aggregation_artist::register_point(const ggo::pos2f & pos, float hue, float sat, float val)
  {
    std::vector<ggo::pos2f> loop_pos{
      pos,
      { pos.get<0>() - get_render_width(), pos.get<1>() }, { pos.get<0>() + get_render_width(), pos.get<1>() },
      { pos.get<0>(), pos.get<1>() - get_render_height() },{ pos.get<0>(), pos.get<1>() + get_render_height() },
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
  void aggregation_artist::update()
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
      ggo::vec2f disp = ggo::from_polar(ggo::rand_float(0.f, 2.f * ggo::pi<float>()), _threshold_dist);
      p.move(disp.get<0>(), disp.get<1>());
      p.set<0>(ggo::pos_fmod(p.get<0>(), static_cast<float>(get_render_width())));
      p.set<1>(ggo::pos_fmod(p.get<1>(), static_cast<float>(get_render_height())));

      ++count;
    }
  }

  //////////////////////////////////////////////////////////////
  void aggregation_artist::update(int points_count)
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
  void aggregation_artist::render(uint8_t * buffer) const
  {
    auto image = make_image_buffer(buffer);

    ggo::fill_solid(image, _background_color);

    {
      std::vector<ggo::rgb_layer> layers;

      for (const auto & cell : _grid)
      {
        for (const auto & point : cell._points)
        {
          layers.emplace_back(std::make_shared<ggo::disc_float>(point._pos, 2.f * _threshold_dist), ggo::color::BLACK);
        }
      }

      ggo::paint(image, layers);
    }

    float stddev = 0.001f * get_render_min_size();
    ggo::gaussian_blur_2d_mirror<3, 3>(buffer + 0, buffer + 0, get_render_width(), get_render_height(), stddev, 0.01f);
    ggo::gaussian_blur_2d_mirror<3, 3>(buffer + 1, buffer + 1, get_render_width(), get_render_height(), stddev, 0.01f);
    ggo::gaussian_blur_2d_mirror<3, 3>(buffer + 2, buffer + 2, get_render_width(), get_render_height(), stddev, 0.01f);

    {
      std::vector<ggo::rgb_layer> layers;

      for (const auto & cell : _grid)
      {
        for (const auto & point : cell._points)
        {
          float hue = point._hue + 0.25f / (1.f + 0.25f * point._counter);
          float sat = point._sat - 0.0015f * point._counter;
          ggo::color c = ggo::color::from_hsv(hue, sat, point._val);
          layers.emplace_back(std::make_shared<ggo::disc_float>(point._pos, _threshold_dist), c);
        }
      }

      ggo::paint(image, layers);
    }
  }

  //////////////////////////////////////////////////////////////
  int aggregation_artist::get_final_points_count() const
  {
    const float width = ggo::to<float>(get_render_width());
    const float height = ggo::to<float>(get_render_height());
    float ratio = std::max(width / height, height / width);
    return ggo::to<int>(150000.f * ratio);
  }
}
