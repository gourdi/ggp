#ifndef __GGO_CANVAS__
#define __GGO_CANVAS__

#include <ggo_pixel_buffer.h>

namespace ggo
{
  class canvas
  {
  public:

    enum class main_direction
    {
      horizontal,
      vertical
    };

    canvas(main_direction direction, void * buffer, int width, int height, int line_byte_step, pixel_buffer_format pbf);

    ggo::pos2i from_canvas_to_render(const ggo::pos2f & p) const;
    ggo::pos2f from_render_to_canvas(const ggo::pos2i & p) const;

  private:

    const main_direction _main_direction;
    void * const _buffer;
    const int _width;
    const int _height;
    const int _line_byte_step;
    const pixel_buffer_format _pbf;
  };
}

#endif

