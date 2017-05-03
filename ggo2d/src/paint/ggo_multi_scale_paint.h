#ifndef __GGO_MULTI_SCALE_PAINT__
#define __GGO_MULTI_SCALE_PAINT__

#include <vector>
#include <ggo_sampling_paint.h>
#include <ggo_rect_int.h>
#include <ggo_coordinates_conversions.h>

/////////////////////////////////////////////////////////////////////
// Declaration.
namespace ggo
{
  // Paint a single shape.
  template <sampling smp, typename shape_t, typename brush_t, typename blend_t, typename read_pixel_t, typename write_pixel_t>
  void paint_multi_scale(int width, int height,
    const shape_t & shape,
    int scale_factor, int first_scale,
    brush_t brush, blend_t blend,
    read_pixel_t read_pixel, write_pixel_t write_pixel,
    const ggo::rect_int & clipping);

  // Paint multiple shapes.
  template <sampling smp, typename iterator_t, typename read_pixel_t, typename write_pixel_t>
    void paint_multi_scale(int width, int height,
      iterator_t begin_it, iterator_t end_it,
      int scale_factor, int first_scale,
      read_pixel_t read_pixel, write_pixel_t write_pixel,
      const ggo::rect_int & clipping);
}

/////////////////////////////////////////////////////////////////////
// Single shape implementation.
namespace ggo
{
  /////////////////////////////////////////////////////////////////////
  template <sampling smp, typename shape_t, typename brush_t, typename blend_t, typename read_pixel_t, typename write_pixel_t>
  void paint_block_single_t(const ggo::rect_int & block_rect,
    const int scale_factor, const int current_scale,
    const shape_t & shape,
    brush_t brush, blend_t blend,
    read_pixel_t read_pixel, write_pixel_t write_pixel)
  {
    GGO_ASSERT(current_scale >= 0);

    using real_t = typename shape_t::type;

    // Check for shape intersecting the current block.
    const ggo::rect_data<real_t> block_rect_data = from_pixel_to_math<real_t>(block_rect);

    const ggo::rect_intersection intersection = shape.get_rect_intersection(block_rect_data);

    switch (intersection)
    {
    case ggo::rect_intersection::disjoints:
      return;

    case ggo::rect_intersection::rect_in_shape:
      block_rect.for_each_pixel([&](int x, int y)
      {
        const auto bkgd_color = read_pixel(x, y);
        const auto brush_color = brush(x, y);

        write_pixel(x, y, blend(x, y, bkgd_color, brush_color));
      });
      break;

    case ggo::rect_intersection::partial_overlap:
    case ggo::rect_intersection::shape_in_rect:
      // The current block is only a pixel => stop the recursion and sample the pixel.
      if (block_rect.is_one() == true)
      {
        const auto bkgd_color = read_pixel(block_rect.left(), block_rect.bottom());
        const auto brush_color = brush(block_rect.left(), block_rect.bottom());

        ggo::accumulator<typename std::remove_const<decltype(bkgd_color)>::type> acc;
        ggo::sampler<smp>::template sample_pixel<typename shape_t::type>(block_rect.left(), block_rect.bottom(), [&](real_t x_f, real_t y_f)
        {
          acc.add(shape.is_point_inside(x_f, y_f) ? blend(block_rect.left(), block_rect.bottom(), bkgd_color, brush_color) : bkgd_color);
        });

        const auto pixel_color = acc.template div<ggo::sampler<smp>::samples_count>();
        write_pixel(block_rect.left(), block_rect.bottom(), pixel_color);

        return;
      }

      // Recursion.
      auto paint_subblock = [&](const ggo::rect_int & block_rect)
      {
        paint_block_single_t<smp>(block_rect,
          scale_factor, current_scale - 1,
          shape,
          brush, blend,
          read_pixel, write_pixel);
      };

      const int subblock_size = ggo::pow(scale_factor, current_scale - 1);
      process_blocks(block_rect, subblock_size, subblock_size, paint_subblock);
      break;
    }
  }

  /////////////////////////////////////////////////////////////////////
  template <sampling smp, typename shape_t, typename brush_t, typename blend_t, typename read_pixel_t, typename write_pixel_t>
  void paint_multi_scale(int width, int height,
    const shape_t & shape,
    int scale_factor, int first_scale,
    brush_t brush, blend_t blend,
    read_pixel_t read_pixel, write_pixel_t write_pixel,
    const ggo::rect_int & clipping)
  {
    using real_t = typename shape_t::type;

    // Bounding rect.
    const auto shape_bounding_rect_data = shape.get_bounding_rect();

    // Clip.
    ggo::rect<real_t> shape_bounding_rect(shape_bounding_rect_data);
    rect_int shape_pixel_rect = from_math_to_pixel(shape_bounding_rect.data());
    if (shape_pixel_rect.clip(width, height) == false || shape_pixel_rect.clip(clipping) == false)
    {
      return;
    }

    // Process blocks.
    auto process_rect = [&](const ggo::rect_int & block_rect)
    {
      paint_block_single_t<smp>(
        block_rect,
        scale_factor, first_scale,
        shape,
        brush, blend,
        read_pixel, write_pixel);
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
  template <sampling smp, typename iterator_t, typename read_pixel_t, typename write_pixel_t>
  void paint_block_multi_t(const ggo::rect_int & block_rect,
    int scale_factor, int current_scale,
    iterator_t begin_it, iterator_t end_it,
    read_pixel_t read_pixel, write_pixel_t write_pixel)
  {
    using item_pointer_t = typename std::iterator_traits<iterator_t>::value_type;
    using item_t = typename std::remove_pointer<item_pointer_t>::type;
    using real_t = typename item_t::real_t;

    GGO_ASSERT(current_scale >= 0);

    // Check for shapes intersecting the current block.
    auto block_rect_data = from_pixel_to_math<real_t>(block_rect);

    bool block_inside_all_shapes = true;

    std::vector<item_pointer_t> current_block_layers;
    for (auto it = begin_it; it != end_it; ++it)
    {
      const item_t * item = *it;

      ggo::rect_intersection intersection = item->get_rect_intersection(block_rect_data);

      switch (intersection)
      {
      case ggo::rect_intersection::disjoints:
        break;
      case ggo::rect_intersection::rect_in_shape:
        current_block_layers.push_back(item);
        break;
      case ggo::rect_intersection::partial_overlap:
      case ggo::rect_intersection::shape_in_rect:
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

    // If the current block is inside all shapes: no need to sample.
    if (block_inside_all_shapes == true)
    {
      block_rect.for_each_pixel([&](int x, int y)
      {
        auto pixel_color = read_pixel(x, y);

        for (auto it = current_block_layers.begin(); it != current_block_layers.end(); ++it)
        {
          const item_t * item = *it;

          const auto brush_color = item->brush(x, y);
          pixel_color = item->blend(x, y, pixel_color, brush_color);
        }

        write_pixel(x, y, pixel_color);
      });

      return;
    }

    // The current block is only a pixel: sample shapes.
    if (block_rect.is_one() == true)
    {
      const auto bkgd_color = read_pixel(block_rect.left(), block_rect.bottom());

      ggo::accumulator<typename std::remove_const<decltype(bkgd_color)>::type> acc;
      ggo::sampler<smp>::template sample_pixel<real_t>(block_rect.left(), block_rect.bottom(), [&](real_t x_f, real_t y_f)
      {
        auto sample_color = bkgd_color;

        for (auto it = current_block_layers.begin(); it != current_block_layers.end(); ++it)
        {
          const item_t * item = *it;
          if (item->is_point_inside(x_f, y_f) == true)
          {
            const auto brush_color = item->brush(block_rect.left(), block_rect.bottom());
            sample_color = item->blend(block_rect.left(), block_rect.bottom(), sample_color, brush_color);
          }
        }

        acc.add(sample_color);
      });

      const auto pixel_color = acc.template div<ggo::sampler<smp>::samples_count>();
      write_pixel(block_rect.left(), block_rect.bottom(), pixel_color);

      return;
    }

    // General case: recursion.
    auto paint_subblock = [&](const ggo::rect_int & block_rect)
    {
      paint_block_multi_t<smp>(block_rect,
        scale_factor, current_scale - 1,
        current_block_layers.begin(), current_block_layers.end(),
        read_pixel, write_pixel);
    };

    int subblock_size = ggo::pow(scale_factor, current_scale - 1);
    process_blocks(block_rect, subblock_size, subblock_size, paint_subblock);
  }

  /////////////////////////////////////////////////////////////////////
  template <sampling smp, typename iterator_t, typename read_pixel_t, typename write_pixel_t>
  void paint_multi_scale(int width, int height,
    iterator_t begin_it, iterator_t end_it,
    int scale_factor, int first_scale,
    read_pixel_t read_pixel, write_pixel_t write_pixel,
    const ggo::rect_int & clipping)
  {
    using item_t = typename std::iterator_traits<iterator_t>::value_type;
    using real_t = typename item_t::real_t;

    // Clip.
    ggo::rect_int safe_clipping(clipping);
    if (safe_clipping.clip(width, height) == false)
    {
      return;
    }

    const ggo::rect_data<real_t> clipping_rect_data = from_pixel_to_math<real_t>(safe_clipping);
    ggo::rect_data<real_t> bounding_rect_data;
    bool first = true;
    std::vector<const item_t *> items;
    for (auto it = begin_it; it != end_it; ++it)
    {
      const ggo::rect_data<real_t> cur_rect_data = it->get_bounding_rect();
      if (ggo::rect_data_intersect(cur_rect_data, clipping_rect_data) == true)
      {
        const auto & item = *it;
        items.push_back(&item);

        if (first == true)
        {
          bounding_rect_data = cur_rect_data;
          first = false;
        }
        else
        {
          bounding_rect_data = ggo::rect_data_union(bounding_rect_data, cur_rect_data);
        }
      }
    }

    if (items.empty() == true)
    {
      return;
    }

    ggo::rect_int bounding_pixel_rect = from_math_to_pixel(bounding_rect_data);
    if (bounding_pixel_rect.clip(safe_clipping) == false)
    {
      return;
    }

    // Process blocks.
    auto process_block = [&](const ggo::rect_int & block_rect)
    {
      paint_block_multi_t<smp>(block_rect,
        scale_factor, first_scale,
        items.begin(), items.end(),
        read_pixel, write_pixel);
    };

    int block_size = ggo::pow(scale_factor, first_scale);
    process_blocks(bounding_pixel_rect, block_size, block_size, process_block);
  }
}

#endif

