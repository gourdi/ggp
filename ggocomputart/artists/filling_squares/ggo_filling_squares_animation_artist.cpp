#include "ggo_filling_squares_animation_artist.h"
#include <ggo_fill.h>
#include <ggo_paint.h>

bool ggo_filling_squares_animation_artist::ggo_animated_square::bkgd_rendering_allowed;

//////////////////////////////////////////////////////////////
bool ggo_filling_squares_animation_artist::ggo_animated_square::update(uint8_t * output_buffer, uint8_t * bkgd_buffer, int width, int height, int counter)
{
	constexpr int FADE_IN_ANIM_DURATION = 25;

	// Compute size.
	float half_size = _colored_square._size / 2;
	half_size = ggo::ease_in(counter, FADE_IN_ANIM_DURATION, 5.f * half_size, half_size);

  auto square = std::make_shared<ggo::polygon2d_float>();
	square->add_point(-half_size, -half_size);
	square->add_point(-half_size,  half_size);
	square->add_point( half_size,  half_size);
	square->add_point( half_size, -half_size);

	// Apply rotation.
	float angle = ggo::ease_in(counter, FADE_IN_ANIM_DURATION, _angle, 0.f);
	square->rotate(angle, ggo::pos2f(0.f, 0.f));

	// Apply translation.
	float translation = ggo::ease_in(counter, FADE_IN_ANIM_DURATION, 1.f, 0.f);
	float dx = 10 * (_pos.get<0>() - width / 2);
	float dy = 10 * (_pos.get<1>() - height / 2);	
	square->move(_pos.get<0>() + translation * dx, _pos.get<1>() + translation * dy);

	// Painting.
  float opacity = ggo::ease_in(counter, FADE_IN_ANIM_DURATION, 0.f, 1.f);
  if (counter >= FADE_IN_ANIM_DURATION && bkgd_rendering_allowed == true)
  {
    ggo::paint(output_buffer, width, height, square, _colored_square._color, opacity);
    ggo::paint(bkgd_buffer, width, height, square, _colored_square._color, opacity);
    return false;
  }
  else
  {
    bkgd_rendering_allowed = false;
    ggo::paint(output_buffer, width, height, square, _colored_square._color, opacity);
    return true;
  }
}

//////////////////////////////////////////////////////////////
ggo_filling_squares_animation_artist::ggo_filling_squares_animation_artist(int render_width, int render_height)
:
ggo_dynamic_background_animation_artist_abc(render_width, render_height)
{

}

//////////////////////////////////////////////////////////////
void ggo_filling_squares_animation_artist::init_sub()
{
	_animator.clear();
	
	_hue = ggo::rand_float();
	std::vector<ggo_filling_squares_artist::ggo_multi_square> multi_squares;
	ggo_filling_squares_artist::build_squares(get_render_width(), get_render_height(), _hue, multi_squares);

	int counter = 0;
	for (const auto & multi_square : multi_squares)
	{
		float angle = ggo::rand_float(-ggo::pi<float>(), ggo::pi<float>());
		
		for (const auto & colored_square : multi_square._squares)
		{
      int start_offset = counter / 3;
			ggo_animated_square * animated_square = new ggo_animated_square(start_offset);
			animated_square->_colored_square = colored_square;
			animated_square->_pos = multi_square._pos;
			animated_square->_angle = angle;

			_animator.add_animate(animated_square);
		}
		
		++counter;
	}
}

//////////////////////////////////////////////////////////////
void ggo_filling_squares_animation_artist::init_bkgd_buffer(uint8_t * bkgd_buffer)
{
  ggo::fill_solid_rgb(bkgd_buffer, get_render_width() * get_render_height(), ggo::color::from_hsv(_hue, ggo::rand_float(), ggo::rand_float()));
}

//////////////////////////////////////////////////////////////
bool ggo_filling_squares_animation_artist::render_next_frame_bkgd(uint8_t * output_buffer, uint8_t * bkgd_buffer, int frame_index)
{
	constexpr int GGO_FILLING_SQUARES_FRAMES_COUNT = 500;

  ggo_animated_square::bkgd_rendering_allowed = true;
	_animator.update(output_buffer, bkgd_buffer, get_render_width(), get_render_height());

	return frame_index < GGO_FILLING_SQUARES_FRAMES_COUNT;
}