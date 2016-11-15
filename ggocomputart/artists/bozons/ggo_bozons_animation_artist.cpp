#include "ggo_bozons_animation_artist.h"
#include <ggo_buffer_fill.h>
#include <ggo_color.h>
#include <ggo_multi_shape_paint.h>

namespace
{
  const float dradius_amp = 0.00001f;
  const int dradius_counter_min = 40;
  const int dradius_counter_max = 80;
  
  const float dangle_amp = ggo::pi<float>() / 100.f;
  const int dangle_counter_min = 200;
  const int dangle_counter_max = 400;
}

//////////////////////////////////////////////////////////////
ggo::bozons_animation_artist::bozons_animation_artist(int width, int height, int line_step, ggo::pixel_buffer_format pbf)
:
static_background_animation_artist_abc(width, height, line_step, pbf)
{
	
}

//////////////////////////////////////////////////////////////
bool ggo::bozons_animation_artist::render_next_frame_bkgd(void * buffer, int frame_index)
{
	if (frame_index >= _color_discs.size())
	{
		return false;
	}
	
	std::vector<solid_color_shape<disc_float, ggo::color_8u>> color_discs;

  for (size_t i = 0; i < _color_discs.size(); ++i)
  {
    for (const auto & color_disc : _color_discs[i])
    {
      disc_float disc(color_disc._disc);
      map_fit(disc, 0, 1);

      color_discs.emplace_back(disc, color_disc._color);
    }
  }
	
  paint_shapes<ggo::rgb_8u_yu, ggo::sampling_4x4>(
    buffer, get_width(), get_height(), get_line_step(), color_discs.cbegin(), color_discs.cend());
	
	return true;
}

//////////////////////////////////////////////////////////////
// Unroll all the process here and save particles for each frame.
void ggo::bozons_animation_artist::init_sub()
{
  _hue = ggo::rand<float>();
  
	// Create particles.
	std::vector<ggo_particle> particles;
	particles.resize(50);

	for (ggo_particle & particle : particles)
	{
		particle._pos.get<0>() = 0.5;
		particle._pos.get<1>() = 0.5;

		particle._color	= from_hsv<ggo::color_8u>(_hue, ggo::rand<float>(), ggo::rand<float>());
		
		particle._angle = ggo::rand<float>(0, 2 * ggo::pi<float>());
		particle._dangle = ggo::rand<float>(-dangle_amp, dangle_amp);
		particle._dangle_counter = ggo::rand<int>(dangle_counter_min, dangle_counter_max);
		
		particle._radius = ggo::rand<float>(0.002f, 0.004f);
		particle._dradius = ggo::rand<float>(-dradius_amp, dradius_amp);
		particle._dradius_counter = ggo::rand<int>(dradius_counter_min, dradius_counter_max);
		
		particle._speed = ggo::rand<float>(0.002f, 0.004f);
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
			particle._pos += ggo::from_polar(particle._angle, particle._speed);
	
			// Update particle.
			--particle._dangle_counter;
			if (particle._dangle_counter <= 0)
			{
				particle._dangle = -particle._dangle * ggo::rand<float>(0.8f, 1.2f);
				particle._dangle_counter = ggo::rand<int>(dangle_counter_min, dangle_counter_max);
			}
	
			--particle._dradius_counter;
			if (particle._dradius_counter <= 0)
			{
				if (frame_index > 100)
				{
					particle._dradius = -dradius_amp;
				}
				else
				{
					particle._dradius = ggo::rand<float>(-dradius_amp, dradius_amp);
					particle._dradius_counter = ggo::rand<int>(dradius_counter_min, dradius_counter_max);
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
void ggo::bozons_animation_artist::init_bkgd_buffer(void * buffer) const
{
	const ggo::color_8u bkgd_color1 = from_hsv<ggo::color_8u>(_hue, ggo::rand<float>(), ggo::rand<float>());
	const ggo::color_8u bkgd_color2 = from_hsv<ggo::color_8u>(_hue, ggo::rand<float>(), ggo::rand<float>());
	const ggo::color_8u bkgd_color3 = from_hsv<ggo::color_8u>(_hue, ggo::rand<float>(), ggo::rand<float>());
	const ggo::color_8u bkgd_color4 = from_hsv<ggo::color_8u>(_hue, ggo::rand<float>(), ggo::rand<float>());
  
	ggo::fill_4_colors<ggo::rgb_8u_yu>(buffer, get_width(), get_height(), get_line_step(), 
    bkgd_color1, bkgd_color2, bkgd_color3, bkgd_color4);
}
