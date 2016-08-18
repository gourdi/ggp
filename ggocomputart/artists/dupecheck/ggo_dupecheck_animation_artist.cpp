#include "ggo_dupecheck_animation_artist.h"
#include <ggo_dct.h>
#include <ggo_harmonic_curve.h>
#include <ggo_paint.h>
#include <ggo_fill.h>
#include <ggo_color.h>
#include <ggo_linear_path.h>
#include <ggo_spiral_path.h>

//////////////////////////////////////////////////////////////
ggo_dupecheck_animation_artist::ggo_dupecheck_animation_artist(int render_width, int render_height)
:
ggo_animation_artist_abc(render_width, render_height)
{
}

//////////////////////////////////////////////////////////////
void ggo_dupecheck_animation_artist::init_sub()
{
	_colors[0].randomize();
	_colors[1].randomize();
	_colors[2].randomize();
	_colors[3].randomize();

	// Push background colors, depending of bitmap or animation rendering.
	_bkgd_colors.clear();
	for (int i = 0; i < 4; ++i)
	{
		ggo_background_color bkgd_color;

		bkgd_color._val = ggo::rand_float(0.1f, 0.15f);
		bkgd_color._radius = ggo::rand_float(0.1f, 0.5f);
		bkgd_color._y = ggo::rand_float();
		bkgd_color._angle = ggo::rand_float(0, 2 * ggo::pi<float>());
		bkgd_color._dangle = ggo::rand_float(0.01f, 0.02f);
		bkgd_color._var = ggo::rand_float(60, 90);

		_bkgd_colors.push_back(bkgd_color);
	}

	_points[0].get<0>() = ggo::rand_float() * get_render_width();
	_points[0].get<1>() = ggo::rand_float() * get_render_height();
	_points[1].get<0>() = ggo::rand_float() * get_render_width();
	_points[1].get<1>() = ggo::rand_float() * get_render_height();
	_points[2].get<0>() = ggo::rand_float() * get_render_width();
	_points[2].get<1>() = ggo::rand_float() * get_render_height();
	_points[3].get<0>() = ggo::rand_float() * get_render_width();
	_points[3].get<1>() = ggo::rand_float() * get_render_height();
}

//////////////////////////////////////////////////////////////
ggo::pos2f ggo_dupecheck_animation_artist::get_position(int frame_index)
{
	const float SCALE = 0.015f;
	
	// Compute current position.
	float t = std::fmod(SCALE * frame_index, 1.f);
	
	ggo::pos2f p;
	p  = t*t*t*(-1.f*_points[0] +  3.f*_points[1] + -3.f*_points[2] + _points[3]);
	p +=   t*t*( 3.f*_points[0] + -6.f*_points[1] +  3.f*_points[2]);
	p +=     t*(-3.f*_points[0]                   +  3.f*_points[2]);
	p +=             _points[0] +  4.f*_points[1] +      _points[2];
	p /= 6.f;		
	
	// Update spline points if needed.
	float t_next = std::fmod(SCALE * (frame_index + 1), 1.f);
	if (std::fmod(t, 1.f) > std::fmod(t_next, 1.f)) 
	{
		_points[0] = _points[1];
		_points[1] = _points[2];
		_points[2] = _points[3];
		_points[3].get<0>() = ggo::rand_float() * get_render_width();
		_points[3].get<1>() = ggo::rand_float() * get_render_height();
	}
	
	return p;
}

//////////////////////////////////////////////////////////////
ggo::color ggo_dupecheck_animation_artist::get_color(int frame_index)
{
	const float SCALE = 0.01f;
	
	// Compute current position.
	float t = std::fmod(SCALE * frame_index, 1.f);
	
	ggo::color c;
	c  = t*t*t*(-1.f*_colors[0] +  3.f*_colors[1] + -3.f*_colors[2] + _colors[3]);
	c +=   t*t*( 3.f*_colors[0] + -6.f*_colors[1] +  3.f*_colors[2]);
	c +=     t*(-3.f*_colors[0]                   +  3.f*_colors[2]);
	c +=             _colors[0] +  4.f*_colors[1] +      _colors[2];
	c /= 6.f;		
	
	// Update spline points if needed.
	float t_next = std::fmod(SCALE * (frame_index + 1), 1.f);
	if (std::fmod(t, 1.f) > std::fmod(t_next, 1.f)) 
	{
		_colors[0] = _colors[1];
		_colors[1] = _colors[2];
		_colors[2] = _colors[3];
		_colors[3].randomize();
	}
	
	c.r() = ggo::clamp<float>(c.r() + ggo::rand_float(-0.1f, 0.1f), 0, 1);
	c.g() = ggo::clamp<float>(c.g() + ggo::rand_float(-0.1f, 0.1f), 0, 1);
	c.b() = ggo::clamp<float>(c.b() + ggo::rand_float(-0.1f, 0.1f), 0, 1);

	return c;
}

//////////////////////////////////////////////////////////////
bool ggo_dupecheck_animation_artist::render_next_frame_sub(uint8_t * buffer, int frame_index)
{
	const int COUNTER_MAX = 400;

	// Check if the artist is finished.
	if ((frame_index > COUNTER_MAX) && (_animator.is_empty() == true))
	{
		return false;
	}
	
	// Background.
	if (buffer != nullptr)
	{
		uint8_t * ptr = buffer;
	
		for (int y = 0; y < get_render_height(); ++y)
		{
			float v_f = 1;
			for (const auto & bkgd_color : _bkgd_colors)
			{
				float pos = bkgd_color._y + bkgd_color._radius * std::cos(bkgd_color._angle);
				float dpos = pos - y / float(get_render_height());
				v_f -= bkgd_color._val * std::exp(-(dpos * dpos) * bkgd_color._var);
			}
			uint8_t v = std::max(0, ggo::to<int>(255 * v_f));
		
			for (int x = 0; x < get_render_width(); ++x)
			{
				*ptr++ = v;
				*ptr++ = v;
				*ptr++ = v;
			} 
		}
	}
	for (auto & bkgd_color : _bkgd_colors)
	{
		bkgd_color._angle += bkgd_color._dangle;
	}

	// Create new anim item.
	ggo::pos2f pos = get_position(frame_index);
	ggo::color color = get_color(frame_index);
	if ((frame_index < COUNTER_MAX) && ((frame_index % 4) == 0))
	{		
		ggo_path_abc * path = NULL;
		
		switch (ggo::rand_int(0, 2))
		{
		case 0:
			break;
		case 1:
		 	path = new ggo_linear_path(ggo::rand_float(0.001f, 0.01f) * get_render_min_size(), ggo::rand_float(0, 2 * ggo::pi<float>()));
			break;
		case 2:
		 	path = new ggo_spiral_path(ggo::rand_float(-ggo::pi<float>() / 32, ggo::pi<float>() / 32), ggo::rand_float(0.001f, 0.01f) * get_render_min_size());
			break;
		}
		
		ggo_animate_abc * animate = NULL;
		switch (ggo::rand_int(0, 2)) 
		{
		case 0:
			animate = new ggo_animate1(pos, path, color, get_render_min_size());
			break;
		case 1:
			animate = new ggo_animate2(pos, path, color, get_render_min_size());
			break;
		case 2:
			animate = new ggo_animate3(pos, path, color, get_render_min_size());
			break;
		default:
			GGO_FAIL();
			break;		
		}
		
		_animator.add_animate(animate);
	}
	
	// Update anim items.
	_animator.update(buffer, nullptr, get_render_width(), get_render_height());

	return true;
}

//////////////////////////////////////////////////////////////
ggo_dupecheck_animation_artist::ggo_dupecheck_animate_abc::ggo_dupecheck_animate_abc(ggo::pos2f pos, ggo_path_abc * path, const ggo::color & color, int render_min_size)
:
ggo_path_animate_abc(pos, path)
{
	_color = color;
	_opacity = 1;
	_radius = 0;
	_dradius = ggo::rand_float(0.0005f, 0.005f) * render_min_size;
}

//////////////////////////////////////////////////////////////
bool ggo_dupecheck_animation_artist::ggo_dupecheck_animate_abc::update(uint8_t * output_buffer, uint8_t * bkgd_buffer, int width, int height, int counter, const ggo::pos2f & pos)
{
	if (counter > 30) 
	{
		_opacity -= 0.1f;
	}
	
	if (_opacity < 0)
	{
		return false;
	}

	_radius += _dradius;
    
    update(output_buffer, width, height, pos);
	
	return true;
}

//////////////////////////////////////////////////////////////
ggo_dupecheck_animation_artist::ggo_animate1::ggo_animate1(const ggo::pos2f & pos, ggo_path_abc * path, const ggo::color & color, int render_min_size)
:
ggo_dupecheck_animate_abc(pos, path, color, render_min_size)
{
	_count = ggo::rand_int(1, 3);
	_width = ggo::rand_float(0.002f, 0.02f) * render_min_size;
	_distance = _width * ggo::rand_float(1.1f, 2);
	_opacity = 1;
}

//////////////////////////////////////////////////////////////
void ggo_dupecheck_animation_artist::ggo_animate1::update(uint8_t * buffer, int width, int height, const ggo::pos2f & pos)
{
	if (buffer != nullptr)
	{
		for (int i = 0; i < _count; ++i)
		{
			float radius = _radius - i * _distance;
			if (radius > 0)
			{
        auto disc1 = std::make_shared<ggo::disc_float>(pos, radius + 0.5f * _width);
        auto disc2 = std::make_shared<ggo::disc_float>(pos, radius - 0.5f * _width);

        auto opened_disc = std::make_shared<ggo::multi_shape<float, ggo::boolean_mode::DIFFERENCE>>();
        opened_disc->add_shapes(disc1, disc2);

				ggo::paint(buffer, width, height, opened_disc, _color, _opacity);
			}
		}
	}
}

//////////////////////////////////////////////////////////////
ggo_dupecheck_animation_artist::ggo_animate2::ggo_animate2(const ggo::pos2f & pos, ggo_path_abc * path, const ggo::color & color, int render_min_size)
:
ggo_dupecheck_animate_abc(pos, path, color, render_min_size)
{
	_width = ggo::rand_float(0.001f, 0.01f) * render_min_size;
}

//////////////////////////////////////////////////////////////
void ggo_dupecheck_animation_artist::ggo_animate2::update(uint8_t * buffer, int width, int height, const ggo::pos2f & pos)
{
	if (buffer != nullptr)
	{
		const int SAMPLES = 120;

		ggo::harmonic_curve<float> harmonic_curve;
	
		harmonic_curve.set_harmonic(ggo::rand_int(8, 16), ggo::rand_float(0.05f, 0.1f) * _radius, ggo::rand_float(0, 2 * ggo::pi<float>()));
		harmonic_curve.set_harmonic(ggo::rand_int(8, 16), ggo::rand_float(0.05f, 0.1f) * _radius, ggo::rand_float(0, 2 * ggo::pi<float>()));
		harmonic_curve.set_harmonic(ggo::rand_int(8, 16), ggo::rand_float(0.05f, 0.1f) * _radius, ggo::rand_float(0, 2 * ggo::pi<float>()));
		harmonic_curve.set_harmonic(ggo::rand_int(8, 16), ggo::rand_float(0.05f, 0.1f) * _radius, ggo::rand_float(0, 2 * ggo::pi<float>()));

    auto multi_shape = std::make_shared<ggo::multi_shape_float>();

		for (int i = 0; i <= SAMPLES; ++i)
      {
        float angle1 = i * 2 * ggo::pi<float>() / SAMPLES;
        float angle2 = (i + 1) * 2 * ggo::pi<float>() / SAMPLES;
        float radius1 = _radius + harmonic_curve.evaluate(angle1);
        float radius2 = _radius + harmonic_curve.evaluate(angle2);
        
        ggo::pos2f p1 = pos + ggo::from_polar(angle1, radius1);
        ggo::pos2f p2 = pos + ggo::from_polar(angle2, radius2);
      
        auto segment = std::make_shared<ggo::extended_segment_float>(p1, p2, _width);
			
        multi_shape->add_shape(segment);
		}
	
		ggo::paint(buffer, width, height, multi_shape, _color, _opacity);
	}
}

//////////////////////////////////////////////////////////////
ggo_dupecheck_animation_artist::ggo_animate3::ggo_animate3(const ggo::pos2f & pos, ggo_path_abc * path, const ggo::color & color, int render_min_size)
:
ggo_dupecheck_animate_abc(pos, path, color, render_min_size)
{
	_shapes_count = ggo::rand_int(5, 20);
	_vertices_count = ggo::rand_int(3, 6);
	_angle = ggo::rand_float(0, 2 * ggo::pi<float>());
	_dangle = ggo::rand_float(-ggo::pi<float>() / 16, ggo::pi<float>() / 16);
	_angle_shape = ggo::rand_float(0, 2 * ggo::pi<float>());
	_dangle_shape = ggo::rand_float(-ggo::pi<float>() / 16, ggo::pi<float>() / 16);
	_shape_size = ggo::rand_float(0.003f, 0.03f) * render_min_size;
}

//////////////////////////////////////////////////////////////
void ggo_dupecheck_animation_artist::ggo_animate3::update(uint8_t * buffer, int width, int height, const ggo::pos2f & pos)
{
	if (buffer != nullptr)
	{
		for (int i = 0; i < _shapes_count; ++i)
		{
			float angle_shape = _angle + i * 2 * ggo::pi<float>() / _shapes_count;
			ggo::pos2f shape_pos = pos + ggo::from_polar(angle_shape, _radius);

			auto shape = std::make_shared<ggo::polygon2d_float>();

			for (int j = 0; j < _vertices_count; ++j)
			{
				float angle_vertex = _angle_shape + j * 2 * ggo::pi<float>() / _vertices_count;
				ggo::pos2f vertex = shape_pos + ggo::from_polar(angle_vertex, _shape_size);

				shape->add_point(vertex);
			}

			ggo::paint(buffer, width, height, shape, _color, _opacity);
		}	
	}
    
  _angle += _dangle;
	_angle_shape += _dangle_shape;
}
