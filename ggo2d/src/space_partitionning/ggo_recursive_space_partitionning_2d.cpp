#include "ggo_recursive_space_partitionning_2d.h"

namespace ggo
{
  /////////////////////////////////////////////////////////////////////
  void recursive_space_partitionning_2d::process_shapes(const std::vector<ggo::paintable_shape_ptr> & shapes,
                                                        const ggo::pixel_rect & pixel_rect,
                                                        std::function<void(const ggo::pixel_rect &)> block_fct,
                                                        std::function<void(int, int, bool)> pixel_fct) const
  {
    GGO_ASSERT(pixel_rect.left() <= pixel_rect.right() && pixel_rect.left() >= 0);
    GGO_ASSERT(pixel_rect.bottom() <= pixel_rect.top() && pixel_rect.bottom() >= 0);

    bool block_outside_all_shapes = true;
    
    // Check if either the current block is outside all shapes or inside a shape.
    for (const auto & shape : shapes)
    {
      ggo::rect_float rect = ggo::rect_float::from_left_right_bottom_top(pixel_rect.left() - 0.5f, pixel_rect.right() + 0.5f, pixel_rect.bottom() - 0.5f, pixel_rect.top() + 0.5f);
      ggo::rect_intersection intersection = shape->get_rect_intersection(rect);
      
      if (intersection == ggo::rect_intersection::RECT_IN_SHAPE)
      {
        block_fct(pixel_rect);
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
      pixel_fct(pixel_rect.left(), pixel_rect.bottom(), true);
    }
    else
    {
      // Recursion.
      ggo::pixel_rect pixel_rect1 = pixel_rect;
      ggo::pixel_rect pixel_rect2 = pixel_rect;
    
      if (pixel_rect.width() > pixel_rect.height())
      {
        int middle          = pixel_rect.left() + (pixel_rect.right() - pixel_rect.left()) / 2;
        pixel_rect1.right() = middle;
        pixel_rect2.left()  = middle + 1;
      }
      else
      {
        int middle            = pixel_rect.bottom() + (pixel_rect.top() - pixel_rect.bottom()) / 2;
        pixel_rect1.top()     = middle;
        pixel_rect2.bottom()  = middle + 1;
      }

      GGO_ASSERT(pixel_rect1.width() >= 1 && pixel_rect1.height() >= 1);
      GGO_ASSERT(pixel_rect2.width() >= 1 && pixel_rect2.height() >= 1);
      
      process_shapes(shapes, pixel_rect1, block_fct, pixel_fct);
      process_shapes(shapes, pixel_rect2, block_fct, pixel_fct);
    }
  }
}
