#ifndef __GGO_BOX_FILTER__
#define __GGO_BOX_FILTER__

#include <stdint.h>

namespace ggo
{
  void box_filter3_1d(uint8_t * data, int size, int stride = 1);
  void box_filter3_1d(float * data, int size, int stride = 1);
  void box_filter3_1d(double * data, int size, int stride = 1);
  
  void box_filter3_2d(uint8_t * data, int size_x, int size_y, int stride = 1);
  void box_filter3_2d(float * data, int size_x, int size_y, int stride = 1);
  void box_filter3_2d(double * data, int size_x, int size_y, int stride = 1);
  
  void box_filter5_1d(uint8_t * data, int size, int stride = 1);
  void box_filter5_1d(float * data, int size, int stride = 1);
  void box_filter5_1d(double * data, int size, int stride = 1);
  
  void box_filter5_2d(uint8_t * data, int size_x, int size_y, int stride = 1);
  void box_filter5_2d(float * data, int size_x, int size_y, int stride = 1);
  void box_filter5_2d(double * data, int size_x, int size_y, int stride = 1);
}

#endif