#include "ggo_distorsion_bitmap_artist.h"
#include "ggo_distorsion_animation_artist.h"

//////////////////////////////////////////////////////////////
ggo::distorsion_bitmap_artist::distorsion_bitmap_artist(int width, int height, int line_step, ggo::pixel_buffer_format pbf)
:
bitmap_artist_abc(width, height, line_step, pbf)
{
}

//////////////////////////////////////////////////////////////
void ggo::distorsion_bitmap_artist::render_bitmap(void * buffer) const
{
  ggo::distorsion_animation_artist artist(get_width(), get_height(), get_line_step(), get_pixel_buffer_format(), ggo::animation_artist_abc::offscreen_rendering);
    
  artist.init_animation();
  artist.render_last_frame(buffer);
}
