#include "ggo_sonson_realtime_artist.h"
#include <2d/ggo_image.h>
#include <2d/fill/ggo_fill.h>

namespace
{
  //////////////////////////////////////////////////////////////
  template <ggo::pixel_type pixel_type, ggo::lines_order memory_lines_order, int channel>
  void paint_glowing_circle(const ggo::sonson_artist::glowing_circle & circle, ggo::vec2_f center_offset,
    void * buffer, int width, int height, int line_byte_step, const ggo::rect_int & clipping)
  {
    const float sq_radius[4] = { 
      ggo::square(circle._radius[0]),
      ggo::square(circle._radius[1]),
      ggo::square(circle._radius[2]),
      ggo::square(circle._radius[3]) };

    auto bounding_rect_f = ggo::rect_f::from_left_right_bottom_top(
      circle._center.x() + center_offset.x() - circle._radius[3], circle._center.x() + center_offset.x() + circle._radius[3],
      circle._center.y() + center_offset.y() - circle._radius[3], circle._center.y() + center_offset.y() + circle._radius[3]);

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
          float opacity = ggo::ease_inout((std::sqrt(hypot) - circle._radius[0]) / circle._outter_size);
          update_pixel(x, y, opacity * circle._opacity);
        }
        else if (hypot > sq_radius[2])
        {
          float opacity = ggo::ease_inout((circle._radius[3] - std::sqrt(hypot)) / circle._outter_size);
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
  ggo::pixel_type pixel_type, ggo::lines_order memory_lines_order)
:
progress_realtime_artist(width, height, line_byte_step, pixel_type, memory_lines_order, sonson_artist::duration)
{

}

//////////////////////////////////////////////////////////////
void ggo::sonson_realtime_artist::preprocess_frame(void* buffer, uint32_t cursor_events, ggo::pos2_i cursor_pos, float progress)
{
  _circles = _artist.get_circles(progress);

  // Unnormalize.
  for (auto& circle : _circles)
  {
    circle._center.x() *= width();
    circle._center.y() *= height();

    circle._outter_size *= min_size();

    circle._radius[0] *= min_size();
    circle._radius[1] *= min_size();
    circle._radius[2] *= min_size();
    circle._radius[3] *= min_size();
  }
}

//////////////////////////////////////////////////////////////
template <ggo::pixel_type pixel_type, ggo::lines_order memory_lines_order>
void ggo::sonson_realtime_artist::render_tile_t(void * buffer, const ggo::rect_int & clipping) const
{
  ggo::fill_black(ggo::image_t<pixel_type, memory_lines_order>(buffer, size(), line_byte_step()), clipping);

  float offset_length = 0.01f * min_size();

  for (auto & circle : _circles)
  {
    ggo::vec2_f r_offset = offset_length * ggo::vec2_f::from_angle(circle._r_angle);
    ggo::vec2_f g_offset = offset_length * ggo::vec2_f::from_angle(circle._g_angle);
    ggo::vec2_f b_offset = offset_length * ggo::vec2_f::from_angle(circle._b_angle);

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
