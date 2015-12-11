#include "ggo_plastic_animation_artist.h"

//////////////////////////////////////////////////////////////
ggo_plastic_animation_artist::ggo_plastic_animation_artist(int render_width, int render_height)
:
ggo_animation_artist_abc(render_width, render_height)
{

}

//////////////////////////////////////////////////////////////
void ggo_plastic_animation_artist::init_sub()
{
	_params.clear();

	for (int i = 0; i < 50; ++i)
	{
		anim_plastic_params params;
		
		params._center_x = ggo::rand_float(-1.5f, 1.5f);
		params._center_y = ggo::rand_float(-1.5f, 1.5f);
		params._radius = ggo::rand_float(0.1f, 0.5f);
		params._angle = ggo::rand_float(0, 2 * ggo::PI<float>());
		params._dangle = ggo::rand_float(0.02f, 0.08f);
		params._den = ggo::rand_float(0.5f, 2.f);
		params._mult = ggo::rand_float(0.5f, 2.f);
		params._power = ggo::rand_float(2.f, 3.f);

		_params.push_back(params);
	}

	_color = ggo::color::from_hsv(ggo::rand_float(), ggo::rand_float(), 1);
	_altitude_factor = ggo::rand_float(0.04f, 0.08f);
}

//////////////////////////////////////////////////////////////
bool ggo_plastic_animation_artist::render_next_frame_sub(uint8_t * buffer, int frame_index)
{
	if (frame_index > 300)
	{
		return false;
	}

	std::vector<ggo_plastic_artist::ggo_plastic_params> bitmap_params;
	for (auto & anim_plastic_params : _params)
	{
		ggo_plastic_artist::ggo_plastic_params	params;
		params._pos_x = anim_plastic_params._center_x + anim_plastic_params._radius * std::cos(anim_plastic_params._angle);
		params._pos_y = anim_plastic_params._center_y + anim_plastic_params._radius * std::sin(anim_plastic_params._angle);
		params._den = anim_plastic_params._den;
		params._mult = anim_plastic_params._mult;
		params._power = anim_plastic_params._power;
		
		bitmap_params.push_back(params);
		
		anim_plastic_params._angle += anim_plastic_params._dangle;
	}
	
	ggo_plastic_artist artist(get_render_width(), get_render_height());
	artist.render(buffer, bitmap_params, _color, _altitude_factor);
	
	return true;
}
