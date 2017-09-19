#include <ggo_buffer_paint.h>
#include <ggo_multi_shape_paint.h>
#include <ggo_blit.h>
#include <ggo_brush.h>
#include <ggo_blend.h>

//////////////////////////////////////////////////////////////
template <ggo::pixel_buffer_format pbf>
void ggo::storni_animation_artist::blit_background(void * buffer, const ggo::rect_int & clipping) const
{
  const int clipping_width_byte_size = clipping.width() * ggo::pixel_buffer_format_info<pbf>::pixel_byte_size;
  for (int y = clipping.bottom(); y <= clipping.top(); ++y)
  {
    const void * src = ggo::get_pixel_ptr<pbf>(_background.data(), clipping.left(), y, get_height(), get_line_step());
    void * dst = ggo::get_pixel_ptr<pbf>(buffer, clipping.left(), y, get_height(), get_line_step());

    memcpy(dst, src, clipping_width_byte_size);
  }
}

//////////////////////////////////////////////////////////////
template <ggo::pixel_buffer_format pbf, ggo::sampling smp>
void ggo::storni_animation_artist::paint_stornies(void * buffer, const ggo::rect_int & clipping)
{
  constexpr ggo::pixel_buffer_format gray_pbf = ggo::pixel_buffer_format_info<pbf>::gray_pbf;

  // Fade out.
  for (int y = clipping.bottom(); y <= clipping.top(); ++y)
  {
    for (int x = clipping.left(); x <= clipping.right(); ++x)
    {
      void * ptr = ggo::get_pixel_ptr<gray_pbf>(_stornis_buffer.data(), x, y, get_height(), get_width());

      auto y_8u = ggo::read_pixel<gray_pbf>(ptr);
      y_8u = y_8u >= 32 ? y_8u - 32 : 0;
      ggo::write_pixel<gray_pbf>(ptr, y_8u);
    }
  }

  // Paint stornis.
  const float storni_radius = 0.0025f * get_min_size();
  for (const auto & storni : _stornis)
  {
    ggo::paint_shape<gray_pbf, smp>(_stornis_buffer.data(), get_width(), get_height(), get_width(),
      ggo::extended_segment_float(storni._pos, storni._pos - storni._vel, storni_radius),
      ggo::solid_brush<uint8_t>(0xff), ggo::overwrite_blender<uint8_t>(), clipping);
  }

  // Blend. 
  for (int y = clipping.bottom(); y <= clipping.top(); ++y)
  {
    for (int x = clipping.left(); x <= clipping.right(); ++x)
    {
      void * ptr = ggo::get_pixel_ptr<pbf>(buffer, x, y, get_height(), get_line_step());

      auto y_8u = ggo::read_pixel<gray_pbf>(_stornis_buffer.data(), x, y, get_height(), get_width());
      auto rgb_8u = ggo::read_pixel<pbf>(ptr);

      const uint32_t weight_rgb = 255 - y_8u;
      const uint32_t weighted_white = y_8u * 255;
      const uint32_t r = ggo::fixed_point_div<8>(weighted_white + weight_rgb * rgb_8u.r());
      const uint32_t g = ggo::fixed_point_div<8>(weighted_white + weight_rgb * rgb_8u.g());
      const uint32_t b = ggo::fixed_point_div<8>(weighted_white + weight_rgb * rgb_8u.b());
      rgb_8u.r() = static_cast<uint8_t>(r);
      rgb_8u.g() = static_cast<uint8_t>(g);
      rgb_8u.b() = static_cast<uint8_t>(b);

      ggo::write_pixel<pbf>(ptr, rgb_8u);
    }
  }
}

//////////////////////////////////////////////////////////////
template <ggo::pixel_buffer_format pbf, ggo::sampling smp>
void ggo::storni_animation_artist::paint_predators(void * buffer, const ggo::rect_int & clipping) const
{
  const ggo::color_8u color = ggo::from_hsv<ggo::color_8u>(_hue + 0.5f, 1.f, 1.f);
  const float direction_length = 0.03f * std::sqrt(float(get_width() * get_height()));
  const float border_size = 0.001f * std::sqrt(float(get_width() * get_height()));

  auto border_brush = std::make_shared<ggo::solid_dyn_brush<ggo::color_8u>>(ggo::white_8u());
  auto fill_brush = std::make_shared<ggo::solid_dyn_brush<ggo::color_8u>>(color);
  auto blender = std::make_shared<ggo::overwrite_dyn_blender<ggo::color_8u, ggo::color_8u>>();

  std::array<ggo::dyn_paint_shape<float, ggo::color_8u, ggo::color_8u>, 4> shapes;
  shapes[0]._blender = blender;
  shapes[0]._brush = fill_brush;
  shapes[1]._blender = blender;
  shapes[1]._brush = border_brush;
  shapes[2]._blender = blender;
  shapes[2]._brush = border_brush;
  shapes[3]._blender = blender;
  shapes[3]._brush = border_brush;

  for (const auto & predator : _predators)
  {
    if (predator._vel.x() != 0.f || predator._vel.y() != 0.f)
    {
      ggo::vec2f direction = predator._vel;
      direction.set_length(direction_length);

      const ggo::vec2f v1{ predator._pos };
      const ggo::vec2f v2{ predator._pos - direction + 0.5f * ggo::vec2f(direction.y(), -direction.x()) };
      const ggo::vec2f v3{ predator._pos - direction + 0.5f * ggo::vec2f(-direction.y(), direction.x()) };

      shapes[0]._shape = std::make_shared<ggo::triangle2d_float>(v1, v2, v3);
      shapes[1]._shape = std::make_shared<ggo::extended_segment_float>(v1, v2, border_size);
      shapes[2]._shape = std::make_shared<ggo::extended_segment_float>(v2, v3, border_size);
      shapes[3]._shape = std::make_shared<ggo::extended_segment_float>(v3, v1, border_size);

      ggo::paint_shapes<pbf, smp>(buffer, get_width(), get_height(), get_line_step(), shapes.cbegin(), shapes.cend(), clipping);
    }
  }
}

//////////////////////////////////////////////////////////////
template <ggo::pixel_buffer_format pbf>
void ggo::storni_animation_artist::paint_obstacles(void * buffer, const ggo::rect_int & clipping, int frame_index) const
{
  const float obstacle_hypot = get_obstacle_hypot();
  const float obstacle_hypot_inv = 1.f / obstacle_hypot;
  const float phase = 0.5f * frame_index;
  const ggo::color_8u color = ggo::from_hsv<ggo::color_8u>(_hue + 0.5f, 1.f, 1.f);

  for (const auto & obstacle : _obstacles)
  {
    const ggo::rect_data<float> obstacle_rect = ggo::rect_data_from_left_right_bottom_top(
      obstacle.x() - obstacle_hypot, obstacle.x() + obstacle_hypot,
      obstacle.y() - obstacle_hypot, obstacle.y() + obstacle_hypot);

    ggo::rect_int obstacle_pixel_rect = from_math_to_pixel_exclusive(obstacle_rect);
    obstacle_pixel_rect.clip(get_width(), get_height());

    if (obstacle_pixel_rect.clip(clipping) == true)
    {
      obstacle_pixel_rect.for_each_pixel([&](int x, int y)
      {
        const float hypot = ggo::hypot(obstacle, { float(x), float(y) });
        if (hypot < obstacle_hypot)
        {
          float opacity = (obstacle_hypot - hypot) * obstacle_hypot_inv;
          opacity *= 0.5f + 0.5f * std::sin(20.f * opacity + phase);

          const ggo::alpha_blender_rgb8u blender(opacity);
          auto pixel = ggo::read_pixel<pbf>(buffer, x, y, get_height(), get_line_step());
          pixel = blender(x, y, pixel, color);
          ggo::write_pixel<pbf>(buffer, x, y, get_height(), get_line_step(), pixel);
        }
      });
    }
  }
}
