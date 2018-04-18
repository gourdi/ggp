#ifndef __GGO_ENTABENI__
#define __GGO_ENTABENI__

#include <ggo_array.h>
#include <ggo_curve.h>
#include <2d/ggo_color.h>
#include <ggo_pixel_buffer.h>

namespace ggo
{
  class entabeni
  {
  public:

    static  ggo::array<float, 2> create_grid(bool loop_x, bool loop_y);
    static  ggo::cubic_curve<float, ggo::color_32f> create_color_map();

    static  void render_bitmap(void * buffer, int width, int height, int line_step, ggo::pixel_buffer_format pbf, const ggo::array<float, 2> & grid,
      const ggo::cubic_curve<float, ggo::color_32f> & color_map, float z, float angle);
  };
}

#endif
