#include "ggo_badaboum_artist.h"
#include <2d/paint/ggo_paint.h>
#include <2d/fill/ggo_fill.h>
#include <2d/processing/ggo_blit.h>

namespace
{
  float friction()
  {
    return ggo::rand(0.1f, 0.6f);
  }
}

//////////////////////////////////////////////////////////////
ggo::badaboum_artist::badaboum_artist(int width, int height, int line_step, ggo::pixel_type pixel_type, ggo::lines_order memory_lines_order, ggo::pixel_sampling sampling)
:
artist(width, height, line_step, pixel_type, memory_lines_order),
_world(100),
_sampling(sampling),
_thickness(0.001f * std::min(width, height)),
_bkgd({ width, height }, pixel_type, memory_lines_order)
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
  if (pixel_type == ggo::pixel_type::bgrx_8u && memory_lines_order == ggo::lines_order::down)
  {
    paint_bkgd_t<ggo::pixel_type::bgrx_8u, ggo::lines_order::down>(floors);
  }
  else if (pixel_type == ggo::pixel_type::rgb_8u && memory_lines_order == ggo::lines_order::up)
  {
    paint_bkgd_t<ggo::pixel_type::rgb_8u, ggo::lines_order::up>(floors);
  }
  else if (pixel_type == ggo::pixel_type::rgb_8u && memory_lines_order == ggo::lines_order::down)
  {
    paint_bkgd_t<ggo::pixel_type::rgb_8u, ggo::lines_order::down>(floors);
  }
  else
  {
    GGO_FAIL();
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
template <ggo::pixel_type pixel_type, ggo::lines_order memory_lines_order>
void ggo::badaboum_artist::paint_bkgd_t(const std::vector<ggo::oriented_box_f> & floors)
{
  image_t<pixel_type, memory_lines_order> bkgd(_bkgd.data(), _bkgd.size(), _bkgd.line_byte_step());

  // Paint the background.
  auto create_color = [&]()
  {
    float hue = _hues[ggo::rand(0, 1)] + ggo::rand(-0.02f, 0.02f);
    float sat = ggo::rand(0.4f, 0.8f);
    return ggo::from_hsv<ggo::rgb_8u>(hue, sat, 1.f);
  };

  fill_4_colors(bkgd, create_color(), create_color(), create_color(), create_color());

  // Paint the floors.
  for (const auto & floor : floors)
  {
    auto box = map_fit(floor);

    render_box_t<ggo::sampling_16x16>(bkgd, box, new_box_color(), ggo::rect_int::from_size(size()));
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
  if (pixel_type() == ggo::pixel_type::bgrx_8u && memory_lines_order() == ggo::lines_order::down)
  {
    render_tile_t<ggo::pixel_type::bgrx_8u, ggo::lines_order::down>(buffer, clipping);
  }
  else if (pixel_type() == ggo::pixel_type::rgb_8u && memory_lines_order() == ggo::lines_order::up)
  {
    render_tile_t<ggo::pixel_type::rgb_8u, ggo::lines_order::up>(buffer, clipping);
  }
  else if (pixel_type() == ggo::pixel_type::rgb_8u && memory_lines_order() == ggo::lines_order::down)
  {
    render_tile_t<ggo::pixel_type::rgb_8u, ggo::lines_order::down>(buffer, clipping);
  }
  else
  {
    GGO_FAIL();
  }
}

//////////////////////////////////////////////////////////////
template <ggo::pixel_type pixel_type, ggo::lines_order memory_lines_order>
void ggo::badaboum_artist::render_tile_t(void * buffer, const ggo::rect_int & clipping) const
{
  image_t<pixel_type, memory_lines_order> img(buffer, size(), line_byte_step());
  const_image_t<pixel_type, memory_lines_order> bkgd(_bkgd.data(), _bkgd.size(), _bkgd.line_byte_step());

  auto img_view = make_image_view(img, clipping);
  auto bkgd_view = make_image_view(bkgd, clipping);

  blit(*bkgd_view, *img_view);

  switch (_sampling)
  {
  case ggo::sampling_1:
    render_boxes_t<ggo::sampling_1>(img, clipping);
    break;
  case ggo::sampling_2x2:
    render_boxes_t<ggo::sampling_2x2>(img, clipping);
    break;
  case ggo::sampling_4x4:
    render_boxes_t<ggo::sampling_4x4>(img, clipping);
    break;
  case ggo::sampling_8x8:
    render_boxes_t<ggo::sampling_8x8>(img, clipping);
    break;
  case ggo::sampling_16x16:
    render_boxes_t<ggo::sampling_16x16>(img, clipping);
    break;
  }
}

//////////////////////////////////////////////////////////////
template <ggo::pixel_sampling sampling, ggo::pixel_type pixel_type_, ggo::lines_order memory_lines_order_>
void  ggo::badaboum_artist::render_boxes_t(image_t<pixel_type_, memory_lines_order_> & img, const ggo::rect_int & clipping) const
{
  for (const auto & box : _boxes)
  {
    render_box_t<sampling>(img, box._box, box._color, clipping);
  }
}

//////////////////////////////////////////////////////////////
template <ggo::pixel_sampling sampling, ggo::pixel_type pixel_type_, ggo::lines_order memory_lines_order_>
void  ggo::badaboum_artist::render_box_t(image_t<pixel_type_, memory_lines_order_> & img, const ggo::oriented_box_f & box, ggo::rgb_8u color, const ggo::rect_int & clipping) const
{
  ggo::paint<sampling>(img, box, color, clipping);

  ggo::oriented_box_f left(box.pos() - (box.half_size_x() - _thickness) * box.dir_x(), box.angle(), _thickness, box.half_size_y());
  ggo::paint<sampling>(img, left, ggo::black_8u(), clipping);

  ggo::oriented_box_f right(box.pos() + (box.half_size_x() - _thickness) * box.dir_x(), box.angle(), _thickness, box.half_size_y());
  ggo::paint<sampling>(img, right, ggo::black_8u(), clipping);

  ggo::oriented_box_f bottom(box.pos() - (box.half_size_y() - _thickness) * box.dir_y(), box.angle(), box.half_size_x(), _thickness);
  ggo::paint<sampling>(img, bottom, ggo::black_8u(), clipping);

  ggo::oriented_box_f top(box.pos() + (box.half_size_y() - _thickness) * box.dir_y(), box.angle(), box.half_size_x(), _thickness);
  ggo::paint<sampling>(img, top, ggo::black_8u(), clipping);
}

//////////////////////////////////////////////////////////////
ggo::oriented_box_f ggo::badaboum_artist::map_fit(const ggo::oriented_box_f & box) const
{
  ggo::rect_f roi = ggo::rect_f::from_left_right_bottom_top(-1.f, 1.f, 0.f, 2.f * height() / width());

  return ggo::map_fit(box, roi, size());
}

