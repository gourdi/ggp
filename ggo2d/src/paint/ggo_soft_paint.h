#ifndef __GGO_SOFT_PAINT__
#define __GGO_SOFT_PAINT__

#include <memory>
#include <vector>
#include <ggo_array.h>
#include <ggo_color.h>
#include <ggo_pixel_sampler_abc.h>
#include <ggo_gray_image_buffer.h>
#include <ggo_rgb_image_buffer.h>
#include <ggo_blender_abc.h>
#include <ggo_brush_abc.h>

/////////////////////////////////////////////////////////////////////
// Layer.
namespace ggo
{
  template <typename color_type>
  struct layer
  {
    layer(std::shared_ptr<const ggo::paintable_shape2d_abc<float>> shape,
          const color_type & color,
          float opacity = 1.f,
          std::shared_ptr<const ggo::blender_abc<color_type>> blender = std::make_shared<ggo::alpha_blender<color_type>>())
      :
      _shape(shape),
      _color_brush(std::make_shared<ggo::solid_brush<color_type>>(color)),
      _opacity_brush(std::make_shared<ggo::opacity_solid_brush>(opacity)),
      _blender(blender)
    {}

    layer(std::shared_ptr<const ggo::paintable_shape2d_abc<float>> shape,
          std::shared_ptr<const ggo::brush_abc<color_type>> color_brush,
          std::shared_ptr<const ggo::opacity_brush_abc> opacity_brush,
          std::shared_ptr<const ggo::blender_abc<color_type>> blender = std::make_shared<ggo::alpha_blender<color_type>>())
      :
      _shape(shape),
      _color_brush(color_brush),
      _opacity_brush(opacity_brush),
      _blender(blender)
    {}

    std::shared_ptr<const ggo::paintable_shape2d_abc<float>>  _shape;
    std::shared_ptr<const ggo::brush_abc<color_type>>         _color_brush;
    std::shared_ptr<const ggo::opacity_brush_abc>             _opacity_brush;
    std::shared_ptr<const ggo::blender_abc<color_type>>       _blender;
  };
}

/////////////////////////////////////////////////////////////////////
// Aliases.
namespace ggo
{
  using rgb_layer  = layer<ggo::color>;
  using gray_layer = layer<float>;
}

/////////////////////////////////////////////////////////////////////
// Paint functions.
namespace ggo
{
  enum class space_partitionning
  {
    recursive,
    block8x8,
    none,
  };

  // Raw pointer functions.
  void paint(uint8_t * buffer, int width, int height,
             std::shared_ptr<const ggo::paintable_shape2d_abc<float>> shape,
             const ggo::color & color,
             float opacity = 1,
             std::shared_ptr<const ggo::rgb_blender_abc> blender = std::make_shared<ggo::rgb_alpha_blender>(),
             const ggo::pixel_sampler_abc & sampler = ggo::pixel_sampler_4X4(),
             ggo::space_partitionning partitionning = space_partitionning::recursive);

  void paint(uint8_t * buffer, int width, int height,
             std::shared_ptr<const ggo::paintable_shape2d_abc<float>> shape,
             std::shared_ptr<const ggo::rgb_brush_abc> color_brush,
             std::shared_ptr<const ggo::opacity_brush_abc> opacity_brush = std::make_shared<opacity_solid_brush>(1.f),
             std::shared_ptr<const ggo::rgb_blender_abc> blender = std::make_shared<ggo::rgb_alpha_blender>(),
             const ggo::pixel_sampler_abc & sampler = ggo::pixel_sampler_4X4(),
             ggo::space_partitionning partitionning = space_partitionning::recursive);

  void paint(uint8_t * buffer, int width, int height,
             const std::vector<rgb_layer> & layers,
             const ggo::pixel_sampler_abc & sampler = ggo::pixel_sampler_4X4(),
             ggo::space_partitionning partitionning = space_partitionning::recursive);

  // Image functions.
  void paint(ggo::gray_image_abc & image,
             std::shared_ptr<const ggo::paintable_shape2d_abc<float>> shape,
             float gray,
             float opacity = 1,
             std::shared_ptr<const ggo::gray_blender_abc> blender = std::make_shared<ggo::gray_alpha_blender>(),
             const ggo::pixel_sampler_abc & sampler = ggo::pixel_sampler_4X4(),
             ggo::space_partitionning partitionning = space_partitionning::recursive);
  void paint(ggo::rgb_image_abc & image,
             std::shared_ptr<const ggo::paintable_shape2d_abc<float>> shape,
             const ggo::color & color,
             float opacity = 1,
             std::shared_ptr<const ggo::rgb_blender_abc> blender = std::make_shared<ggo::rgb_alpha_blender>(),
             const ggo::pixel_sampler_abc & sampler = ggo::pixel_sampler_4X4(),
             ggo::space_partitionning partitionning = space_partitionning::recursive);

  void paint(ggo::gray_image_abc & image,
             std::shared_ptr<const ggo::paintable_shape2d_abc<float>> shape,
             std::shared_ptr<const ggo::gray_brush_abc> color_brush,
             std::shared_ptr<const ggo::opacity_brush_abc> opacity_brush = std::make_shared<opacity_solid_brush>(1.f),
             std::shared_ptr<const ggo::gray_blender_abc> blender = std::make_shared<ggo::gray_alpha_blender>(),
             const ggo::pixel_sampler_abc & sampler = ggo::pixel_sampler_4X4(),
             ggo::space_partitionning partitionning = space_partitionning::recursive);
  void paint(ggo::rgb_image_abc & image,
             std::shared_ptr<const ggo::paintable_shape2d_abc<float>> shape,
             std::shared_ptr<const ggo::rgb_brush_abc> color_brush,
             std::shared_ptr<const ggo::opacity_brush_abc> opacity_brush = std::make_shared<opacity_solid_brush>(1.f),
             std::shared_ptr<const ggo::rgb_blender_abc> blender = std::make_shared<ggo::rgb_alpha_blender>(),
             const ggo::pixel_sampler_abc & sampler = ggo::pixel_sampler_4X4(),
             ggo::space_partitionning partitionning = space_partitionning::recursive);

  void paint(ggo::gray_image_abc & image,
             const std::vector<gray_layer> & layers,
             const ggo::pixel_sampler_abc & sampler = ggo::pixel_sampler_4X4(),
             ggo::space_partitionning partitionning = space_partitionning::recursive);
  void paint(ggo::rgb_image_abc & image,
             const std::vector<rgb_layer> & layers,
             const ggo::pixel_sampler_abc & sampler = ggo::pixel_sampler_4X4(),
             ggo::space_partitionning partitionning = space_partitionning::recursive);

  // Array 2D functions.
  void paint(ggo::array<float, 2> & array2d, 
             std::shared_ptr<const ggo::paintable_shape2d_abc<float>> shape,
             float gray,
             float opacity = 1,
             std::shared_ptr<const ggo::gray_blender_abc> blender = std::make_shared<ggo::gray_alpha_blender>(),
             const ggo::pixel_sampler_abc & sampler = ggo::pixel_sampler_4X4(),
             ggo::space_partitionning partitionning = space_partitionning::recursive);
}

#endif

