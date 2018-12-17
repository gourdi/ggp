#include "ggo_filling_squares_animation_artist.h"
#include <2d/fill/ggo_fill.h>
#include <2d/paint/ggo_paint.h>
#include <2d/paint/ggo_brush.h>
#include <2d/paint/ggo_blend.h>

//////////////////////////////////////////////////////////////
ggo::filling_squares_animation_artist::animated_square::animated_square(const ggo::pos2_f & pos, int start_offset, float angle, filling_squares_artist::colored_square colored_square)
:
ggo::position_animate_abc(pos, start_offset),
_angle(angle),
_colored_square(colored_square)
{
}

//////////////////////////////////////////////////////////////
bool ggo::filling_squares_animation_artist::animated_square::update(int frame_index, const ggo::pos2_f & pos)
{
  return true;
}

//////////////////////////////////////////////////////////////
void ggo::filling_squares_animation_artist::animated_square::render(void * buffer, int width, int height, int line_step, ggo::image_format format,
  const ggo::rect_int & clipping, int frame_index, const ggo::pos2_f & pos) const
{
  constexpr int fade_in_anim_duration = 25;

  frame_index = std::min(frame_index, fade_in_anim_duration);

  // Compute size.
  float half_size = _colored_square._size / 2;
  half_size = ggo::ease_in(frame_index, fade_in_anim_duration, 5.f * half_size, half_size);

  ggo::polygon2d_f square;
  square.add_point(-half_size, -half_size);
  square.add_point(-half_size, half_size);
  square.add_point(half_size, half_size);
  square.add_point(half_size, -half_size);

  // Apply rotation.
  float angle = ggo::ease_in(frame_index, fade_in_anim_duration, _angle, 0.f);
  square.rotate(angle, ggo::pos2_f(0.f, 0.f));

  // Apply translation.
  float translation = ggo::ease_in(frame_index, fade_in_anim_duration, 1.f, 0.f);
  float dx = 10 * (pos.x() - width / 2.f);
  float dy = 10 * (pos.y() - height / 2.f);
  square.move({ pos.x() + translation * dx, pos.y() + translation * dy });

  // Painting.
  float opacity = ggo::ease_in(frame_index, fade_in_anim_duration, 0.f, 1.f);

  ggo::paint<ggo::rgb_8u_yu, ggo::sampling_4x4>(buffer, width, height, 3 * width,
    square, ggo::make_solid_brush(_colored_square._color), ggo::alpha_blender_rgb8u(opacity));
}

//////////////////////////////////////////////////////////////
ggo::filling_squares_animation_artist::filling_squares_animation_artist(int width, int height, int line_step, ggo::image_format format)
:
fixed_frames_count_animation_artist_abc(width, height, line_step, format, 500)
{
  _hue = ggo::rand<float>();
  auto multi_squares = ggo::filling_squares_artist::build_squares(width, height, _hue);

  int counter = 0;
  for (const auto & multi_square : multi_squares)
  {
    float angle = ggo::rand<float>(-ggo::pi<float>(), ggo::pi<float>());

    for (const auto & colored_square : multi_square._squares)
    {
      int start_offset = counter / 3;

      _animator.add_animate(new animated_square(multi_square._pos, start_offset, angle, colored_square));
    }

    ++counter;
  }

  _bkgd_color = ggo::from_hsv<ggo::rgb_8u>(_hue, ggo::rand<float>(), ggo::rand<float>());
}

//////////////////////////////////////////////////////////////
void ggo::filling_squares_animation_artist::render_frame(void * buffer, int frame_index)
{
  _animator.update();

  ggo::fill_solid<ggo::rgb_8u_yu>(buffer, width(), height(), line_step(), _bkgd_color);

	_animator.render(buffer, width(), height(), line_step(), format());
}