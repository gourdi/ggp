#include "ggo_plastic_bitmap_artist.h"

//////////////////////////////////////////////////////////////
ggo_plastic_bitmap_artist::ggo_plastic_bitmap_artist(int render_width, int render_height)
:
ggo_bitmap_artist_abc(render_width, render_height)
{

}

//////////////////////////////////////////////////////////////
/*ggo_plastic_bitmap_artist::ggo_plastic_bitmap_artist(int render_width, int render_height, float radius_max, float dangle_max)
:
ggo_bitmap_artist_abc(render_width, render_height)
{
	ggo_plastic_params params;
	for (int i = 0; i < 50; ++i)
	{
		params._center_x = ggo::rand_float(-2 + radius_max, 2 - radius_max);
		params._center_y = ggo::rand_float(-2 + radius_max, 2 - radius_max);
		params._radius = ggo::rand_float(0, radius_max);
		params._angle = ggo::rand_float(0, 2 * GGO_PI);
		params._dangle = ggo::rand_float(0, dangle_max);
		params._den = ggo::rand_float(0.5, 2);
		params._mult = ggo::rand_float(0.5, 2);
		params._power = ggo::rand_float(2, 3);

		_params.push_back(params);
	}

	_color = ggo_color::from_hsv(ggo::rand_float(), ggo::rand_float(), 1);
	_altitude_factor = ggo::rand_float(0.04, 0.08);
}*/

//////////////////////////////////////////////////////////////
void ggo_plastic_bitmap_artist::render_bitmap(uint8_t * buffer)
{
	std::vector<ggo_plastic_artist::ggo_plastic_params> plastic_params;
	for (int i = 0; i < 50; ++i)
	{
		ggo_plastic_artist::ggo_plastic_params params;
		
		params._pos_x = ggo::rand_float(-2, 2);
		params._pos_y = ggo::rand_float(-2, 2);
		params._den = ggo::rand_float(0.5, 2);
		params._mult = ggo::rand_float(0.5, 2);
		params._power = ggo::rand_float(5, 10);
		
		plastic_params.push_back(params);
	}
	
	ggo::color color = ggo::color::from_hsv(ggo::rand_float(), ggo::rand_float(), 1);
	float altitude_factor = ggo::rand_float(0.04f, 0.08f);
	
	ggo_plastic_artist artist(get_render_width(), get_render_height());
	artist.render(buffer, plastic_params, color, altitude_factor);
}
/*	
//////////////////////////////////////////////////////////////
void ggo_plastic_bitmap_artist::render_bitmap(uint8_t * buffer)
{
	float range_x = get_render_width() > get_render_height() ? get_render_width() / float(get_render_height()) : 1;
	float range_y = get_render_width() > get_render_height() ? 1 : get_render_height() / float(get_render_width());

	for (int y = 0; y < get_render_height(); ++y)
	{
		float y1 = ggo_map<float>(y - 3 / 8.f, 0, get_render_height(), -range_y, range_y);
		float y2 = ggo_map<float>(y + 3 / 8.f, 0, get_render_height(), -range_y, range_y);
	
		for (int x = 0; x < get_render_width(); ++x)
		{
			// Compute altitude.
			float x1 = ggo_map<float>(x - 3 / 8.f, 0, get_render_width(), -range_x, range_x);
			float x2 = ggo_map<float>(x + 3 / 8.f, 0, get_render_width(), -range_x, range_x);

			float z11 = evaluate(x1, y1);
			float z12 = evaluate(x1, y2);
			float z21 = evaluate(x2, y1);
			float z22 = evaluate(x2, y2);

			// Render the slope.
			float dx = x2 - x1;
			float dy = y2 - y1;
			ggo_vector3d_float v1(dx, dy, z22 - z11);
			ggo_vector3d_float v2(dx, -dy, z21 - z12);
			ggo_vector3d_float normal = ggo_cross(v1, v2);
			normal.normalize();
		
			ggo_color color = _color;
			color *= _altitude_factor + std::abs(normal.z()) / _altitude_factor;
		
			buffer[0] = color.r8();
			buffer[1] = color.g8();
			buffer[2] = color.b8();
		
			buffer += 3;
		}
	}
}

//////////////////////////////////////////////////////////////
float ggo_plastic_bitmap_artist::evaluate(float x, float y) const
{
	float z = 0;
	for (std::vector<ggo_plastic_params>::const_iterator it = _params.begin(); it != _params.end(); ++it)
	{
		float plastic_x = it->_center_x + it->_radius * std::cos(it->_angle);
		float plastic_y = it->_center_y + it->_radius * std::sin(it->_angle);
		
		float dx = plastic_x - x;
		float dy = plastic_y - y;
		
		float r = dx * dx + dy * dy;
		z += it->_den / (0.1 + std::pow(r * it->_mult, it->_power));
		z += it->_den / (0.1 + std::pow(r * it->_mult, it->_power));
	}

	return z;
}

//////////////////////////////////////////////////////////////
void ggo_plastic_bitmap_artist::update(uint8_t * buffer)
{
	for (std::vector<ggo_plastic_params>::iterator it = _params.begin(); it != _params.end(); ++it)
	{
		it->_angle += it->_dangle;
	}
}*/
