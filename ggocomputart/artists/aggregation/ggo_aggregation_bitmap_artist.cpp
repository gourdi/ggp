#include "ggo_aggregation_bitmap_artist.h"
#include "ggo_aggregation_artist.h"

//////////////////////////////////////////////////////////////
ggo_aggregation_bitmap_artist::ggo_aggregation_bitmap_artist(int render_width, int render_height)
:
ggo_bitmap_artist_abc(render_width, render_height)
{
}

//////////////////////////////////////////////////////////////
void ggo_aggregation_bitmap_artist::render_bitmap(uint8_t * buffer)
{/*
  // Compute points.
  float attraction_factor = ggo::rand_float(0.005f, 0.02f);
  float rotation_factor = ggo::rand_float(0, 0.01f);
  ggo::vec3f rotation_vector(ggo::rand_float(-1, 1), ggo::rand_float(-1, 1), ggo::rand_float(-1, 1));
  
  auto points = ggo::aggregation_artist::compute_points(attraction_factor, rotation_vector, rotation_factor);
  
  // Render output image.
  ggo::aggregation_artist::render(ggo::rand_float(0.5f, 0.9f), points, 0, buffer, get_render_width(), get_render_height());*/
}
