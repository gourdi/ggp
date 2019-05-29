#include "ggo_badaboum_animation_artist.h"

//////////////////////////////////////////////////////////////
ggo::badaboum_animation_artist::badaboum_animation_artist(int width, int height, int line_step, ggo::image_format format)
:
fixed_frames_count_animation_artist_abc(width, height, line_step, format, badaboum_artist::frames_count()),
_artist(width, height, line_step, format, ggo::sampling_8x8)
{
}

//////////////////////////////////////////////////////////////
void ggo::badaboum_animation_artist::render_frame(void * buffer, int frame_index, float time_step)
{
  _artist.preprocess_frame(frame_index, 0, { 0, 0 }, time_step);
  _artist.render_tile(buffer, frame_index, ggo::rect_int::from_size(size()));
}

