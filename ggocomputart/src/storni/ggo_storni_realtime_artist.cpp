#include "ggo_storni_realtime_artist.h"
#include <2d/fill/ggo_fill.h>
#include <2d/paint/ggo_brush.h>
#include <2d/paint/ggo_blend.h>
#include <2d/paint/ggo_paint.h>
#include <2d/processing/ggo_blit.h>

namespace
{
  //////////////////////////////////////////////////////////////
  bool compare_angles(const float angle1, const float angle2, const float tolerance)
  {
    const float diff = angle1 - angle2;

    if (std::abs(diff) < tolerance)
    {
      return true;
    }

    if (std::abs(diff + 2 * ggo::pi<float>()) < tolerance)
    {
      return true;
    }

    if (std::abs(diff - 2 * ggo::pi<float>()) < tolerance)
    {
      return true;
    }

    return false;
  }
}

//////////////////////////////////////////////////////////////
void ggo::storni_realtime_artist::storni::avoid_obstacles(int width, int height, const std::vector<ggo::pos2_f> & obstacles)
{
  const float obstacle_influence_hypot = get_obstacle_hypot(width, height);

  ggo::vec2_f obstacle_force{ 0.f, 0.f };
  for (const auto & obstacle : obstacles)
  {
    const ggo::vec2_f diff_obstacle(obstacle - _pos);
    const float hypot_obstacle = ggo::hypot(diff_obstacle);
    if (hypot_obstacle < obstacle_influence_hypot)
    {
      const float factor = 1.f - hypot_obstacle / obstacle_influence_hypot;
      obstacle_force += factor * diff_obstacle;
    }
  }
  const float obstacle_hypot = ggo::hypot(obstacle_force);
  if (obstacle_hypot != 0.f)
  {
    obstacle_force *= 0.0008f * std::min(width, height) / std::sqrt(obstacle_hypot);
    _vel -= obstacle_force;
  }
}

//////////////////////////////////////////////////////////////
void ggo::storni_realtime_artist::storni::avoid_cursor(int width, int height, ggo::pos2_i cursor_pos, float influence_hypot, float weight)
{
  const ggo::vec2_f diff(ggo::from_discrete_to_continuous<float>(cursor_pos) - _pos);
  const float hypot = ggo::hypot(diff);

  if (hypot < influence_hypot)
  {
    const ggo::vec2_f force = weight * std::min(width, height) * ggo::normalize(diff);
    _vel -= force;
  }
}

//////////////////////////////////////////////////////////////
void ggo::storni_realtime_artist::storni::avoid_stornis(int width, int height, const std::vector<storni> & stornis, float influence_hypot, float weight)
{
  ggo::vec2_f force{ 0.f, 0.f };
  for (const auto & storni : stornis)
  {
    if (&storni != this)
    {
      const ggo::vec2_f diff(storni._pos - _pos);
      const float hypot = ggo::hypot(diff);
      if (hypot < influence_hypot)
      {
        const float factor = 1.f - hypot / influence_hypot;
        force += factor * diff;
      }
    }
  }
  const float force_hypot = ggo::hypot(force);
  if (force_hypot != 0.f)
  {
    force *= weight * std::min(width, height) / std::sqrt(force_hypot);
    _vel -= force;
  }
}

//////////////////////////////////////////////////////////////
void ggo::storni_realtime_artist::storni::avoid_borders(int width, int height, float margin)
{
  const int min_size = std::min(width, height);

  if (_pos.x() < margin)
  {
    _vel.x() += 0.002f * min_size;
  }
  if (_pos.y() < margin)
  {
    _vel.y() += 0.002f * min_size;
  }
  if (_pos.x() > width - 1 - margin)
  {
    _vel.x() -= 0.002f * min_size;
  }
  if (_pos.y() > height - 1 - margin)
  {
    _vel.y() -= 0.002f * min_size;
  }
}

//////////////////////////////////////////////////////////////
void ggo::storni_realtime_artist::storni::clamp_velocity(float velocity_hypot_max)
{
  const float velocity_hypot = ggo::hypot(_vel);
 
  if (velocity_hypot >= velocity_hypot_max)
  {
    _vel *= std::sqrt(velocity_hypot_max / velocity_hypot);
  }
}

//////////////////////////////////////////////////////////////
ggo::storni_realtime_artist::storni_realtime_artist(
  int width, int height, int line_byte_step,
  ggo::pixel_type pixel_type, ggo::lines_order memory_lines_order,
  ggo::ratio fps)
:
realtime_artist(width, height, line_byte_step, pixel_type, memory_lines_order),
_background_buffer(height * line_byte_step),
_fps(fps)
{
  _hue = ggo::rand<float>();

  std::fill(_background_buffer.data(), _background_buffer.data() + _background_buffer.size(), 0xff);

  const float velocity = std::sqrt(get_velocity_hypot_max());

  auto create_storni = [&](std::vector<storni> & container, ggo::rgb_8u c)
  {
    container.emplace_back(get_random_point(), velocity * ggo::vec2_f::from_angle(ggo::rand(0.f, 2 * ggo::pi<float>())), c);
  };

  for (int i = 0; i < 256; ++i)
  {
    create_storni(_stornis, ggo::from_hsv<ggo::rgb_8u>(_hue, ggo::rand<float>(0.5f, 1.f), ggo::rand<float>()));
  }

  for (int i = 0; i < 2; ++i)
  {
    create_storni(_predators, ggo::red_8u());
  }

  for (int i = 0; i < 4; ++i)
  {
    while (true)
    {
      const ggo::pos2_f new_obstacle = get_random_point(0.1f * min_size());

      bool inserted = true;
      for (const auto & obstacle : _obstacles)
      {
        if (ggo::hypot(new_obstacle, obstacle) < 4 * get_obstacle_hypot(width, height))
        {
          inserted = false;
          break;
        }
      }

      if (inserted == true)
      {
        _obstacles.push_back(new_obstacle);
        break;
      }
    }
  }
}

//////////////////////////////////////////////////////////////
void ggo::storni_realtime_artist::update_predators(float velocity_hypot_max, float border_margin)
{
  const float pray_visibility_hypot = ggo::square(0.12f) * width() * height();
  const float repulsion_influence_hypot = ggo::square(0.05f) * width() * height();

  for (auto & predator : _predators)
  {
    ggo::vec2_f attraction(0.f, 0.f);

    for (const auto & pray : _stornis)
    {
      // Check if pray is visible.
      const ggo::vec2_f diff = pray._pos - predator._pos;
      const float angle = ggo::angle(predator._vel);
      const float angle2 = ggo::angle(diff);
      if (compare_angles(angle, angle2, 120.f) == false)
      {
        continue;
      }

      // Chase the prays.
      const float hypot = ggo::hypot(diff);
      if (hypot < pray_visibility_hypot)
      {
        const float factor = 1.f - hypot / pray_visibility_hypot;
        attraction += factor * diff;
      }
    }

    // Apply attraction.
    const float attraction_hypot = ggo::hypot(attraction);
    if (attraction_hypot != 0.0f)
    {
      attraction *= to<float>(min_size() / (20 * _fps)) / std::sqrt(attraction_hypot);
      predator._vel += attraction;
    }

    // Avoid obstacles, other predators and borders.
    predator.avoid_obstacles(width(), height(), _obstacles);
    predator.avoid_stornis(width(), height(), _predators, repulsion_influence_hypot, 0.003f);
    predator.avoid_borders(width(), height(), border_margin);

    // Clamp velocity.
    predator.clamp_velocity(velocity_hypot_max);
  }
}

//////////////////////////////////////////////////////////////
void ggo::storni_realtime_artist::update_stornis(float velocity_hypot_max, float border_margin, ggo::pos2_i cursor_pos)
{
  const float repulsion_influence_hypot = ggo::square(0.02f) * width() * height();
  const float alignment_influence_hypot = ggo::square(0.04f) * width() * height();
  const float attraction_influence_hypot = ggo::square(0.05f) * width() * height();
  const float predator_visibility_hypot = ggo::square(0.05f) * width() * height();
  const float cursor_visibility_hypot = ggo::square(0.05f) * width() * height();

  for (auto & storni : _stornis)
  {
    ggo::vec2_f repulsion(0.f, 0.f);
    ggo::vec2_f alignment(0.f, 0.f);
    ggo::vec2_f attraction(0.f, 0.f);

    for (const auto & storni2 : _stornis)
    {
      // Check self.
      if (&storni == &storni2)
      {
        continue;
      }

      // Check if visible.
      const ggo::vec2_f diff = storni2._pos - storni._pos;
      const float angle = ggo::angle(storni._vel);
      const float angle2 = ggo::angle(diff);
      if (compare_angles(angle, angle2, 120.f) == false)
      {
        continue;
      }

      const float hypot = ggo::hypot(diff);

      // Repulsion of closest neighbors.
      if (hypot < repulsion_influence_hypot)
      {
        const float factor = 1.f - hypot / repulsion_influence_hypot;
        repulsion -= factor * diff;
      }

      // Align with some more neighbors. 
      if (hypot < alignment_influence_hypot)
      {
        const float factor = 1.f - hypot / alignment_influence_hypot;
        alignment += factor * storni2._vel;
      }

      // Get close to neighbors.
      if (hypot < attraction_influence_hypot)
      {
        const float factor = 1.f - hypot / attraction_influence_hypot;
        attraction += factor * diff;
      }
    }

    // Apply repulsion.
    const float repulsion_hypot = ggo::hypot(repulsion);
    if (repulsion_hypot != 0.0f)
    {
      repulsion *= to<float>(25 * min_size() / (1000 * _fps)) / std::sqrt(repulsion_hypot);

      storni._vel += repulsion;
    }

    // Apply alignment.
    const float aligment_hypot = ggo::hypot(alignment);
    if (aligment_hypot != 0.0f)
    {
      alignment *= to<float>(15 * min_size() / (1000 * _fps)) / std::sqrt(aligment_hypot);
      storni._vel += alignment;
    }

    // Apply attraction.
    const float attraction_hypot = ggo::hypot(attraction);

    if (attraction_hypot != 0.0f)
    {
      attraction *= to<float>(12 * min_size() / (1000 * _fps)) / std::sqrt(attraction_hypot);
      storni._vel += attraction;
    }

    // Avoid obstacles, predators and borders.
    storni.avoid_obstacles(width(), height(), _obstacles);
    storni.avoid_stornis(width(), height(), _predators, predator_visibility_hypot, 0.003f);
    storni.avoid_cursor(width(), height(), cursor_pos, cursor_visibility_hypot, 0.002f);
    storni.avoid_borders(width(), height(), border_margin);

    // Clamp velocity.
    storni.clamp_velocity(velocity_hypot_max);
  }
}

//////////////////////////////////////////////////////////////
void ggo::storni_realtime_artist::preprocess_frame(void * buffer, uint32_t cursor_events, ggo::pos2_i cursor_pos)
{
  // Update items.
  const float velocity_hypot_max = get_velocity_hypot_max();
  const float border_margin = 0.04f * min_size();

  update_predators(velocity_hypot_max, border_margin);
  update_stornis(velocity_hypot_max, border_margin, cursor_pos);

  // Move stornis and predators once all have been updated.
  for (auto & storni : _stornis)
  {
    storni._pos += storni._vel;
  }
  for (auto & predator : _predators)
  {
    predator._pos += predator._vel;
  }
}

//////////////////////////////////////////////////////////////
bool ggo::storni_realtime_artist::finished()
{
  _elapsed_time += 1 / _fps;
  return _elapsed_time > 10;
}

//////////////////////////////////////////////////////////////
void ggo::storni_realtime_artist::render_tile(void * buffer, const ggo::rect_int & clipping)
{
  const float storni_size = 0.01f * std::sqrt(float(width() * height()));
  const float predator_size = 0.03f * std::sqrt(float(width() * height()));

  if (pixel_type() == ggo::pixel_type::bgrx_8u && memory_lines_order() == ggo::lines_order::down)
  {
    fade_background_to_white<ggo::pixel_type::bgrx_8u, ggo::lines_order::down>(clipping);
    paint_stornies_background<ggo::pixel_type::bgrx_8u, ggo::lines_order::down, ggo::sampling_2x2>(clipping);
    blit_background<ggo::pixel_type::bgrx_8u, ggo::lines_order::down>(buffer, clipping);
    paint_obstacles<ggo::pixel_type::bgrx_8u, ggo::lines_order::down>(buffer, clipping, _elapsed_time);
    paint_stornies<ggo::pixel_type::bgrx_8u, ggo::lines_order::down, ggo::sampling_2x2>(buffer, _predators, predator_size, clipping);
    paint_stornies<ggo::pixel_type::bgrx_8u, ggo::lines_order::down, ggo::sampling_2x2>(buffer, _stornis, storni_size, clipping);
  }
  else if (pixel_type() == ggo::pixel_type::rgb_8u && memory_lines_order() == ggo::lines_order::up)
  {
    fade_background_to_white<ggo::pixel_type::rgb_8u, ggo::lines_order::up>(clipping);
    paint_stornies_background<ggo::pixel_type::rgb_8u, ggo::lines_order::up, ggo::sampling_2x2>(clipping);
    blit_background<ggo::pixel_type::rgb_8u, ggo::lines_order::up>(buffer, clipping);
    paint_obstacles<ggo::pixel_type::rgb_8u, ggo::lines_order::up>(buffer, clipping, _elapsed_time);
    paint_stornies<ggo::pixel_type::rgb_8u, ggo::lines_order::up, ggo::sampling_2x2>(buffer, _predators, predator_size, clipping);
    paint_stornies<ggo::pixel_type::rgb_8u, ggo::lines_order::up, ggo::sampling_2x2>(buffer, _stornis, storni_size, clipping);
  }
  else
  {
    GGO_FAIL();
  }
}

//////////////////////////////////////////////////////////////
template <ggo::pixel_type pixel_type, ggo::lines_order memory_lines_order>
void ggo::storni_realtime_artist::fade_background_to_white(const ggo::rect_int & clipping)
{
  ggo::image_t<pixel_type, memory_lines_order> img_bkgd(_background_buffer.data(), size(), line_byte_step());

  auto fade = [](uint8_t v)
  {
    return std::min(0xff, v + (0xff - v) / 16 + 1);
  };

  for (int y = clipping.bottom(); y <= clipping.top(); ++y)
  {
    void * ptr_left = img_bkgd.pixel_ptr(clipping.left(), y);
    void * ptr_right = img_bkgd.pixel_ptr(clipping.right(), y);

    for (void * ptr = ptr_left; ptr <= ptr_right; ptr = move_ptr(ptr, pixel_type_traits<pixel_type>::pixel_byte_size))
    {
      auto pixel = ggo::pixel_type_traits<pixel_type>::read(ptr);

      pixel.r() = fade(pixel.r());
      pixel.g() = fade(pixel.g());
      pixel.b() = fade(pixel.b());

      ggo::pixel_type_traits<pixel_type>::write(ptr, pixel);
    }
  }
}

//////////////////////////////////////////////////////////////
template <ggo::pixel_type pixel_type, ggo::lines_order memory_lines_order, ggo::pixel_sampling sampling>
void ggo::storni_realtime_artist::paint_stornies_background(const ggo::rect_int & clipping)
{
  ggo::image_t<pixel_type, memory_lines_order> img_bkgd(_background_buffer.data(), size(), line_byte_step());

  alpha_blender_rgb8u blender(0.4f);
  const float storni_radius = 0.005f * min_size();
  for (const auto & storni : _stornis)
  {
    // Paint 2 stamps.
    ggo::paint<sampling>(img_bkgd, ggo::disc_f(storni._pos, storni_radius),
      ggo::make_solid_brush(storni._color), blender, clipping);

    ggo::paint<sampling>(img_bkgd, ggo::disc_f(storni._pos - 0.5f * storni._vel, storni_radius),
      ggo::make_solid_brush(storni._color), blender, clipping);
  }
}

//////////////////////////////////////////////////////////////
template <ggo::pixel_type pixel_type, ggo::lines_order memory_lines_order>
void ggo::storni_realtime_artist::blit_background(void * buffer, const ggo::rect_int & clipping) const
{
  ggo::const_image_t<pixel_type, memory_lines_order> img_bkgd(_background_buffer.data(), size(), line_byte_step());
  ggo::image_t<pixel_type, memory_lines_order> img(buffer, size(), line_byte_step());

  auto bkgd_view = make_image_view(img_bkgd, clipping);
  auto img_view = make_image_view(img, clipping);

  ggo::blit(*bkgd_view, *img_view);
}

//////////////////////////////////////////////////////////////
template <ggo::pixel_type pixel_type, ggo::lines_order memory_lines_order, ggo::pixel_sampling sampling>
void ggo::storni_realtime_artist::paint_stornies(void * buffer, const std::vector<storni> & stornis, float storni_size, const ggo::rect_int & clipping) const
{
  const float border_size = storni_size * 0.05f;

  for (const auto & storni : stornis)
  {
    if (storni._vel.x() != 0.f || storni._vel.y() != 0.f)
    {
      ggo::image_t<pixel_type, memory_lines_order> img(buffer, size(), line_byte_step());

      const ggo::vec2_f direction = ggo::normalize(storni._vel) * storni_size;

      const ggo::vec2_f v1{ storni._pos + direction };
      const ggo::vec2_f v2{ storni._pos + 0.5f * ggo::vec2_f(direction.y(), -direction.x()) };
      const ggo::vec2_f v3{ storni._pos + 0.5f * ggo::vec2_f(-direction.y(), direction.x()) };

      ggo::scene2d<ggo::rgb_8u> scene;
      scene.make_paint_shape(triangle2d_f({ v1, v2, v3 }), storni._color);
      scene.make_paint_shape(capsule_f({ v1, v2, border_size }), ggo::black_8u());
      scene.make_paint_shape(capsule_f({ v2, v3, border_size }), ggo::black_8u());
      scene.make_paint_shape(capsule_f({ v3, v1, border_size }), ggo::black_8u());

      ggo::paint<sampling>(img, scene, clipping);
    }
  }
}

//////////////////////////////////////////////////////////////
template <ggo::pixel_type pixel_type, ggo::lines_order memory_lines_order>
void ggo::storni_realtime_artist::paint_obstacles(void * buffer, const ggo::rect_int & clipping, ggo::ratio elapsed_time) const
{
  const float obstacle_hypot = get_obstacle_hypot(width(), height());
  const float obstacle_hypot_inv = 1.f / obstacle_hypot;
  const float phase = to<float>(10 * elapsed_time);
  const ggo::rgb_8u color = ggo::from_hsv<ggo::rgb_8u>(_hue, 1.f, 1.f);

  image_t<pixel_type, memory_lines_order> img(buffer, size(), line_byte_step());

  for (const auto & obstacle : _obstacles)
  {
    const ggo::rect_data<float> obstacle_rect = ggo::rect_data_from_left_right_bottom_top(
      obstacle.x() - obstacle_hypot, obstacle.x() + obstacle_hypot,
      obstacle.y() - obstacle_hypot, obstacle.y() + obstacle_hypot);

    ggo::rect_int obstacle_pixel_rect = from_continuous_to_discrete_exclusive(obstacle_rect);
    obstacle_pixel_rect.clip(width(), height());

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
          auto pixel = img.read_pixel(x, y);
          pixel = blender(x, y, pixel, color);
          img.write_pixel(x, y, pixel);
        }
      });
    }
  }
}

