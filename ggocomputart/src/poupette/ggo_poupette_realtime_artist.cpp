#include "ggo_poupette_realtime_artist.h"

//////////////////////////////////////////////////////////////
ggo::poupette_realtime_artist::poupette_realtime_artist(int width, int height, int line_step, ggo::image_format format)
:
ggo::realtime_artist_abc(width, height, line_step, format)
{

}

//////////////////////////////////////////////////////////////
void ggo::poupette_realtime_artist::preprocess_frame(int frame_index)
{

}

//////////////////////////////////////////////////////////////
void ggo::poupette_realtime_artist::render_tile(void * buffer, int frame_index, const ggo::rect_int & clipping)
{
  _artist.render_tile(buffer, width(), height(), line_step(), format(), frame_index, clipping, ggo::sampling_2x2);
}

//////////////////////////////////////////////////////////////
bool ggo::poupette_realtime_artist::finished(int frame_index) const
{
  return _artist.finished(frame_index);
}


