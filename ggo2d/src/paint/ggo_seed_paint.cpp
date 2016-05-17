#include "ggo_seed_paint.h"

namespace
{
  /////////////////////////////////////////////////////////////////////
  template <typename color_type>
  void paint_shape_seed(ggo::image_abc<color_type> & image,
                        const ggo::seed_paintable_shape2d_abc<float> & shape,
                        const color_type & color,
                        float opacity,
                        const ggo::pixel_aligned_sampler_abc & sampler,
                        const ggo::blender_abc<color_type> & blender)
  {
    ggo::rect_float rect = shape.get_seed_rect();

    int seed_left   = ggo::clamp<int>(static_cast<int>(rect.left()),      0, image.get_width() - 1);
    int seed_right  = ggo::clamp<int>(static_cast<int>(rect.right() + 1), 0, image.get_width() - 1);
    int seed_bottom = ggo::clamp<int>(static_cast<int>(rect.bottom()),    0, image.get_height() - 1);
    int seed_top    = ggo::clamp<int>(static_cast<int>(rect.top() + 1),   0, image.get_height() - 1);

    // First paint the seed.
    for (int y = seed_bottom; y <= seed_top; ++y)
    {
      for (int x = seed_left; x <= seed_right; ++x)
      {
        color_type pixel_color = image.get(x, y);
        pixel_color = blender.blend(pixel_color, opacity, color);
        image.set(x, y, pixel_color);
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
            color_type pixel_color = image.get(seed_left, y);
            pixel_color = blender.blend(pixel_color, sampling * opacity, color);
            image.set(seed_left, y, pixel_color);
          }
        }
      }
      
      // Right
      if (seed_right < image.get_width() - 1)
      {
        seed_right += 1;
        for (int y = seed_bottom; y <= seed_top; ++y)
        {
          float sampling = sampler.sample_shape(seed_right, y, shape);
          if (sampling > 0)
          {
            done = false;
            color_type pixel_color = image.get(seed_right, y);
            pixel_color = blender.blend(pixel_color, sampling * opacity, color);
            image.set(seed_right, y, pixel_color);
          }
        }
      }

      // Top
      if (seed_top < image.get_height() - 1)
      {
        seed_top += 1;
        for (int x = seed_left; x <= seed_right; ++x)
        {
          float sampling = sampler.sample_shape(x, seed_top, shape);
          if (sampling > 0)
          {
            done = false;
            color_type pixel_color = image.get(x, seed_top);
            pixel_color = blender.blend(pixel_color, sampling * opacity, color);
            image.set(x, seed_top, pixel_color);
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
            color_type pixel_color = image.get(x, seed_bottom);
            pixel_color = blender.blend(pixel_color, sampling * opacity, color);
            image.set(x, seed_bottom, pixel_color);
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
  void paint_seed_shape(ggo::rgb_image_abc & image,
                        const ggo::seed_paintable_shape2d_abc<float> & shape,
                        const ggo::color & color,
                        float opacity,
                        const ggo::pixel_aligned_sampler_abc & sampler,
                        const ggo::rgb_blender_abc & blender)
  {
    paint_shape_seed<ggo::color>(image, shape, color, opacity, sampler, blender);
  }                          

  /////////////////////////////////////////////////////////////////////
  void paint_seed_shape(ggo::gray_image_abc & image,
                        const ggo::seed_paintable_shape2d_abc<float> & shape,
                        float gray,
                        float opacity,
                        const ggo::pixel_aligned_sampler_abc & sampler,
                        const ggo::gray_blender_abc & blender)
  {
    paint_shape_seed<float>(image, shape, gray, opacity, sampler, blender);
  }
}

