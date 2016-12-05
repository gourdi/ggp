#include <ggo_buffer_paint.h>
#include <ggo_multi_shape_paint.h>
#include <ggo_blit.h>
#include <ggo_brush.h>
#include <ggo_blender.h>

//////////////////////////////////////////////////////////////
template <ggo::pixel_buffer_format pbf, ggo::sampling smp>
void ggo::storni_animation_artist::paint_stornies(void * buffer, const ggo::pixel_rect & clipping) const
{
  for (int i = 0; i < get_width() * get_height(); ++i)
  {
    _stornis_buffer.get()[i] = _stornis_buffer.get()[i] >= 32 ? _stornis_buffer.get()[i] - 32 : 0;
  }

  const float storni_radius = 0.0025f * get_min_size();
  for (const auto & storni : _stornis)
  {
    ggo::paint_shape<ggo::pixel_buffer_format_info<pbf>::gray_pbf, smp>(
      _stornis_buffer.get(), get_width(), get_height(), get_width(),
      ggo::extended_segment_float(storni._pos, storni._pos - storni._vel, storni_radius),
      ggo::solid_brush<uint8_t>(0xff), ggo::overwrite_blender<uint8_t>(), clipping);
  }

  auto blend = [&](int x, int y, uint8_t c)
  {
    auto rgb_8u = ggo::read_pixel<pbf>(buffer, x, y, get_height(), get_line_step());

    const uint32_t weight_rgb = 255 - c;
    const uint32_t weighted_white = c * 255;
    const uint32_t r = ggo::fixed_point_div<8>(weighted_white + weight_rgb * rgb_8u.r());
    const uint32_t g = ggo::fixed_point_div<8>(weighted_white + weight_rgb * rgb_8u.g());
    const uint32_t b = ggo::fixed_point_div<8>(weighted_white + weight_rgb * rgb_8u.b());
    return ggo::color_8u(static_cast<uint8_t>(r), static_cast<uint8_t>(g), static_cast<uint8_t>(b));
  };

  ggo::blit<ggo::pixel_buffer_format_info<pbf>::gray_pbf, pbf>(_stornis_buffer.get(), get_width(), get_height(), get_width(),
    buffer, get_width(), get_height(), get_line_step(), 0, 0, blend);
}

//////////////////////////////////////////////////////////////
template <ggo::pixel_buffer_format pbf, ggo::sampling smp>
void ggo::storni_animation_artist::paint_predators(void * buffer, const ggo::pixel_rect & clipping) const
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

      ggo::paint_shapes<pbf, smp>(buffer, get_width(), get_height(), get_line_step(), shapes.cbegin(), shapes.cend(), ggo::pixel_rect::from_width_height(get_width(), get_height()));
    }
  }
}

//////////////////////////////////////////////////////////////
template <ggo::pixel_buffer_format pbf>
void ggo::storni_animation_artist::paint_obstacles(void * buffer, const ggo::pixel_rect & clipping, int frame_index) const
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

    ggo::pixel_rect obstacle_pixel_rect(obstacle_rect);
    obstacle_pixel_rect.clip(get_width(), get_height());

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
