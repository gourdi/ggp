#include "ggo_circles_bitmap_artist.h"
#include "ggo_circles_artist.h"
#include <ggo_buffer_fill.h>

//////////////////////////////////////////////////////////////
ggo::circles_bitmap_artist::circles_bitmap_artist(int render_width, int render_height)
:
bitmap_artist_abc(render_width, render_height)
{
}

//////////////////////////////////////////////////////////////
void ggo::circles_bitmap_artist::render_bitmap(void * buffer) const
{
  const ggo::color_8u bkgd_color(ggo::rand<uint8_t>(), ggo::rand<uint8_t>(), ggo::rand<uint8_t>());
  ggo::fill_solid<ggo::rgb_8u_yu>(buffer, get_render_width(), get_render_height(), 3 * get_render_width(), bkgd_color);

  auto all_discs = ggo::circles_artist::generate_discs(get_render_width(), get_render_height());

  for (const auto & discs : all_discs)
  {
    for (const auto & colored_disc : discs)
    {
      ggo::circles_artist::paint_disc(buffer, get_render_width(), get_render_height(), colored_disc);
    }
  }
}