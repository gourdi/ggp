#include "ggo_alpha_bitmap_artist.h"
#include "ggo_alpha_animation_artist.h"

//////////////////////////////////////////////////////////////
void ggo::alpha_bitmap_artist::render_bitmap(void * buffer, int render_width, int render_height) const
{
  alpha_animation_artist artist(render_width, render_height);

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
