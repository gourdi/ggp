#include "ggo_badaboum_realtime_artist.h"

//////////////////////////////////////////////////////////////
ggo::badaboum_realtime_artist::badaboum_realtime_artist(int width, int height, int line_step, ggo::image_format format)
:
fixed_frames_count_realtime_artist_abc(width, height, line_step, format),
_artist(width, height, line_step, format, ggo::sampling_1)
{
}

//////////////////////////////////////////////////////////////
void ggo::badaboum_realtime_artist::preprocess_frame(int frame_index, uint32_t cursor_events, ggo::pos2_i cursor_pos, float time_step)
{
  return _artist.preprocess_frame(frame_index, cursor_events, cursor_pos, time_step);
}

//////////////////////////////////////////////////////////////
void ggo::badaboum_realtime_artist::render_tile(void * buffer, int frame_index, const ggo::rect_int & clipping)
{
  return _artist.render_tile(buffer, frame_index, clipping);
}

//////////////////////////////////////////////////////////////
int ggo::badaboum_realtime_artist::frames_count() const
{
  return badaboum_artist::frames_count();
}

