#include "ggo_layer.h"
#include <ggo_color.h>

namespace 
{
  //////////////////////////////////////////////////////////////
  template <typename T, typename Func>
  void flatten_sprite_func(ggo::image_data_abc<T> & image,
                           const ggo::gray_image_data_abc & sprite,
                           Func fct, 
                           int sprite_pos_x, int sprite_pos_y,
                           const ggo::blender_abc<T> & blender)
  {
    for (int y_sprite = 0; y_sprite < sprite.get_height(); ++y_sprite)
    {
      int y_image = sprite_pos_y + y_sprite;
          
      if (y_image >= 0 && y_image < image.get_height())
      {
        for (int x_sprite = 0; x_sprite < sprite.get_width(); ++x_sprite)
        {
          int x_image = sprite_pos_x + x_sprite;
                  
          if (x_image >= 0 && x_image < image.get_width())
          {
            T color = image.unpack(x_image, y_image);   
            float opacity = sprite.unpack(x_sprite, y_sprite);
                      
            T sprite_color = fct(x_sprite, y_sprite);
      
            color = blender.blend(color, opacity, sprite_color);
                      
            image.pack(x_image, y_image, color);
          }
        }
      }
    }
  }
  
  //////////////////////////////////////////////////////////////
  template <typename T>
  void flatten_sprite_t(ggo::image_data_abc<T> & image,
                        const ggo::gray_image_data_abc & sprite,
                        const ggo::brush_abc<T> & brush,
                        const ggo::pixel_aligned_sampler_abc & brush_sampler, 
                        int sprite_pos_x, int sprite_pos_y,
                        const ggo::blender_abc<T> & blender)
  {
    float left    = sprite_pos_x - 0.5f;
    float right   = sprite_pos_y + sprite.get_width() - 0.5f;
    float bottom  = sprite_pos_x - 0.5f;
    float top     = sprite_pos_y + sprite.get_height() - 0.5f;
    ggo::rect_float rect = ggo::rect_float::from_left_right_bottom_top(left, right, bottom, top);
      
    auto fct = [&](int x_sprite, int y_sprite)
    {
      T sprite_color(0);
      brush_sampler.sample_pixel(x_sprite, y_sprite, [&](float x_f, float y_f)
      {
        sprite_color += brush.get(x_f, y_f, rect, sprite.get_width(), sprite.get_height());
      });
      sprite_color /= static_cast<float>(brush_sampler.get_samples_count());
      return sprite_color;
    };
      
    flatten_sprite_func(image, sprite, fct, sprite_pos_x, sprite_pos_y, blender);
  }

  //////////////////////////////////////////////////////////////
  template <typename T>
  void flatten_sprite_t(ggo::image_data_abc<T> & image,
                        const ggo::gray_image_data_abc & sprite_opacity,
                        const ggo::image_data_abc<T> & sprite_image,
                        int sprite_pos_x, int sprite_pos_y,
                        const ggo::blender_abc<T> & blender)
  {
    auto fct = [&](int x_sprite, int y_sprite)
    {
      return sprite_image.unpack(x_sprite, y_sprite);
    };

    flatten_sprite_func(image, sprite_opacity, fct, sprite_pos_x, sprite_pos_y, blender);
  }
}

//////////////////////////////////////////////////////////////
// SPRITE
namespace ggo
{
  //////////////////////////////////////////////////////////////
  void flatten_sprite(ggo::gray_image_data_abc & image,
                      const ggo::gray_image_data_abc & sprite,
                      const ggo::gray_brush_abc & brush,
                      const ggo::pixel_aligned_sampler_abc & brush_sampler, 
                      int sprite_pos_x, int sprite_pos_y,
                      const ggo::gray_blender_abc & blender)
  {
    flatten_sprite_t<float>(image, sprite, brush, brush_sampler, sprite_pos_x, sprite_pos_y, blender);
  }
  
  //////////////////////////////////////////////////////////////
  void flatten_sprite(ggo::rgb_image_data_abc & image,
                      const ggo::gray_image_data_abc & sprite,
                      const ggo::rgb_brush_abc & brush,
                      const ggo::pixel_aligned_sampler_abc & brush_sampler, 
                      int sprite_pos_x, int sprite_pos_y,
                      const ggo::rgb_blender_abc & blender)
  {
    flatten_sprite_t<ggo::color>(image, sprite, brush, brush_sampler, sprite_pos_x, sprite_pos_y, blender);
  }
  

  //////////////////////////////////////////////////////////////
  void flatten_sprite(ggo::rgb_image_data_abc & image,
                      const ggo::gray_image_data_abc & sprite_opacity,
                      const ggo::rgb_image_data_abc & sprite_image,
                      int sprite_pos_x, int sprite_pos_y,
                      const ggo::rgb_blender_abc & blender)
  {
    flatten_sprite_t<ggo::color>(image, sprite_opacity, sprite_image, sprite_pos_x, sprite_pos_y, blender);
  }
}

//////////////////////////////////////////////////////////////
// LAYER
namespace ggo
{
  //////////////////////////////////////////////////////////////
  void flatten_layer(ggo::rgb_image_data_abc & image,
                     const ggo::gray_image_data_abc & opacity,
                     const ggo::rgb_brush_abc & brush,
                     const ggo::pixel_aligned_sampler_abc & brush_sampler, 
                     const ggo::rgb_blender_abc & blender)
  {
    if ((image.get_width() != opacity.get_width()) || (image.get_height() != opacity.get_height()))
    {
      throw dimension_mismatch_exception();
    }
    
    ggo::flatten_sprite(image, opacity, brush, brush_sampler, 0, 0, blender);
  }
  
  //////////////////////////////////////////////////////////////
  void flatten_layer(ggo::gray_image_data_abc & image,
                     const ggo::gray_image_data_abc & opacity,
                     const ggo::gray_brush_abc & brush,
                     const ggo::pixel_aligned_sampler_abc & brush_sampler, 
                     const ggo::gray_blender_abc & blender)
  {
    if ((image.get_width() != opacity.get_width()) || (image.get_height() != opacity.get_height()))
    {
      throw dimension_mismatch_exception();
    }
    
    ggo::flatten_sprite(image, opacity, brush, brush_sampler, 0, 0, blender);
  }

  //////////////////////////////////////////////////////////////
  void flatten_layer(ggo::rgb_image_data_abc & image,
                     const ggo::gray_image_data_abc & opacity,
                     const ggo::rgb_image_data_abc & layer,
                     const ggo::rgb_blender_abc & blender)
  {
    if ((image.get_width() != opacity.get_width()) || (image.get_height() != opacity.get_height()))
    {
      throw dimension_mismatch_exception();
    }
    
    if ((image.get_width() != layer.get_width()) || (image.get_height() != layer.get_height()))
    {
      throw dimension_mismatch_exception();
    }

    ggo::flatten_sprite(image, opacity, layer, 0, 0, blender);
  }
}

