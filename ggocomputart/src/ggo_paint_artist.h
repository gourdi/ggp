#ifndef __GGO_PAINT_ARTIST_ABC__
#define __GGO_PAINT_ARTIST_ABC__

#include <ggo_artist.h>
#include <2d/ggo_pixel_type.h>

namespace ggo
{
  class paint_artist : public artist
  {
  public:

                      paint_artist(int width, int height, int line_byte_step, ggo::pixel_type pixel_type, ggo::lines_order memory_lines_order);

    int               line_byte_step() const { return _line_byte_step; }
    ggo::pixel_type   pixel_type() const { return _pixel_type; }
    ggo::lines_order  memory_lines_order() const { return _memory_lines_order; }

  private:

    const int _line_byte_step;
    const ggo::pixel_type _pixel_type;
    const ggo::lines_order _memory_lines_order;
  };
}

#endif
