#include "ggo_kanji_artist.h"
#include <ggo_buffer_paint.h>
#include <ggo_brush.h>
#include <ggo_blend.h>

//////////////////////////////////////////////////////////////
ggo::kanji_artist::kanji_artist(int width, int height)
:
artist(width, height)
{
}

//////////////////////////////////////////////////////////////
void ggo::kanji_artist::init_animation()
{
	_parts_color	= ggo::from_hsv<ggo::color_8u>(ggo::rand<float>(0, 1), ggo::rand<float>(0, 1), 1);
	_timer_max		= ggo::rand<int>(500, 750);

	// Create the attractor.
  _attractor = { ggo::rand<float>(), ggo::rand<float>() };
  _attractor_counter = 10;
  _shake_counter = 5;

	// Create the particles.
	ggo::pos2f pos(ggo::rand<float>(0.3f, 0.7f), ggo::rand<float>(0.3f, 0.7f));
	
	_particles.clear();
	
	for (int i = 0; i < 400; ++i)
	{
		const float a = ggo::rand<float>(0, 2 * ggo::pi<float>());
		const float l = ggo::rand<float>(0.f, 0.02f);
		
		std::array<ggo::pos2f, substeps_count> particle;
		particle.fill(ggo::vec2f(l * cos(a), l * sin(a)));
		_particles.push_back(particle);
	}
}

//////////////////////////////////////////////////////////////
bool ggo::kanji_artist::prepare_frame(int frame_index)
{
  if (frame_index >= _timer_max)
  {
    return false;
  }

  // Update the particles system.
  for (auto & particle : _particles)
  {
    ggo::pos2f cur_pos = particle[particle.size() - 1];
    ggo::pos2f prv_pos = particle[particle.size() - 2];

    for (int substep = 0; substep < substeps_count; ++substep)
    {
      // The further the attractor, the more the particle is attracted.
      ggo::vec2f force = _attractor - cur_pos;

      // Clamp force.
      float l = force.get_length();
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
      particle[particle.size() - 1] = particle[particle.size() - 2] + ggo::vec2f(l * cos(a), l * sin(a));
    }

    _shake_counter = ggo::rand<int>(30, 80);
  }

  // Update attractor.
  if (--_attractor_counter <= 0)
  {
    float hypot_max = 0.f;
    ggo::pos2f new_attractor;
    for (int i = 0; i < 4; ++i)
    {
      ggo::pos2f pos{ ggo::rand<float>(), ggo::rand<float>() };
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

  return true;
}

//////////////////////////////////////////////////////////////
void ggo::kanji_artist::render_frame(void * buffer, int line_step, ggo::pixel_buffer_format pbf, int frame_index, const ggo::rect_int & clipping) const
{
  const float radius = 0.0005f * get_min_size();
  const auto brush = ggo::make_solid_brush(_parts_color);
  const ggo::alpha_blender_rgb8u alpha_blender(0.2f);

  switch (pbf)
  {
  case ggo::rgb_8u_yu:
    for (const auto & particle : _particles)
    {
      for (const auto & pos : particle)
      {
        ggo::pos2f render_pt = map_fit(pos, 0, 1);  

        ggo::paint_shape<ggo::rgb_8u_yu, ggo::sampling_4x4>(buffer, get_width(), get_height(), line_step,
          ggo::disc_float(render_pt, radius), brush, alpha_blender, clipping, 8, 0);
      }
    }
    break;
  case ggo::bgra_8u_yd:
    for (const auto & particle : _particles)
    {
      for (const auto & pos : particle)
      {
        ggo::pos2f render_pt = map_fit(pos, 0, 1);

        ggo::paint_shape<ggo::bgra_8u_yd, ggo::sampling_4x4>(buffer, get_width(), get_height(), line_step,
          ggo::disc_float(render_pt, radius), brush, alpha_blender, clipping, 8, 0);
      }
    }
    break;
  default:
    GGO_FAIL();
    break;
  }
}
