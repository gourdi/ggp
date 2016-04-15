#include "ggo_fill.h"
#include <ggo_array.h>
#include <ggo_brush_abc.h>
#include <ggo_perlin_noise_field_2d.h>
#include <ggo_gaussian_field_2d.h>

namespace
{
  //////////////////////////////////////////////////////////////
  template <typename T>
  void fill_scalar_field2d(ggo::image_buffer_abc<T> & image_data, 
                           const ggo::scalar_field_2d_abc<float> & scalar_field2d,
                           const T & color1,
                           const T & color2)
  {
    const int width = image_data.get_width();
    const int height = image_data.get_height();

    image_data.for_each_pixel([&](int x, int y)
    {
      float val = scalar_field2d.evaluate(static_cast<float>(x), static_cast<float>(y));
      
      image_data.pack(x, y, val * color1 + (1 - val) * color2);
    });
  }
}

//////////////////////////////////////////////////////////////
// SOLID COLOR
namespace ggo
{
  //////////////////////////////////////////////////////////////
  template <typename T>
  void fill_solid(ggo::image_buffer_abc<T> & image_data, const T & color)
  {
    image_data.for_each_pixel([&](int x, int y)
    {
      image_data.pack(x, y, color);
    });
  }

  //////////////////////////////////////////////////////////////
  void fill_solid(ggo::gray_image_buffer_abc & gray_image_data, float gray)
  {
    fill_solid<float>(gray_image_data, gray);
  }

  //////////////////////////////////////////////////////////////
  void fill_solid(ggo::rgb_image_buffer_abc & rgb_image_data, const ggo::color & color)
  {
    fill_solid<ggo::color>(rgb_image_data, color);
  }

  //////////////////////////////////////////////////////////////
  void fill_solid_rgb(uint8_t * buffer, int count, const ggo::color & color)
  {
    if (buffer != nullptr)
    {
      uint8_t r = color.r8();
      uint8_t g = color.g8();
      uint8_t b = color.b8();
    
      while (--count >= 0)
      {
        *buffer++ = r;
        *buffer++ = g;
        *buffer++ = b;
      }
    }
  }

  //////////////////////////////////////////////////////////////
  void fill_solid_rgb(float * buffer, int count, const ggo::color & color)
  {
    if (buffer != nullptr)
    {
      while (--count >= 0)
      {
        *buffer++ = color.r();
        *buffer++ = color.g();
        *buffer++ = color.b();
      }
    }
  }
}

//////////////////////////////////////////////////////////////
// CHECKER
namespace ggo
{
  //////////////////////////////////////////////////////////////
  template <typename T>
  void fill_checker(ggo::image_buffer_abc<T> & image_data, 
                    const T & color1,
                    const T & color2,
                    int tile_size)
  {
    const int width = image_data.get_width();
    const int height = image_data.get_height();

    image_data.for_each_pixel([&](int x, int y)
    {
      int index_x = x / tile_size;
      int index_y = y / tile_size;
                  
      image_data.pack(x, y, ((index_x + index_y) % 2) ? color1 : color2);
    });
  }

  //////////////////////////////////////////////////////////////
  void fill_checker(ggo::gray_image_buffer_abc & gray_image_data,
                    float gray1, 
                    float gray2, 
                    int tile_size)
  {
    fill_checker<float>(gray_image_data, gray1, gray2, tile_size);
  }

  //////////////////////////////////////////////////////////////
  void fill_checker(ggo::rgb_image_buffer_abc & rgb_image_data,
                    const ggo::color & color1,
                    const ggo::color & color2,
                    int tile_size)
  {
    fill_checker<ggo::color>(rgb_image_data, color1, color2, tile_size);
  }
}

//////////////////////////////////////////////////////////////
// CURVE
namespace ggo
{
  //////////////////////////////////////////////////////////////
  template <typename T>
  void fill_color_curve(ggo::image_buffer_abc<T> & image_data, 
                        const ggo::curve_abc<float, T> & color_curve)
  {
    T color;

    const int width = image_data.get_width();
    const int height = image_data.get_height();
    
    image_data.for_each_pixel([&](int y)
    {
      color = color_curve.evaluate(y / float(height));
    },
    [&](int x, int y)
    {
      image_data.pack(x, y, color);
    });
  }

  //////////////////////////////////////////////////////////////
  void fill_color_curve(ggo::gray_image_buffer_abc & gray_image_data,
                        const ggo::curve_abc<float, float> & color_curve)
  {
    fill_color_curve<float>(gray_image_data, color_curve);
  }
       
  //////////////////////////////////////////////////////////////
  void fill_color_curve(ggo::rgb_image_buffer_abc & rgb_image_data,
                        const ggo::curve_abc<float, ggo::color> & color_curve)
  {
    fill_color_curve<ggo::color>(rgb_image_data, color_curve);
  }
}

//////////////////////////////////////////////////////////////
// GAUSSIAN
namespace ggo
{
  //////////////////////////////////////////////////////////////
  template <typename T>
  void fill_gaussian(ggo::image_buffer_abc<T> & image_data, 
                     float var,
                     const T & color1,
                     const T & color2)
  {
    ggo::gaussian_field2d<float> gaussian_field2d;
    
    gaussian_field2d._x = 0.5f * image_data.get_width();
    gaussian_field2d._y = 0.5f * image_data.get_height();
    gaussian_field2d._var = var;
    gaussian_field2d._amp = 1;
    
    fill_scalar_field2d<T>(image_data, gaussian_field2d, color1, color2);
  }

  //////////////////////////////////////////////////////////////
  void fill_gaussian(ggo::gray_image_buffer_abc & gray_image_data,
                     float var,
                     float gray1,
                     float gray2)
  {
    fill_gaussian<float>(gray_image_data, var, gray1, gray2);
  }
                          
  //////////////////////////////////////////////////////////////
  void fill_gaussian(ggo::rgb_image_buffer_abc & rgb_image_data,
                     float var,
                     const ggo::color & color1,
                     const ggo::color & color2)
  {
    fill_gaussian<ggo::color>(rgb_image_data, var, color1, color2);
  }
}

//////////////////////////////////////////////////////////////
// 4 COLORS
namespace ggo
{
  //////////////////////////////////////////////////////////////
  template <typename T>
  void fill_4_colors(ggo::image_buffer_abc<T> & image_data,
                     const T & color1,
                     const T & color2,
                     const T & color3,
                     const T & color4)
  {
    T color5, color6;

    const int width = image_data.get_width();
    const int height = image_data.get_height();
    
    image_data.for_each_pixel([&](int y)
    {
      color5 = (static_cast<float>(y) * color1 + static_cast<float>(height - y) * color2) / static_cast<float>(height);
      color6 = (static_cast<float>(y) * color3 + static_cast<float>(height - y) * color4) / static_cast<float>(height);
    },
    [&](int x, int y)
    { 
      image_data.pack(x, y, (static_cast<float>(x) * color5 + static_cast<float>(width - x) * color6) / static_cast<float>(width));
    });
  }

  //////////////////////////////////////////////////////////////
  void fill_4_colors(ggo::gray_image_buffer_abc & gray_image_data,
                     float gray1,
                     float gray2,
                     float gray3,
                     float gray4)
  {
    fill_4_colors<float>(gray_image_data, gray1, gray2, gray3, gray4);
  }
                          
  //////////////////////////////////////////////////////////////
  void fill_4_colors(ggo::rgb_image_buffer_abc & rgb_image_data,
                     const ggo::color & color1,
                     const ggo::color & color2,
                     const ggo::color & color3,
                     const ggo::color & color4)
  {
    fill_4_colors<ggo::color>(rgb_image_data, color1, color2, color3, color4);
  }
}

//////////////////////////////////////////////////////////////
// BRUSH
namespace ggo
{
  //////////////////////////////////////////////////////////////
  template <typename T>
  void fill_brush(ggo::image_buffer_abc<T> & image_data, 
                  const ggo::brush_abc<T> & brush,
                  const ggo::pixel_sampler_abc & sampler)
  {
    ggo::rect_float bounding_rect = ggo::rect_float::from_left_right_bottom_top(-0.5f, image_data.get_width() - 0.5f, -0.5f, image_data.get_height() - 0.5f);

    image_data.for_each_pixel([&](int x, int y)
    {
      T color(0);

      sampler.sample_pixel(x, y, [&](float x_f, float y_f)
      {
        color += brush.get(x_f, y_f, bounding_rect, image_data.get_width(), image_data.get_height());
      });
      color /= static_cast<float>(sampler.get_samples_count());
      
      image_data.pack(x, y, color);
    });
  }

  //////////////////////////////////////////////////////////////
  void fill_brush(ggo::gray_image_buffer_abc & gray_image_data,
                  const ggo::gray_brush_abc & brush,
                  const ggo::pixel_sampler_abc & sampler)
  {
    fill_brush<float>(gray_image_data, brush, sampler);
  }
        
  //////////////////////////////////////////////////////////////                                         
  void fill_brush(ggo::rgb_image_buffer_abc & rgb_image_data,
                  const ggo::rgb_brush_abc & brush,
                  const ggo::pixel_sampler_abc & sampler)
  {
    fill_brush<ggo::color>(rgb_image_data, brush, sampler);
  }    
}         

//////////////////////////////////////////////////////////////
// PERLIN

namespace ggo
{
  //////////////////////////////////////////////////////////////
  template <typename T>
  void fill_perlin(ggo::image_buffer_abc<T> & image_data, 
                   float size,
                   const T & color1,
                   const T & color2)
  {
    ggo::perlin_noise_field2d<float> perlin_field2d;
    
    perlin_field2d.add_octave(size, 1);
    
    fill_scalar_field2d<T>(image_data, perlin_field2d, color1, color2);
  }

  //////////////////////////////////////////////////////////////
  void fill_perlin(ggo::gray_image_buffer_abc & gray_image_data,
                   float size,
                   float gray1,
                   float gray2)
  {
    fill_perlin<float>(gray_image_data, size, gray1, gray2);
  }
         
  //////////////////////////////////////////////////////////////                      
  void fill_perlin(ggo::rgb_image_buffer_abc & rgb_image_data,
                   float size,
                   const ggo::color & color1,
                   const ggo::color & color2)
  {
    fill_perlin<ggo::color>(rgb_image_data, size, color1, color2);
  }
}
