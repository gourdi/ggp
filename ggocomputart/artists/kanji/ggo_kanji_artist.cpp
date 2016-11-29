#include "ggo_kanji_artist.h"
#include <ggo_buffer_fill.h>
#include <ggo_buffer_paint.h>
#include <ggo_brush.h>
#include <ggo_blender.h>

//////////////////////////////////////////////////////////////
ggo::kanji_artist::kanji_artist(int width, int height, int line_step, ggo::pixel_buffer_format pbf)
:
artist(width, height, line_step, pbf)
{
}

//////////////////////////////////////////////////////////////
void ggo::kanji_artist::init()
{
	_parts_color	= ggo::from_hsv<ggo::color_8u>(ggo::rand<float>(0, 1), ggo::rand<float>(0, 1), 1);
	_timer_max		= ggo::rand<int>(20000, 25000);

	// Create the attractor.
	init_shaker();
	init_attractor();

	// Create the particles.
	ggo::pos2f pos(ggo::rand<float>(0.3f, 0.7f), ggo::rand<float>(0.3f, 0.7f));
	
	_particles.clear();
	
	for (int i = 0; i < 400; ++i)
	{
		float a = ggo::rand<float>(0, 2 * ggo::pi<float>());
		float l = ggo::rand<float>(0, 0.02f);
		
		ggo::kanji_artist::particle particle;
		particle._prv_pos = pos + ggo::vec2f(l * cos(a), l * sin(a));
		particle._cur_pos = pos + ggo::vec2f(l * cos(a), l * sin(a));
		_particles.push_back(particle);
	}
}

//////////////////////////////////////////////////////////////
void ggo::kanji_artist::init_output_buffer(void * buffer) const
{
  switch (get_pixel_buffer_format())
  {
  case ggo::rgb_8u_yu:
    ggo::fill_solid<ggo::rgb_8u_yu>(buffer, get_width(), get_height(), get_line_step(), ggo::black<ggo::color_8u>());
    break;
  case ggo::bgra_8u_yd:
    ggo::fill_solid<ggo::bgra_8u_yd>(buffer, get_width(), get_height(), get_line_step(), ggo::black<ggo::color_8u>());
    break;
	default:
	  GGO_FAIL();
		break;
  }
}

//////////////////////////////////////////////////////////////
bool ggo::kanji_artist::render_frame(void * buffer, int frame_index)
{
	if (frame_index >= _timer_max)
	{
		return false;
	}

	// Update the particles system.
	for (ggo::kanji_artist::particle & particle : _particles)
	{
		// The further the attractor, the more the particle is attracted.
		float dx = particle._cur_pos.get<0>() - _attractor.get<0>();
		float dy = particle._cur_pos.get<1>() - _attractor.get<1>();
		ggo::vec2f force = ggo::vec2f(-dx, -dy);
		
		// Clamp force.
		float l = force.get_length();
		if (l > 0)
		{
			force /= l; // Normalize;
			force *= 0.0000005f;
		}

		// Newton formula.
		ggo::pos2f pos_new	= 2.0f * (particle._cur_pos) - particle._prv_pos + force;

		// Clamp the displacement.
		ggo::vec2f diff = pos_new - particle._cur_pos;
		l = diff.get_length();
		if (l > 0.0002)
		{ 
			diff /= l;
			diff *= 0.0002f;
		}
		
		particle._prv_pos = particle._cur_pos;
		particle._cur_pos += diff;
	}

	// Shake particles a bit...
	if (--_shake_counter <= 0)
	{
		for (ggo::kanji_artist::particle & particle : _particles)
		{
			particle._prv_pos = particle._cur_pos;
			float a = ggo::rand<float>(0, 2 * ggo::pi<float>());
			float l = ggo::rand<float>(0.8f, 1 / 0.8f) * 0.00002f;
			particle._cur_pos	= particle._prv_pos + ggo::vec2f(l * cos(a), l * sin(a));
		}
		
		init_shaker();
	}

	// Update attractor.
	if (--_attractor_counter <= 0)
	{
		init_attractor();
	}

	// Paint the particles.
	float radius = 0.0005f * get_min_size();

	for (const auto & particle :_particles)
	{
		ggo::pos2f render_pt = map_fit(particle._cur_pos, 0, 1);

    switch (get_pixel_buffer_format())
    {
    case ggo::rgb_8u_yu:
      ggo::paint_shape<ggo::rgb_8u_yu, ggo::sampling_4x4>(
        buffer, get_width(), get_height(), get_line_step(),
        ggo::disc_float(render_pt, radius), ggo::make_solid_brush(_parts_color), ggo::alpha_blender_rgb8u(0.02f));
      break;
    case ggo::bgra_8u_yd:
      ggo::paint_shape<ggo::bgra_8u_yd, ggo::sampling_4x4>(
        buffer, get_width(), get_height(), get_line_step(),
        ggo::disc_float(render_pt, radius), ggo::make_solid_brush(_parts_color), ggo::alpha_blender_rgb8u(0.02f));
      break;
		default:
			GGO_FAIL();
			break;
    }
	}
	
	return true;
}

//////////////////////////////////////////////////////////////
void ggo::kanji_artist::init_shaker()
{
	_shake_counter = ggo::rand<int>(1000, 3000);
}

//////////////////////////////////////////////////////////////
void ggo::kanji_artist::init_attractor()
{
	_attractor = ggo::pos2f(ggo::rand<float>(), ggo::rand<float>());
	_attractor_counter = ggo::rand<int>(1000, 2000);
}
