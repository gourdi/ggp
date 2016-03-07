#include "ggo_soft_paint.h"
#include <ggo_pixel_rect.h>

namespace
{
  /////////////////////////////////////////////////////////////////////
  template <typename color_type>
  color_type get_color_at_pixel(const std::vector<ggo::layer<color_type>> & layers,
                                const color_type & bkgd_color,
                                int x,
                                int y,
                                int width,
                                int height,
                                const ggo::pixel_sampler_abc & sampler,
                                bool sample_shapes)
  {
    color_type pixel_color(0);

    sampler.sample_pixel(x, y, [&](float x_f, float y_f)
    {
      color_type sample_color(bkgd_color);

      for (const auto & layer : layers)
      {
        // The current sample is inside the layer's shape.
        if (sample_shapes == false || layer._shape->is_point_inside(x_f, y_f) == true)
        {
          float opacity = layer._opacity_brush->get(x_f, y_f, *layer._shape, width, height);

          // Then get brush color and blend it.
          if (opacity > 0)
          {
            color_type shape_color = layer._color_brush->get(x_f, y_f, *layer._shape, width, height);
            sample_color = layer._blender->blend(sample_color, opacity, shape_color);
          }
        }
      }

      pixel_color += sample_color;
    });

    return pixel_color / static_cast<float>(sampler.get_samples_count());
  }

  /////////////////////////////////////////////////////////////////////
  template <typename color_type>
  void process_block(ggo::image_data_abc<color_type> & image_data,
                     const std::vector<ggo::layer<color_type>> & layers,
                     const ggo::pixel_rect & pixel_rect,
                     const ggo::pixel_sampler_abc & sampler,
                     bool sample_shapes)
  {
    pixel_rect.for_each_pixel([&](int x, int y)
    {
      color_type color = image_data.unpack(x, y);
      color = get_color_at_pixel(layers, color, x, y, image_data.get_width(), image_data.get_height(), sampler, sample_shapes);
      image_data.pack(x, y, color);
    });
  }

  /////////////////////////////////////////////////////////////////////
  template <typename color_type>
  void paint_recursive(ggo::image_data_abc<color_type> & image_data,
                       const std::vector<ggo::layer<color_type>> & layers,
                       const ggo::pixel_rect & pixel_rect,
                       const ggo::pixel_sampler_abc & sampler)
  {
    GGO_ASSERT(pixel_rect.left() <= pixel_rect.right() && pixel_rect.left() >= 0);
    GGO_ASSERT(pixel_rect.bottom() <= pixel_rect.top() && pixel_rect.bottom() >= 0);

    bool block_outside_all_shapes = true;

    // Check if either the current block is outside all shapes or inside a shape.
    for (const auto & layer : layers)
    {
      ggo::rect_float rect = ggo::rect_float::from_left_right_bottom_top(pixel_rect.left() - 0.5f, pixel_rect.right() + 0.5f, pixel_rect.bottom() - 0.5f, pixel_rect.top() + 0.5f);
      ggo::rect_intersection intersection = layer._shape->get_rect_intersection(rect);

      if (intersection == ggo::rect_intersection::RECT_IN_SHAPE)
      {
        process_block(image_data, layers, pixel_rect, sampler, false);
        return;
      }

      if (intersection != ggo::rect_intersection::DISJOINTS)
      {
        block_outside_all_shapes = false;
        break;
      }
    }

    // The current block is outside all shapes: just discard it.
    if (block_outside_all_shapes == true)
    {
      return;
    }

    // If the block matches a single pixel, paint the pixel.
    if (pixel_rect.is_one_pixel() == true)
    {
      int x = pixel_rect.left();
      int y = pixel_rect.bottom();

      color_type color = image_data.unpack(x, y);
      color = get_color_at_pixel(layers, color, x, y, image_data.get_width(), image_data.get_height(), sampler, true);
      image_data.pack(x, y, color);
    }
    else
    {
      // Recursion.
      ggo::pixel_rect pixel_rect1 = pixel_rect;
      ggo::pixel_rect pixel_rect2 = pixel_rect;

      if (pixel_rect.width() > pixel_rect.height())
      {
        int middle = pixel_rect.left() + (pixel_rect.right() - pixel_rect.left()) / 2;
        pixel_rect1.right() = middle;
        pixel_rect2.left() = middle + 1;
      }
      else
      {
        int middle = pixel_rect.bottom() + (pixel_rect.top() - pixel_rect.bottom()) / 2;
        pixel_rect1.top() = middle;
        pixel_rect2.bottom() = middle + 1;
      }

      GGO_ASSERT(pixel_rect1.width() >= 1 && pixel_rect1.height() >= 1);
      GGO_ASSERT(pixel_rect2.width() >= 1 && pixel_rect2.height() >= 1);

      paint_recursive(image_data, layers, pixel_rect1, sampler);
      paint_recursive(image_data, layers, pixel_rect2, sampler);
    }
  }

  /////////////////////////////////////////////////////////////////////
  template <typename color_type>
  void paint(ggo::image_data_abc<color_type> & image_data, 
             const std::vector<ggo::layer<color_type>> & layers,
             const ggo::pixel_sampler_abc & sampler,
             ggo::space_partitionning partitionning)
  {
    if (layers.empty() == true)
    {
      return;
    }

    // Bounding rect.
    auto it = layers.begin();
    auto rect_data = it->_shape->get_bounding_rect();
    it++;

    while (it != layers.end())
    {
      rect_data = ggo::rect_data_union(rect_data, it->_shape->get_bounding_rect());

      it++;
    }

    ggo::rect_float bounding_rect(rect_data);
    bounding_rect.inflate(sampler.get_horz_extent(), sampler.get_vert_extent());

    ggo::pixel_rect pixel_rect(bounding_rect);
    if (pixel_rect.crop(image_data.get_width(), image_data.get_height()) == false)
    {
      return;
    }

    // Painting itself.
    if (sampler.is_pixel_aligned() == true)
    {
      switch (partitionning)
      {
      case ggo::space_partitionning::RECURSIVE:
        paint_recursive(image_data, layers, pixel_rect, sampler);
        break;
      case ggo::space_partitionning::NONE:
        process_block(image_data, layers, pixel_rect, sampler, true);
        break;
      }
    }
    else
    {
      process_block(image_data, layers, pixel_rect, sampler, true);
    }
  }
}

namespace ggo
{
  /////////////////////////////////////////////////////////////////////
  void paint(uint8_t * buffer, int width, int height,
             std::shared_ptr<const ggo::paintable_shape2d_abc<float>> shape,
             const ggo::color & color,
             float opacity,
             std::shared_ptr<const ggo::rgb_blender_abc> blender,
             const ggo::pixel_sampler_abc & sampler,
             ggo::space_partitionning partitionning)
  {
    ggo::rgb_image_data_uint8 image_data(buffer, width, height);

    std::vector<ggo::layer<ggo::color>> layers{ { shape,
                                                  std::make_shared<ggo::rgb_solid_brush>(color),
                                                  std::make_shared<ggo::opacity_solid_brush>(opacity),
                                                  blender } };

    ::paint<ggo::color>(image_data, layers, sampler, partitionning);
  }

  /////////////////////////////////////////////////////////////////////
  void paint(uint8_t * buffer, int width, int height,
             std::shared_ptr<const ggo::paintable_shape2d_abc<float>> shape,
             std::shared_ptr<const ggo::rgb_brush_abc> color_brush,
             std::shared_ptr<const ggo::opacity_brush_abc> opacity_brush,
             std::shared_ptr<const ggo::rgb_blender_abc> blender,
             const ggo::pixel_sampler_abc & sampler,
             ggo::space_partitionning partitionning)
  {
    ggo::rgb_image_data_uint8 image_data(buffer, width, height);

    std::vector<ggo::layer<ggo::color>> layers{ { shape, color_brush, opacity_brush, blender } };

    ::paint<ggo::color>(image_data, layers, sampler, partitionning);
  }

  /////////////////////////////////////////////////////////////////////
  void paint(uint8_t * buffer, int width, int height,
             const std::vector<ggo::rgb_layer> & layers,
             const ggo::pixel_sampler_abc & sampler,
             ggo::space_partitionning partitionning)
  {
    ggo::rgb_image_data_uint8 image_data(buffer, width, height);

    ::paint<ggo::color>(image_data, layers, sampler, partitionning);
  }

  /////////////////////////////////////////////////////////////////////
  void paint(ggo::gray_image_data_abc & image_data,
             std::shared_ptr<const ggo::paintable_shape2d_abc<float>> shape,
             float gray,
             float opacity,
             std::shared_ptr<const ggo::gray_blender_abc> blender,
             const ggo::pixel_sampler_abc & sampler,
             ggo::space_partitionning partitionning)
  {
    std::vector<ggo::layer<float>> layers;
    
    layers.emplace_back(shape, gray, opacity, blender);

    ::paint<float>(image_data, layers, sampler, partitionning);
  }

  /////////////////////////////////////////////////////////////////////
  void paint(ggo::rgb_image_data_abc & image_data,
             std::shared_ptr<const ggo::paintable_shape2d_abc<float>> shape,
             const ggo::color & color,
             float opacity,
             std::shared_ptr<const ggo::rgb_blender_abc> blender,
             const ggo::pixel_sampler_abc & sampler,
             ggo::space_partitionning partitionning)
  {
    std::vector<ggo::layer<ggo::color>> layers;
    
    layers.emplace_back(shape, color, opacity, blender);

    ::paint<ggo::color>(image_data, layers, sampler, partitionning);
  }

  /////////////////////////////////////////////////////////////////////
  void paint(ggo::gray_image_data_abc & image_data,
             std::shared_ptr<const ggo::paintable_shape2d_abc<float>> shape,
             std::shared_ptr<const ggo::gray_brush_abc> color_brush,
             std::shared_ptr<const ggo::opacity_brush_abc> opacity_brush,
             std::shared_ptr<const ggo::gray_blender_abc> blender,
             const ggo::pixel_sampler_abc & sampler,
             ggo::space_partitionning partitionning)
  {
    std::vector<ggo::layer<float>> layers;
    
    layers.emplace_back(shape, color_brush, opacity_brush, blender);

    ::paint<float>(image_data, layers, sampler, partitionning);
  }

  /////////////////////////////////////////////////////////////////////
  void paint(ggo::rgb_image_data_abc & image_data,
             std::shared_ptr<const ggo::paintable_shape2d_abc<float>> shape,
             std::shared_ptr<const ggo::rgb_brush_abc> color_brush,
             std::shared_ptr<const ggo::opacity_brush_abc> opacity_brush,
             std::shared_ptr<const ggo::rgb_blender_abc> blender,
             const ggo::pixel_sampler_abc & sampler,
             ggo::space_partitionning partitionning)
  {
    std::vector<ggo::layer<ggo::color>> layers{ { shape, color_brush, opacity_brush, blender } };

    ::paint<ggo::color>(image_data, layers, sampler, partitionning);
  }

  /////////////////////////////////////////////////////////////////////
  void paint(ggo::gray_image_data_abc & image_data,
             const std::vector<ggo::gray_layer> & layers,
             const ggo::pixel_sampler_abc & sampler,
             ggo::space_partitionning partitionning)
  {
    ::paint<float>(image_data, layers, sampler, partitionning);
  }               

  /////////////////////////////////////////////////////////////////////
  void paint(ggo::rgb_image_data_abc & image_data,
             const std::vector<ggo::rgb_layer> & layers,
             const ggo::pixel_sampler_abc & sampler,
             ggo::space_partitionning partitionning)
  {
    ::paint<ggo::color>(image_data, layers, sampler, partitionning);
  }
}
