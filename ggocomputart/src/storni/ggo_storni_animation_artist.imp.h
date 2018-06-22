#include <2d/paint/ggo_paint.h>
#include <2d/ggo_blit.h>
#include <2d/paint/ggo_brush.h>
#include <2d/paint/ggo_blend.h>

//////////////////////////////////////////////////////////////
template <ggo::image_format format>
void ggo::storni_animation_artist::fade_background_to_white(const ggo::rect_int & clipping)
{
  using format_traits = ggo::image_format_traits<format>;

  auto fade = [](uint8_t v)
  {
    return std::min(0xff, v + (0xff - v) / 16 + 1);
  };

  for (int y = clipping.bottom(); y <= clipping.top(); ++y)
  {
    void * ptr_line  = ggo::get_line_ptr<format_traits::lines_order>(_background_buffer.data(), y, get_height(), get_line_step());
    void * ptr_left  = ptr_offset(ptr_line, clipping.left() * format_traits::pixel_byte_size);
    void * ptr_right = ptr_offset(ptr_line, clipping.right() * format_traits::pixel_byte_size);

    for (void * ptr = ptr_left; ptr <= ptr_right; ptr = ptr_offset(ptr, format_traits::pixel_byte_size))
    {
      auto pixel = ggo::read_pixel<format>(ptr);

      pixel.r() = fade(pixel.r());
      pixel.g() = fade(pixel.g());
      pixel.b() = fade(pixel.b());

      ggo::write_pixel<format>(ptr, pixel);
    }
  }
}

//////////////////////////////////////////////////////////////
template <ggo::image_format format, ggo::sampling smp>
void ggo::storni_animation_artist::paint_stornies_background(const ggo::rect_int & clipping)
{
  alpha_blender_rgb8u blender(0.4f);
  const float storni_radius = 0.005f * get_min_size();
  for (const auto & storni : _stornis)
  {
    // Paint 2 stamps.
    ggo::paint_shape<format, smp>(_background_buffer.data(), get_width(), get_height(), get_line_step(),
      ggo::disc_float(storni._pos, storni_radius),
      ggo::make_solid_brush(storni._color), blender, clipping);

    ggo::paint_shape<format, smp>(_background_buffer.data(), get_width(), get_height(), get_line_step(),
      ggo::disc_float(storni._pos - 0.5f * storni._vel, storni_radius),
      ggo::make_solid_brush(storni._color), blender, clipping);
  }
}

//////////////////////////////////////////////////////////////
template <ggo::image_format format>
void ggo::storni_animation_artist::blit_background(void * buffer, const ggo::rect_int & clipping) const
{
  using format_traits = ggo::image_format_traits<format>;

  const int clipped_line_byte_size = clipping.width() * format_traits::pixel_byte_size;
  
  for (int y = clipping.bottom(); y <= clipping.top(); ++y)
  {
    const void * ptr_in = ggo::get_line_ptr<format_traits::lines_order>(_background_buffer.data(), y, get_height(), get_line_step());
    ptr_in = ptr_offset(ptr_in, clipping.left() * format_traits::pixel_byte_size);

    void * ptr_out = ggo::get_line_ptr<format_traits::lines_order>(buffer, y, get_height(), get_line_step());
    ptr_out = ptr_offset(ptr_out, clipping.left() * format_traits::pixel_byte_size);

    memcpy(ptr_out, ptr_in, clipped_line_byte_size);
  }
}

//////////////////////////////////////////////////////////////
template <ggo::image_format format, ggo::sampling smp>
void ggo::storni_animation_artist::paint_stornies(void * buffer, const std::vector<storni> & stornis, float size, const ggo::rect_int & clipping) const
{
  const float border_size = size * 0.05f;

  for (const auto & storni : stornis)
  {
    if (storni._vel.x() != 0.f || storni._vel.y() != 0.f)
    {
      ggo::vec2f direction = storni._vel;
      direction.set_length(size);

      const ggo::vec2f v1{ storni._pos + direction };
      const ggo::vec2f v2{ storni._pos + 0.5f * ggo::vec2f(direction.y(), -direction.x()) };
      const ggo::vec2f v3{ storni._pos + 0.5f * ggo::vec2f(-direction.y(), direction.x()) };

      ggo::solid_color_shape<ggo::triangle2d_float, ggo::color_8u> triangle({ v1, v2, v3 }, storni._color);
      ggo::solid_color_shape<ggo::capsule_float, ggo::color_8u> border1({ v1, v2, border_size }, ggo::black_8u());
      ggo::solid_color_shape<ggo::capsule_float, ggo::color_8u> border2({ v2, v3, border_size }, ggo::black_8u());
      ggo::solid_color_shape<ggo::capsule_float, ggo::color_8u> border3({ v3, v1, border_size }, ggo::black_8u());

      const std::vector<const ggo::paint_shape_abc<float, ggo::color_8u> *> paint_shapes{ &triangle, &border1, &border2, &border3 };

      ggo::paint_shapes<format, smp>(buffer, get_width(), get_height(), get_line_step(), paint_shapes, clipping);
    }
  }
}

//////////////////////////////////////////////////////////////
template <ggo::image_format format>
void ggo::storni_animation_artist::paint_obstacles(void * buffer, const ggo::rect_int & clipping, int frame_index) const
{
  const float obstacle_hypot = get_obstacle_hypot(get_width(), get_height());
  const float obstacle_hypot_inv = 1.f / obstacle_hypot;
  const float phase = 0.5f * frame_index;
  const ggo::color_8u color = ggo::from_hsv<ggo::color_8u>(_hue, 1.f, 1.f);

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
