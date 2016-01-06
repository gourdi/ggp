#include "ggo_shape_sampling_coef.h"

namespace ggo
{
  /////////////////////////////////////////////////////////////////////
  uint16_t get_sampling_coef4X4(const ggo::samplable_shape2d_abc<float> & shape, int x, int y)
  {
    uint16_t sampling = 0;
    
    float x1(x - 3.0f / 8);
    float x2(x - 1.0f / 8);
    float x3(x + 1.0f / 8);
    float x4(x + 3.0f / 8);
					    
    float y1(y - 3.0f / 8);
    float y2(y - 1.0f / 8);
    float y3(y + 1.0f / 8);
    float y4(y + 3.0f / 8);

    if (shape.is_point_inside(x1, y1) == true) { sampling |= 1 << 0; }
    if (shape.is_point_inside(x1, y2) == true) { sampling |= 1 << 1; }
    if (shape.is_point_inside(x1, y3) == true) { sampling |= 1 << 2; }
    if (shape.is_point_inside(x1, y4) == true) { sampling |= 1 << 3; }

    if (shape.is_point_inside(x2, y1) == true) { sampling |= 1 << 4; }
    if (shape.is_point_inside(x2, y2) == true) { sampling |= 1 << 5; }
    if (shape.is_point_inside(x2, y3) == true) { sampling |= 1 << 6; }
    if (shape.is_point_inside(x2, y4) == true) { sampling |= 1 << 7; }

    if (shape.is_point_inside(x3, y1) == true) { sampling |= 1 << 8; }
    if (shape.is_point_inside(x3, y2) == true) { sampling |= 1 << 9; }
    if (shape.is_point_inside(x3, y3) == true) { sampling |= 1 << 10; }
    if (shape.is_point_inside(x3, y4) == true) { sampling |= 1 << 11; }

    if (shape.is_point_inside(x4, y1) == true) { sampling |= 1 << 12; }
    if (shape.is_point_inside(x4, y2) == true) { sampling |= 1 << 13; }
    if (shape.is_point_inside(x4, y3) == true) { sampling |= 1 << 14; }
    if (shape.is_point_inside(x4, y4) == true) { sampling |= 1 << 15; }
    
    return sampling;
  }

  /////////////////////////////////////////////////////////////////////
  uint64_t get_sampling_coef8X8(const ggo::samplable_shape2d_abc<float> & shape, int x, int y)
  {
    uint64_t sampling = 0;
    int i = 0;
    
    for (int y_i = -7; y_i <= 7; y_i += 2)
    {
      float y_f = y + y_i / 16.f;
      
      for (int x_i = -7; x_i <= 7; x_i += 2)
      {
        float x_f = x + x_i / 16.f;
        
        if (shape.is_point_inside(x_f, y_f) == true)
        {
          uint64_t mask = 1;
          mask <<= i;
          sampling |= mask;
        }
        
        ++i;
      }
    }

    return sampling;
  }
}

