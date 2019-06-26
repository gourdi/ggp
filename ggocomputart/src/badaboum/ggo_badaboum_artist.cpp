#include "ggo_badaboum_artist.h"
#include <2d/paint/ggo_paint.h>
#include <2d/fill/ggo_fill.h>

namespace
{
  float friction()
  {
    return ggo::rand(0.1f, 0.6f);
  }
}

//////////////////////////////////////////////////////////////
ggo::badaboum_artist::badaboum_artist(int width, int height, int line_step, ggo::image_format format, ggo::pixel_sampling sampling)
:
artist(width, height, line_step, format),
_world(100),
_sampling(sampling),
_thickness(0.001f * std::min(width, height)),
_bkgd({ width, height }, format)
{
  _world.set_external_force([](const ggo::rigid_body & body)
  {
    return ggo::vec2_f(0.f, -1.f);
  });

  _hues[0] = ggo::rand(0.f, 1.f);
  _hues[1] = _hues[0] + ggo::rand(0.25f, 0.75f);

  std::vector<ggo::oriented_box_f> walls;

  // Setup left, right and bottom walls.
  auto wall_bottom = std::make_unique<ggo::rigid_body>(10.f, 1.f, 0.f, friction());
  wall_bottom->_position = { 0.f, -1.f };
  walls.push_back(wall_bottom->box());
  _world.add(std::move(wall_bottom));

  auto wall_left = std::make_unique<ggo::rigid_body>(1.f, 10.f, 0.f, friction());
  wall_left->_position = { -2.f, 0.f };
  walls.push_back(wall_left->box());
  _world.add(std::move(wall_left));

  auto wall_right = std::make_unique<ggo::rigid_body>(1.f, 10.f, 0.f, friction());
  wall_right->_position = { 2.f, 0.f };
  walls.push_back(wall_right->box());
  _world.add(std::move(wall_right));

  // Setup some random floors.
  std::vector<ggo::oriented_box_f> floors;
  while (floors.size() < 5)
  {
    auto floor = std::make_unique<ggo::rigid_body>(ggo::rand(0.2f, 0.3f), ggo::rand(0.01f, 0.02f), 0.f, friction());
    floor->_position = { ggo::rand(-1.f, 1.f),  ggo::rand(0.f, 1.f) };
    floor->_rotation = ggo::rand(-ggo::pi_f / 4, ggo::pi_f / 4);

    bool intersection = false;

    for (const auto & other : walls)
    {
      if (test_intersection(floor->box(), other) == true)
      {
        intersection = true;
        break;
      }
    }
    for (const auto & other : floors)
    {
      if (test_intersection(floor->box(), other) == true)
      {
        intersection = true;
        break;
      }
    }

    if (intersection == false)
    {
      floors.push_back(floor->box());
      _world.add(std::move(floor));
    }
  }

  // Pre-render the background.
  switch (format)
  {
  case ggo::rgb_8u_yd:
    paint_bkgd<rgb_8u_yd>(floors);
    break;
  case ggo::rgb_8u_yu:
    paint_bkgd<rgb_8u_yu>(floors);
    break;
  case ggo::bgrx_8u_yd:
    paint_bkgd<bgrx_8u_yd>(floors);
    break;
  }
}

//////////////////////////////////////////////////////////////
ggo::rgb_8u ggo::badaboum_artist::new_box_color() const
{
  float hue = _hues[ggo::rand(0, 1)] + ggo::rand(-0.02f, 0.02f);
  float sat = ggo::rand(0.5f, 1.f);

  return ggo::from_hsv<ggo::rgb_8u>(hue, sat, 1.f);
}

//////////////////////////////////////////////////////////////
template <ggo::image_format format>
void ggo::badaboum_artist::paint_bkgd(const std::vector<ggo::oriented_box_f> & floors)
{
  // Paint the background.
  auto create_color = [&]()
  {
    float hue = _hues[ggo::rand(0, 1)] + ggo::rand(-0.02f, 0.02f);
    float sat = ggo::rand(0.4f, 0.8f);
    return ggo::from_hsv<ggo::rgb_8u>(hue, sat, 1.f);
  };

  fill_4_colors<format>(_bkgd.data(), _bkgd.width(), _bkgd.height(), _bkgd.line_byte_step(),
    create_color(), create_color(), create_color(), create_color());

  // Paint the floors.
  for (const auto & floor : floors)
  {
    auto box = map_fit(floor);

    render_box<format, ggo::sampling_16x16>(_bkgd.data(), _bkgd.line_byte_step(), box, new_box_color(), ggo::rect_int::from_size(size()));
  }
}

//////////////////////////////////////////////////////////////
void ggo::badaboum_artist::preprocess_frame(int frame_index, uint32_t cursor_events, ggo::pos2_i cursor_pos, float time_step)
{
  // Create new boxes.
  const float boxes_per_second = 100.f;

  if (frame_index < 200)
  {
    _new_bodies_count += boxes_per_second * time_step;
    for (; _new_bodies_count >= 1.f; _new_bodies_count -= 1.f)
    {
      auto body = std::make_unique<ggo::rigid_body>(ggo::rand(0.01f, 0.03f), ggo::rand(0.01f, 0.03f), 0.001f, friction());
      body->_position = { ggo::rand(-1.f, 1.f), ggo::rand(2.f, 3.f) };
      body->_rotation = ggo::rand(0.f, ggo::pi<float>());
      body->_angular_velocity = ggo::rand(-10.f, 10.f);
      
      _bodies.push_back({ body.get(), new_box_color() });

      _world.add(std::move(body));
    }
  }

  // Update the physics.
  if (time_step <= 1 / 60.f)
  {
    _world.update(time_step);
  }
  else if (time_step <= 1 / 30.f)
  {
    _world.update(time_step / 2);
    _world.update(time_step / 2);
  }
  else
  {
    _world.update(time_step / 3);
    _world.update(time_step / 3);
    _world.update(time_step / 3);
  }

  // Create boxes to be rendered.
  _boxes.clear();
  for (const auto & body : _bodies)
  {
    _boxes.emplace_back(map_fit(body._body->box()), body._color);
  }
}

//////////////////////////////////////////////////////////////
void ggo::badaboum_artist::render_tile(void * buffer, int frame_index, const ggo::rect_int & clipping)
{
  switch (format())
  {
  case ggo::rgb_8u_yd:
    render_tile<ggo::rgb_8u_yd>(buffer, clipping);
    break;
  case ggo::rgb_8u_yu:
    render_tile<ggo::rgb_8u_yu>(buffer, clipping);
    break;
  case ggo::bgrx_8u_yd:
    render_tile<ggo::bgrx_8u_yd>(buffer, clipping);
    break;
  }
}

//////////////////////////////////////////////////////////////
template <ggo::image_format format>
void ggo::badaboum_artist::render_tile(void * buffer, const ggo::rect_int & clipping) const
{
  const int line_size = clipping.width() * ggo::image_format_traits<format>::pixel_byte_size;
  for (int y = clipping.bottom(); y <= clipping.top(); ++y)
  {
    const void * src = ggo::get_pixel_ptr<format>(_bkgd.data(), clipping.left(), y, _bkgd.height(), _bkgd.line_byte_step());
    void * dst = ggo::get_pixel_ptr<format>(buffer, clipping.left(), y, height(), line_step());
    memcpy(dst, src, line_size);
  }

  switch (_sampling)
  {
  case ggo::sampling_1:
    render_boxes<format, ggo::sampling_1>(buffer, clipping);
    break;
  case ggo::sampling_2x2:
    render_boxes<format, ggo::sampling_2x2>(buffer, clipping);
    break;
  case ggo::sampling_4x4:
    render_boxes<format, ggo::sampling_4x4>(buffer, clipping);
    break;
  case ggo::sampling_8x8:
    render_boxes<format, ggo::sampling_8x8>(buffer, clipping);
    break;
  case ggo::sampling_16x16:
    render_boxes<format, ggo::sampling_16x16>(buffer, clipping);
    break;
  }
}

//////////////////////////////////////////////////////////////
template <ggo::image_format format, ggo::pixel_sampling sampling>
void  ggo::badaboum_artist::render_boxes(void * buffer, const ggo::rect_int & clipping) const
{
  for (const auto & box : _boxes)
  {
    render_box<format, sampling>(buffer, line_step(), box._box, box._color, clipping);
  }
}

//////////////////////////////////////////////////////////////
template <ggo::image_format format, ggo::pixel_sampling sampling>
void  ggo::badaboum_artist::render_box(void * buffer, int line_byte_step, const ggo::oriented_box_f & box, ggo::rgb_8u color, const ggo::rect_int & clipping) const
{
  ggo::paint<format, sampling>(buffer, width(), height(), line_byte_step, box, color, clipping);

  ggo::oriented_box_f left(box.pos() - (box.half_size_x() - _thickness) * box.dir_x(), box.angle(), _thickness, box.half_size_y());
  ggo::paint<format, sampling>(buffer, width(), height(), line_byte_step, left, ggo::black_8u(), clipping);

  ggo::oriented_box_f right(box.pos() + (box.half_size_x() - _thickness) * box.dir_x(), box.angle(), _thickness, box.half_size_y());
  ggo::paint<format, sampling>(buffer, width(), height(), line_byte_step, right, ggo::black_8u(), clipping);

  ggo::oriented_box_f bottom(box.pos() - (box.half_size_y() - _thickness) * box.dir_y(), box.angle(), box.half_size_x(), _thickness);
  ggo::paint<format, sampling>(buffer, width(), height(), line_byte_step, bottom, ggo::black_8u(), clipping);

  ggo::oriented_box_f top(box.pos() + (box.half_size_y() - _thickness) * box.dir_y(), box.angle(), box.half_size_x(), _thickness);
  ggo::paint<format, sampling>(buffer, width(), height(), line_byte_step, top, ggo::black_8u(), clipping);
}

//////////////////////////////////////////////////////////////
ggo::oriented_box_f ggo::badaboum_artist::map_fit(const ggo::oriented_box_f & box) const
{
  ggo::rect_f roi = ggo::rect_f::from_left_right_bottom_top(-1.f, 1.f, 0.f, 2.f * height() / width());

  return ggo::map_fit(box, roi, size());
}

