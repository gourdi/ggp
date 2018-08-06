#include "ggo_aggregation_animation_artist.h"
#include <2d/blur/ggo_gaussian_blur.h>
#include <2d/ggo_color.h>
#include <2d/fill/ggo_fill.h>
#include <2d/paint/ggo_paint.h>
#include <2d/paint/ggo_color_triangle.h>

////////////////////////////////////////////////////////
ggo::aggregation_animation_artist::aggregation_animation_artist(int width, int height, int line_step, ggo::image_format format)
:
fixed_frames_count_animation_artist_abc(width, height, line_step, format)
{
  _threshold_dist = 0.00125f * std::min(width, height);
  _threshold_hypot = _threshold_dist * _threshold_dist;

  float hue = ggo::rand<float>();
  _background_color = ggo::from_hsv<ggo::rgb_8u>(hue, ggo::rand(0.f, 0.25f), ggo::rand(0.25f, 0.75f));

  // Create grid.
  const float cell_size = 0.1f * std::min(width, height);
  int grid_size_x = ggo::round_to<int>(width / cell_size);
  int grid_size_y = ggo::round_to<int>(height / cell_size);

  _grid.resize(grid_size_x, grid_size_y);

  for (int y = 0; y < _grid.height(); ++y)
  {
    for (int x = 0; x < _grid.width(); ++x)
    {
      float left = x * cell_size - _threshold_dist;
      float right = (x + 1) * cell_size + _threshold_dist;
      float bottom = y * cell_size - _threshold_dist;
      float top = (y + 1) * cell_size + _threshold_dist;
      _grid(x, y)._rect = ggo::rect_float::from_left_right_bottom_top(left, right, bottom, top);
    }
  }

  // Create seeds.
  for (int i = 0; i < 5; ++i)
  {
    register_point(get_random_point(), hue, ggo::rand<float>(0.5f, 1.0f), ggo::rand<float>(0.9f, 1.0f));
  }
}

//////////////////////////////////////////////////////////////
void ggo::aggregation_animation_artist::update()
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
    p.move(disp.x(), disp.y());
    p.set<0>(ggo::pos_mod(p.x(), static_cast<float>(width())));
    p.set<1>(ggo::pos_mod(p.y(), static_cast<float>(height())));

    ++count;
  }
}

//////////////////////////////////////////////////////////////
void ggo::aggregation_animation_artist::update(int points_count)
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
template <ggo::image_format format>
void ggo::aggregation_animation_artist::render_t(void * buffer) const
{
  ggo::fill_solid<format>(buffer, width(), height(), line_step(), _background_color, ggo::rect_int::from_size(size()));

  {
    std::vector<ggo::static_paint_shape<ggo::disc_float, ggo::rgb_8u>> shapes;

    for (const auto & cell : _grid)
    {
      for (const auto & point : cell._points)
      {
        shapes.emplace_back(ggo::disc_float(point._pos, 2.f * _threshold_dist), ggo::black_8u());
      }
    }

    ggo::fill_solid<format>(buffer, width(), height(), line_step(), _background_color);
    ggo::paint_shapes<format, ggo::sampling_4x4>(buffer, width(), height(), line_step(), shapes);
  }

  float stddev = 0.001f * min_size();
  ggo::gaussian_blur2d<format>(buffer, line_step(), size(), stddev);

  {
    std::vector<ggo::static_paint_shape<ggo::disc_float, ggo::rgb_8u>> shapes;

    for (const auto & cell : _grid)
    {
      for (const auto & point : cell._points)
      {
        float hue = point._hue + 0.25f / (1.f + 0.25f * point._counter);
        float sat = point._sat - 0.0015f * point._counter;
        ggo::rgb_8u c = ggo::from_hsv<ggo::rgb_8u>(hue, sat, point._val);
        shapes.emplace_back(ggo::disc_float(point._pos, _threshold_dist), c);
      }
    }

    ggo::paint_shapes<format, ggo::sampling_4x4>(buffer, width(), height(), line_step(), shapes);
  }
}

//////////////////////////////////////////////////////////////
void ggo::aggregation_animation_artist::render_frame(void * buffer, int frame_index)
{
  int points_count = get_final_points_count() / frames_count();

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

  if (buffer != nullptr)
  {
    switch (format())
    {
    case ggo::rgb_8u_yu:
      render_t<ggo::rgb_8u_yu>(buffer);
      break;
    case ggo::bgrx_8u_yd:
      render_t<ggo::bgrx_8u_yd>(buffer);
      break;
    default:
      GGO_FAIL();
      break;
    }
  }
}

//////////////////////////////////////////////////////////////
void ggo::aggregation_animation_artist::register_point(const ggo::pos2f & pos, float hue, float sat, float val)
{
  std::vector<ggo::pos2f> loop_pos{
    pos,
  { pos.x() - width(), pos.y() },{ pos.x() + width(), pos.y() },
  { pos.x(), pos.y() - height() },{ pos.x(), pos.y() + height() },
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
int ggo::aggregation_animation_artist::get_final_points_count() const
{
  const float w = static_cast<float>(width());
  const float h = static_cast<float>(height());
  float ratio = std::max(w / h, h / w);
  return ggo::round_to<int>(150000.f * ratio);
}

