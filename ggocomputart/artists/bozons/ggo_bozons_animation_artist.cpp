#include "ggo_bozons_animation_artist.h"
#include <ggo_paint.h>
#include <ggo_fill.h>
#include <ggo_color.h>

#define GGO_DRADIUS_AMP 0.00001f
#define GGO_DRADIUS_COUNTER_MIN 40
#define GGO_DRADIUS_COUNTER_MAX 80

#define GGO_DANGLE_AMP (ggo::PI<float>() / 100.f)
#define GGO_DANGLE_COUNTER_MIN 200
#define GGO_DANGLE_COUNTER_MAX 400

//////////////////////////////////////////////////////////////
ggo_bozons_animation_artist::ggo_bozons_animation_artist(int render_width, int render_height)
:
ggo_accumulation_animation_artist_abc(render_width, render_height)
{
	
}

//////////////////////////////////////////////////////////////
bool ggo_bozons_animation_artist::render_next_frame_acc(uint8_t * buffer, int frame_index)
{
	if (frame_index >= _color_discs.size())
	{
		return false;
	}
	
	const std::vector<ggo_color_disc> & color_discs = _color_discs[frame_index];
	
	for (const ggo_color_disc & color_disc : color_discs)
	{
		auto circle = std::make_shared<ggo::disc_float>(color_disc._disc);
		map_fit(*circle, 0, 1);

		ggo::paint(buffer, get_render_width(), get_render_height(), 
               circle,
               color_disc._color, 0.5);
	}
	
	return true;
}

//////////////////////////////////////////////////////////////
// Unroll all the process here and save particles for each frame.
void ggo_bozons_animation_artist::init_sub()
{
  _hue = ggo::rand_float();
  
	// Create particles.
	std::vector<ggo_particle> particles;
	particles.resize(50);

	for (ggo_particle & particle : particles)
	{
		particle._pos.x() = 0.5;
		particle._pos.y() = 0.5;

		particle._color	= ggo::color::from_hsv(_hue, ggo::rand_float(), ggo::rand_float());
		
		particle._angle = ggo::rand_float(0, 2 * ggo::PI<float>());
		particle._dangle = ggo::rand_float(-GGO_DANGLE_AMP, GGO_DANGLE_AMP);
		particle._dangle_counter = ggo::rand_int(GGO_DANGLE_COUNTER_MIN, GGO_DANGLE_COUNTER_MAX);
		
		particle._radius = ggo::rand_float(0.002f, 0.004f);
		particle._dradius = ggo::rand_float(-GGO_DRADIUS_AMP, GGO_DRADIUS_AMP);
		particle._dradius_counter = ggo::rand_int(GGO_DRADIUS_COUNTER_MIN, GGO_DRADIUS_COUNTER_MAX);
		
		particle._speed = ggo::rand_float(0.002f, 0.004f);
	}
	
	// Move them.
	int frame_index = 0;
	
	_color_discs.clear();
	
	while (particles.empty() == false)
	{
		std::vector<ggo_color_disc> color_discs;
		
		for (int i = static_cast<int>(particles.size()) - 1; i >= 0; --i)
		{	
			ggo_particle & particle = particles[i];

			// Move particle.
			particle._pos += ggo::point2d_float::from_polar(particle._angle, particle._speed);
	
			// Update particle.
			--particle._dangle_counter;
			if (particle._dangle_counter <= 0)
			{
				particle._dangle = -particle._dangle * ggo::rand_float(0.8f, 1.2f);
				particle._dangle_counter = ggo::rand_int(GGO_DANGLE_COUNTER_MIN, GGO_DANGLE_COUNTER_MAX);
			}
	
			--particle._dradius_counter;
			if (particle._dradius_counter <= 0)
			{
				if (frame_index > 100)
				{
					particle._dradius = -GGO_DRADIUS_AMP;
				}
				else
				{
					particle._dradius = ggo::rand_float(-GGO_DRADIUS_AMP, GGO_DRADIUS_AMP);
					particle._dradius_counter = ggo::rand_int(GGO_DRADIUS_COUNTER_MIN, GGO_DRADIUS_COUNTER_MAX);
				}
			}
	
			particle._radius += particle._dradius;
			particle._angle += particle._dangle;
			particle._speed *= 0.999f;
	
			if (particle._radius <= 0)
			{
				particles.erase(particles.begin() + i);
			}
			else 
			{
				ggo_color_disc color_disc;
				color_disc._disc = ggo::disc_float(particle._pos, particle._radius);
				color_disc._color = particle._color;
				
				color_discs.push_back(color_disc);
			}
		}
		
		_color_discs.push_back(color_discs);
		
		++frame_index;
	}
}

//////////////////////////////////////////////////////////////
void ggo_bozons_animation_artist::init_output_buffer(uint8_t * buffer)
{
	ggo::color bkgd_color1 = ggo::color::from_hsv(_hue, ggo::rand_float(), ggo::rand_float());
	ggo::color bkgd_color2 = ggo::color::from_hsv(_hue, ggo::rand_float(), ggo::rand_float());
	ggo::color bkgd_color3 = ggo::color::from_hsv(_hue, ggo::rand_float(), ggo::rand_float());
	ggo::color bkgd_color4 = ggo::color::from_hsv(_hue, ggo::rand_float(), ggo::rand_float());
  
  ggo::rgb_image_buffer_uint8 image_data(buffer, get_render_width(), get_render_height());
  
	ggo::fill_4_colors(image_data, bkgd_color1, bkgd_color2, bkgd_color3, bkgd_color4);
}
