#include "ggo_soft_paint.h"

namespace ggo
{
  /////////////////////////////////////////////////////////////////////
  void paint(uint8_t * buffer, int width, int height,
             ggo::paintable_shape_ptr shape,
             const ggo::color & color,
             float opacity,
             std::shared_ptr<const ggo::rgb_blender_abc> blender,
             const ggo::pixel_sampler_abc & sampler)
  {
    ggo::rgb_image_data_uint8 image_data(buffer, width, height);

    ggo::shapes_collection_rgb(shape, color, opacity, blender).paint(image_data, sampler);
  }

  /////////////////////////////////////////////////////////////////////
  void paint(uint8_t * buffer, int width, int height,
             ggo::paintable_shape_ptr shape,
             std::shared_ptr<const ggo::rgb_brush_abc> color_brush,
             std::shared_ptr<const ggo::opacity_brush_abc> opacity_brush,
             std::shared_ptr<const ggo::rgb_blender_abc> blender,
             const ggo::pixel_sampler_abc & sampler)
  {
    ggo::rgb_image_data_uint8 image_data(buffer, width, height);

    ggo::shapes_collection_rgb(shape, color_brush, opacity_brush, blender).paint(image_data, sampler);
  }

  /////////////////////////////////////////////////////////////////////
  void paint(uint8_t * buffer,
             int width,
             int height,
             const ggo::shapes_collection_rgb & shapes_collection,
             const ggo::pixel_sampler_abc & sampler)
  {
    ggo::rgb_image_data_uint8 image_data(buffer, width, height);
    
    shapes_collection.paint(image_data, sampler);
  }

  /////////////////////////////////////////////////////////////////////
  void paint(ggo::gray_image_data_abc & image_data,
             ggo::paintable_shape_ptr shape,
             float gray,
             float opacity,
             std::shared_ptr<const ggo::gray_blender_abc> blender,
             const ggo::pixel_sampler_abc & sampler)
  {
    ggo::shapes_collection_gray(shape, gray, opacity, blender).paint(image_data, sampler);
  }

  /////////////////////////////////////////////////////////////////////
  void paint(ggo::rgb_image_data_abc & image_data,
             ggo::paintable_shape_ptr shape,
             const ggo::color & color,
             float opacity,
             std::shared_ptr<const ggo::rgb_blender_abc> blender,
             const ggo::pixel_sampler_abc & sampler)
  {
    ggo::shapes_collection_rgb(shape, color, opacity, blender).paint(image_data, sampler);
  }

  /////////////////////////////////////////////////////////////////////
  void paint(ggo::gray_image_data_abc & image_data,
             ggo::paintable_shape_ptr shape,
             std::shared_ptr<const ggo::gray_brush_abc> color_brush,
             std::shared_ptr<const ggo::opacity_brush_abc> opacity_brush,
             std::shared_ptr<const ggo::gray_blender_abc> blender,
             const ggo::pixel_sampler_abc & sampler)
  {
    ggo::shapes_collection_gray(shape, color_brush, opacity_brush, blender).paint(image_data, sampler);
  }

  /////////////////////////////////////////////////////////////////////
  void paint(ggo::rgb_image_data_abc & image_data,
             ggo::paintable_shape_ptr shape,
             std::shared_ptr<const ggo::rgb_brush_abc> color_brush,
             std::shared_ptr<const ggo::opacity_brush_abc> opacity_brush,
             std::shared_ptr<const ggo::rgb_blender_abc> blender,
             const ggo::pixel_sampler_abc & sampler)
  {
    ggo::shapes_collection_rgb(shape, color_brush, opacity_brush, blender).paint(image_data, sampler);
  }

  /////////////////////////////////////////////////////////////////////
  void paint(ggo::gray_image_data_abc & image_data,
             const ggo::shapes_collection_gray & shapes_collection,
             const ggo::pixel_sampler_abc & sampler)
  {
    shapes_collection.paint(image_data, sampler);
  }               

  /////////////////////////////////////////////////////////////////////
  void paint(ggo::rgb_image_data_abc & image_data,
             const ggo::shapes_collection_rgb & shapes_collection,
             const ggo::pixel_sampler_abc & sampler)
  {
    shapes_collection.paint(image_data, sampler);
  }
}
