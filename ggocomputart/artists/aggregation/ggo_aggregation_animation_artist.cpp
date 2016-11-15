#include "ggo_aggregation_animation_artist.h"

////////////////////////////////////////////////////////
ggo::aggregation_animation_artist::aggregation_animation_artist(int width, int height, int line_step, ggo::pixel_buffer_format pbf)
:
animation_artist_abc(width, height, line_step, pbf),
_artist(width, height, line_step, pbf)
{
}

//////////////////////////////////////////////////////////////
void ggo::aggregation_animation_artist::init_sub()
{
}

//////////////////////////////////////////////////////////////
bool ggo::aggregation_animation_artist::render_next_frame_sub(void * buffer, int frame_index)
{
  const int frames_count = 300;

  if (frame_index > frames_count)
  {
    return false;
  }

  int points_count = _artist.get_final_points_count() / frames_count;

  _artist.update(points_count);

  if (buffer != nullptr)
  {
    _artist.render(buffer);
  }

  return true;
}
