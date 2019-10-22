#include "ggo_kanji_realtime_artist.h"
#include <2d/fill/ggo_fill.h>
#include <2d/paint/ggo_paint.h>
#include <2d/paint/ggo_brush.h>
#include <2d/paint/ggo_blend.h>

//////////////////////////////////////////////////////////////
ggo::kanji_realtime_artist::kanji_realtime_artist(int width, int height, int line_byte_step, ggo::pixel_type pixel_type, ggo::lines_order memory_lines_order)
:
fixed_frames_count_realtime_artist_abc(width, height, line_byte_step, pixel_type, memory_lines_order)
{
  _parts_color = ggo::from_hsv<ggo::rgb_8u>(ggo::rand<float>(0, 1), ggo::rand<float>(0, 1), 1);
  _timer_max = ggo::rand<int>(500, 750);

  // Create the attractor.
  _attractor = { ggo::rand<float>(), ggo::rand<float>() };
  _attractor_counter = 10;
  _shake_counter = 5;

  // Create the particles.
  ggo::pos2_f pos(ggo::rand<float>(0.3f, 0.7f), ggo::rand<float>(0.3f, 0.7f));

  for (int i = 0; i < 400; ++i)
  {
    const float a = ggo::rand<float>(0, 2 * ggo::pi<float>());
    const float l = ggo::rand<float>(0.f, 0.02f);

    std::array<ggo::pos2_f, substeps_count> particle;
    particle.fill(pos + ggo::vec2_f(l * cos(a), l * sin(a)));
    _particles.push_back(particle);
  }
}

//////////////////////////////////////////////////////////////
void ggo::kanji_realtime_artist::preprocess_frame(int frame_index, uint32_t cursor_events, ggo::pos2_i cursor_pos, float time_step)
{
  // Update the particles system.
  for (auto & particle : _particles)
  {
    ggo::pos2_f cur_pos = particle[particle.size() - 1];
    ggo::pos2_f prv_pos = particle[particle.size() - 2];

    for (int substep = 0; substep < substeps_count; ++substep)
    {
      // The further the attractor, the more the particle is attracted.
      ggo::vec2_f force = _attractor - cur_pos;

      // Clamp force.
      float l = length(force);
      if (l > 0)
      {
        force /= l; // Normalize;
        force *= 0.000005f;
      }

      // Newton formula.
      particle[substep] = 2.f * cur_pos - prv_pos + force;

      prv_pos = cur_pos;
      cur_pos = particle[substep];
    }
  }

  // Shake particles a bit...
  if (--_shake_counter <= 0)
  {
    for (auto & particle : _particles)
    {
      particle[particle.size() - 2] = particle[particle.size() - 1];
      float a = ggo::rand<float>(0, 2 * ggo::pi<float>());
      float l = ggo::rand<float>(0.8f, 1 / 0.8f) * 0.00005f;
      particle[particle.size() - 1] = particle[particle.size() - 2] + ggo::vec2_f(l * cos(a), l * sin(a));
    }

    _shake_counter = ggo::rand<int>(30, 80);
  }

  // Update attractor.
  if (--_attractor_counter <= 0)
  {
    float hypot_max = 0.f;
    ggo::pos2_f new_attractor;
    for (int i = 0; i < 4; ++i)
    {
      ggo::pos2_f pos{ ggo::rand<float>(), ggo::rand<float>() };
      float hypot_cur = ggo::hypot(_particles.front().back(), _attractor);
      if (hypot_cur > hypot_max)
      {
        hypot_max = hypot_cur;
        new_attractor = pos;
      }
    }
    _attractor = new_attractor;

    _attractor_counter = ggo::rand<int>(20, 50);
  }
}

//////////////////////////////////////////////////////////////
template <ggo::pixel_type pixel_type, ggo::lines_order memory_lines_order>
void ggo::kanji_realtime_artist::render_tile_t(void * buffer, int frame_index, const ggo::rect_int & clipping)
{
  ggo::image_t<pixel_type, memory_lines_order> img(buffer, size(), line_byte_step());

  if (frame_index == 0)
  {
    ggo::fill_black(img);
  }

  const float radius = 0.0005f * min_size();
  const auto brush = ggo::make_solid_brush(_parts_color);
  const ggo::alpha_blender_rgb8u alpha_blender(0.2f);

  for (const auto & particle : _particles)
  {
    for (const auto & pos : particle)
    {
      ggo::pos2_f render_pt = map_fit(pos, 0, 1);

      ggo::paint<ggo::sampling_4x4>(img, ggo::disc_f(render_pt, radius), brush, alpha_blender, clipping);
    }
  }
}

//////////////////////////////////////////////////////////////
void ggo::kanji_realtime_artist::render_tile(void * buffer, int frame_index, const ggo::rect_int & clipping)
{
  if (pixel_type() == ggo::pixel_type::bgrx_8u && memory_lines_order() == ggo::lines_order::down)
  {
    render_tile_t<ggo::pixel_type::bgrx_8u, ggo::lines_order::down>(buffer, frame_index, clipping);
  }
  else if (pixel_type() == ggo::pixel_type::rgb_8u && memory_lines_order() == ggo::lines_order::up)
  {
    render_tile_t<ggo::pixel_type::rgb_8u, ggo::lines_order::up>(buffer, frame_index, clipping);
  }
  else if (pixel_type() == ggo::pixel_type::rgb_8u && memory_lines_order() == ggo::lines_order::down)
  {
    render_tile_t<ggo::pixel_type::rgb_8u, ggo::lines_order::down>(buffer, frame_index, clipping);
  }
  else
  {
    GGO_FAIL();
  }
}

