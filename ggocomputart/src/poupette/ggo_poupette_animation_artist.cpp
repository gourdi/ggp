#include "ggo_poupette_animation_artist.h"

//////////////////////////////////////////////////////////////
ggo::poupette_animation_artist::poupette_animation_artist(
  int width, int height, int line_byte_step,
  ggo::pixel_type pixel_type, ggo::lines_order memory_lines_order,
  ggo::ratio fps)
:
ggo::progress_animation_artist(width, height, line_byte_step, pixel_type, memory_lines_order, 10, fps),
_artist(2000.f)
{

}

//////////////////////////////////////////////////////////////
void ggo::poupette_animation_artist::render_frame(void * buffer, float progress)
{
  _artist.render_tile(buffer, width(), height(), line_byte_step(), pixel_type(), memory_lines_order(),
    progress, ggo::rect_int::from_size(size()), ggo::sampling_16x16);
}
