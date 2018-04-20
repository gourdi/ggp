#include "ggo_entabeni_bitmap_artist.h"
#include "ggo_entabeni.h"
#include "ggo_entabeni_animation_artist.h"

//////////////////////////////////////////////////////////////
ggo::entabeni_bitmap_artist::entabeni_bitmap_artist(int width, int height, int line_step, ggo::image_format format)
:
bitmap_artist_abc(width, height, line_step, format)
{
}

//////////////////////////////////////////////////////////////
void ggo::entabeni_bitmap_artist::render_bitmap(void * buffer) const
{
  auto grid = ggo::entabeni::create_grid(true, false);
  auto color_map = ggo::entabeni::create_color_map();
  float dangle = ggo::rand<float>(0.f, 2 * ggo::pi<float>());

  ggo::entabeni::render_bitmap(buffer, get_width(), get_height(), get_line_step(), get_format(), grid, color_map, 0.f, dangle);
}

