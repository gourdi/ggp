#include "ggo_aggregation_animation_artist.h"

////////////////////////////////////////////////////////
ggo::aggregation_animation_artist::aggregation_animation_artist(int width, int height, int line_step, ggo::pixel_buffer_format pbf, rendering_type rt)
:
animation_artist_abc(width, height, line_step, pbf, rt),
_artist(width, height)
{
}

//////////////////////////////////////////////////////////////
void ggo::aggregation_animation_artist::init()
{
  _frame_index = -1;
}

//////////////////////////////////////////////////////////////
bool ggo::aggregation_animation_artist::update()
{
  ++_frame_index;

  const int frames_count = 300;

  if (_frame_index > frames_count)
  {
    return false;
  }

  int points_count = _artist.get_final_points_count() / frames_count;

  _artist.update(points_count);

  return true;
}

//////////////////////////////////////////////////////////////
void ggo::aggregation_animation_artist::render_frame(void * buffer, const ggo::rect_int & clipping)
{
  if (buffer != nullptr)
  {
    _artist.render(buffer, get_line_step(), get_pixel_buffer_format());
  }
}
