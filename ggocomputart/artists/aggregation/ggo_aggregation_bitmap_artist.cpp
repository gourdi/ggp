#include "ggo_aggregation_bitmap_artist.h"
#include "ggo_aggregation_artist.h"

//////////////////////////////////////////////////////////////
ggo_aggregation_bitmap_artist::ggo_aggregation_bitmap_artist(int render_width, int render_height)
:
ggo_bitmap_artist_abc(render_width, render_height)
{
}

//////////////////////////////////////////////////////////////
void ggo_aggregation_bitmap_artist::render_bitmap(uint8_t * buffer)
{
  ggo::aggregation_artist artist(get_render_width(), get_render_height());

  artist.update(300 * 750);
  artist.render(buffer);
}
