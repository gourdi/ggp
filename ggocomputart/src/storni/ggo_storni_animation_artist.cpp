#include "ggo_storni_animation_artist.h"
#include <ggo_buffer_paint.h>
#include <ggo_buffer_fill.h>

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
void ggo::storni_animation_artist::storni::avoid_obstacles(int width, int height, const std::vector<ggo::pos2f> & obstacles)
{
  const float obstacle_influence_hypot = ggo::square(0.12f) * width * height;

  ggo::vec2f obstacle_force{ 0.f, 0.f };
  for (const auto & obstacle : obstacles)
  {
    const ggo::vec2f diff_obstacle(obstacle - _pos);
    const float hypot_obstacle = diff_obstacle.get_hypot();
    if (hypot_obstacle < obstacle_influence_hypot)
    {
      const float factor = 1.f - hypot_obstacle / obstacle_influence_hypot;
      obstacle_force += factor * diff_obstacle;
    }
  }
  const float obstacle_hypot = obstacle_force.get_hypot();
  if (obstacle_hypot != 0.f)
  {
    obstacle_force *= 0.0008f * std::min(width, height) / std::sqrt(obstacle_hypot);
    _vel -= obstacle_force;
  }
}

//////////////////////////////////////////////////////////////
void ggo::storni_animation_artist::storni::avoid_stornis(int width, int height, const std::vector<storni> & stornis, float influence_hypot, float weight)
{
  ggo::vec2f force{ 0.f, 0.f };
  for (const auto & storni : stornis)
  {
    if (&storni != this)
    {
      const ggo::vec2f diff(storni._pos - _pos);
      const float hypot = diff.get_hypot();
      if (hypot < influence_hypot)
      {
        const float factor = 1.f - hypot / influence_hypot;
        force += factor * diff;
      }
    }
  }
  const float force_hypot = force.get_hypot();
  if (force_hypot != 0.f)
  {
    force *= weight * std::min(width, height) / std::sqrt(force_hypot);
    _vel -= force;
  }
}

//////////////////////////////////////////////////////////////
void ggo::storni_animation_artist::storni::avoid_borders(int width, int height, float margin)
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
void ggo::storni_animation_artist::storni::clamp_velocity(float velocity_hypot_min, float velocity_hypot_max)
{
  const float velocity_hypot = _vel.get_hypot();

  if (velocity_hypot >= velocity_hypot_max)
  {
    _vel *= std::sqrt(velocity_hypot_max / velocity_hypot);
  }
  else if (velocity_hypot <= velocity_hypot_min)
  {
    _vel *= std::sqrt(velocity_hypot_min / velocity_hypot);
  }
}

//////////////////////////////////////////////////////////////
ggo::storni_animation_artist::storni_animation_artist(int width, int height, int line_step, ggo::pixel_buffer_format pbf, rendering_type rt)
:
animation_artist_abc(width, height, line_step, pbf, rt),
_stornis_buffer(width * height),
_background(width * line_step)
{

}

//////////////////////////////////////////////////////////////
void ggo::storni_animation_artist::update_predators(float velocity_hypot_min, float velocity_hypot_max, float border_margin)
{
  const float pray_visibility_hypot = ggo::square(0.12f) * get_width() * get_height();
  const float repulsion_influence_hypot = ggo::square(0.05f) * get_width() * get_height();

  for (auto & predator : _predators)
  {
    ggo::vec2f attraction(0.f, 0.f);

    for (const auto & pray : _stornis)
    {
      // Check if pray is visible.
      const ggo::vec2f diff = pray._pos - predator._pos;
      const float angle = ggo::get_angle(predator._vel);
      const float angle2 = ggo::get_angle(diff);
      if (compare_angles(angle, angle2, 120.f) == false)
      {
        continue;
      }

      // Chase the prays.
      const float hypot = diff.get_hypot();
      if (hypot < pray_visibility_hypot)
      {
        const float factor = 1.f - hypot / pray_visibility_hypot;
        attraction += factor * diff;
      }
    }

    // Apply attraction.
    const float attraction_hypot = attraction.get_hypot();
    if (attraction_hypot != 0.0f)
    {
      attraction *= 0.002f * get_min_size() / std::sqrt(attraction_hypot);
      predator._vel += attraction;
    }

    // Avoid obstacles, other predators and borders.
    predator.avoid_obstacles(get_width(), get_height(), _obstacles);
    predator.avoid_stornis(get_width(), get_height(), _predators, repulsion_influence_hypot, 0.003f);
    predator.avoid_borders(get_width(), get_height(), border_margin);

    // Clamp velocity.
    predator.clamp_velocity(velocity_hypot_min, velocity_hypot_max);
  }
}

//////////////////////////////////////////////////////////////
void ggo::storni_animation_artist::update_stornis(float velocity_hypot_min, float velocity_hypot_max, float border_margin)
{
  const float repulsion_influence_hypot = ggo::square(0.02f) * get_width() * get_height();
  const float alignment_influence_hypot = ggo::square(0.04f) * get_width() * get_height();
  const float attraction_influence_hypot = ggo::square(0.05f) * get_width() * get_height();
  const float predator_visibility_hypot = ggo::square(0.05f) * get_width() * get_height();

  for (auto & storni : _stornis)
  {
    ggo::vec2f repulsion(0.f, 0.f);
    ggo::vec2f alignment(0.f, 0.f);
    ggo::vec2f attraction(0.f, 0.f);

    for (const auto & storni2 : _stornis)
    {
      // Check self.
      if (&storni == &storni2)
      {
        continue;
      }

      // Check if visible.
      const ggo::vec2f diff = storni2._pos - storni._pos;
      const float angle = ggo::get_angle(storni._vel);
      const float angle2 = ggo::get_angle(diff);
      if (compare_angles(angle, angle2, 120.f) == false)
      {
        continue;
      }

      const float hypot = diff.get_hypot();

      // Repulsion of closent neighbors.
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
    const float repulsion_hypot = repulsion.get_hypot();
    if (repulsion_hypot != 0.0f)
    {
      repulsion *= 0.0009f * get_min_size() / std::sqrt(repulsion_hypot);
      storni._vel += repulsion;
    }

    // Apply alignment.
    const float aligment_hypot = alignment.get_hypot();
    if (aligment_hypot != 0.0f)
    {
      alignment *= 0.0006f * get_min_size() / std::sqrt(aligment_hypot);
      storni._vel += alignment;
    }

    // Apply attraction.
    const float attraction_hypot = attraction.get_hypot();

    if (attraction_hypot != 0.0f)
    {
      attraction *= 0.0005f * get_min_size() / std::sqrt(attraction_hypot);
      storni._vel += attraction;
    }

    // Avoid obstacles, predators and borders.
    storni.avoid_obstacles(get_width(), get_height(), _obstacles);
    storni.avoid_stornis(get_width(), get_height(), _predators, predator_visibility_hypot, 0.003f);
    storni.avoid_borders(get_width(), get_height(), border_margin);

    // Clamp velocity.
    storni.clamp_velocity(velocity_hypot_min, velocity_hypot_max);
  }
}

//////////////////////////////////////////////////////////////
void ggo::storni_animation_artist::init_animation()
{
  _frame_index = -1;

  _hue = ggo::rand<float>();

  _background_colors[0] = ggo::from_hsv<ggo::color_8u>(_hue, 1.f, ggo::rand<float>(0.f, 0.75f));
  _background_colors[1] = ggo::from_hsv<ggo::color_8u>(_hue, 1.f, ggo::rand<float>(0.f, 0.75f));
  _background_colors[2] = ggo::from_hsv<ggo::color_8u>(_hue, 1.f, ggo::rand<float>(0.f, 0.75f));
  _background_colors[3] = ggo::from_hsv<ggo::color_8u>(_hue, 1.f, ggo::rand<float>(0.f, 0.75f));

  std::fill(_stornis_buffer.data(), _stornis_buffer.data() + get_width() * get_height(), 0);

  _stornis.clear();
  switch (get_rendering_type())
  {
  case ggo::animation_artist_abc::offscreen_rendering:
    for (int i = 0; i < 512; ++i)
    {
      _stornis.emplace_back(get_random_point(), ggo::vec2f(0.f, 0.f));
    }
    break;
  case ggo::animation_artist_abc::realtime_rendering_android:
  case ggo::animation_artist_abc::realtime_rendering_pc:
    for (int i = 0; i < 256; ++i)
    {
      _stornis.emplace_back(get_random_point(), ggo::vec2f(0.f, 0.f));
    }
    break;
  default:
    throw ggo::invalid_parameter_exception();
    break;
  }

  _predators.clear();
  for (int i = 0; i < 2; ++i)
  {
    _predators.emplace_back(get_random_point(), ggo::vec2f(0.f, 0.f));
  }

  _obstacles.clear();
  for (int i = 0; i < 4; ++i)
  {
    while (true)
    {
      const ggo::pos2f new_obstacle = get_random_point(0.1f * get_min_size());

      bool inserted = true;
      for (const auto & obstacle : _obstacles)
      {
        if (ggo::hypot(new_obstacle, obstacle) < 4 * get_obstacle_hypot())
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
bool ggo::storni_animation_artist::prepare_frame()
{
  ++_frame_index;

  if (_frame_index > 800)
  {
    return false;
  }

  // Update items.
  const float velocity_hypot_min = ggo::square(0.003f * get_min_size());
  const float velocity_hypot_max = ggo::square(0.004f * get_min_size());
  const float border_margin = 0.04f * get_min_size();

  update_predators(velocity_hypot_min, velocity_hypot_max, border_margin);
  update_stornis(velocity_hypot_min, velocity_hypot_max, border_margin);

  // Move stornis and predators once all have been updated.
  for (auto & storni : _stornis)
  {
    storni._pos += storni._vel;
  }
  for (auto & predator : _predators)
  {
    predator._pos += predator._vel;
  }

  return true;
}

//////////////////////////////////////////////////////////////
void ggo::storni_animation_artist::render_frame(void * buffer, const ggo::rect_int & clipping)
{
  if (_frame_index == 0)
  {
    switch (get_pixel_buffer_format())
    {
    case ggo::rgb_8u_yu:
      ggo::fill_4_colors<ggo::rgb_8u_yu>(_background.data(), get_width(), get_height(), get_line_step(),
        _background_colors[0], _background_colors[1], _background_colors[2], _background_colors[3],
        clipping);
      break;
    case ggo::bgra_8u_yd:
      ggo::fill_4_colors<ggo::bgra_8u_yd>(_background.data(), get_width(), get_height(), get_line_step(),
        _background_colors[0], _background_colors[1], _background_colors[2], _background_colors[3],
        clipping);
      break;
    default:
      GGO_FAIL();
      break;
    }
  }

  switch (get_pixel_buffer_format())
  {
  case ggo::rgb_8u_yu:
    blit_background<ggo::rgb_8u_yu>(buffer, clipping);

    if (get_rendering_type() == ggo::animation_artist_abc::offscreen_rendering)
    {
      paint_stornies<ggo::rgb_8u_yu, ggo::sampling_8x8>(buffer, clipping);
      paint_predators<ggo::rgb_8u_yu, ggo::sampling_8x8>(buffer, clipping);
    }
    else
    {
      paint_stornies<ggo::rgb_8u_yu, ggo::sampling_2x2>(buffer, clipping);
      paint_predators<ggo::rgb_8u_yu, ggo::sampling_2x2>(buffer, clipping);
    }
    paint_obstacles<ggo::rgb_8u_yu>(buffer, clipping, _frame_index);
    break;
  case ggo::bgra_8u_yd:
    blit_background<ggo::bgra_8u_yd>(buffer, clipping);

    if (get_rendering_type() == ggo::animation_artist_abc::offscreen_rendering)
    {
      paint_stornies<ggo::bgra_8u_yd, ggo::sampling_8x8>(buffer, clipping);
      paint_predators<ggo::bgra_8u_yd, ggo::sampling_8x8>(buffer, clipping);
    }
    else
    {
      paint_stornies<ggo::bgra_8u_yd, ggo::sampling_2x2>(buffer, clipping);
      paint_predators<ggo::bgra_8u_yd, ggo::sampling_2x2>(buffer, clipping);
    }
    paint_obstacles<ggo::bgra_8u_yd>(buffer, clipping, _frame_index);
    break;
  default:
    GGO_FAIL();
    break;
  }
}
