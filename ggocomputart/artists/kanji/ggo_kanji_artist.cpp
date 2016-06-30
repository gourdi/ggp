#include "ggo_kanji_artist.h"
#include <ggo_paint.h>
#include <ggo_fill.h>
#include <ggo_color.h>
#include <algorithm>
using namespace std;

//////////////////////////////////////////////////////////////
ggo_kanji_artist::ggo_kanji_artist(int render_width, int render_height)
:
ggo_artist_abc(render_width, render_height)
{
}

//////////////////////////////////////////////////////////////
void ggo_kanji_artist::init()
{
	_parts_color	= ggo::color::from_hsv(ggo::rand_float(0, 1), ggo::rand_float(0, 1), 1);
	_timer_max		= ggo::rand_int(20000, 25000);

	// Create the attractor.
	init_shaker();
	init_attractor();

	// Create the particles.
	ggo::pos2f pos(ggo::rand_float(0.3f, 0.7f), ggo::rand_float(0.3f, 0.7f));
	
	_particles.clear();
	
	for (int i = 0; i < 400; ++i)
	{
		float a = ggo::rand_float(0, 2 * ggo::PI<float>());
		float l = ggo::rand_float(0, 0.02f);
		
		ggo_particle particle;
		particle._prv_pos = pos + ggo::vec2f(l * cos(a), l * sin(a));
		particle._cur_pos = pos + ggo::vec2f(l * cos(a), l * sin(a));
		_particles.push_back(particle);
	}
}

//////////////////////////////////////////////////////////////
void ggo_kanji_artist::init_output_buffer(uint8_t * buffer) const
{
    ggo::fill_solid_rgb(buffer, get_render_width() * get_render_height(), ggo::color::BLACK);
}

//////////////////////////////////////////////////////////////
bool ggo_kanji_artist::render_frame(uint8_t * buffer, int frame_index)
{
	if (frame_index >= _timer_max)
	{
		return false;
	}

	// Update the particles system.
	for (ggo_particle & particle : _particles)
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
		for (ggo_particle & particle : _particles)
		{
			particle._prv_pos = particle._cur_pos;
			float a = ggo::rand_float(0, 2 * ggo::PI<float>());
			float l = ggo::rand_float(0.8f, 1 / 0.8f) * 0.00002f;
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
	float radius = 0.0005f * get_render_min_size();

	for (const auto & particle :_particles)
	{
		ggo::pos2f render_pt = map_fit(particle._cur_pos, 0, 1);
		
		ggo::paint(buffer, get_render_width(), get_render_height(),
               std::make_shared<ggo::disc_float>(render_pt, radius),
               _parts_color, 0.02f);
	}
	
	return true;
}

//////////////////////////////////////////////////////////////
void ggo_kanji_artist::init_shaker()
{
	_shake_counter = ggo::rand_int(1000, 3000);
}

//////////////////////////////////////////////////////////////
void ggo_kanji_artist::init_attractor()
{
	_attractor = ggo::pos2f(ggo::rand_float(), ggo::rand_float());
	_attractor_counter = ggo::rand_int(1000, 2000);
}
