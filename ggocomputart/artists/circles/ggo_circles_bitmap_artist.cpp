#include "ggo_circles_bitmap_artist.h"
#include "ggo_circles_artist.h"
#include <ggo_fill.h>

//////////////////////////////////////////////////////////////
ggo_circles_bitmap_artist::ggo_circles_bitmap_artist(int render_width, int render_height)
:
ggo_bitmap_artist_abc(render_width, render_height)
{
}

//////////////////////////////////////////////////////////////
void ggo_circles_bitmap_artist::render_bitmap(uint8_t * buffer)
{
  ggo::fill_solid_rgb_8u(buffer, get_render_width(), get_render_height(), 3 * get_render_width(), ggo::color::get_random().color_8u());

  ggo_circles_artist circles_artist(get_render_width(), get_render_height());
  auto all_discs = circles_artist.generate_discs();

  for (const auto & discs : all_discs)
  {
    for (const auto & colored_disc : discs)
    {
      ggo_circles_artist::paint_disc(buffer, get_render_width(), get_render_height(), colored_disc);
    }
  }
}