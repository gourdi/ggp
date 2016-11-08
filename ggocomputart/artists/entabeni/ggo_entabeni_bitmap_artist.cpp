#include "ggo_entabeni_bitmap_artist.h"
#include "ggo_entabeni.h"
#include "ggo_entabeni_animation_artist.h"

//////////////////////////////////////////////////////////////
ggo::entabeni_bitmap_artist::entabeni_bitmap_artist(int render_width, int render_height)
:
bitmap_artist_abc(render_width, render_height)
{
}

//////////////////////////////////////////////////////////////
void ggo::entabeni_bitmap_artist::render_bitmap(void * buffer) const
{
  auto grid = ggo::entabeni::create_grid(true, false);
  auto color_map = ggo::entabeni::create_color_map();
  float dangle = ggo::rand<float>(0.f, 2 * ggo::pi<float>());

  ggo::entabeni::render_bitmap(buffer, get_render_width(), get_render_height(), grid, color_map, 0.f, dangle);
}

