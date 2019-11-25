#include "ggo_kanji_realtime_artist.h"
#include <2d/fill/ggo_fill.h>
#include <2d/paint/ggo_paint.h>
#include <2d/paint/ggo_brush.h>
#include <2d/paint/ggo_blend.h>

//////////////////////////////////////////////////////////////
ggo::kanji_realtime_artist::kanji_realtime_artist(int width, int height, int line_byte_step, ggo::pixel_type pixel_type, ggo::lines_order memory_lines_order, ggo::ratio fps)
:
realtime_artist(width, height, line_byte_step, pixel_type, memory_lines_order)
{
  _substeps_per_frame = 500 / fps;

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

    const ggo::pos2_f p = pos + l * ggo::vec2_f::from_angle(a);

    _particles.push_back({ p, p });
  }
}

//////////////////////////////////////////////////////////////
void ggo::kanji_realtime_artist::preprocess_frame(void * buffer, uint32_t cursor_events, ggo::pos2_i cursor_pos)
{
  if (_substeps_count == 0)
  {
    memset(buffer, 0, line_byte_step() * height());
  }

  _points.clear();

  _substeps += to<float>(_substeps_per_frame);

  for (; _substeps >= 1.f; _substeps -= 1.f, ++_substeps_count)
  {
    // Update the particles system.
    for (auto & particle : _particles)
    {
      // The further the attractor, the more the particle is attracted.
      ggo::vec2_f force = _attractor - particle._cur_pos;

      // Clamp force.
      float l = length(force);
      if (l > 0)
      {
        force /= l; // Normalize;
        force *= 0.000005f;
      }

      // Newton formula.
      auto nxt_pos = 2.f * particle._cur_pos - particle._prv_pos + force;
      particle._prv_pos = particle._cur_pos;
      particle._cur_pos = nxt_pos;

      _points.push_back(nxt_pos);
    }

    // Shake particles a bit...
    if (--_shake_counter <= 0)
    {
      for (auto & particle : _particles)
      {
        particle._prv_pos = particle._cur_pos;
        float a = ggo::rand<float>(0, 2 * ggo::pi<float>());
        float l = ggo::rand<float>(0.8f, 1 / 0.8f) * 0.00005f;
        particle._cur_pos = particle._prv_pos + l * ggo::vec2_f::from_angle(a);
      }

      _shake_counter = ggo::rand<int>(300, 800);
    }

    // Update attractor.
    if (--_attractor_counter <= 0)
    {
      float hypot_max = 0.f;
      ggo::pos2_f new_attractor;
      for (int i = 0; i < 4; ++i)
      {
        ggo::pos2_f pos{ ggo::rand<float>(), ggo::rand<float>() };
        float hypot_cur = ggo::hypot(_particles.front()._cur_pos, _attractor);
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
}

//////////////////////////////////////////////////////////////
template <ggo::pixel_type pixel_type, ggo::lines_order memory_lines_order>
void ggo::kanji_realtime_artist::render_tile_t(void * buffer, const ggo::rect_int & clipping)
{
  ggo::image_t<pixel_type, memory_lines_order> img(buffer, size(), line_byte_step());

  const float radius = 0.0005f * min_size();
  const auto brush = ggo::make_solid_brush(_parts_color);
  const ggo::alpha_blender_rgb8u alpha_blender(0.2f);

  for (const auto & p : _points)
  {
    ggo::pos2_f render_pt = map_fit(p, 0, 1);

    ggo::paint<ggo::sampling_4x4>(img, ggo::disc_f(render_pt, radius), brush, alpha_blender, clipping);
  }
}

//////////////////////////////////////////////////////////////
void ggo::kanji_realtime_artist::render_tile(void * buffer, const ggo::rect_int & clipping)
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
bool ggo::kanji_realtime_artist::finished()
{
  return _substeps_count > 6000;
}


