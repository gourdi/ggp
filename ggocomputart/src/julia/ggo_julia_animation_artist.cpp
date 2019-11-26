#include "ggo_julia_animation_artist.h"

//////////////////////////////////////////////////////////////
ggo::julia_animation_artist::julia_animation_artist(
  int width, int height, int line_byte_step,
  ggo::pixel_type pixel_type, ggo::lines_order memory_lines_order,
  ggo::ratio fps)
:
progress_animation_artist(width, height, line_byte_step, pixel_type, memory_lines_order, 8, fps)
{
  _angle = ggo::rand<float>(0, 2 * ggo::pi<float>());
  _radius = ggo::rand<float>(0.254f, 0.256f);
}

//////////////////////////////////////////////////////////////
void ggo::julia_animation_artist::render_frame(void * buffer, float progress)
{
  float angle = _angle + ggo::ease_inout(progress) * ggo::pi<float>() / 8;

  float x = -1 + _radius * std::cos(angle);
  float y = _radius * std::sin(angle);

  _artist.render_bitmap(buffer, width(), height(), line_byte_step(), pixel_type(), memory_lines_order(), std::complex<float>(x, y), 1);
}

