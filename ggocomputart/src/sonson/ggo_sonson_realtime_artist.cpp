#include "ggo_sonson_realtime_artist.h"
#include <2d/ggo_image.h>
#include <2d/fill/ggo_fill.h>

namespace
{
  //////////////////////////////////////////////////////////////
  template <ggo::pixel_type pixel_type, ggo::lines_order memory_lines_order, int channel>
  void paint_glowing_circle(const ggo::sonson_realtime_artist::glowing_circle & circle, ggo::vec2_f center_offset,
    void * buffer, int width, int height, int line_byte_step, const ggo::rect_int & clipping)
  {
    const float radius[4] = {
      circle._radius - circle._outter_size - circle._inner_size,
      circle._radius - circle._inner_size,
      circle._radius + circle._inner_size,
      circle._radius + circle._outter_size + circle._inner_size };

    const float sq_radius[4] = { 
      ggo::square(radius[0]),
      ggo::square(radius[1]),
      ggo::square(radius[2]),
      ggo::square(radius[3]) };

    auto bounding_rect_f = ggo::rect_f::from_left_right_bottom_top(
      circle._center.x() + center_offset.x() - radius[3], circle._center.x() + center_offset.x() + radius[3],
      circle._center.y() + center_offset.y() - radius[3], circle._center.y() + center_offset.y() + radius[3]);

    auto bounding_rect_i = ggo::from_continuous_to_discrete_exclusive(bounding_rect_f.data());

    if (bounding_rect_i.clip(clipping) == false)
    {
      return;
    }

    ggo::image_t<pixel_type, memory_lines_order> img(buffer, { width, height }, line_byte_step);

    auto update_pixel = [&](int x, int y, float opacity)
    {
      int v = ggo::round_to<int>(opacity * 255);
      uint8_t * pixel = static_cast<uint8_t *>(img.pixel_ptr(x, y));
      pixel[channel] = std::min(255, pixel[channel] + v);
    };

    auto paint_pixel = [&](int x, int y)
    {
      ggo::pos2_f pixel = ggo::from_discrete_to_continuous<float>(ggo::pos2_i(x, y));
      float hypot = ggo::hypot(circle._center + center_offset, pixel);

      if (hypot > sq_radius[0] && hypot < sq_radius[3])
      {
        if (hypot < sq_radius[1])
        {
          float opacity = ggo::ease_inout((std::sqrt(hypot) - radius[0]) / circle._outter_size);
          update_pixel(x, y, opacity * circle._opacity);
        }
        else if (hypot > sq_radius[2])
        {
          float opacity = ggo::ease_inout((radius[3] - std::sqrt(hypot)) / circle._outter_size);
          update_pixel(x, y, opacity * circle._opacity);
        }
        else
        {
          update_pixel(x, y, circle._opacity);
        }
      }
    };
    bounding_rect_i.for_each_pixel(paint_pixel);
  }
}

//////////////////////////////////////////////////////////////
ggo::sonson_realtime_artist::sonson_realtime_artist(int width, int height, int line_byte_step,
  ggo::pixel_type pixel_type, ggo::lines_order memory_lines_order, ggo::ratio fps)
:
realtime_artist(width, height, line_byte_step, pixel_type, memory_lines_order),
_frame_duration(1 / fps)
{
  float velocity_factor = to<float>(_frame_duration * min_size());

  for (int i = 0; i < 32; ++i)
  {
    glowing_circle circle;
    circle._center = { width * ggo::rand(-0.25f, 1.25f),  height * ggo::rand(-0.25f, 1.25f) };
    circle._radius = min_size() * ggo::rand<float>(0.1f, 0.25f);
    circle._inner_size = min_size() * ggo::rand<float>(0.01f, 0.04f);
    circle._outter_size = min_size() * ggo::rand<float>(0.002f, 0.08f);
    circle._opacity = ggo::rand<float>(0.25f, 0.75f);
    circle._velocity = velocity_factor * ggo::rand<float>(0.01f, 0.02f) * ggo::vec2_f::from_angle(ggo::rand(0.f, 2 * ggo::pi<float>()));
    _circles.push_back(circle);
  }

  _r_angle = ggo::rand(0.f, 2 * ggo::pi<float>());
  _g_angle = ggo::rand(0.f, 2 * ggo::pi<float>());
  _b_angle = ggo::rand(0.f, 2 * ggo::pi<float>());

  _dr_angle = ggo::rand(0.01f, 0.02f);
  _dg_angle = ggo::rand(0.01f, 0.02f);
  _db_angle = ggo::rand(0.01f, 0.02f);
}

//////////////////////////////////////////////////////////////
void ggo::sonson_realtime_artist::preprocess_frame(void* buffer, uint32_t cursor_events, ggo::pos2_i cursor_pos)
{
  _r_angle += _dr_angle;
  _g_angle += _dg_angle;
  _b_angle += _db_angle;

  for (auto & circle : _circles)
  {
    circle._center += circle._velocity;
  }
}

//////////////////////////////////////////////////////////////
template <ggo::pixel_type pixel_type, ggo::lines_order memory_lines_order>
void ggo::sonson_realtime_artist::render_tile_t(void * buffer, const ggo::rect_int & clipping) const
{
  ggo::fill_black(ggo::image_t<pixel_type, memory_lines_order>(buffer, size(), line_byte_step()), clipping);

  float offset_length = 0.01f * min_size();
  ggo::vec2_f r_offset = offset_length * ggo::vec2_f::from_angle(_r_angle);
  ggo::vec2_f g_offset = offset_length * ggo::vec2_f::from_angle(_g_angle);
  ggo::vec2_f b_offset = offset_length * ggo::vec2_f::from_angle(_b_angle);

  for (auto & circle : _circles)
  {
    paint_glowing_circle<pixel_type, memory_lines_order, 0>(circle, r_offset, buffer, width(), height(), line_byte_step(), clipping);
    paint_glowing_circle<pixel_type, memory_lines_order, 1>(circle, g_offset, buffer, width(), height(), line_byte_step(), clipping);
    paint_glowing_circle<pixel_type, memory_lines_order, 2>(circle, b_offset, buffer, width(), height(), line_byte_step(), clipping);
  }
}

//////////////////////////////////////////////////////////////
void ggo::sonson_realtime_artist::render_tile(void* buffer, const ggo::rect_int& clipping)
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
bool ggo::sonson_realtime_artist::finished()
{
  if (_elapsed_time > 10)
  {
    return true;
  }

  _elapsed_time += _frame_duration;

  return false;
}
