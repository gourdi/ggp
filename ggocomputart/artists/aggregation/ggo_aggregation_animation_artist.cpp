#include "ggo_aggregation_animation_artist.h"

namespace
{
  const int frames_count = 300;
}

////////////////////////////////////////////////////////
ggo_aggregation_animation_artist::ggo_aggregation_animation_artist(int render_width, int render_height)
:
ggo_animation_artist_abc(render_width, render_height)
{
}

//////////////////////////////////////////////////////////////
void ggo_aggregation_animation_artist::init_sub()
{
  _artist.reset(new ggo::aggregation_artist(get_render_width(), get_render_height()));
}

//////////////////////////////////////////////////////////////
bool ggo_aggregation_animation_artist::render_next_frame_sub(uint8_t * buffer, int frame_index)
{
  if (frame_index >= frames_count)
  {
    return false;
  }

  _artist->update(400);
  _artist->render(buffer);

	return true;
}

