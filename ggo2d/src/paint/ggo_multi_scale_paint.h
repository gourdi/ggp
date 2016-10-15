#ifndef __GGO_MULTI_SCALE_PAINT__
#define __GGO_MULTI_SCALE_PAINT__

#include <vector>
#include <ggo_sampling_paint.h>
#include <ggo_pixel_rect.h>

/////////////////////////////////////////////////////////////////////
// Declaration.
namespace ggo
{
  // Paint a single shape.
  template <sampling smp, typename shape_t, typename brush_t, typename blend_t,
    typename get_pixel_t, typename set_pixet_t, typename paint_block_t>
  void paint_multi_scale(int width, int height,
    const shape_t & shape,
    int scale_factor, int first_scale,
    brush_t brush, blend_t blend,
    get_pixel_t get_pixel,
    set_pixet_t set_pixet,
    paint_block_t paint_block);

  // Paint multiple shapes.
  template <typename shape_t, typename shape_iterator_t, typename paint_block_func, typename paint_pixel_func, typename sample_pixel_func>
  void paint_multi_scale(int width, int height,
    shape_iterator_t shape_begin_it, shape_iterator_t shape_end_it,
    int scale_factor, int first_scale,
    paint_block_func paint_block,
    paint_pixel_func paint_pixel,
    sample_pixel_func sample_pixel);
}

/////////////////////////////////////////////////////////////////////
// Single shape implementation.
namespace ggo
{
  /////////////////////////////////////////////////////////////////////
  template <sampling smp, typename shape_t, typename brush_t, typename blend_t,
    typename get_pixel_t, typename set_pixet_t, typename paint_block_t, typename real_t = typename shape_t::type>
  void paint_block_single_t(const ggo::pixel_rect & block_rect,
    const int scale_factor, const int current_scale,
    const shape_t & shape,
    brush_t brush, blend_t blend,
    get_pixel_t get_pixel,
    set_pixet_t set_pixel,
    paint_block_t paint_block)
  {
    GGO_ASSERT(current_scale >= 0);

    // Check for shape intersecting the current block.
    const ggo::rect<typename shape_t::type> block_rect_real = block_rect.get_rect<typename shape_t::type>();

    const ggo::rect_intersection intersection = shape.get_rect_intersection(block_rect_real);

    switch (intersection)
    {
    case ggo::rect_intersection::disjoints:
      return;

    case ggo::rect_intersection::rect_in_shape:
      paint_block(block_rect);
      break;

    default:
      // The current block is only a pixel => stop the recursion and sample the pixel.
      if (current_scale == 0 || block_rect.is_one_pixel() == true)
      {
        const auto bkgd_color = get_pixel(block_rect.left(), block_rect.bottom());
        const auto brush_color = brush(block_rect.left(), block_rect.bottom());

        ggo::accumulator<std::remove_const<decltype(bkgd_color)>::type> acc;
        ggo::sampler<smp>::sample_pixel<shape_t::type>(block_rect.left(), block_rect.bottom(), [&](real_t x_f, real_t y_f)
        {
          acc.add(shape.is_point_inside(x_f, y_f) ? blend(bkgd_color, brush_color) : bkgd_color);
        });

        const auto pixel_color = acc.div<ggo::sampler<smp>::samples_count>();
        set_pixel(block_rect.left(), block_rect.bottom(), pixel_color);
      }
      else
      {
        // Process the sub-blocks.
        auto paint_subblock = [&](const ggo::pixel_rect & block_rect)
        {
          paint_block_single_t<smp>(block_rect,
            scale_factor, current_scale - 1,
            shape,
            brush, blend, 
            get_pixel, set_pixel, paint_block);
        };

        const int subblock_size = ggo::pow(scale_factor, current_scale - 1);
        process_blocks(block_rect, subblock_size, subblock_size, paint_subblock);
      }
      break;
    }
  }

  /////////////////////////////////////////////////////////////////////
  template <sampling smp, typename shape_t, typename brush_t, typename blend_t,
    typename get_pixel_t, typename set_pixet_t, typename paint_block_t>
  void paint_multi_scale(int width, int height,
    const shape_t & shape,
    int scale_factor, int first_scale,
    brush_t brush, blend_t blend,
    get_pixel_t get_pixel,
    set_pixet_t set_pixel,
    paint_block_t paint_block)
  {
    // Bounding rect.
    const auto shape_bounding_rect_data = shape.get_bounding_rect();

    // Clip.
    ggo::rect<typename shape_t::type> shape_bounding_rect(shape_bounding_rect_data);
    pixel_rect shape_pixel_rect(shape_bounding_rect);
    if (shape_pixel_rect.crop(width, height) == false)
    {
      return;
    }

    // Process blocks.
    auto process_rect = [&](const ggo::pixel_rect & block_rect)
    {
      paint_block_single_t<smp>(
        block_rect,
        scale_factor, first_scale,
        shape,
        brush, blend,
        get_pixel, set_pixel, paint_block);
    };

    int block_size = ggo::pow(scale_factor, first_scale);
    process_blocks(shape_pixel_rect, block_size, block_size, process_rect);
  }
}

#if 0
/////////////////////////////////////////////////////////////////////
// Multiple shape implementation.
namespace ggo
{
  /////////////////////////////////////////////////////////////////////
  template <typename shape_t, typename shape_iterator_t, typename paint_block_func, typename paint_pixel_func, typename sample_pixel_func>
  void paint_block_multi_t(const ggo::pixel_rect & block_rect,
    int scale_factor, int current_scale,
    shape_iterator_t shape_begin_it, shape_iterator_t shape_end_it,
    paint_block_func paint_block,
    paint_pixel_func paint_pixel,
    sample_pixel_func sample_pixel)
  {
    GGO_ASSERT(current_scale >= 0);

    // Check for shapes intersecting the current block.
    ggo::rect<float> block_rect_float = block_pixel_rect.get_rect<float>();

    bool block_inside_all_shapes = true;

    std::vector<shape_t> current_block_shapes;
    for (auto it = shape_begin_it; it != shape_end_it; ++it)
    {
      ggo::rect_intersection intersection = it->get_rect_intersection(block_rect_float);

      switch (intersection)
      {
      case ggo::rect_intersection::disjoints:
        break;
      case ggo::rect_intersection::rect_in_shape:
        current_block_shapes.push_back(*it);
        break;
      default:
        block_inside_all_shapes = false;
        current_block_shapes.push_back(*it);
        break;
      }
    }

    // Check if the current block intersects some shapes.
    if (current_block_shapes.empty() == true)
    {
      return;
    }

    // The current block is only a pixel.
    if (current_scale == 0 || block_rect.is_one_pixel() == true)
    {
      if (block_inside_all_shapes == true)
      {
        paint_pixel(block_rect.left, block_rect.top,
          current_block_shapes.begin(), current_block_shapes.end());
      }
      else
      {
        sample_pixel(block_rect.left, block_rect.top,
          current_block_shapes.begin(), current_block_shapes.end());
      }
    }
    else
    {
      // The block is inside the block, process it
      if (block_inside_all_shapes == true)
      {
        paint_block(block_rect, current_block_layers.begin(), current_block_layers.end());
      }
      else
      {
        // Process the sub-blocks.
        auto paint_subblock = [&](const ggo::pixel_rect & block_rect)
        {
          paint_block_t(block_rect,
            scale_factor, current_scale - 1,
            current_block_layers.begin(), current_block_layers.end(),
            paint_block, paint_pixel);
        };
        
        int subblock_size = ggo::pow(scale_factor, current_scale - 1);
        process_blocks(block_rect, subblock_size, subblock_size, paint_subblock);
      }
    }
  }

  /////////////////////////////////////////////////////////////////////
  template <typename shape_t, typename shape_iterator_t, typename paint_block_func, typename paint_pixel_func, typename sample_pixel_func>
  void paint_multi_scale(int width, int height,
    shape_iterator_t shape_begin_it, shape_iterator_t shape_end_it,
    int scale_factor, int first_scale,
    paint_block_func paint_block,
    paint_pixel_func paint_pixel,
    sample_pixel_func sample_pixel)
  {
    if (shape_begin_it == shape_end_it)
    {
      return;
    }

    // Bounding rect.
    auto it = shape_begin_it;
    auto rect_data = it->get_bounding_rect();
    it++;

    for (; it != shape_end_it; ++it)
    {
      rect_data = ggo::rect_data_union(rect_data, it->get_bounding_rect());
    }

    // Clip.
    ggo::pixel_rect pixel_rect(ggo::rect<data_t>(rect_data));
    if (pixel_rect.crop(width, height) == false)
    {
      return;
    }

    // Process blocks.
    auto process_block = [&](const ggo::pixel_rect & block_rect)
    {
      paint_block_multi_t<shape_t, shape_iterator_t>(block_rect,
        scale_factor, first_scale,
        shape_begin_it, shape_end_it,
        paint_block, paint_pixel);
    };

    int block_size = ggo::pow(scale_factor, first_scale);
    process_blocks(pixel_rect, block_size, block_size, process_block);
  }
}

#endif

#endif

