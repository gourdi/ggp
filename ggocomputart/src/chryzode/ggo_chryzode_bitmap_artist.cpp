#include "ggo_chryzode_bitmap_artist.h"
#include "ggo_chryzode_artist.h"
#include <2d/processing/ggo_gaussian_blur.h>

//////////////////////////////////////////////////////////////
ggo::chryzode_bitmap_artist::chryzode_bitmap_artist(int width, int height, int line_byte_step, ggo::pixel_type pixel_type, ggo::lines_order memory_lines_order)
:
bitmap_artist_abc(width, height, line_byte_step, pixel_type, memory_lines_order)
{
}

//////////////////////////////////////////////////////////////
void ggo::chryzode_bitmap_artist::render_bitmap(void * buffer) const
{
  chryzode_artist artist(width(), height(), line_byte_step(), pixel_type(), memory_lines_order());

  chryzode_params params;
  params._multiplier1 = ggo::rand<float>(3, 11);
  params._multiplier2 = ggo::rand<float>(3, 11);
  params._offset1 = ggo::rand<float>(-15, 15);
  params._offset2 = ggo::rand<float>(-15, 15);
  params._modulo_start = ggo::rand<int>(16, 1024);
  
  params._modulo_end = params._modulo_start;
  int lines_count = params._modulo_start;
  while (lines_count < (1 << 16))
  {
    params._modulo_end += 1;
    lines_count += params._modulo_end;
  }
  
  float radius = ggo::rand<float>(0.2f, 0.35f) * min_size();
  
  float hue_start = ggo::rand<float>();
  float hue_end = hue_start + (ggo::rand<bool>() ? -0.2f : 0.2f);
    
  artist.render_chryzode(buffer, radius, params, hue_start, hue_end);
}
