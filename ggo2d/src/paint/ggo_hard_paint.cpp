#include "ggo_hard_paint.h"
#include <ggo_kernel.h>
#include <ggo_shapes2d.h>
#include <ggo_color.h>
#include <ggo_blender_abc.h>
#include <ggo_rgb_image_buffer.h>

namespace
{
  /////////////////////////////////////////////////////////////////////
  template <typename color_type>
  void paint_pixel(ggo::image_abc<color_type> & image,
                   int x, int y,
                   const color_type & color,
                   float opacity = 1,
                   const ggo::blender_abc<color_type> & blender = ggo::alpha_blender<color_type>())
  {
    if (x >= 0 && x < image.get_width() && y >= 0 && y < image.get_height())
    {
      color_type bkgd_color = image.get(x, y);
      color_type final_color = blender.blend(bkgd_color, opacity, color);
      image.set(x, y, final_color);
    }
  }

  /////////////////////////////////////////////////////////////////////
  template <typename color_type>
  void paint_horz_line(ggo::image_abc<color_type> & image,
                       int x_from, int x_to, int y, 
                       const color_type & color,
                       float opacity = 1,
                       const ggo::blender_abc<color_type> & blender = ggo::alpha_blender<color_type>())
  {
    if ((y < 0 ) || (y > image.get_height() - 1))
    {
      return;
    }

    if (x_from > x_to)
    {
      std::swap(x_from, x_to);
    }
    GGO_ASSERT(x_from <= x_to);
    
    if (x_to < 0 || x_from >= image.get_width())
    {
      return;
    }

    x_from  = ggo::clamp(x_from, 0, image.get_width() - 1);
    x_to    = ggo::clamp(x_to, 0, image.get_width() - 1);
    
    for (int x = x_from; x <= x_to; ++x)
    {
      paint_pixel<color_type>(image, x, y, color, opacity, blender);
    }
  }

  /////////////////////////////////////////////////////////////////////
  template <typename color_type>
  void paint_vert_line(ggo::image_abc<color_type> & image,
                       int y_from, int y_to, int x,
                       const color_type & color,
                       float opacity = 1,
                       const ggo::blender_abc<color_type> & blender = ggo::alpha_blender<color_type>())
  {
    if ((x < 0 ) || (x > image.get_width() - 1))
    {
      return;
    }

    if (y_from > y_to)
    {
      std::swap(y_from, y_to);
    }
    GGO_ASSERT(y_from <= y_to);
    
    if (y_to < 0 || y_from >= image.get_height())
    {
      return;
    }

    ggo::rect_float bounding_rect = ggo::rect_float::from_left_right_bottom_top(x - 0.5f, x + 0.5f, y_from - 0.5f, y_to + 0.5f);

    y_from	= ggo::clamp(y_from, 0, image.get_height() -1);
    y_to	  = ggo::clamp(y_to, 0, image.get_height() - 1); 

    for (int y = y_from; y <= y_to; ++y)
    {
      paint_pixel<color_type>(image, x, y, color, opacity, blender);
    }
  }

  /////////////////////////////////////////////////////////////////////
  template <typename color_type>
  void paint_line(ggo::image_abc<color_type> & image,
                  float x_from, float y_from, float x_to, float y_to,
                  const color_type & color,
                  float opacity = 1,
                  const ggo::blender_abc<color_type> & blender = ggo::alpha_blender<color_type>())
  {
    // Clamp line coordinates.
    
    // Make sure 'from' is on the left of 'to'.
    if (x_from > x_to)
    {
      std::swap(x_from, x_to);
      std::swap(y_from, y_to);
    }
    
    // Left clamp.
    float t_left = -x_from / (x_to - x_from);
    if (t_left > 1) { return; }
    if (t_left > 0)
    {
      x_from = x_from + t_left * (x_to - x_from);
      y_from = y_from + t_left * (y_to - y_from);
    }
    
    // Right clamp.
    float t_right = (image.get_width() - x_from) / (x_to - x_from);
    if (t_right < 0) { return; }
    if (t_right < 1)
    {
      x_to = x_from + t_right * (x_to - x_from);
      y_to = y_from + t_right * (y_to - y_from);		
    }
    
    // Make sure 'from' is below 'to'.
    if (y_from > y_to)
    {
      std::swap(x_from, x_to);
      std::swap(y_from, y_to);
    }
    
    // Bottom clamp.
    float t_bottom = -y_from / (y_to - y_from);
    if (t_bottom > 1) { return; }
    if (t_bottom > 0)
    {
      x_from = x_from + t_bottom * (x_to - x_from);
      y_from = y_from + t_bottom * (y_to - y_from);
    }
    
    // Top clamp.
    float t_top = (image.get_height() - y_from) / (y_to - y_from);
    if (t_top < 0) { return; }
    if (t_top < 1)
    {
      x_to = x_from + t_top * (x_to - x_from);
      y_to = y_from + t_top * (y_to - y_from);	
    }

    int x_from_i = ggo::clamp(ggo::to<int>(x_from), 0, image.get_width() - 1);
    int y_from_i = ggo::clamp(ggo::to<int>(y_from), 0, image.get_height() - 1);
    int x_to_i   = ggo::clamp(ggo::to<int>(x_to), 0, image.get_width() - 1);
    int y_to_i   = ggo::clamp(ggo::to<int>(y_to), 0, image.get_height() - 1);

    // Since the last point isn't painted, well, paint it.
    paint_pixel<color_type>(image, x_from_i, y_from_i, color, opacity, blender);
    
    // Paint the line.
    int dx = x_from_i - x_to_i;
    int dy = y_from_i - y_to_i;
    if (abs(y_from_i - y_to_i) > abs(x_from_i - x_to_i))
    {
      if (y_from_i > y_to_i)
      {
        std::swap(x_from_i, x_to_i);
        std::swap(y_from_i, y_to_i);
      }
       
      for (int y = y_from_i; y != y_to_i; ++y)
      {
        int x = static_cast<int>(x_from + (y - y_from_i) * dx / dy);
        paint_pixel<color_type>(image, x, y, color, opacity, blender);
      }
    }
    else
    {
      if (x_from_i > x_to_i)
      {
        std::swap(x_from_i, x_to_i);
        std::swap(y_from_i, y_to_i);
      }
        
      for (int x = x_from_i; x != x_to_i; ++x)
      {
        int y = static_cast<int>(y_from + (x - x_from_i) * dy / dx);
        paint_pixel<color_type>(image, x, y, color, opacity, blender);
      }
    }
  }

  /////////////////////////////////////////////////////////////////////
  template <typename color_type>
  void paint_rect(ggo::image_abc<color_type> & image,
                  int left, int right, int top, int bottom,
                  const color_type & color,
                  float opacity = 1,
                  const ggo::blender_abc<color_type> & blender = ggo::alpha_blender<color_type>())
  {
    if (left > right)
    {
      std::swap(left, right);
    }
    if (top > bottom)
    {
      std::swap(top, bottom);
    }

    if (left >= image.get_width() || right < 0 || bottom >= image.get_height() || top < 0)
    {
      return;
    }

    for (int y = top; y <= bottom; ++y)
    {
      for (int x = left; x <= right; ++x)
      {
        paint_pixel<color_type>(image, x, y, color, opacity, blender);
      }
    }
  }
}

namespace ggo
{
  /////////////////////////////////////////////////////////////////////
  void paint_horz_line_rgb(uint8_t * buffer, int width, int height, int x_from, int x_to, int y, const ggo::color & color)
  {
    ggo::rgb_image_buffer_uint8 image(width, height, buffer);
    
    paint_horz_line<ggo::color>(image, x_from, x_to, y, color);
  }

  /////////////////////////////////////////////////////////////////////
  void paint_vert_line_rgb(uint8_t * buffer, int width, int height, int x, int y_from, int y_to, const ggo::color & color)
  {
    ggo::rgb_image_buffer_uint8 image(width, height, buffer);
    
    paint_vert_line<ggo::color>(image, x, y_from, y_to, color);
  }

  /////////////////////////////////////////////////////////////////////
  void paint_line_rgb(uint8_t * buffer, int width, int height, float x_from, float y_from, float x_to, float y_to, const ggo::color & color)
  {
    ggo::rgb_image_buffer_uint8 image(width, height, buffer);
    
    paint_line<ggo::color>(image, x_from, y_from, x_to, y_to, color);
  }

  /////////////////////////////////////////////////////////////////////
  void paint_rect_rgb(uint8_t * buffer, int width, int height, int left, int right, int top, int bottom, const ggo::color & color, float opacity)
  {
    ggo::rgb_image_buffer_uint8 image(width, height, buffer);
    
    paint_rect<ggo::color>(image, left, right, top, bottom, color, opacity);
  }

  /////////////////////////////////////////////////////////////////////
  void paint_rect_rgb(float * buffer, int width, int height, int left, int right, int top, int bottom, const ggo::color & color, float opacity)
  {
    ggo::rgb_image_buffer_float image(width, height, buffer);
    
    paint_rect<ggo::color>(image, left, right, top, bottom, color, opacity);
  }
  
  /////////////////////////////////////////////////////////////////////
  void paint_pixel(uint8_t * buffer, int width, int height, int x, int y, const ggo::color & color, float opacity)
  {
    ggo::rgb_image_buffer_uint8 image_data(width, height, buffer);
    
    ::paint_pixel(image_data, x, y, color, opacity);
  }
}
