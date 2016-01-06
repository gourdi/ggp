#include "ggo_pixel_sampler_abc.h"

namespace ggo
{
  /////////////////////////////////////////////////////////////////////
  void pixel_sampler_abc::sample_shape(int x, int y, const ggo::samplable_shape2d_abc<float> & shape, ggo::sample_func fct) const
  {
    sample_pixel(x, y, [&](float x_f, float y_f)
    {
      if (shape.is_point_inside(x_f, y_f) == true)
      {
        fct(x_f, y_f);
      }
    });
  }

  /////////////////////////////////////////////////////////////////////
  float pixel_aligned_sampler_abc::sample_shape(int x, int y, const ggo::samplable_shape2d_abc<float> & shape) const
  {
    float result = 0;

    sample_pixel(x, y, [&](float x_f, float y_f)
    {
      if (shape.is_point_inside(x_f, y_f) == true)
      {
        result += 1;
      }
    });
    
    return result / get_samples_count();
  }

  /////////////////////////////////////////////////////////////////////
  void pixel_sampler_1::sample_pixel(int x, int y, ggo::sample_func fct) const
  {
    fct(static_cast<float>(x), static_cast<float>(y));
  }

  /////////////////////////////////////////////////////////////////////
  void pixel_sampler_2X2::sample_pixel(int x, int y, ggo::sample_func fct) const
  {
    float x0 = x - 0.25f;
    float x1 = x + 0.25f;
      
    float y0 = y - 0.25f;
    float y1 = y + 0.25f;
      
    fct(x0, y0);
    fct(x0, y1);
      
    fct(x1, y0);
    fct(x1, y1);
  }

  /////////////////////////////////////////////////////////////////////
  void pixel_sampler_4X4::sample_pixel(int x, int y, ggo::sample_func fct) const
  {
    float x0 = x - 3 / 8.0f;
    float x1 = x - 1 / 8.0f;
    float x2 = x + 1 / 8.0f;
    float x3 = x + 3 / 8.0f;
      
    float y0 = y - 3 / 8.0f;
    float y1 = y - 1 / 8.0f;
    float y2 = y + 1 / 8.0f;
    float y3 = y + 3 / 8.0f;
      
    fct(x0, y0);
    fct(x0, y1);
    fct(x0, y2);
    fct(x0, y3);
          
    fct(x1, y0);
    fct(x1, y1);
    fct(x1, y2);
    fct(x1, y3);
          
    fct(x2, y0);
    fct(x2, y1);
    fct(x2, y2);
    fct(x2, y3);
          
    fct(x3, y0);
    fct(x3, y1);
    fct(x3, y2);
    fct(x3, y3);
  }

  /////////////////////////////////////////////////////////////////////
  void pixel_sampler_8X8::sample_pixel(int x, int y, ggo::sample_func fct) const
  {
    for (int y_i = -7; y_i <= 7; y_i += 2)
    {
      float y_f = y + y_i / 16.f;
          
      for (int x_i = -7; x_i <= 7; x_i += 2)
      {
        float x_f = x + x_i / 16.f;

        fct(x_f, y_f);
      }
    }
  }

  /////////////////////////////////////////////////////////////////////
  void pixel_sampler_16X16::sample_pixel(int x, int y, ggo::sample_func fct) const
  {
    for (int y_i = -15; y_i <= 15; y_i += 2)
    {
      float y_f = y + y_i / 32.f;
         
      for (int x_i = -15; x_i <= 15; x_i += 2)
      {
         float x_f = x + x_i / 32.f;

         fct(x_f, y_f);
      }
    }
  }
}
