#include "ggo_aggregation_animation_artist.h"
#include <2d/ggo_color.h>
#include <2d/processing/ggo_gaussian_blur.h>
#include <2d/fill/ggo_fill.h>
#include <2d/paint/ggo_paint.h>
#include <2d/paint/ggo_color_triangle.h>

////////////////////////////////////////////////////////
ggo::aggregation_animation_artist::aggregation_animation_artist(int width, int height, int line_step, ggo::pixel_type pixel_type, ggo::lines_order memory_lines_order)
:
fixed_frames_count_animation_artist_abc(width, height, line_step, pixel_type, memory_lines_order, 300)
{
  _threshold_dist = 0.00125f * std::min(width, height);
  _threshold_hypot = _threshold_dist * _threshold_dist;

  float hue = ggo::rand<float>();
  _background_color = ggo::from_hsv<ggo::rgb_8u>(hue, ggo::rand(0.f, 0.25f), ggo::rand(0.25f, 0.75f));

  // Create grid.
  const float cell_size = 0.1f * std::min(width, height);
  int grid_size_x = ggo::round_to<int>(width / cell_size);
  int grid_size_y = ggo::round_to<int>(height / cell_size);

  _grid = ggo::array<cell, 2>(grid_size_x, grid_size_y);

  for (int y = 0; y < _grid.height(); ++y)
  {
    for (int x = 0; x < _grid.width(); ++x)
    {
      float left = x * cell_size - _threshold_dist;
      float right = (x + 1) * cell_size + _threshold_dist;
      float bottom = y * cell_size - _threshold_dist;
      float top = (y + 1) * cell_size + _threshold_dist;
      _grid(x, y)._rect = ggo::rect_f::from_left_right_bottom_top(left, right, bottom, top);
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
  ggo::pos2_f p = get_random_point();
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
    p += _threshold_dist * ggo::vec2_f::from_angle(ggo::rand<float>(0.f, 2.f * ggo::pi<float>()));
    p.x() = ggo::pos_mod(p.x(), static_cast<float>(width()));
    p.y() = ggo::pos_mod(p.y(), static_cast<float>(height()));

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
template <ggo::pixel_type pixel_type, ggo::lines_order memory_lines_order>
void ggo::aggregation_animation_artist::render_t(void * buffer) const
{
  ggo::image_t<pixel_type, memory_lines_order> view(buffer, size(), line_byte_step());

  ggo::fill_solid(view, _background_color);

  {
    scene2d<pixel_type_traits<pixel_type>::color_t> scene;

    for (const auto & cell : _grid)
    {
      for (const auto & point : cell._points)
      {
        scene.make_paint_shape_t(ggo::disc_f(point._pos, 2.f * _threshold_dist), ggo::black_8u());
      }
    }

    ggo::fill_solid(view, _background_color);
    ggo::paint<ggo::sampling_4x4>(view, scene);
  }

  float stddev = 0.001f * min_size();
  ggo::gaussian_blur(view, stddev);

  {
    scene2d<pixel_type_traits<pixel_type>::color_t> scene;

    for (const auto & cell : _grid)
    {
      for (const auto & point : cell._points)
      {
        float hue = point._hue + 0.25f / (1.f + 0.25f * point._counter);
        float sat = point._sat - 0.0015f * point._counter;
        ggo::rgb_8u c = ggo::from_hsv<ggo::rgb_8u>(hue, sat, point._val);

        scene.make_paint_shape_t(ggo::disc_f(point._pos, _threshold_dist), c);
      }
    }

    ggo::paint<ggo::sampling_4x4>(view, scene);
  }
}

//////////////////////////////////////////////////////////////
void ggo::aggregation_animation_artist::render_frame(void * buffer, int frame_index, float time_step)
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

  // Dispatch.
  if (buffer != nullptr)
  {
    if (pixel_type() == ggo::pixel_type::rgb_8u && memory_lines_order() == ggo::lines_order::up)
    {
      render_t<ggo::pixel_type::rgb_8u, ggo::lines_order::up>(buffer);
    }
    else if (pixel_type() == ggo::pixel_type::bgrx_8u && memory_lines_order() == ggo::lines_order::down)
    {
      render_t<ggo::pixel_type::bgrx_8u, ggo::lines_order::down>(buffer);
    }
    else
    {
      GGO_FAIL();
    }
  }
}

//////////////////////////////////////////////////////////////
void ggo::aggregation_animation_artist::register_point(const ggo::pos2_f & pos, float hue, float sat, float val)
{
  std::vector<ggo::pos2_f> loop_pos{
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

