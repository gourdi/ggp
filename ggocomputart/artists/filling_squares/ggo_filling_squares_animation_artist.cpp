#include "ggo_filling_squares_animation_artist.h"
#include <ggo_buffer_fill.h>
#include <ggo_buffer_paint.h>
#include <ggo_brush.h>
#include <ggo_blender.h>

//////////////////////////////////////////////////////////////
bool ggo::filling_squares_animation_artist::animated_square::update(void * buffer, int width, int height, int line_step, ggo::pixel_buffer_format pbf, int counter)
{
	constexpr int fade_in_anim_duration = 25;

  counter = std::min(counter, fade_in_anim_duration);

	// Compute size.
	float half_size = _colored_square._size / 2;
	half_size = ggo::ease_in(counter, fade_in_anim_duration, 5.f * half_size, half_size);

  ggo::polygon2d_float square;
	square.add_point(-half_size, -half_size);
	square.add_point(-half_size,  half_size);
	square.add_point( half_size,  half_size);
	square.add_point( half_size, -half_size);

	// Apply rotation.
	float angle = ggo::ease_in(counter, fade_in_anim_duration, _angle, 0.f);
	square.rotate(angle, ggo::pos2f(0.f, 0.f));

	// Apply translation.
	float translation = ggo::ease_in(counter, fade_in_anim_duration, 1.f, 0.f);
	float dx = 10 * (_pos.x() - width / 2);
	float dy = 10 * (_pos.y() - height / 2);	
	square.move(_pos.x() + translation * dx, _pos.y() + translation * dy);

	// Painting.
  float opacity = ggo::ease_in(counter, fade_in_anim_duration, 0.f, 1.f);

  ggo::paint_shape<ggo::rgb_8u_yu, ggo::sampling_4x4>(buffer, width, height, 3 * width,
    square, ggo::make_solid_brush(_colored_square._color), ggo::alpha_blender_rgb8u(opacity));

  return true;
}

//////////////////////////////////////////////////////////////
ggo::filling_squares_animation_artist::filling_squares_animation_artist(int width, int height, int line_step, ggo::pixel_buffer_format pbf, rendering_type rt)
:
animation_artist_abc(width, height, line_step, pbf, rt)
{

}

//////////////////////////////////////////////////////////////
void ggo::filling_squares_animation_artist::init_sub()
{
	_animator.clear();
	
	_hue = ggo::rand<float>();
	auto multi_squares = ggo::filling_squares_artist::build_squares(get_width(), get_height(), _hue);

	int counter = 0;
	for (const auto & multi_square : multi_squares)
	{
		float angle = ggo::rand<float>(-ggo::pi<float>(), ggo::pi<float>());
		
		for (const auto & colored_square : multi_square._squares)
		{
      int start_offset = counter / 3;
			animated_square * animated_square = new ggo::filling_squares_animation_artist::animated_square(start_offset);
			animated_square->_colored_square = colored_square;
			animated_square->_pos = multi_square._pos;
			animated_square->_angle = angle;

			_animator.add_animate(animated_square);
		}
		
		++counter;
	}

  _bkgd_color = ggo::from_hsv<ggo::color_8u>(_hue, ggo::rand<float>(), ggo::rand<float>());
}

//////////////////////////////////////////////////////////////
bool ggo::filling_squares_animation_artist::render_next_frame_sub(void * buffer, int frame_index)
{
	constexpr int frames_count = 500;

  if (frame_index > frames_count)
  {
    return false;
  }

  ggo::fill_solid<ggo::rgb_8u_yu>(buffer, get_width(), get_height(), get_line_step(), _bkgd_color);

	_animator.update(buffer, get_width(), get_height(), get_line_step(), get_pixel_buffer_format());

	return true;
}