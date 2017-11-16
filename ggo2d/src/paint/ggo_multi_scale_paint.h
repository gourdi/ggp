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
  template <sampling smp, typename data_t, typename color_t, typename brush_color_t, typename get_next_paint_shape_t, typename read_pixel_t, typename write_pixel_t>
  void paint_multi_scale(int width, int height,
    get_next_paint_shape_t get_next_paint_shape,
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

    using data_t = typename shape_t::data_t;

    // Check for shape intersecting the current block.
    const ggo::rect_data<data_t> block_rect_data = from_pixel_to_math<data_t>(block_rect);

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
        ggo::sampler<smp>::template sample_pixel<typename shape_t::data_t>(block_rect.left(), block_rect.bottom(), [&](data_t x_f, data_t y_f)
        {
          acc.add(shape.is_point_inside({ x_f, y_f }) ? blend(block_rect.left(), block_rect.bottom(), bkgd_color, brush_color) : bkgd_color);
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
    using data_t = typename shape_t::data_t;

    // Bounding rect.
    const auto shape_bounding_rect_data = shape.get_bounding_rect();

    // Clip.
    ggo::rect<data_t> shape_bounding_rect(shape_bounding_rect_data);
    rect_int shape_pixel_rect = from_math_to_pixel_exclusive(shape_bounding_rect.data());
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
  template <sampling smp, typename paint_shape_t, typename read_pixel_t, typename write_pixel_t>
  void paint_block_multi_t(const ggo::rect_int & block_rect,
    int scale_factor, int current_scale,
    const std::vector<const paint_shape_t *> & paint_shapes,
    read_pixel_t read_pixel, write_pixel_t write_pixel)
  {
    using data_t = typename paint_shape_t::data_t;

    GGO_ASSERT(current_scale >= 0);

    // Check for shapes intersecting the current block.
    auto block_rect_data = from_pixel_to_math<data_t>(block_rect);

    bool block_inside_all_shapes = true;

    std::vector<const paint_shape_t *> current_block_paint_shapes;
    for (const auto * paint_shape : paint_shapes)
    {
      ggo::rect_intersection intersection = paint_shape->get_rect_intersection(block_rect_data);

      switch (intersection)
      {
      case ggo::rect_intersection::disjoints:
        break;
      case ggo::rect_intersection::rect_in_shape:
        current_block_paint_shapes.push_back(paint_shape);
        break;
      case ggo::rect_intersection::partial_overlap:
      case ggo::rect_intersection::shape_in_rect:
        block_inside_all_shapes = false;
        current_block_paint_shapes.push_back(paint_shape);
        break;
      }
    }

    // Check if the current block intersects some shapes.
    if (current_block_paint_shapes.empty() == true)
    {
      return;
    }

    // If the current block is inside all shapes: no need to sample.
    if (block_inside_all_shapes == true)
    {
      block_rect.for_each_pixel([&](int x, int y)
      {
        auto pixel_color = read_pixel(x, y);

        for (const auto * paint_shape : current_block_paint_shapes)
        {
          const auto brush_color = paint_shape->brush(x, y);
          pixel_color = paint_shape->blend(x, y, pixel_color, brush_color);
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
      ggo::sampler<smp>::template sample_pixel<data_t>(block_rect.left(), block_rect.bottom(), [&](data_t x_f, data_t y_f)
      {
        auto sample_color = bkgd_color;

        for (const auto * paint_shape : current_block_paint_shapes)
        {
          if (paint_shape->is_point_inside({ x_f, y_f }) == true)
          {
            const auto brush_color = paint_shape->brush(block_rect.left(), block_rect.bottom());
            sample_color = paint_shape->blend(block_rect.left(), block_rect.bottom(), sample_color, brush_color);
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
        current_block_paint_shapes,
        read_pixel, write_pixel);
    };

    int subblock_size = ggo::pow(scale_factor, current_scale - 1);
    process_blocks(block_rect, subblock_size, subblock_size, paint_subblock);
  }

  /////////////////////////////////////////////////////////////////////
  template <sampling smp, typename data_t, typename color_t, typename brush_color_t, typename get_next_paint_shape_t, typename read_pixel_t, typename write_pixel_t>
  void paint_multi_scale(int width, int height,
    get_next_paint_shape_t get_next_paint_shape,
    int scale_factor, int first_scale,
    read_pixel_t read_pixel, write_pixel_t write_pixel,
    const ggo::rect_int & clipping)
  {
    // Clip.
    ggo::rect_int safe_clipping(clipping);
    if (safe_clipping.clip(width, height) == false)
    {
      return;
    }

    const ggo::rect_data<data_t> clipping_rect_data = from_pixel_to_math<data_t>(safe_clipping);
    ggo::rect_data<data_t> bounding_rect_data;
    bool first = true;
    std::vector<const paint_shape_abc<data_t, color_t, brush_color_t> *> paint_shapes;
    while (const paint_shape_abc<data_t, color_t, brush_color_t> * paint_shape = get_next_paint_shape())
    {
      const ggo::rect_data<data_t> cur_rect_data = paint_shape->get_bounding_rect();
      if (ggo::test_intersection(cur_rect_data, clipping_rect_data) == true)
      {
        paint_shapes.push_back(paint_shape);

        if (first == true)
        {
          bounding_rect_data = cur_rect_data;
          first = false;
        }
        else
        {
          bounding_rect_data = ggo::get_union(bounding_rect_data, cur_rect_data);
        }
      }
    }

    if (paint_shapes.empty() == true)
    {
      return;
    }

    ggo::rect_int bounding_pixel_rect = from_math_to_pixel_exclusive(bounding_rect_data);
    if (bounding_pixel_rect.clip(safe_clipping) == false)
    {
      return;
    }

    // Process blocks.
    auto process_block = [&](const ggo::rect_int & block_rect)
    {
      paint_block_multi_t<smp>(block_rect,
        scale_factor, first_scale,
        paint_shapes,
        read_pixel, write_pixel);
    };

    int block_size = ggo::pow(scale_factor, first_scale);
    process_blocks(bounding_pixel_rect, block_size, block_size, process_block);
  }
}

#endif

