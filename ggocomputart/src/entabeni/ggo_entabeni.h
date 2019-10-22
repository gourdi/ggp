#ifndef __GGO_ENTABENI__
#define __GGO_ENTABENI__

#include <kernel/memory/ggo_array.h>
#include <kernel/math/interpolation/ggo_curve.h>
#include <2d/ggo_pixel_type.h>

namespace ggo
{
  class entabeni
  {
  public:

    static  ggo::array<float, 2> create_grid(bool loop_x, bool loop_y);
    static  ggo::cubic_curve<float, ggo::rgb_32f> create_color_map();

    static  void render_bitmap(void * buffer, int width, int height, int line_byte_step, ggo::pixel_type pixel_type, ggo::lines_order memory_lines_order,
      const ggo::array<float, 2> & grid, const ggo::cubic_curve<float, ggo::rgb_32f> & color_map, float z, float angle);
  };
}

#endif
