#include "ggo_julia_animation_artist.h"

//////////////////////////////////////////////////////////////
ggo::julia_animation_artist::julia_animation_artist(int width, int height, int line_step, ggo::image_format format)
:
fixed_frames_count_animation_artist_abc(width, height, line_step, format),
_artist(width, height, line_step, format)
{
  _angle = ggo::rand<float>(0, 2 * ggo::pi<float>());
  _radius = ggo::rand<float>(0.254f, 0.256f);
}

//////////////////////////////////////////////////////////////
void ggo::julia_animation_artist::render_frame(void * buffer, int frame_index)
{
  float angle = ggo::ease_inout(frame_index, frames_count(), _angle, _angle + ggo::pi<float>() / 8);

  float x = -1 + _radius * std::cos(angle);
  float y = _radius * std::sin(angle);

  _artist.render_bitmap(buffer, std::complex<float>(x, y), 1);
}

