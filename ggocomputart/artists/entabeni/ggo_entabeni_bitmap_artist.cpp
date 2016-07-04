#include "ggo_entabeni_bitmap_artist.h"
#include "ggo_entabeni.h"
#include "ggo_entabeni_animation_artist.h"

//////////////////////////////////////////////////////////////
ggo_entabeni_bitmap_artist::ggo_entabeni_bitmap_artist(int render_width, int render_height)
:
ggo_bitmap_artist_abc(render_width, render_height)
{
}

//////////////////////////////////////////////////////////////
void ggo_entabeni_bitmap_artist::render_bitmap(uint8_t * buffer)
{
  auto grid = ggo_entabeni::create_grid(true, false);
  auto color_map = ggo_entabeni::create_color_map();
  float dangle = ggo::rand_float(0.f, 2 * ggo::PI<float>());

  auto image = make_image_buffer(buffer);
  ggo_entabeni::render_bitmap(image, grid, color_map, 0.f, dangle);
}

