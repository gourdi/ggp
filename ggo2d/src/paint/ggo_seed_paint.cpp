#include "ggo_seed_paint.h"

namespace
{
  /////////////////////////////////////////////////////////////////////
  template <typename T>
  void paint_shape_seed(ggo::image_buffer_abc<T> & image_data,
                        const ggo::seed_paintable_shape2d_abc<float> & shape,
                        const T & color,
                        float opacity,
                        const ggo::pixel_aligned_sampler_abc & sampler,
                        const ggo::blender_abc<T> & blender)
  {
    ggo::rect_float rect = shape.get_seed_rect();

    int seed_left   = ggo::clamp<int>(static_cast<int>(rect.left()),      0, image_data.get_width() - 1);
    int seed_right  = ggo::clamp<int>(static_cast<int>(rect.right() + 1), 0, image_data.get_width() - 1);
    int seed_bottom = ggo::clamp<int>(static_cast<int>(rect.bottom()),    0, image_data.get_height() - 1);
    int seed_top    = ggo::clamp<int>(static_cast<int>(rect.top() + 1),   0, image_data.get_height() - 1);

    // First paint the seed.
    for (int y = seed_bottom; y <= seed_top; ++y)
    {
      for (int x = seed_left; x <= seed_right; ++x)
      {
        T pixel_color = image_data.unpack(x, y);
        pixel_color = blender.blend(pixel_color, opacity, color);
        image_data.pack(x, y, pixel_color);
      }
    }
    
    // Then make it grow.
    while (true)
    {
      bool done = true;
      
      // Left
      if (seed_left > 0)
      {
        seed_left -= 1;
        for (int y = seed_bottom; y <= seed_top; ++y)
        {
          float sampling = sampler.sample_shape(seed_left, y, shape);
          if (sampling > 0)
          {
            done = false;
            T pixel_color = image_data.unpack(seed_left, y);
            pixel_color = blender.blend(pixel_color, sampling * opacity, color);
            image_data.pack(seed_left, y, pixel_color);
          }
        }
      }
      
      // Right
      if (seed_right < image_data.get_width() - 1)
      {
        seed_right += 1;
        for (int y = seed_bottom; y <= seed_top; ++y)
        {
          float sampling = sampler.sample_shape(seed_right, y, shape);
          if (sampling > 0)
          {
            done = false;
            T pixel_color = image_data.unpack(seed_right, y);
            pixel_color = blender.blend(pixel_color, sampling * opacity, color);
            image_data.pack(seed_right, y, pixel_color);
          }
        }
      }

      // Top
      if (seed_top < image_data.get_height() - 1)
      {
        seed_top += 1;
        for (int x = seed_left; x <= seed_right; ++x)
        {
          float sampling = sampler.sample_shape(x, seed_top, shape);
          if (sampling > 0)
          {
            done = false;
            T pixel_color = image_data.unpack(x, seed_top);
            pixel_color = blender.blend(pixel_color, sampling * opacity, color);
            image_data.pack(x, seed_top, pixel_color);
          }
        }
      }
      
      // Bottom
      if (seed_bottom > 0)
      {
        seed_bottom -= 1;
        for (int x = seed_left; x <= seed_right; ++x)
        {
          float sampling = sampler.sample_shape(x, seed_bottom, shape);
          if (sampling > 0)
          {
            done = false;
            T pixel_color = image_data.unpack(x, seed_bottom);
            pixel_color = blender.blend(pixel_color, sampling * opacity, color);
            image_data.pack(x, seed_bottom, pixel_color);
          }
        }
      }

      if (done == true)
      {
        break;
      }
    }
  }
}

namespace ggo
{
  /////////////////////////////////////////////////////////////////////
  void paint_seed_shape(ggo::rgb_image_buffer_abc & image_data,
                        const ggo::seed_paintable_shape2d_abc<float> & shape,
                        const ggo::color & color,
                        float opacity,
                        const ggo::pixel_aligned_sampler_abc & sampler,
                        const ggo::rgb_blender_abc & blender)
  {
    paint_shape_seed<ggo::color>(image_data, shape, color, opacity, sampler, blender);
  }                          

  /////////////////////////////////////////////////////////////////////
  void paint_seed_shape(ggo::gray_image_buffer_abc & image_data,
                        const ggo::seed_paintable_shape2d_abc<float> & shape,
                        float gray,
                        float opacity,
                        const ggo::pixel_aligned_sampler_abc & sampler,
                        const ggo::gray_blender_abc & blender)
  {
    paint_shape_seed<float>(image_data, shape, gray, opacity, sampler, blender);
  }
}

