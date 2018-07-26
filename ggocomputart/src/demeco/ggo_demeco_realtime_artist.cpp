#include "ggo_demeco_realtime_artist.h"

//////////////////////////////////////////////////////////////
ggo::demeco_realtime_artist::demeco_realtime_artist(int width, int height, int line_step, ggo::image_format format)
:
realtime_artist_abc(width, height, line_step, format),
_artist(width, height)
{
}

//////////////////////////////////////////////////////////////
void ggo::demeco_realtime_artist::preprocess_frame(int frame_index)
{
  _finished = _artist.update();
}

//////////////////////////////////////////////////////////////
void ggo::demeco_realtime_artist::render_tile(void * buffer, int frame_index, const ggo::rect_int & clipping)
{
  _artist.render_tile(buffer, line_step(), frame_index, clipping);
}
