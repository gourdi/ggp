#include "ggo_alpha_anim_artist.h"
#include <ggo_array.h>
#include <ggo_paint.h>
#include <ggo_fill.h>
#include <ggo_layer.h>
#include <ggo_dct.h>

namespace
{
  const int GGO_OSCILLO_SIZE = 750;
}

//////////////////////////////////////////////////////////////
ggo_alpha_anim_artist::ggo_alpha_anim_artist(int render_width, int render_height)
:
ggo_animation_artist_abc(render_width, render_height)
{
	
}

//////////////////////////////////////////////////////////////
void ggo_alpha_anim_artist::init_sub()
{
	_remaining_counter = 7;
	_creation_counter = ggo::rand_int(40, 60);
	
	_items.clear();
	
	add_new_item();
	
	_bkgd_color1 = ggo::color(ggo::rand_float(0.75, 1));
	_bkgd_color2 = ggo::color(ggo::rand_float(0.75, 1));
	_bkgd_color3 = ggo::color(ggo::rand_float(0.75, 1));
	_bkgd_color4 = ggo::color(ggo::rand_float(0.75, 1));
}

//////////////////////////////////////////////////////////////
void ggo_alpha_anim_artist::add_new_item()
{
	float margin = 0.1f * get_render_min_size();

	ggo::pos2f center = get_random_point(margin);

	// Try to create an item far away from the other already created items.
	if (_items.empty() == false)
	{
		float dist = 0;
		for (int i = 0; i < 5; ++i)
		{
			ggo::pos2f center_cur = get_random_point(margin);
			float dist_cur = ggo::distance(_items.front().get_center(), center_cur);
			for (int i = 1; i < _items.size(); ++i)
			{
				const ggo_item & item = _items[i];

				dist_cur = std::min(dist_cur, ggo::distance(item.get_center(), center_cur));
			}
			
			if (dist_cur > dist)
			{
				dist = dist_cur;
				center = center_cur;
			}
		}
	}

	float inner_radius = ggo::rand_float(0.04f, 0.08f) * get_render_min_size();
	float outter_radius = inner_radius + ggo::rand_float(0.04f, 0.08f) * get_render_min_size();
	float hue = ggo::rand_float();
	float sat = ggo::rand_float(0.5, 1);
	float val = ggo::rand_float(0.5, 1);
	_items.push_back(ggo_item(center, inner_radius, outter_radius, hue, sat, val));
}

//////////////////////////////////////////////////////////////
bool ggo_alpha_anim_artist::render_next_frame_sub(uint8_t * buffer, int frame_index)
{
	if (buffer != nullptr)
	{
  	auto image = make_image_buffer(buffer);
		ggo::fill_4_colors(image, _bkgd_color1, _bkgd_color2, _bkgd_color3, _bkgd_color4);
	}
	
	// Oscillos.
  ggo::remove_if(_oscillos, [&](ggo_oscillo & oscillo) { return oscillo.update() == false; });

	if (buffer != nullptr)
	{
		for (const ggo_oscillo & oscillo : _oscillos)
		{
			oscillo.draw(buffer, get_render_width(), get_render_height());
		}
	}
	
	if ((_remaining_counter > 0) && (_oscillos.size() < 5) && (ggo::rand_int(0, 2) == 0))
	{
		float y = ggo::rand_float(0.2f, 0.8f) * get_render_height();
		float dy = ggo::rand_float(-0.01f, 0.01f) * get_render_min_size();
		
		if (_oscillos.empty() == false)
		{
			int index = ggo::rand_int(0, static_cast<int>(_oscillos.size()) - 1);
			const ggo_oscillo & src = _oscillos[index];
			
			if ((src.y() > 0.2 * get_render_height()) && (src.y() < 0.8 * get_render_height()))
			{
				y = src.y();
			}
		}

		_oscillos.push_back(ggo_oscillo(y, dy));
	}

	// Items.
  ggo::remove_if(_items, [&](ggo_item & item) { return item.update(get_render_width(), get_render_height()) == false; });
	
	if (buffer != nullptr)
	{
		for (const auto & item : _items)
		{
			item.draw(buffer, get_render_width(), get_render_height());
		}
	}
	
	if (_creation_counter > 0)
	{
		--_creation_counter;
		
		if ((_creation_counter == 0) && (_remaining_counter > 0))
		{
			--_remaining_counter;

			_creation_counter = ggo::rand_int(40, 60);

			add_new_item();
		}
	}

	return _remaining_counter > 0 || !_items.empty() || !_oscillos.empty();
}

//////////////////////////////////////////////////////////////
// ggo_oscillo

//////////////////////////////////////////////////////////////
ggo_alpha_anim_artist::ggo_oscillo::ggo_oscillo(float y, float dy)
{
	_y = y;
	_dy = dy;
	_threshold = ggo::rand_int(GGO_OSCILLO_SIZE / 6, GGO_OSCILLO_SIZE / 3);
	_counter = ggo::rand_int(40, 50);
	_opacity = 0;
}

//////////////////////////////////////////////////////////////
bool ggo_alpha_anim_artist::ggo_oscillo::update()
{
	--_counter;
	
	if (_counter <= 0)
	{
		_opacity -= 0.05f;
	}
	else
	{
		_opacity = std::min(0.5f, _opacity + 0.1f);
	}
	
	_y += _dy;
	
	return _opacity > 0;
}

//////////////////////////////////////////////////////////////
void ggo_alpha_anim_artist::ggo_oscillo::draw(uint8_t * buffer, int width, int height) const
{
	int min_size = std::min(width, height);
	
	ggo::array_float freq(GGO_OSCILLO_SIZE, 0.f), spat(GGO_OSCILLO_SIZE, 0.f);
	for (int i = 0; i < _threshold; ++i)
	{
		freq(i) = ggo::to<float>(ggo::rand_int(-1, 1));
	}
	ggo::dct(freq.data(), spat.data(), GGO_OSCILLO_SIZE);

  auto multi_shape = std::make_shared<ggo::multi_shape_float>();
	for (int i = 1; i < GGO_OSCILLO_SIZE; ++i)
	{
		float x1 = (i - 1) * width / float(GGO_OSCILLO_SIZE);
		float y1 = _y + 0.025f * spat(i - 1) * min_size;
        
    float x2 = i * width / float(GGO_OSCILLO_SIZE);
		float y2 = _y + 0.025f * spat(i) * min_size;

    multi_shape->add_shape(std::make_shared<ggo::extended_segment_float>(ggo::pos2f(x1, y1), ggo::pos2f(x2, y2), 0.001f * min_size));
	}
    
  ggo::paint(buffer, width, height, multi_shape, ggo::color::BLACK, _opacity);
}

//////////////////////////////////////////////////////////////
// ggo_item

//////////////////////////////////////////////////////////////
ggo_alpha_anim_artist::ggo_item::ggo_item(const ggo::pos2f & center, float inner_radius, float outter_radius, float hue, float sat, float val)
:
_center(center)
{
	const int LINES_COUNT = 60;

	for (int i = 0; i < LINES_COUNT; ++i)
	{
		ggo::color color = ggo::color::from_hsv(hue, sat + ggo::rand_float(-0.2f, 0.2f), val + ggo::rand_float(-0.2f, 0.2f));

		_lines.push_back(ggo_line(center, 2 * ggo::pi<float>() * i / LINES_COUNT, inner_radius, outter_radius, color));
	}
}

//////////////////////////////////////////////////////////////
bool ggo_alpha_anim_artist::ggo_item::update(int width, int height)
{
  ggo::remove_if(_lines, [&](ggo_line & line) { return line.update(width, height) == false; });

	return !_lines.empty();
}

//////////////////////////////////////////////////////////////
void ggo_alpha_anim_artist::ggo_item::draw(uint8_t * buffer, int width, int height) const
{
	for (const ggo_line & line : _lines)
	{
		line.draw(buffer, width, height);
	}
}

//////////////////////////////////////////////////////////////
// ggo_line

//////////////////////////////////////////////////////////////
ggo_alpha_anim_artist::ggo_line::ggo_line(const ggo::pos2f & center, float angle, float inner_radius, float outter_radius, const ggo::color & color)
{
	_center = center;
	_angle = angle;
	_inner_radius = inner_radius;
	_outter_radius = outter_radius;
	_color = color;
	_angle_offset = ggo::rand_float(-2 * ggo::pi<float>(), 2 * ggo::pi<float>());
	_counter = ggo::rand_int(-30, 0);
}

//////////////////////////////////////////////////////////////
float ggo_alpha_anim_artist::ggo_line::get_start_factor() const
{
    return 1 / std::pow(1.f + 0.75f * _counter, 1.5f);
}

//////////////////////////////////////////////////////////////
bool ggo_alpha_anim_artist::ggo_line::update(int width, int height)
{
	++_counter;
	
	int min_size = std::min(width, height);
	float start_factor = get_start_factor();

	_opacity = 1.f - start_factor;
	_blur = 1.f + 0.1f * min_size * start_factor;
	
	float inner_radius = _inner_radius * (1 + 10 * start_factor);
	float outter_radius = _outter_radius * (1 + 10 * start_factor);
	float angle = _angle + _angle_offset * start_factor;

	ggo::vec2f offset1 = ggo::from_polar(angle, inner_radius);
	ggo::vec2f offset2 = ggo::from_polar(angle, outter_radius);
	
	const int END_COUNTER = 100;
	if (_counter >= END_COUNTER)
	{
		ggo::vec2f line_center = 0.5f * (offset1 + offset2);
		
		float end_factor = std::pow(0.1f * (_counter - END_COUNTER), 2.5f);
		float length = ggo::distance(offset1, offset2);
		float angle = atan2(line_center.get<1>(), line_center.get<0>());
		
		angle = _angle + end_factor;
		line_center *= (1 + end_factor);
		offset1 = line_center + ggo::from_polar(angle, length / 2);
		offset2 = line_center - ggo::from_polar(angle, length / 2);
		
		_opacity -= 0.1f * end_factor;
		if (_opacity <= 0)
		{
			return false;
		}
	}
    
	_p1 = _center + offset1;
  _p2 = _center + offset2;
	
	return true;
}

//////////////////////////////////////////////////////////////
void ggo_alpha_anim_artist::ggo_line::draw(uint8_t * buffer, int width, int height) const
{
	if (_counter >= 0)
	{
    int min_size = std::min(width, height);
    float line_width = 0.0015f * min_size + 0.1f * min_size * get_start_factor();
    
    auto segment = std::make_shared<ggo::extended_segment_float>(_p1, _p2, line_width);

    ggo::paint(buffer, width, height,
               segment, 
               _color, _opacity, std::make_shared<ggo::rgb_alpha_blender>(),
               ggo::blur_pixel_sampler(_blur));
	}
}
