#include "ggo_poupette_animation_artist.h"

//////////////////////////////////////////////////////////////
ggo::poupette_animation_artist::poupette_animation_artist(int width, int height, int line_step, ggo::image_format format)
:
ggo::fixed_frames_count_animation_artist_abc(width, height, line_step, format, ggo::poupette_artist::_frames_count)
{

}

//////////////////////////////////////////////////////////////
void ggo::poupette_animation_artist::render_frame(void * buffer, int frame_index, float time_step)
{
  _artist.render_tile(buffer, width(), height(), line_step(), format(), frame_index, ggo::rect_int::from_size(size()), ggo::sampling_16x16);
}
