#include "ggo_circles_bitmap_artist.h"
#include "ggo_circles_artist.h"
#include <2d/fill/ggo_fill.h>

//////////////////////////////////////////////////////////////
ggo::circles_bitmap_artist::circles_bitmap_artist(int width, int height, int line_step, ggo::image_format format)
:
bitmap_artist_abc(width, height, line_step, format)
{
}

//////////////////////////////////////////////////////////////
void ggo::circles_bitmap_artist::render_bitmap(void * buffer) const
{
  const ggo::color_8u bkgd_color(ggo::rand<uint8_t>(), ggo::rand<uint8_t>(), ggo::rand<uint8_t>());
  ggo::fill_solid<ggo::rgb_8u_yu>(buffer, get_width(), get_height(), get_line_step(), bkgd_color, ggo::rect_int::from_width_height(get_width(), get_height()));

  auto all_discs = ggo::circles_artist::generate_discs(get_width(), get_height());

  for (const auto & discs : all_discs)
  {
    for (const auto & colored_disc : discs)
    {
      ggo::circles_artist::paint_disc(buffer, get_width(), get_height(), get_line_step(), get_format(), colored_disc);
    }
  }
}
