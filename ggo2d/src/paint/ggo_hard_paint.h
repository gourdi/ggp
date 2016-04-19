#ifndef __GGO_HARD_PAINT__
#define __GGO_HARD_PAINT__

#include <stdint.h>

namespace ggo
{
  class color;
  
  void	paint_vert_line_rgb(uint8_t * buffer, int width, int height, int x, int y_from, int y_to, const ggo::color & color);
  void	paint_horz_line_rgb(uint8_t * buffer, int width, int height, int x_from, int x_to, int y, const ggo::color & color);
  void	paint_line_rgb(uint8_t * buffer, int width, int height, float x_from, float y_from, float x_to, float y_to, const ggo::color & color);

  void	paint_rect_rgb(uint8_t * buffer, int width, int height, int left, int right, int top, int bottom, const ggo::color & color, float opacity = 1);
  void	paint_rect_rgb(float * buffer, int width, int height, int left, int right, int top, int bottom, const ggo::color & color, float opacity = 1);
  
  void  paint_pixel(uint8_t * buffer, int width, int height, int x, int y, const ggo::color & color, float opacity = 1);
}

#endif
