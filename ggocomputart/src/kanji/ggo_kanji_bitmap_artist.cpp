#include "ggo_kanji_bitmap_artist.h"
#include "ggo_kanji_artist.h"
#include <2d/fill/ggo_fill.h>

//////////////////////////////////////////////////////////////
ggo::kanji_bitmap_artist::kanji_bitmap_artist(int width, int height, int line_step, ggo::image_format format)
:
bitmap_artist_abc(width, height, line_step, format)
{
	
}

//////////////////////////////////////////////////////////////
void ggo::kanji_bitmap_artist::render_bitmap(void * buffer) const
{
  switch (get_format())
  {
  case ggo::rgb_8u_yu:
    ggo::fill_solid<ggo::rgb_8u_yu>(buffer, get_width(), get_height(), get_line_step(), ggo::black_8u(), ggo::rect_int::from_width_height(get_width(), get_height()));
    break;
  case ggo::bgrx_8u_yd:
    ggo::fill_solid<ggo::bgrx_8u_yd>(buffer, get_width(), get_height(), get_line_step(), ggo::black_8u(), ggo::rect_int::from_width_height(get_width(), get_height()));
    break;
  default:
    GGO_FAIL();
    break;
  }

  ggo::kanji_artist	artist(get_width(), get_height());

	artist.init_animation();
	
	int frame_index = 0;
	while (artist.prepare_frame(frame_index) == true)
	{
    artist.render_frame(buffer, get_line_step(), get_format(), frame_index, ggo::rect_int::from_width_height(get_width(), get_height()));
		++frame_index;
	}
}