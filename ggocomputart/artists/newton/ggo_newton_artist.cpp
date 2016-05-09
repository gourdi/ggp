#include "ggo_newton_artist.h"
#include <ggo_gaussian_blur.h>
#include <ggo_paint.h>
#include <ggo_fill.h>

//////////////////////////////////////////////////////////////
ggo_newton_artist::ggo_newton_artist(int render_width, int render_height)
:
ggo_static_background_animation_artist_abc(render_width, render_height)
{
	
}

//////////////////////////////////////////////////////////////
bool ggo_newton_artist::find_string(int index1, int index2) const
{
	for (std::vector<ggo_string>::const_iterator it = _strings.begin(); it != _strings.end(); ++it)
	{
		const ggo_string & string = *it;
		
		if ((string._index1 == index1) && (string._index2 == index2))
		{
			return true;
		}
		
		if ((string._index1 == index2) && (string._index2 == index1))
		{
			return true;
		}
	}
	
	return false;
}

//////////////////////////////////////////////////////////////
void ggo_newton_artist::init_sub()
{
	_hue = ggo::rand_float();
	
	_shadow_offset = ggo::vector2d_float(ggo::rand_float(-0.01f, 0.01f) * get_render_min_size(), ggo::rand_float(0.02f, 0.05f) * get_render_min_size());
	
	_newtons.clear();
	
	while (_newtons.size() < 10)
	{
		ggo_newton newton;
		
		ggo::point2d_float pos(ggo::rand_float() * get_render_width(), ggo::rand_float() * get_render_height());
		newton._cur_pos = newton._prv_pos = pos;
		newton._weight = ggo::rand_float(1, 2);
		newton._timer = 0;
		if (ggo::rand_bool())
		{
			newton._timer = ggo::rand_int(0, 200);
		}
		
		_newtons.push_back(newton);
	}
	
	while (_strings.size() < 25)
	{
		int index1 = ggo::rand_int(0, static_cast<int>(_newtons.size()) - 1);
		int index2 = ggo::rand_int(0, static_cast<int>(_newtons.size()) - 1);
		
		if ((index1 != index2) && (find_string(index1, index2) == false))
		{
			ggo_string string;
			string._index1 = index1;
			string._index2 = index2;
			string._length = ggo::rand_float(0.1f, 0.5f) * get_render_min_size();
			
			_strings.push_back(string);
		}
	}
}

//////////////////////////////////////////////////////////////
void ggo_newton_artist::init_bkgd_buffer(uint8_t * bkgd_buffer)
{	
  ggo::color color1 = ggo::color::from_hsv(_hue, ggo::rand_float(0, 0.5), 1);
	ggo::color color2 = ggo::color::from_hsv(_hue, ggo::rand_float(0, 0.5), 1);
	ggo::color color3 = ggo::color::from_hsv(_hue, ggo::rand_float(0, 0.5), 1);
	ggo::color color4 = ggo::color::from_hsv(_hue, ggo::rand_float(0, 0.5), 1);
  
  auto bkgd_image = make_image_buffer(bkgd_buffer);
	ggo::fill_4_colors(bkgd_image, color1, color2, color3, color4);

  ggo::gray_image_buffer_uint8 perlin_image_data(get_render_width(), get_render_height());
	ggo::fill_perlin(perlin_image_data, 0.4f * get_render_min_size(), 0, 192);
	
	for (int i = 0; i < get_render_width() * get_render_height(); ++i)
	{
		float val = perlin_image_data.get_buffer()[i] / 255.f;
		
		bkgd_buffer[3 * i + 0] = ggo::to<uint8_t>(val * bkgd_buffer[3 * i + 0]);
		bkgd_buffer[3 * i + 1] = ggo::to<uint8_t>(val * bkgd_buffer[3 * i + 1]);
		bkgd_buffer[3 * i + 2] = ggo::to<uint8_t>(val * bkgd_buffer[3 * i + 2]);
	}
}

//////////////////////////////////////////////////////////////
bool ggo_newton_artist::render_next_frame_bkgd(uint8_t * buffer, int frame_index)
{
	// Remove some strings at random.
	if ((frame_index < 300) && (ggo::rand_int(0, 50) == 0))
	{
		_strings.erase(_strings.begin() + ggo::rand_int(0, static_cast<int>(_strings.size()) - 1));
	}
	
 	newton_update();
	newton_paint(buffer);

	// Look for maximum velocity.
	float max_velocity = 0;
	for (std::vector<ggo_newton>::const_iterator it = _newtons.begin(); it != _newtons.end(); ++it)
	{
		const ggo_newton & newton = *it;
		
		float velocity = ggo::distance(newton._prv_pos, newton._cur_pos);
		
		max_velocity = std::max(max_velocity, velocity);
	}

	_max_velocities.push_back(max_velocity);
	
	if (_max_velocities.size() < 25)
	{
		return true;
	}
	else
	{
		_max_velocities.erase(_max_velocities.begin());
	}
	
	max_velocity = 0;
	for (std::vector<float>::const_iterator it = _max_velocities.begin(); it != _max_velocities.end(); ++it)
	{
		max_velocity = std::max(max_velocity, *it);
	}

	return max_velocity > 0.0001 * get_render_min_size();
}

//////////////////////////////////////////////////////////////
void ggo_newton_artist::newton_update()
{
	ggo::vector2d_float gravity(0, 0.001f * get_render_min_size());
	
	// Computes forces.
	for (int i = 0; i < _newtons.size(); ++i)
	{
		ggo_newton & newton = _newtons[i];
		
		newton._forces = ggo::vector2d_float(0, 0);//gravity;
		
		if (newton._timer > 0)
		{
			--newton._timer;
			continue;
		}
		
		// Look for a string.
		for (std::vector<ggo_string>::const_iterator it = _strings.begin(); it != _strings.end(); ++it)
		{
			const ggo_string & string = *it;
			
			std::vector<ggo_newton>::const_iterator it_diff = _newtons.end();
			
			if (string._index1 == i)
			{
				it_diff = _newtons.begin() + string._index2;
			}
			if (string._index2 == i)
			{
				it_diff = _newtons.begin() + string._index1;
			}
			
			// Compute string force.
			if (it_diff != _newtons.end())
			{
				ggo::vector2d_float diff = it_diff->_cur_pos - newton._cur_pos;
				float length = diff.get_length();
				diff /= length; // Normalize.
				
				newton._forces += 0.02f * diff * std::min(0.025f * get_render_min_size(), length - string._length);
			}
		}
	}
		
	// Apply Newton.
	for (std::vector<ggo_newton>::iterator it = _newtons.begin(); it != _newtons.end(); ++it)
	{
		ggo_newton & newton = *it;
		
		ggo::point2d_float nxt_pos = newton._weight * newton._forces + 2.f * newton._cur_pos - newton._prv_pos;
		
		// Add friction.
		ggo::vector2d_float diff(nxt_pos - newton._cur_pos);
		diff *= 0.95f;
		nxt_pos = newton._cur_pos + diff;

		newton._prv_pos = newton._cur_pos;
		newton._cur_pos = nxt_pos;
		
		// Border bound.
	/*	if (newton._cur_pos.x() < 0)
		{
			newton._prv_pos.x() = -newton._prv_pos.x();
			newton._cur_pos.x() = -newton._cur_pos.x();
		}
		
		if (newton._cur_pos.x() > get_render_width())
		{
			newton._prv_pos.x() = 2 * get_render_width() - newton._prv_pos.x();
			newton._cur_pos.x() = 2 * get_render_width() - newton._cur_pos.x();
		}
		
		if (newton._cur_pos.y() < 0)
		{
			newton._prv_pos.y() = -newton._prv_pos.y();
			newton._cur_pos.y() = -newton._cur_pos.y();
		}
		
		if (newton._cur_pos.y() > get_render_height())
		{
			newton._prv_pos.y() = 2 * get_render_height() - newton._prv_pos.y();
			newton._cur_pos.y() = 2 * get_render_height() - newton._cur_pos.y();
		}*/
	}
}

//////////////////////////////////////////////////////////////
void ggo_newton_artist::newton_paint(uint8_t * buffer) const
{
	// Painting shadow.
	ggo::gray_image_buffer_uint8 shadow_image_data(get_render_width(), get_render_height());
  shadow_image_data.fill(0xFF);

	for (const auto & string : _strings)
	{
		const ggo::point2d_float & p1 = _newtons[string._index1]._cur_pos + _shadow_offset;
		const ggo::point2d_float & p2 = _newtons[string._index2]._cur_pos + _shadow_offset;
		
		auto line = std::make_shared<ggo::polygon2d_float>();
		ggo::polygon2d_float::create_oriented_box((p1 + p2) / 2.f, p2 - p1, ggo::distance(p1, p2) / 2.f, 0.005f * get_render_min_size(), *line);
		
		float dist = ggo::distance(p1, p2);
		float coef = 1 - 1 / (1 + 25 * std::fabs(dist - string._length) / get_render_min_size());
		float opacity = 0.25f + 0.75f * coef;
		ggo::paint(shadow_image_data,
               line,
               0, opacity, std::make_shared<ggo::gray_alpha_blender>(),
               ggo::pixel_sampler_2X2());
	}
	
	for (const auto & newton : _newtons)
	{
		ggo::paint(shadow_image_data,
               std::make_shared<ggo::disc_float>(newton._cur_pos + _shadow_offset, 0.025f * get_render_min_size()),
               0, 1, std::make_shared<ggo::gray_alpha_blender>(),
               ggo::pixel_sampler_2X2());
	}
	
	ggo::gaussian_blur_2d_mirror(shadow_image_data.get_buffer(),
                               shadow_image_data.get_buffer(),
                               get_render_width(),
                               get_render_height(),
                               0.05f * get_render_min_size(), 1, 1, 0.001f);
	
  auto image_buffer = make_image_buffer(buffer);
  image_buffer.from(shadow_image_data);
	
	// Paint shapes.
	for (const auto & string : _strings)
	{
		const ggo::point2d_float & p1 = _newtons[string._index1]._cur_pos;
		const ggo::point2d_float & p2 = _newtons[string._index2]._cur_pos;
		
		auto line = std::make_shared<ggo::polygon2d_float>();
		ggo::polygon2d_float::create_oriented_box((p1 + p2) / 2.f, p2 - p1, ggo::distance(p1, p2) / 2, 0.005f * get_render_min_size(), *line);
		
		float dist = ggo::distance(p1, p2);
		float coef = 1 - 1 / (1 + 25 * std::fabs(dist - string._length) / get_render_min_size());
		float opacity = 0.25f + 0.75f * coef;
    
		ggo::paint(buffer, get_render_width(), get_render_height(),
               line,
               ggo::color::WHITE, opacity);
	}
	
	for (const auto & newton : _newtons)
	{
		auto disc = std::make_shared<ggo::disc_float>();
		disc->center() = newton._cur_pos;
		disc->radius() = 0.025f * get_render_min_size();
		ggo::paint(buffer, get_render_width(), get_render_height(), disc, ggo::color::BLACK);
		
		float velocity = ggo::distance(newton._cur_pos, newton._prv_pos) / get_render_min_size();
		float coef = 1 - 1 / (1 + 500 * std::fabs(velocity));
		float sat = 1 - 0.5f * coef;
		float val = 0.25f + 0.75f * coef;
		disc->radius() = 0.02f * get_render_min_size();
		ggo::paint(buffer, get_render_width(), get_render_height(), disc, ggo::color::from_hsv(_hue, sat, val));
		
		disc->center().y() -= 0.0125f * get_render_min_size();
		disc->radius() = 0.002f * get_render_min_size();
		ggo::paint(buffer, get_render_width(), get_render_height(), disc, ggo::color::WHITE, 0.75f);
	}
}


