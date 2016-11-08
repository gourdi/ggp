#include "ggo_distorsion_bitmap_artist.h"
#include "ggo_distorsion_animation_artist.h"

//////////////////////////////////////////////////////////////
ggo::distorsion_bitmap_artist::distorsion_bitmap_artist(int render_width, int render_height)
:
bitmap_artist_abc(render_width, render_height)
{
}

//////////////////////////////////////////////////////////////
void ggo::distorsion_bitmap_artist::render_bitmap(void * buffer) const
{
  ggo::distorsion_animation_artist artist(get_render_width(), get_render_height());
    
  artist.init();
  artist.render_last_frame(buffer);
}
