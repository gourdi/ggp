#include "ggo_pixel_space_partitionning_2d.h"

namespace ggo
{
  /////////////////////////////////////////////////////////////////////
  void pixel_space_partitionning_2d::process_shapes(const std::vector<ggo::paintable_shape_ptr> & shapes,
                                                    const ggo::pixel_rect & pixel_rect,
                                                    std::function<void(const ggo::pixel_rect &)> block_fct,
                                                    std::function<void(int, int, bool)> pixel_fct) const
  {
    GGO_ASSERT(pixel_rect.left() <= pixel_rect.right() && pixel_rect.left() >= 0);
    GGO_ASSERT(pixel_rect.bottom() <= pixel_rect.top() && pixel_rect.bottom() >= 0);

    for (int y = pixel_rect.bottom(); y <= pixel_rect.top(); ++y)
    {
      for (int x = pixel_rect.left(); x <= pixel_rect.right(); ++x)
      {
        process_pixel(shapes, x, y, pixel_fct);
      }
    }
  }
  
  /////////////////////////////////////////////////////////////////////
  void pixel_space_partitionning_2d::process_pixel(const std::vector<ggo::paintable_shape_ptr> & shapes,
                                                   int x, int y,
                                                   std::function<void(int, int, bool)> pixel_fct) const
  {
    bool pixel_outside_all_shapes = true;

    // Check if either the current block is outside all shapes or inside a shape.
    for (const auto & shape : shapes)
    {
      ggo::rect_float rect = ggo::rect_float::from_left_right_bottom_top(x - 0.5f, x + 0.5f, y - 0.5f, y + 0.5f);
      ggo::rect_intersection intersection = shape->get_rect_intersection(rect);

      // The current pixel is inside a shape: no need to sample shapes.
      if (intersection == ggo::rect_intersection::RECT_IN_SHAPE)
      {
        pixel_fct(x, y, false);
        return;
      }
      
      if (intersection != ggo::rect_intersection::DISJOINTS)
      {
        pixel_outside_all_shapes = false;
        break;
      }
    }
    
    // The current pixel is not outside all shapes => full processing.
    if (pixel_outside_all_shapes == false)
    {
      pixel_fct(x, y, true);
    }
  }
}
