#include <2d/paint/ggo_paint.h>
#include <2d/ggo_blit.h>
#include <2d/paint/ggo_brush.h>
#include <2d/paint/ggo_blend.h>

//////////////////////////////////////////////////////////////
template <ggo::image_format format>
void ggo::storni_animation_artist::blit_background(void * buffer, const ggo::rect_int & clipping) const
{
  const int clipping_width_byte_size = clipping.width() * ggo::image_format_traits<format>::pixel_byte_size;
  for (int y = clipping.bottom(); y <= clipping.top(); ++y)
  {
    const void * src = ggo::get_pixel_ptr<format>(_background.data(), clipping.left(), y, get_height(), get_line_step());
    void * dst = ggo::get_pixel_ptr<format>(buffer, clipping.left(), y, get_height(), get_line_step());

    memcpy(dst, src, clipping_width_byte_size);
  }
}

//////////////////////////////////////////////////////////////
template <ggo::image_format format, ggo::sampling smp>
void ggo::storni_animation_artist::paint_stornies(void * buffer, const ggo::rect_int & clipping)
{
  constexpr ggo::image_format gray_format = ggo::image_format_traits<format>::gray_format;

  // Fade out.
  for (int y = clipping.bottom(); y <= clipping.top(); ++y)
  {
    for (int x = clipping.left(); x <= clipping.right(); ++x)
    {
      void * ptr = ggo::get_pixel_ptr<gray_format>(_stornis_buffer.data(), x, y, get_height(), get_width());

      auto y_8u = ggo::read_pixel<gray_format>(ptr);
      y_8u = y_8u >= 32 ? y_8u - 32 : 0;
      ggo::write_pixel<gray_format>(ptr, y_8u);
    }
  }

  // Paint stornis.
  const float storni_radius = 0.0025f * get_min_size();
  for (const auto & storni : _stornis)
  {
    ggo::paint_shape<gray_format, smp>(_stornis_buffer.data(), get_width(), get_height(), get_width(),
      ggo::extended_segment_float(storni._pos, storni._pos - storni._vel, storni_radius),
      ggo::solid_color_brush<uint8_t>(0xff), ggo::overwrite_blender<uint8_t>(), clipping);
  }

  // Blend. 
  for (int y = clipping.bottom(); y <= clipping.top(); ++y)
  {
    for (int x = clipping.left(); x <= clipping.right(); ++x)
    {
      void * ptr = ggo::get_pixel_ptr<format>(buffer, x, y, get_height(), get_line_step());

      auto y_8u = ggo::read_pixel<gray_format>(_stornis_buffer.data(), x, y, get_height(), get_width());
      auto rgb_8u = ggo::read_pixel<format>(ptr);

      const uint32_t weight_rgb = 255 - y_8u;
      const uint32_t weighted_white = y_8u * 255;
      const uint32_t r = ggo::fixed_point_div<8>(weighted_white + weight_rgb * rgb_8u.r());
      const uint32_t g = ggo::fixed_point_div<8>(weighted_white + weight_rgb * rgb_8u.g());
      const uint32_t b = ggo::fixed_point_div<8>(weighted_white + weight_rgb * rgb_8u.b());
      rgb_8u.r() = static_cast<uint8_t>(r);
      rgb_8u.g() = static_cast<uint8_t>(g);
      rgb_8u.b() = static_cast<uint8_t>(b);

      ggo::write_pixel<format>(ptr, rgb_8u);
    }
  }
}

//////////////////////////////////////////////////////////////
template <ggo::image_format format, ggo::sampling smp>
void ggo::storni_animation_artist::paint_predators(void * buffer, const ggo::rect_int & clipping) const
{
  const ggo::color_8u color = ggo::from_hsv<ggo::color_8u>(_hue + 0.5f, 1.f, 1.f);
  const float direction_length = 0.03f * std::sqrt(float(get_width() * get_height()));
  const float border_size = 0.001f * std::sqrt(float(get_width() * get_height()));

  for (const auto & predator : _predators)
  {
    if (predator._vel.x() != 0.f || predator._vel.y() != 0.f)
    {
      ggo::vec2f direction = predator._vel;
      direction.set_length(direction_length);

      const ggo::vec2f v1{ predator._pos };
      const ggo::vec2f v2{ predator._pos - direction + 0.5f * ggo::vec2f(direction.y(), -direction.x()) };
      const ggo::vec2f v3{ predator._pos - direction + 0.5f * ggo::vec2f(-direction.y(), direction.x()) };

      ggo::solid_color_shape<ggo::triangle2d_float, ggo::color_8u> triangle({ v1, v2, v3 }, color);
      ggo::solid_color_shape<ggo::extended_segment_float, ggo::color_8u> border1({ v1, v2, border_size }, ggo::white_8u());
      ggo::solid_color_shape<ggo::extended_segment_float, ggo::color_8u> border2({ v2, v3, border_size }, ggo::white_8u());
      ggo::solid_color_shape<ggo::extended_segment_float, ggo::color_8u> border3({ v3, v1, border_size }, ggo::white_8u());

      const std::vector<const ggo::paint_shape_abc<float, ggo::color_8u> *> paint_shapes{ &triangle, &border1, &border2, &border3 };

      ggo::paint_shapes<format, smp>(buffer, get_width(), get_height(), get_line_step(), paint_shapes, clipping);
    }
  }
}

//////////////////////////////////////////////////////////////
template <ggo::image_format format>
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
          auto pixel = ggo::read_pixel<format>(buffer, x, y, get_height(), get_line_step());
          pixel = blender(x, y, pixel, color);
          ggo::write_pixel<format>(buffer, x, y, get_height(), get_line_step(), pixel);
        }
      });
    }
  }
}
