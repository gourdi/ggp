#include "ggo_kanji_animation_artist.h"
#include <ggo_buffer_fill.h>

//////////////////////////////////////////////////////////////
ggo::kanji_animation_artist::kanji_animation_artist(int width, int height, int line_step, ggo::pixel_buffer_format pbf, rendering_type rt)
:
animation_artist_abc(width, height, line_step, pbf, rt),
_kanji_artist(width, height)
{
	
}

//////////////////////////////////////////////////////////////
void ggo::kanji_animation_artist::init_animation()
{
  _frame_index = -1;
  _kanji_artist.init_animation();
}

//////////////////////////////////////////////////////////////
bool ggo::kanji_animation_artist::prepare_frame()
{
  ++_frame_index;
  return _kanji_artist.prepare_frame(_frame_index);
}

//////////////////////////////////////////////////////////////
void ggo::kanji_animation_artist::render_frame(void * buffer, const ggo::rect_int & clipping)
{
  if (_frame_index == 0)
  {
    switch (get_pixel_buffer_format())
    {
    case ggo::rgb_8u_yu:
      ggo::fill_solid<ggo::rgb_8u_yu>(buffer, get_width(), get_height(), get_line_step(), ggo::black_8u(), clipping);
      break;
    case ggo::bgra_8u_yd:
      ggo::fill_solid<ggo::bgra_8u_yd>(buffer, get_width(), get_height(), get_line_step(), ggo::black_8u(), clipping);
      break;
    default:
      GGO_FAIL();
      break;
    }
  }

  _kanji_artist.render_frame(buffer, get_line_step(), get_pixel_buffer_format(), _frame_index, clipping);
}

