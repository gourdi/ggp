#include "ggo_paint_helpers.h"
#include <ggo_kernel.h>
#include <ggo_rgb_image_buffer.h>
#include <ggo_gray_image_buffer.h>

/////////////////////////////////////////////////////////////////////
// Paint dots.
namespace
{
  template <typename T>
  void paint_dot(ggo::image_abc<T> & image,
                 float pos_x, float pos_y, float radius,
                 const T & color,
                 float opacity,
                 const ggo::pixel_sampler_abc & sampler,
                 const ggo::blender_abc<T> & blender)
  {
    if (radius <= 0)
    {
      return;
    }
    
    int	left 	  = int(pos_x - radius - 0.5);
    int	right 	= int(pos_x + radius + 0.5);
    int	bottom 	= int(pos_y - radius - 0.5);
    int	top 	  = int(pos_y + radius + 0.5);
    
    if ((left >= image.get_width()) || (right < 0) || (bottom >= image.get_height()) || (top < 0))
    {
      return;
    }
    
    left	  = ggo::clamp(left, 0, image.get_width() - 1);
    right	  = ggo::clamp(right, 0, image.get_width() - 1);
    bottom	= ggo::clamp(bottom, 0, image.get_height() - 1);
    top		  = ggo::clamp(top, 0, image.get_height() - 1);

    const float sqr_radius = radius * radius;
    const float optim = ggo::PI<float>() / radius;
    
    for (int y = bottom; y <= top; ++y)
    {
      float dy2 = ggo::square(y - pos_y);
      
      for (int x = left; x <= right; ++x)
      {
        float dx2 = ggo::square(x - pos_x);
        float hypot = dx2 + dy2;
        
        if (hypot < sqr_radius)
        {
          float v = std::sqrt(hypot);
          v *= optim;
          v = 0.5f + 0.5f * std::cos(v);
          v *= opacity;
          
          T bkdg_color = image.read(x, y);
          T pixel_color(0);
          sampler.sample_pixel(x, y, [&](float x_f, float y_f)
          {
            pixel_color += blender.blend(bkdg_color, opacity, color);
          });
          pixel_color /= static_cast<float>(sampler.get_samples_count());
          
          image.write(x, y, pixel_color);
        }
      }
    }
  }
}

/////////////////////////////////////////////////////////////////////
namespace ggo
{
  /////////////////////////////////////////////////////////////////////
  void paint_dot_rgb(uint8_t * buffer, int width, int height, float x, float y, float radius, const ggo::color & color, float opacity)
  {
    ggo::rgb_image_buffer_uint8 image_data(width, height, buffer);
    
    ::paint_dot<ggo::color>(image_data, x, y, radius, color, opacity, ggo::pixel_sampler_1(), ggo::rgb_alpha_blender());
  }

  /////////////////////////////////////////////////////////////////////
  void paint_dot(ggo::rgb_image_abc & image,
                 float x, float y, float radius,
                 const ggo::color & color,
                 float opacity,
                 const ggo::pixel_sampler_abc & sampler,
                 const ggo::rgb_blender_abc & blender)
  {
    ::paint_dot<ggo::color>(image, x, y, radius, color, opacity, sampler, blender);
  }
                      
  /////////////////////////////////////////////////////////////////////
  void paint_dot(ggo::gray_image_abc & image,
                 float x, float y, float radius,
                 float gray,
                 float opacity,
                 const ggo::pixel_sampler_abc & sampler,
                 const ggo::gray_blender_abc & blender)
  {
    ::paint_dot<float>(image, x, y, radius, gray, opacity, sampler, blender);
  }
}

