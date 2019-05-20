#include "ggo_sonson_realtime_artist.h"
#include <2d/fill/ggo_fill.h>

namespace
{
  //////////////////////////////////////////////////////////////
  template <ggo::image_format format, int channel>
  void paint_glowing_circle(const ggo::sonson_realtime_artist::glowing_circle & circle, ggo::vec2_f center_offset,
    void * buffer, int width, int height, int line_step, const ggo::rect_int & clipping)
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

    auto bounding_rect_i = ggo::from_continuous_to_pixel_exclusive(bounding_rect_f.data());

    if (bounding_rect_i.clip(clipping) == false)
    {
      return;
    }

    auto update_pixel = [&](int x, int y, float opacity)
    {
      int v = ggo::round_to<int>(opacity * 255);
      uint8_t * pixel = static_cast<uint8_t *>(ggo::get_pixel_ptr<format>(buffer, x, y, height, line_step));
      pixel[channel] = std::min(255, pixel[channel] + v);
    };

    auto paint_pixel = [&](int x, int y)
    {
      ggo::pos2_f pixel = ggo::from_pixel_to_continuous<float>(ggo::pos2_i(x, y));
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
ggo::sonson_realtime_artist::sonson_realtime_artist(int width, int height, int line_step, ggo::image_format format)
:
realtime_artist_abc(width, height, line_step, format)
{
  for (int i = 0; i < 32; ++i)
  {
    glowing_circle circle;
    circle._center = { width * ggo::rand(-0.25f, 1.25f),  height * ggo::rand(-0.25f, 1.25f) };
    circle._radius = min_size() * ggo::rand<float>(0.1f, 0.25f);
    circle._inner_size = min_size() * ggo::rand<float>(0.01f, 0.04f);
    circle._outter_size = min_size() * ggo::rand<float>(0.002f, 0.08f);
    circle._opacity = ggo::rand<float>(0.25f, 0.75f);
    circle._velocity = ggo::rand<float>(0.0004f, 0.0008f) * min_size() * ggo::vec2_f::from_angle(ggo::rand(0.f, 2 * ggo::pi<float>()));
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
void ggo::sonson_realtime_artist::preprocess_frame(int frame_index, uint32_t cursor_events, ggo::pos2_i cursor_pos)
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
template <ggo::image_format format>
void ggo::sonson_realtime_artist::render_tile_t(void * buffer, const ggo::rect_int & clipping) const
{
  ggo::fill_solid<format>(buffer, width(), height(), line_step(), ggo::black_8u(), clipping);

  float offset_length = 0.01f * min_size();
  ggo::vec2_f r_offset = offset_length * ggo::vec2_f::from_angle(_r_angle);
  ggo::vec2_f g_offset = offset_length * ggo::vec2_f::from_angle(_g_angle);
  ggo::vec2_f b_offset = offset_length * ggo::vec2_f::from_angle(_b_angle);

  for (auto & circle : _circles)
  {
    paint_glowing_circle<format, 0>(circle, r_offset, buffer, width(), height(), line_step(), clipping);
    paint_glowing_circle<format, 1>(circle, g_offset, buffer, width(), height(), line_step(), clipping);
    paint_glowing_circle<format, 2>(circle, b_offset, buffer, width(), height(), line_step(), clipping);
  }
}

//////////////////////////////////////////////////////////////
void ggo::sonson_realtime_artist::render_tile(void * buffer, int frame_index, const ggo::rect_int & clipping)
{
  switch (format())
  {
  case ggo::bgrx_8u_yd:
    render_tile_t<ggo::bgrx_8u_yd>(buffer, clipping);
    break;
  case ggo::rgb_8u_yu:
    render_tile_t<ggo::rgb_8u_yu>(buffer, clipping);
    break;
  case ggo::rgb_8u_yd:
    render_tile_t<ggo::rgb_8u_yd>(buffer, clipping);
    break;
  }
}

//////////////////////////////////////////////////////////////
bool ggo::sonson_realtime_artist::finished(int frame_index) const
{
  return frame_index > 300;
}