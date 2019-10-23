#include "ggo_entabeni_bitmap_artist.h"
#include "ggo_entabeni.h"
#include "ggo_entabeni_animation_artist.h"

//////////////////////////////////////////////////////////////
void ggo::entabeni_bitmap_artist::render_bitmap(void * buffer, int width, int height, int line_byte_step, ggo::pixel_type pixel_type, ggo::lines_order memory_lines_order) const
{
  auto grid = ggo::entabeni::create_grid(true, false);
  auto color_map = ggo::entabeni::create_color_map();
  float dangle = ggo::rand<float>(0.f, 2 * ggo::pi<float>());

  ggo::entabeni::render_bitmap(buffer, width, height, line_byte_step, pixel_type, memory_lines_order, grid, color_map, 0.f, dangle);
}

