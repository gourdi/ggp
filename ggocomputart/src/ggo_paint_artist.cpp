#include "ggo_paint_artist.h"

namespace ggo
{
  //////////////////////////////////////////////////////////////
  paint_artist::paint_artist(int width, int height, int line_byte_step, ggo::pixel_type pixel_type, lines_order memory_lines_order)
    :
    artist(width, height), _line_byte_step(line_byte_step), _pixel_type(pixel_type), _memory_lines_order(memory_lines_order)
  {
  }
}

