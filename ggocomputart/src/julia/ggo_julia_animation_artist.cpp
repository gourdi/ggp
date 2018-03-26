#include "ggo_julia_animation_artist.h"

namespace
{
  const int frames_count = 500;
}

//////////////////////////////////////////////////////////////
ggo::julia_animation_artist::julia_animation_artist(int width, int height, int line_step, ggo::pixel_buffer_format pbf, rendering_type rt)
:
animation_artist_abc(width, height, line_step, pbf, rt),
_artist(width, height)
{
	
}

//////////////////////////////////////////////////////////////
void ggo::julia_animation_artist::init_animation()
{
  _frame_index = -1;
  _angle = ggo::rand<float>(0, 2 * ggo::pi<float>());
  _radius = ggo::rand<float>(0.254f, 0.256f);
}

//////////////////////////////////////////////////////////////
bool ggo::julia_animation_artist::prepare_frame()
{
  ++_frame_index;

  if (_frame_index > frames_count)
  {
    return false;
  }

  return true;
}

//////////////////////////////////////////////////////////////
void ggo::julia_animation_artist::render_frame(void * buffer, const ggo::rect_int & clipping)
{
  float angle = ggo::ease_inout(_frame_index, frames_count, _angle, _angle + ggo::pi<float>() / 8);

  float x = -1 + _radius * std::cos(angle);
  float y = _radius * std::sin(angle);

  _artist.render_bitmap(buffer, get_line_step(), get_pixel_buffer_format(), std::complex<float>(x, y), 1);
}

