#ifndef __GGO_LAYER__
#define __GGO_LAYER__

#include <ggo_gray_image_data.h>
#include <ggo_rgb_image_data.h>
#include <ggo_brush_abc.h>
#include <ggo_pixel_sampler_abc.h>
#include <ggo_blender_abc.h>

//////////////////////////////////////////////////////////////
// SPRITE

namespace ggo
{
  // Brush.
  void flatten_sprite(ggo::gray_image_data_abc & image,
                      const ggo::gray_image_data_abc & sprite,
                      const ggo::gray_brush_abc & brush,
                      const ggo::pixel_aligned_sampler_abc & brush_sampler, 
                      int sprite_pos_x, int sprite_pos_y,
                      const ggo::gray_blender_abc & blender);
  void flatten_sprite(ggo::rgb_image_data_abc & image,
                      const ggo::gray_image_data_abc & sprite,
                      const ggo::rgb_brush_abc & brush,
                      const ggo::pixel_aligned_sampler_abc & brush_sampler, 
                      int sprite_pos_x, int sprite_pos_y,
                      const ggo::rgb_blender_abc & blender);

  // Buffer.
  void flatten_sprite(ggo::rgb_image_data_abc & image,
                      const ggo::gray_image_data_abc & sprite_opacity,
                      const ggo::rgb_image_data_abc & sprite_image,
                      int sprite_pos_x, int sprite_pos_y,
                      const ggo::rgb_blender_abc & blender);
}
                        
//////////////////////////////////////////////////////////////
// LAYER
namespace ggo
{
  // Brush.
  void flatten_layer(ggo::gray_image_data_abc & image,
                     const ggo::gray_image_data_abc & opacity,
                     const ggo::gray_brush_abc & brush,
                     const ggo::pixel_aligned_sampler_abc & brush_sampler, 
                     const ggo::gray_blender_abc & blender);
  void flatten_layer(ggo::rgb_image_data_abc & image,
                     const ggo::gray_image_data_abc & opacity,
                     const ggo::rgb_brush_abc & brush,
                     const ggo::pixel_aligned_sampler_abc & brush_sampler, 
                     const ggo::rgb_blender_abc & blender);
           
  // Buffer.         
  void flatten_layer(ggo::rgb_image_data_abc & image,
                     const ggo::gray_image_data_abc & opacity,
                     const ggo::rgb_image_data_abc & layer,
                     const ggo::rgb_blender_abc & blender);
}

#endif

