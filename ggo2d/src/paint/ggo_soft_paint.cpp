#include "ggo_soft_paint.h"
#include <ggo_pixel_rect.h>

namespace
{
  /////////////////////////////////////////////////////////////////////
  template <typename color_type>
  color_type get_color_at_pixel(const std::vector<ggo::layer<color_type>> & layers,
                                const ggo::image_abc<color_type> & image,
                                int x,
                                int y,
                                int width,
                                int height,
                                const ggo::pixel_sampler_abc & sampler,
                                bool bypass_shape_sampling = false)
  {
    color_type pixel_color(0);
    color_type bkgd_color(image.read(x, y));

    sampler.sample_pixel(x, y, [&](float x_f, float y_f)
    {
      color_type sample_color(bkgd_color);

      for (const auto & layer : layers)
      {
        // The current sample is inside the layer's shape.
        if (bypass_shape_sampling == true || layer._shape->is_point_inside(x_f, y_f) == true)
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
  void process_block(ggo::image_abc<color_type> & image,
                     const std::vector<ggo::layer<color_type>> & layers,
                     const ggo::pixel_rect & pixel_rect,
                     const ggo::pixel_sampler_abc & sampler)
  {
    pixel_rect.for_each_pixel([&](int x, int y)
    {
      image.write(x, y, get_color_at_pixel(layers, image, x, y, image.get_width(), image.get_height(), sampler));
    });
  }

  /////////////////////////////////////////////////////////////////////
  template <typename color_type>
  void paint_recursive(ggo::image_abc<color_type> & image,
                       const std::vector<ggo::layer<color_type>> & layers,
                       const ggo::pixel_rect & pixel_rect,
                       const ggo::pixel_sampler_abc & sampler)
  {
    GGO_ASSERT(pixel_rect.left() <= pixel_rect.right() && pixel_rect.left() >= 0);
    GGO_ASSERT(pixel_rect.bottom() <= pixel_rect.top() && pixel_rect.bottom() >= 0);

    bool block_inside_all_shapes = true;

    std::vector<ggo::layer<color_type>> block_layers;

    // Check if either the current block is outside all shapes or inside a shape.
    ggo::rect_float block_rect = pixel_rect.get_rect_float();
    for (const auto & layer : layers)
    {
      ggo::rect_intersection intersection = layer._shape->get_rect_intersection(block_rect);

      switch (intersection)
      {
      case ggo::rect_intersection::DISJOINTS:
        break;
      case ggo::rect_intersection::RECT_IN_SHAPE:
        block_layers.push_back(layer);
        break;
      default:
        block_inside_all_shapes = false;
        block_layers.push_back(layer);
        break;
      }
    }

    // The current block is outside all shapes: just discard it.
    if (block_layers.empty() == true)
    {
      return;
    }

    // The current block is inside all shapes. Process block without checking if pixel samples are inside a shape.
    if (block_inside_all_shapes == true)
    {
      pixel_rect.for_each_pixel([&](int x, int y)
      {
        image.write(x, y, get_color_at_pixel(block_layers, image, x, y, image.get_width(), image.get_height(), sampler, true));
      });
      return;
    }

    // If the block matches a single pixel, paint the pixel.
    if (pixel_rect.is_one_pixel() == true)
    {
      int x = pixel_rect.left();
      int y = pixel_rect.bottom();

      image.write(x, y, get_color_at_pixel(block_layers, image, x, y, image.get_width(), image.get_height(), sampler));
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

      paint_recursive(image, block_layers, pixel_rect1, sampler);
      paint_recursive(image, block_layers, pixel_rect2, sampler);
    }
  }

  /////////////////////////////////////////////////////////////////////
  template <typename color_type>
  void paint_block8x8(ggo::image_abc<color_type> & image,
                      const std::vector<ggo::layer<color_type>> & layers,
                      const ggo::pixel_rect & pixel_rect,
                      const ggo::pixel_sampler_abc & sampler)
  {
    int x = pixel_rect.left();
    int y = pixel_rect.bottom();

    int x_end = std::min(x + 7, pixel_rect.right());
    int y_end = std::min(y + 7, pixel_rect.top());

    while (true)
    {
      // Check for shapes intersecting the current block.
      ggo::pixel_rect block_pixel_rect = ggo::pixel_rect::from_left_right_bottom_top(x, x_end, y, y_end);
      ggo::rect_float block_rect_float = block_pixel_rect.get_rect_float();

      std::vector<ggo::layer<color_type>> current_block_layers;
      for (const auto & layer : layers)
      {
        switch (layer._shape->get_rect_intersection(block_rect_float))
        {
        case ggo::rect_intersection::RECT_IN_SHAPE:
        case ggo::rect_intersection::SHAPE_IN_RECT:
        case ggo::rect_intersection::PARTIAL_OVERLAP:
          current_block_layers.push_back(layer);
          break;
        case ggo::rect_intersection::DISJOINTS:
          break;
        }
      }

      // Paint.
      process_block(image, current_block_layers, block_pixel_rect, sampler);

      // Move to the next block.
      if (x_end < pixel_rect.right())
      {
        x += 8;
        x_end = std::min(x + 7, pixel_rect.right());
      }
      else
      {
        GGO_ASSERT(x_end == pixel_rect.right());

        if (y_end == pixel_rect.top())
        {
          // Done.
          break;
        }
        else
        {
          // Go to next line.
          x = pixel_rect.left();
          x_end = std::min(x + 7, pixel_rect.right());
          
          y += 8;
          y_end = std::min(y + 7, pixel_rect.top());
        }
      }
    }
  }

  /////////////////////////////////////////////////////////////////////
  template <typename color_type>
  void paint(ggo::image_abc<color_type> & image, 
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
    if (pixel_rect.crop(image.get_width(), image.get_height()) == false)
    {
      return;
    }

    // Painting itself.
    if (sampler.is_pixel_aligned() == true)
    {
      switch (partitionning)
      {
      case ggo::space_partitionning::recursive  :
        paint_recursive(image, layers, pixel_rect, sampler);
        break;
      case ggo::space_partitionning::none:
        process_block(image, layers, pixel_rect, sampler);
        break;
      case ggo::space_partitionning::block8x8:
        paint_block8x8(image, layers, pixel_rect, sampler);
        break;
      }
    }
    else
    {
      process_block(image, layers, pixel_rect, sampler);
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
    ggo::rgb_image_buffer_uint8 image(width, height, buffer);

    std::vector<ggo::layer<ggo::color>> layers{ { shape,
                                                  std::make_shared<ggo::rgb_solid_brush>(color),
                                                  std::make_shared<ggo::opacity_solid_brush>(opacity),
                                                  blender } };

    ::paint<ggo::color>(image, layers, sampler, partitionning);
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
    ggo::rgb_image_buffer_uint8 image(width, height, buffer);

    std::vector<ggo::layer<ggo::color>> layers{ { shape, color_brush, opacity_brush, blender } };

    ::paint<ggo::color>(image, layers, sampler, partitionning);
  }

  /////////////////////////////////////////////////////////////////////
  void paint(uint8_t * buffer, int width, int height,
             const std::vector<ggo::rgb_layer> & layers,
             const ggo::pixel_sampler_abc & sampler,
             ggo::space_partitionning partitionning)
  {
    ggo::rgb_image_buffer_uint8 image(width, height, buffer);

    ::paint<ggo::color>(image, layers, sampler, partitionning);
  }

  /////////////////////////////////////////////////////////////////////
  void paint(ggo::gray_image_abc & image,
             std::shared_ptr<const ggo::paintable_shape2d_abc<float>> shape,
             float gray,
             float opacity,
             std::shared_ptr<const ggo::gray_blender_abc> blender,
             const ggo::pixel_sampler_abc & sampler,
             ggo::space_partitionning partitionning)
  {
    std::vector<ggo::layer<float>> layers;
    
    layers.emplace_back(shape, gray, opacity, blender);

    ::paint<float>(image, layers, sampler, partitionning);
  }

  /////////////////////////////////////////////////////////////////////
  void paint(ggo::rgb_image_abc & image,
             std::shared_ptr<const ggo::paintable_shape2d_abc<float>> shape,
             const ggo::color & color,
             float opacity,
             std::shared_ptr<const ggo::rgb_blender_abc> blender,
             const ggo::pixel_sampler_abc & sampler,
             ggo::space_partitionning partitionning)
  {
    std::vector<ggo::layer<ggo::color>> layers;
    
    layers.emplace_back(shape, color, opacity, blender);

    ::paint<ggo::color>(image, layers, sampler, partitionning);
  }

  /////////////////////////////////////////////////////////////////////
  void paint(ggo::gray_image_abc & image,
             std::shared_ptr<const ggo::paintable_shape2d_abc<float>> shape,
             std::shared_ptr<const ggo::gray_brush_abc> color_brush,
             std::shared_ptr<const ggo::opacity_brush_abc> opacity_brush,
             std::shared_ptr<const ggo::gray_blender_abc> blender,
             const ggo::pixel_sampler_abc & sampler,
             ggo::space_partitionning partitionning)
  {
    std::vector<ggo::layer<float>> layers;
    
    layers.emplace_back(shape, color_brush, opacity_brush, blender);

    ::paint<float>(image, layers, sampler, partitionning);
  }

  /////////////////////////////////////////////////////////////////////
  void paint(ggo::rgb_image_abc & image,
             std::shared_ptr<const ggo::paintable_shape2d_abc<float>> shape,
             std::shared_ptr<const ggo::rgb_brush_abc> color_brush,
             std::shared_ptr<const ggo::opacity_brush_abc> opacity_brush,
             std::shared_ptr<const ggo::rgb_blender_abc> blender,
             const ggo::pixel_sampler_abc & sampler,
             ggo::space_partitionning partitionning)
  {
    std::vector<ggo::layer<ggo::color>> layers{ { shape, color_brush, opacity_brush, blender } };

    ::paint<ggo::color>(image, layers, sampler, partitionning);
  }

  /////////////////////////////////////////////////////////////////////
  void paint(ggo::gray_image_abc & image,
             const std::vector<ggo::gray_layer> & layers,
             const ggo::pixel_sampler_abc & sampler,
             ggo::space_partitionning partitionning)
  {
    ::paint<float>(image, layers, sampler, partitionning);
  }               

  /////////////////////////////////////////////////////////////////////
  void paint(ggo::rgb_image_abc & image,
             const std::vector<ggo::rgb_layer> & layers,
             const ggo::pixel_sampler_abc & sampler,
             ggo::space_partitionning partitionning)
  {
    ::paint<ggo::color>(image, layers, sampler, partitionning);
  }
}
