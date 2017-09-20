#include "ggo_toutouyoutou_bitmap_artist.h"
#include "ggo_toutouyoutou_animation_artist.h"

//////////////////////////////////////////////////////////////
ggo::toutouyoutou_bitmap_artist::toutouyoutou_bitmap_artist(int width, int height, int line_step, ggo::pixel_buffer_format pbf)
:
bitmap_artist_abc(width, height, line_step, pbf)
{
}

//////////////////////////////////////////////////////////////
void ggo::toutouyoutou_bitmap_artist::render_bitmap(void * buffer) const
{
  int frames_count = ggo::rand<int>(100, 400);

  ggo::toutouyoutou_animation_artist anim_artist(get_width(), get_height(), get_line_step(), get_pixel_buffer_format(), ggo::animation_artist_abc::offscreen_rendering);

  anim_artist.init();

  anim_artist.update();
  anim_artist.render_frame(buffer, ggo::rect_int::from_width_height(get_width(), get_height()));

  for (int i = 1; i < frames_count; ++i)
  {
    anim_artist.update();
    anim_artist.render_frame(nullptr, ggo::rect_int::from_width_height(get_width(), get_height()));
  }

  anim_artist.update();
  anim_artist.render_frame(buffer, ggo::rect_int::from_width_height(get_width(), get_height()));
}
