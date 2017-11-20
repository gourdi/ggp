#include "ggo_alpha_bitmap_artist.h"
#include "ggo_alpha_animation_artist.h"

//////////////////////////////////////////////////////////////
ggo::alpha_bitmap_artist::alpha_bitmap_artist(int width, int height, int line_step, ggo::pixel_buffer_format pbf)
:
bitmap_artist_abc(width, height, line_step, pbf)
{

}

//////////////////////////////////////////////////////////////
void ggo::alpha_bitmap_artist::render_bitmap(void * buffer) const
{
  alpha_animation_artist artist(get_width(), get_height(), get_line_step(), get_pixel_buffer_format(), ggo::animation_artist_abc::offscreen_rendering);

  int frame_index = 0;

  while (artist.prepare_frame() == true)
  {
    if (artist.get_items_count() >= 3)
    {
      break;
    }
  }

  int timer = ggo::rand<int>(5, 10);
  for (int i = 0; i < timer; ++i)
  {
    artist.prepare_frame();
    artist.process_frame(nullptr, ggo::rect_int::from_width_height(get_width(), get_height()));
  }

  artist.process_frame(buffer, ggo::rect_int::from_width_height(get_width(), get_height()));
}
