#include "ggo_blur_pixel_sampler.h"

namespace ggo
{
  /////////////////////////////////////////////////////////////////////
  blur_pixel_sampler::blur_pixel_sampler(float radius, int steps)
  :
  _radius(radius)
  {
    float radius_step = radius / steps;

    for (int dy = -steps; dy <= steps; ++dy)
    {
      float dy_f = dy * radius_step;
    
      for (int dx = -steps; dx <= steps; ++dx)
      {
        float dx_f = dx * radius_step;
        
        if (dx_f * dx_f + dy_f * dy_f < radius * radius)
        {
          _samples.emplace_back(dx_f, dy_f);
        }
      }
    }
  }

  /////////////////////////////////////////////////////////////////////
  void blur_pixel_sampler::sample_pixel(int x, int y, ggo::sample_func fct) const
  {
    for (const auto & sample : _samples)
    {
      float x_f = x + sample.x();
      float y_f = y + sample.y();
      fct(x_f, y_f);
    }
  }
}


