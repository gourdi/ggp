#ifndef __GGO_KERNEL_PROCESSING__
#define __GGO_KERNEL_PROCESSING__

#if 0

#include <stdint.h>

namespace ggo
{
  void	dilate_rectangle_kernel(const uint8_t * input, uint8_t * output, int width, int height, int kernel_width, int kernel_height);
  void	erode_rectangle_kernel(const uint8_t * input, uint8_t * output, int width, int height, int kernel_width, int kernel_height);

  void	dilate_circle_kernel(const uint8_t * input, uint8_t * output, int width, int height, float radius);
  void	erode_circle_kernel(const uint8_t * input, uint8_t * output, int width, int height, float radius);
}

#endif

#endif

