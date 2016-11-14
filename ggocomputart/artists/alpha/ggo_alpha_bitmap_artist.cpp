#include "ggo_alpha_bitmap_artist.h"
#include "ggo_alpha_animation_artist.h"

//////////////////////////////////////////////////////////////
ggo::alpha_bitmap_artist::alpha_bitmap_artist(int render_width, int render_height)
:
bitmap_artist_abc(render_width, render_height)
{

}

//////////////////////////////////////////////////////////////
void ggo::alpha_bitmap_artist::render_bitmap(void * buffer) const
{
  alpha_animation_artist artist(get_render_width(), get_render_height());

  int frame_index = 0;

  artist.init_sub();
  while (artist.render_next_frame_sub(nullptr, frame_index) == true)
  {
    if (artist.get_items_count() >= 3)
    {
      break;
    }

    ++frame_index;
  }

  int timer = ggo::rand<int>(5, 10);
  for (int i = 0; i < timer; ++i)
  {
    artist.render_next_frame_sub(nullptr, frame_index);

    frame_index++;
  }

  artist.render_next_frame_sub(buffer, frame_index);
}
