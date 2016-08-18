#include "ggo_lagaude_animation_artist.h"
#include <ggo_paint.h>
#include <ggo_fill.h>

//////////////////////////////////////////////////////////////
ggo_lagaude_animation_artist::ggo_lagaude_animation_artist(int render_width, int render_height)
:
ggo_animation_artist_abc(render_width, render_height)
{
	
}

//////////////////////////////////////////////////////////////
void ggo_lagaude_animation_artist::init_sub()
{
	_bkgd_discs.clear();
	for (int i = 0; i < 100; ++i)
	{
		ggo_bkgd_disc bkgd_disc;
		bkgd_disc._pos.get<0>() = ggo::rand_float(-1, 3);
		bkgd_disc._pos.get<1>() = ggo::rand_float(-1, 3);
		bkgd_disc._vel = ggo::from_polar(ggo::rand_float(0, 2 * ggo::pi<float>()), 0.001f);
		bkgd_disc._radius = 0.75;
		
		_bkgd_discs.push_back(bkgd_disc);
	}
	
	_animator.clear();
	for (int i = 0; i < 1000; ++i)
	{
		float scale = ggo::rand_float(0.5, 1);
		ggo::pos2f pos(ggo::rand_float(-2, 2), ggo::rand_float(-0.1f, 1.1f));
		
		ggo_sinuoid_path * path = new ggo_sinuoid_path();
		path->_amplitude = ggo::rand_float(0.01f, 0.02f);
		path->_dx = ggo::rand_float(0.001f, 0.002f);
		path->_frequency = ggo::rand_float(0.02f, 0.05f);

		ggo_dust * dust = new ggo_dust(pos, path, scale);
		dust->_radius = ggo::rand_float(0.004f, 0.008f);
		dust->_width = dust->_radius * ggo::rand_float(0.1f, 0.5f);
		dust->_val = ggo::rand_float(0, 0.5f);

		insert_scale_animator(dust);
	}
	
	_hue = ggo::rand_float();
}

//////////////////////////////////////////////////////////////
bool ggo_lagaude_animation_artist::render_next_frame_sub(uint8_t * buffer, int frame_index)
{
	if (frame_index > 450)
	{
		return false;
	}

  auto image_buffer = make_image_buffer(buffer);
	
	// Render background.
	ggo::fill_solid(image_buffer, ggo::color::WHITE);

	for (auto & bkgd_disc : _bkgd_discs)
	{
		float x = get_render_width() * bkgd_disc._pos.get<0>();
		float y = get_render_height() * bkgd_disc._pos.get<1>();
		float radius = get_render_min_size() * bkgd_disc._radius;
		
    ggo::paint(image_buffer, std::make_shared<ggo::disc_float>(x, y, radius), ggo::color::BLACK, 0.1f);
			
		bkgd_disc._pos += bkgd_disc._vel;
	}
		
	// Add seed.
	if ((frame_index % 10 == 0) && (frame_index < 350))
	{
		float scale = ggo::rand_float(0.5, 1);
		ggo::pos2f pos(ggo::rand_float(), ggo::rand_float());
		ggo_path_abc * path = new ggo_linear_path(scale * ggo::rand_float(0.002f, 0.005f), ggo::rand_float(0, 2 * ggo::pi<float>()));
		
		insert_scale_animator(new ggo_seed(pos, path, scale, _hue));
	}

	_animator.update(buffer, nullptr, get_render_width(), get_render_height());
	
	// Fade out.
	if (frame_index > 400)
	{
		float fade = 1 - (frame_index - 400) / 50.f;

		for (int i = 0; i < 3 * get_render_width() * get_render_height(); ++i)
		{
			buffer[i] = ggo::to<int>(fade * buffer[i]);
		}
	}

	return true;
}

//////////////////////////////////////////////////////////////
void ggo_lagaude_animation_artist::insert_scale_animator(ggo_scale_animate_abc * scale_animator)
{
	// From smaller to largest scale.
	int i = 0;
	for (i = 0; i < _animator.get_animates_count(); ++i)
	{
		const ggo_scale_animate_abc * current = static_cast<const ggo_scale_animate_abc *>(_animator.get_animate(i));
		if (current->get_scale() < scale_animator->get_scale())
		{
			break;
		}
	}
		
	if (i >= _animator.get_animates_count())
	{
		_animator.add_animate(scale_animator);
	}
	else 
	{
		_animator.insert_animate(scale_animator, i);
	}
}

//////////////////////////////////////////////////////////////
void ggo_lagaude_animation_artist::ggo_angle_generator::get_random_data(float & data, float & dt)
{
	data = ggo::rand_float(-ggo::pi<float>() / 4, ggo::pi<float>() / 4);
	dt = ggo::rand_float(3, 15);
}

//////////////////////////////////////////////////////////////
ggo_lagaude_animation_artist::ggo_seed::ggo_seed(const ggo::pos2f & pos, ggo_path_abc * path, float scale, float hue)
:
ggo_scale_animate_abc(pos, path, scale)
{
	_angle_generators.resize(ggo::rand_int(3, 5));
	_life = ggo::rand_int(80, 120);
	_dangle = ggo::rand_float(-0.1f, 0.1f);
	_color = ggo::color::from_hsv(hue, ggo::rand_float(0.5f, 1), ggo::rand_float(0.5f, 1));
}

//////////////////////////////////////////////////////////////
bool ggo_lagaude_animation_artist::ggo_seed::update(uint8_t * output_buffer, uint8_t * bkgd_buffer, int width, int height, int counter, const ggo::pos2f & pos)
{
	// Create the particles if not dead.
	float opacity = 1;
	if (counter > _life)
	{
		opacity = 1 - 0.15f * (counter - _life);
	}
	
	if (opacity > 0) 
	{
		for (int i = 0; i < _angle_generators.get_count(); ++i)
		{
			float angle = _angle_generators(i).update(1) + 2 * ggo::pi<float>() * i / _angle_generators.get_count();

			ggo_particle * particle = new ggo_particle(pos + ggo::from_polar(angle, 0.02f * _scale), new ggo_linear_path(0.02f * _scale, angle));
			particle->_angle = angle;
			particle->_dangle = _dangle;
			particle->_radius = 0.025f * _scale;
			particle->_dradius = 0.0025f;
			particle->_opacity = opacity;
			particle->_color = _color;

			_particles_animator.add_animate(particle);
		}
	}

	_particles_animator.update(output_buffer, nullptr, width, height);

	return !_particles_animator.is_empty();
}

//////////////////////////////////////////////////////////////
ggo_lagaude_animation_artist::ggo_particle::ggo_particle(const ggo::pos2f & pos, ggo_linear_path * path)
:
ggo_path_animate_abc(pos, path)
{
}

//////////////////////////////////////////////////////////////
bool ggo_lagaude_animation_artist::ggo_particle::update(uint8_t * output_buffer, uint8_t * bkgd_buffer, int width, int height, int counter, const ggo::pos2f & pos)
{
	ggo::pos2f p1 = map_fit(ggo::from_polar(_angle, _radius), 0, 1, width, height);
	ggo::pos2f p2 = map_fit(ggo::from_polar(_angle + 2 * ggo::pi<float>() / 3, _radius), 0, 1, width, height);
	ggo::pos2f p3 = map_fit(ggo::from_polar(_angle + 4 * ggo::pi<float>() / 3, _radius), 0, 1, width, height);
    
  ggo::vec2f disp(pos.get<0>() * width, pos.get<1>() * height);
  p1 += disp;
  p2 += disp;
  p3 += disp;    
	
  auto multi_shape = std::make_shared<ggo::multi_shape_float>();
  
  float size = 0.003f * std::min(width, height);
  multi_shape->add_shape(std::make_shared<ggo::extended_segment_float>(p1, p2, size));
  multi_shape->add_shape(std::make_shared<ggo::extended_segment_float>(p2, p3, size));
  multi_shape->add_shape(std::make_shared<ggo::extended_segment_float>(p3, p1, size));

	ggo::paint(output_buffer, width, height, multi_shape, _color, _opacity);
	
	_angle -= _dangle;
	_radius -= _dradius;
	
	return _radius >= 0;
}

//////////////////////////////////////////////////////////////
bool ggo_lagaude_animation_artist::ggo_dust::update(uint8_t * output_buffer, uint8_t * bkgd_buffer, int width, int height, int counter, const ggo::pos2f & pos)
{
	if (pos.get<0>() > 1.1)
  {
    return false;
  }

  ggo::pos2f center(width * pos.get<0>(), height * pos.get<1>());
  float disc_radius = std::min(width, height) * _radius;
  float disc_width = std::min(width, height) * _width;

  auto disc1 = std::make_shared<ggo::disc_float>(center, disc_radius + 0.5f * disc_width);
  auto disc2 = std::make_shared<ggo::disc_float>(center, disc_radius - 0.5f * disc_width);

  auto opened_disc = std::make_shared<ggo::multi_shape<float, ggo::boolean_mode::DIFFERENCE>>();
  opened_disc->add_shapes(disc1, disc2);

	ggo::paint(output_buffer, width, height, opened_disc, ggo::color(_val));
    
  return true;
}

