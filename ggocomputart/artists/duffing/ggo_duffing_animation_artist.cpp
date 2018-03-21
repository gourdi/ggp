#include "ggo_duffing_animation_artist.h"
#include "ggo_duffing_offscreen_animation_artist.h"
#include "ggo_duffing_realtime_animation_artist.h"

//////////////////////////////////////////////////////////////
ggo::duffing_animation_artist::duffing_animation_artist(int width, int height, int line_step, ggo::pixel_buffer_format pbf, rendering_type rt)
:
animation_artist_abc(width, height, line_step, pbf, rt)
{
  if (rt == offscreen_rendering)
  {
    _artist.reset(new ggo::duffing_offscreen_animation_artist(width, height, line_step, pbf));
  }
  else
  {
    _artist.reset(new ggo::duffing_realtime_animation_artist(width, height, line_step, pbf, rt));
  }
}

//////////////////////////////////////////////////////////////
void ggo::duffing_animation_artist::init_animation()
{
  _artist->init_animation();
}

//////////////////////////////////////////////////////////////
bool ggo::duffing_animation_artist::prepare_frame()
{
  return _artist->prepare_frame();
}

//////////////////////////////////////////////////////////////
void ggo::duffing_animation_artist::render_frame(void * buffer, const ggo::rect_int & clipping)
{
  _artist->render_frame(buffer, clipping);
}
