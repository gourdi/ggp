#include "ggo_kanji_bitmap_artist.h"
#include "ggo_kanji_artist.h"
#include <ggo_buffer_fill.h>

//////////////////////////////////////////////////////////////
ggo::kanji_bitmap_artist::kanji_bitmap_artist(int width, int height, int line_step, ggo::pixel_buffer_format pbf)
:
bitmap_artist_abc(width, height, line_step, pbf)
{
	
}

//////////////////////////////////////////////////////////////
void ggo::kanji_bitmap_artist::render_bitmap(void * buffer) const
{
  switch (get_pixel_buffer_format())
  {
  case ggo::rgb_8u_yu:
    ggo::fill_solid<ggo::rgb_8u_yu>(buffer, get_width(), get_height(), get_line_step(), ggo::black_8u(), ggo::rect_int::from_width_height(get_width(), get_height()));
    break;
  case ggo::bgra_8u_yd:
    ggo::fill_solid<ggo::bgra_8u_yd>(buffer, get_width(), get_height(), get_line_step(), ggo::black_8u(), ggo::rect_int::from_width_height(get_width(), get_height()));
    break;
  default:
    GGO_FAIL();
    break;
  }

  ggo::kanji_artist	artist(get_width(), get_height());

	artist.init();
	
	int frame_index = 0;
	while (artist.update(frame_index) == true)
	{
    artist.render_frame(buffer, get_line_step(), get_pixel_buffer_format(), frame_index, ggo::rect_int::from_width_height(get_width(), get_height()));
		++frame_index;
	}
}