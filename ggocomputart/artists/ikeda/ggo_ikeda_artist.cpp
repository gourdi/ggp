#include "ggo_ikeda_artist.h"
#include <ggo_brush_abc.h>
#include <ggo_fill.h>
#include <ggo_paint.h>

#define GGO_IKEDA_FRAMES_COUNT 200

//////////////////////////////////////////////////////////////
ggo_ikeda_artist::ggo_ikeda_artist(int render_width, int render_height)
:
ggo_animation_artist_abc(render_width, render_height)
{

}

//////////////////////////////////////////////////////////////
void ggo_ikeda_artist::init_sub()
{
	_u0.set_harmonic(0, 0, 0);
	_u0.set_harmonic(1, ggo::rand_float(-1, 1), 0);
	_u0.set_harmonic(2, ggo::rand_float(-1, 1), 0);
	_u0.set_harmonic(3, ggo::rand_float(-1, 1), 0);

	_u1.set_harmonic(0, 0, 0);
	_u1.set_harmonic(1, ggo::rand_float(-1, 1), 0);
	_u1.set_harmonic(2, ggo::rand_float(-1, 1), 0);
	_u1.set_harmonic(3, ggo::rand_float(-1, 1), 0);

	_u2.set_harmonic(0, 0, 0);
	_u2.set_harmonic(1, ggo::rand_float(-10, 10), 0);
	_u2.set_harmonic(2, ggo::rand_float(-10, 10), 0);
	_u2.set_harmonic(3, ggo::rand_float(-10, 10), 0);

	_hue1 = ggo::rand_float();
	_hue2 = ggo::rand_float();
	_range = ggo::rand_float(3, 5);
	
	_seeds.clear();
	for (int i = 0; i < 500; ++i) 
	{
		_seeds.push_back(create_seed());
	}
	
	_bkgd_colors[0] = ggo::color::from_hsv(0, 0, ggo::rand_float(0, 0.5));
	_bkgd_colors[1] = ggo::color::from_hsv(0, 0, ggo::rand_float(0, 0.5));
	_bkgd_colors[2] = ggo::color::from_hsv(0, 0, ggo::rand_float(0, 0.5));
	_bkgd_colors[3] = ggo::color::from_hsv(0, 0, ggo::rand_float(0, 0.5));
}

//////////////////////////////////////////////////////////////
bool ggo_ikeda_artist::render_next_frame_sub(uint8_t * buffer, int frame_index)
{
	if (frame_index > GGO_IKEDA_FRAMES_COUNT)
	{
		return false;
	}
	
  auto image_buffer = make_image_buffer(buffer);
  
	ggo::fill_4_colors(image_buffer, _bkgd_colors[0], _bkgd_colors[1], _bkgd_colors[2], _bkgd_colors[3]);
	
	std::vector<ggo_particle> particles = _seeds;
	
	float u0 = _u0.evaluate(frame_index * ggo::pi<float>() / GGO_IKEDA_FRAMES_COUNT);
	float u1 = _u1.evaluate(frame_index * ggo::pi<float>() / GGO_IKEDA_FRAMES_COUNT);
	float u2 = _u2.evaluate(frame_index * ggo::pi<float>() / GGO_IKEDA_FRAMES_COUNT);
	
	while (particles.empty() == false)
	{
		for (int i = static_cast<int>(particles.size()) - 1; i >= 0; --i)
		{
			ggo_particle & particle = particles[i];
			
			float k = 1 + particle._pos.get<0>() * particle._pos.get<0>() + particle._pos.get<1>() * particle._pos.get<1>();
			if (std::abs(k) < 0.00001)
			{
				particles.erase(particles.begin() + i);
				continue;
			}
				
			float t = u1 - u2 / k;
		
			ggo::pos2f next_pt;
			next_pt.get<0>() = 1 + u0 * (particle._pos.get<0>() * cos(t) - particle._pos.get<1>() * sin(t));
			next_pt.get<1>() =     u0 * (particle._pos.get<0>() * sin(t) - particle._pos.get<1>() * cos(t));

			float speed = ggo::distance(particle._pos, next_pt);
			particle._radius = speed;

			if (speed < 0.05)
			{
				particles.erase(particles.begin() + i);
				continue;
			}
			
			// Paint the point.
			ggo::pos2f point = map_fill(particle._pos, -_range, _range);
				
			float radius = 0.0025f * particle._radius * get_render_max_size();
			radius = std::max(1.5f, radius);

      ggo::paint(image_buffer, std::make_shared<ggo::disc_float>(point, radius), particle._color,0.15f);
				
			// Move points slowly.
			particle._pos.get<0>() += 0.005f * (next_pt.get<0>() - particle._pos.get<0>());
			particle._pos.get<1>() += 0.005f * (next_pt.get<1>() - particle._pos.get<1>());
		}
	}

	return true;
}

//////////////////////////////////////////////////////////////
ggo_ikeda_artist::ggo_particle ggo_ikeda_artist::create_seed() const
{
	ggo_particle particle;

	switch (ggo::rand_int(0, 3))
	{
		case 0:
			particle._pos.get<0>() = ggo::rand_float(-1.5f * _range, 1.5f * _range);
			particle._pos.get<1>() = 1.5f * _range;
			break;
		case 1:
			particle._pos.get<0>() = ggo::rand_float(-1.5f * _range, 1.5f * _range);
			particle._pos.get<1>() = -1.5f * _range;
			break;
		case 2:
			particle._pos.get<0>() = 1.5f * _range;
			particle._pos.get<1>() = ggo::rand_float(-1.5f * _range, 1.5f * _range);
			break;
		case 3:
			particle._pos.get<0>() = -1.5f * _range;
			particle._pos.get<1>() = ggo::rand_float(-1.5f * _range, 1.5f * _range);
			break;
	}

	if (ggo::rand_bool())
	{
		particle._color = ggo::color::from_hsv(_hue1, ggo::rand_float(0.4f, 0.6f), ggo::rand_float(0.4f, 1.f));
	}
	else
	{
		particle._color = ggo::color::from_hsv(_hue2, ggo::rand_float(0.4f, 0.6f), ggo::rand_float(0.4f, 1.f));
	}

	return particle;
}


