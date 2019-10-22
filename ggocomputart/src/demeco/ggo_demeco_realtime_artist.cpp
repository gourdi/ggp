#include "ggo_demeco_realtime_artist.h"

//////////////////////////////////////////////////////////////
ggo::demeco_realtime_artist::demeco_realtime_artist(int width, int height, int line_byte_step, ggo::pixel_type pixel_type, ggo::lines_order memory_lines_order)
:
realtime_artist_abc(width, height, line_byte_step, pixel_type, memory_lines_order),
_artist(width, height)
{
}

//////////////////////////////////////////////////////////////
void ggo::demeco_realtime_artist::preprocess_frame(int frame_index, uint32_t cursor_events, ggo::pos2_i cursor_pos, float time_step)
{
  _finished = _artist.update();
}

//////////////////////////////////////////////////////////////
void ggo::demeco_realtime_artist::render_tile(void * buffer, int frame_index, const ggo::rect_int & clipping)
{
  _artist.render_tile(buffer, line_byte_step(), frame_index, clipping);
}
