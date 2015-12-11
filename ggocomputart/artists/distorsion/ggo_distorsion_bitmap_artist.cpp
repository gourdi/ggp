#include "ggo_distorsion_bitmap_artist.h"
#include "ggo_distorsion_animation_artist.h"

//////////////////////////////////////////////////////////////
ggo_distorsion_bitmap_artist::ggo_distorsion_bitmap_artist(int render_width, int render_height)
:
ggo_bitmap_artist_abc(render_width, render_height)
{
}

//////////////////////////////////////////////////////////////
void ggo_distorsion_bitmap_artist::render_bitmap(uint8_t * buffer)
{
  ggo_distorsion_animation_artist artist(get_render_width(), get_render_height());
    
  artist.init();
  artist.render_last_frame(buffer);
}
