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
    typename read_pixel_t, typename write_pixel_t, typename paint_block_t>
  void paint_multi_scale(int width, int height,
    const shape_t & shape,
    int scale_factor, int first_scale,
    brush_t brush, blend_t blend,
    read_pixel_t read_pixel,
    write_pixel_t write_pixel,
    paint_block_t paint_block);

  // Paint multiple shapes.
  template <sampling smp, typename iterator_t, typename read_pixel_t, typename write_pixel_t, typename paint_block_t>
    void paint_multi_scale(int width, int height,
      iterator_t begin_it, iterator_t end_it,
      int scale_factor, int first_scale,
      read_pixel_t read_pixel,
      write_pixel_t write_pixel,
      paint_block_t paint_block);
}

/////////////////////////////////////////////////////////////////////
// Single shape implementation.
namespace ggo
{
  /////////////////////////////////////////////////////////////////////
  template <sampling smp, typename shape_t, typename brush_t, typename blend_t,
    typename read_pixel_t, typename write_pixel_t, typename paint_block_t>
  void paint_block_single_t(const ggo::pixel_rect & block_rect,
    const int scale_factor, const int current_scale,
    const shape_t & shape,
    brush_t brush, blend_t blend,
    read_pixel_t read_pixel,
    write_pixel_t write_pixel,
    paint_block_t paint_block)
  {
    GGO_ASSERT(current_scale >= 0);

    using real_t = shape_t::type;

    // Check for shape intersecting the current block.
    const ggo::rect<real_t> block_rect_real = block_rect.get_rect<real_t>();

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
      if (block_rect.is_one_pixel() == true)
      {
        const auto bkgd_color = read_pixel(block_rect.left(), block_rect.bottom());
        const auto brush_color = brush(block_rect.left(), block_rect.bottom());

        ggo::accumulator<std::remove_const<decltype(bkgd_color)>::type> acc;
        ggo::sampler<smp>::sample_pixel<shape_t::type>(block_rect.left(), block_rect.bottom(), [&](real_t x_f, real_t y_f)
        {
          acc.add(shape.is_point_inside(x_f, y_f) ? blend(bkgd_color, brush_color) : bkgd_color);
        });

        const auto pixel_color = acc.div<ggo::sampler<smp>::samples_count>();
        write_pixel(block_rect.left(), block_rect.bottom(), pixel_color);
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
            read_pixel, write_pixel, paint_block);
        };

        const int subblock_size = ggo::pow(scale_factor, current_scale - 1);
        process_blocks(block_rect, subblock_size, subblock_size, paint_subblock);
      }
      break;
    }
  }

  /////////////////////////////////////////////////////////////////////
  template <sampling smp, typename shape_t, typename brush_t, typename blend_t,
    typename read_pixel_t, typename write_pixel_t, typename paint_block_t>
  void paint_multi_scale(int width, int height,
    const shape_t & shape,
    int scale_factor, int first_scale,
    brush_t brush, blend_t blend,
    read_pixel_t read_pixel,
    write_pixel_t write_pixel,
    paint_block_t paint_block)
  {
    using real_t = shape_t::type;

    // Bounding rect.
    const auto shape_bounding_rect_data = shape.get_bounding_rect();

    // Clip.
    ggo::rect<real_t> shape_bounding_rect(shape_bounding_rect_data);
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
        read_pixel, write_pixel, paint_block);
    };

    int block_size = ggo::pow(scale_factor, first_scale);
    process_blocks(shape_pixel_rect, block_size, block_size, process_rect);
  }
}

/////////////////////////////////////////////////////////////////////
// Multiple shape implementation.
namespace ggo
{
  /////////////////////////////////////////////////////////////////////
  template <sampling smp, typename iterator_t,
    typename read_pixel_t, typename write_pixel_t, typename paint_block_t>
    void paint_block_multi_t(const ggo::pixel_rect & block_rect,
    int scale_factor, int current_scale,
    iterator_t begin_it, iterator_t end_it,
    read_pixel_t read_pixel,
    write_pixel_t write_pixel,
    paint_block_t paint_block)
  {
    using item_pointer_t = std::iterator_traits<iterator_t>::value_type;
    using item_t = std::remove_pointer<item_pointer_t>::type;
    using real_t = item_t::real_t;

    GGO_ASSERT(current_scale >= 0);

    // Check for shapes intersecting the current block.
    auto block_rect_float = block_rect.get_rect<real_t>();

    bool block_inside_all_shapes = true;

    std::vector<item_pointer_t> current_block_layers;
    for (auto it = begin_it; it != end_it; ++it)
    {
      const item_t * item = *it;

      ggo::rect_intersection intersection = item->get_rect_intersection(block_rect_float);

      switch (intersection)
      {
      case ggo::rect_intersection::disjoints:
        break;
      case ggo::rect_intersection::rect_in_shape:
        current_block_layers.push_back(item);
        break;
      default:
        block_inside_all_shapes = false;
        current_block_layers.push_back(item);
        break;
      }
    }

    // Check if the current block intersects some shapes.
    if (current_block_layers.empty() == true)
    {
      return;
    }

    // The current block is only a pixel.
    if (block_rect.is_one_pixel() == true)
    {
      if (block_inside_all_shapes == true)
      {
        paint_block(block_rect, current_block_layers.begin(), current_block_layers.end());
      }
      else
      {
        auto bkgd_color = read_pixel(block_rect.left(), block_rect.bottom());

        ggo::accumulator<decltype(bkgd_color)> acc;
        ggo::sampler<smp>::sample_pixel<real_t>(block_rect.left(), block_rect.bottom(), [&](real_t x_f, real_t y_f)
        {
          auto sample_color = bkgd_color;

          for (auto it = current_block_layers.begin(); it != current_block_layers.end(); ++it)
          {
            const item_t * item = *it;
            if (item->is_point_inside(x_f, y_f) == true)
            {
              sample_color = item->blend(sample_color, item->brush(block_rect.left(), block_rect.bottom()));
            }
          }

          acc.add(sample_color);
        });

        const auto pixel_color = acc.div<ggo::sampler<smp>::samples_count>();
        write_pixel(block_rect.left(), block_rect.bottom(), pixel_color);
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
          paint_block_multi_t<smp>(block_rect,
            scale_factor, current_scale - 1,
            current_block_layers.begin(), current_block_layers.end(),
            read_pixel, write_pixel, paint_block);
        };
        
        int subblock_size = ggo::pow(scale_factor, current_scale - 1);
        process_blocks(block_rect, subblock_size, subblock_size, paint_subblock);
      }
    }
  }

  /////////////////////////////////////////////////////////////////////
  template <sampling smp, typename iterator_t, typename read_pixel_t, typename write_pixel_t, typename paint_block_t>
  void paint_multi_scale(int width, int height,
    iterator_t begin_it, iterator_t end_it,
    int scale_factor, int first_scale,
    read_pixel_t read_pixel,
    write_pixel_t write_pixel,
    paint_block_t paint_block)
  {
    using item_t = std::iterator_traits<iterator_t>::value_type;
    using real_t = item_t::real_t;

    if (begin_it == end_it)
    {
      return;
    }

    // Bounding rect.
    auto it = begin_it;
    auto layers_rect_data = it->get_bounding_rect();
    it++;
    for (; it != end_it; ++it)
    {
      layers_rect_data = ggo::rect_data_union(layers_rect_data, it->get_bounding_rect());
    }

    // Clip.
    ggo::rect<real_t> bounding_rect(layers_rect_data);
    pixel_rect bounding_pixel_rect(bounding_rect);
    if (bounding_pixel_rect.crop(width, height) == false)
    {
      return;
    }

    // Process blocks.
    std::vector<const item_t *> items;
    for (auto it = begin_it; it != end_it; ++it)
    {
      const auto & item = *it;
      items.push_back(&item);
    }

    auto process_block = [&](const ggo::pixel_rect & block_rect)
    {
      paint_block_multi_t<smp>(block_rect,
        scale_factor, first_scale,
        items.begin(), items.end(),
        read_pixel, write_pixel, paint_block);
    };

    int block_size = ggo::pow(scale_factor, first_scale);
    process_blocks(bounding_pixel_rect, block_size, block_size, process_block);
  }
}

#endif

