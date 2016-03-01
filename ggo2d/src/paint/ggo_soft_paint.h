#ifndef __GGO_SOFT_PAINT__
#define __GGO_SOFT_PAINT__

#include <memory>
#include <ggo_pixel_sampler_abc.h>
#include <ggo_gray_image_data.h>
#include <ggo_rgb_image_data.h>
#include <ggo_shapes_collection.h>

namespace ggo
{
  // Pointer functions.
  void paint(uint8_t * buffer, int width, int height,
             ggo::paintable_shape_ptr shape,
             const ggo::color & color,
             float opacity = 1,
             std::shared_ptr<const ggo::rgb_blender_abc> blender = std::make_shared<ggo::rgb_alpha_blender>(),
             const ggo::pixel_sampler_abc & sampler = ggo::pixel_sampler_4X4());

  void paint(uint8_t * buffer, int width, int height,
             ggo::paintable_shape_ptr shape,
             std::shared_ptr<const ggo::rgb_brush_abc> color_brush,
             std::shared_ptr<const ggo::opacity_brush_abc> opacity_brush = std::make_shared<opacity_solid_brush>(1.f),
             std::shared_ptr<const ggo::rgb_blender_abc> blender = std::make_shared<ggo::rgb_alpha_blender>(),
             const ggo::pixel_sampler_abc & sampler = ggo::pixel_sampler_4X4());

  void paint(uint8_t * buffer, int width, int height,
             const ggo::shapes_collection_rgb & shapes_collection,
             const ggo::pixel_sampler_abc & sampler = ggo::pixel_sampler_4X4());
             
  // ImageData functions.
  void paint(ggo::gray_image_data_abc & image_data,
             ggo::paintable_shape_ptr shape,
             float gray,
             float opacity = 1,
             std::shared_ptr<const ggo::gray_blender_abc> blender = std::make_shared<ggo::gray_alpha_blender>(),
             const ggo::pixel_sampler_abc & sampler = ggo::pixel_sampler_4X4());
  void paint(ggo::rgb_image_data_abc & image_data,
             ggo::paintable_shape_ptr shape,
             const ggo::color & color,
             float opacity = 1,
             std::shared_ptr<const ggo::rgb_blender_abc> blender = std::make_shared<ggo::rgb_alpha_blender>(),
             const ggo::pixel_sampler_abc & sampler = ggo::pixel_sampler_4X4());

  void paint(ggo::gray_image_data_abc & image_data,
             ggo::paintable_shape_ptr shape,
             std::shared_ptr<const ggo::gray_brush_abc> color_brush,
             std::shared_ptr<const ggo::opacity_brush_abc> opacity_brush = std::make_shared<opacity_solid_brush>(1.f),
             std::shared_ptr<const ggo::gray_blender_abc> blender = std::make_shared<ggo::gray_alpha_blender>(),
             const ggo::pixel_sampler_abc & sampler = ggo::pixel_sampler_4X4());               
  void paint(ggo::rgb_image_data_abc & image_data,
             ggo::paintable_shape_ptr shape,
             std::shared_ptr<const ggo::rgb_brush_abc> color_brush,
             std::shared_ptr<const ggo::opacity_brush_abc> opacity_brush = std::make_shared<opacity_solid_brush>(1.f),
             std::shared_ptr<const ggo::rgb_blender_abc> blender = std::make_shared<ggo::rgb_alpha_blender>(),
             const ggo::pixel_sampler_abc & sampler = ggo::pixel_sampler_4X4());

  void paint(ggo::gray_image_data_abc & image_data,
             const ggo::shapes_collection_gray & shapes_collection,
             const ggo::pixel_sampler_abc & sampler = ggo::pixel_sampler_4X4());               
  void paint(ggo::rgb_image_data_abc & image_data,
             const ggo::shapes_collection_rgb & shapes_collection,
             const ggo::pixel_sampler_abc & sampler = ggo::pixel_sampler_4X4());
}

#endif

