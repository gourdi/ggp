#pragma once

#include <vector>
#include <kernel/ggo_rect_int.h>
#include <kernel/math/ggo_discretization.h>
#include <kernel/math/ggo_pixel_sampling.h>
#include <2d/paint/ggo_scene2d.h>

/////////////////////////////////////////////////////////////////////
// Single shape implementation.
namespace ggo
{
  /////////////////////////////////////////////////////////////////////
  template <pixel_sampling sampling, typename image_t, typename shape_t, typename brush_t, typename blend_t>
  void paint_block_single_t(image_t & image, const ggo::rect_int & block_rect,
    const int scale_factor, const int current_scale,
    const shape_t & shape,
    brush_t brush, blend_t blend)
  {
    GGO_ASSERT(current_scale >= 0);

    using data_t = typename shape_t::data_t;

    // Check for shape intersecting the current block.
    const ggo::rect_data<data_t> block_rect_data = to_continuous<data_t>(block_rect);

    const ggo::rect_intersection intersection = shape.get_rect_intersection(block_rect_data);

    switch (intersection)
    {
    case ggo::rect_intersection::disjoints:
      return;

    case ggo::rect_intersection::rect_in_shape:
      block_rect.for_each_pixel([&](int x, int y)
      {
        const auto bkgd_color = image.read_pixel(x, y);
        const auto brush_color = brush(x, y);

        image.write_pixel(x, y, blend(x, y, bkgd_color, brush_color));
      });
      break;

    case ggo::rect_intersection::partial_overlap:
    case ggo::rect_intersection::shape_in_rect:
    case ggo::rect_intersection::unknown:
      // The current block is only a pixel => stop the recursion and sample the pixel.
      if (block_rect.is_one() == true)
      {
        // Sampling.
        using sampler_t = ggo::sampler<sampling>;
        using fract_t = ggo::log2_fract<sampler_t::samples_count_log2>;

        fract_t fract;
        sampler_t::sample_pixel<data_t>(block_rect.left(), block_rect.bottom(), [&](data_t x_f, data_t y_f)
        {
          if (shape.is_point_inside({ x_f, y_f }) == true)
          {
            ++fract._num;
          }
        });
        GGO_ASSERT(fract._num <= fract._den);

        // Blending.
        auto bkgd_color     = image.read_pixel(block_rect.left(), block_rect.bottom());
        auto brush_color    = brush(block_rect.left(), block_rect.bottom());
        auto blended_color  = blend(block_rect.left(), block_rect.bottom(), bkgd_color, brush_color);
        auto pixel_color    = linerp(blended_color, bkgd_color, fract);

        image.write_pixel(block_rect.left(), block_rect.bottom(), pixel_color);

        return;
      }

      // Recursion.
      auto paint_subblock = [&](const ggo::rect_int & block_rect)
      {
        paint_block_single_t<sampling>(image, block_rect,
          scale_factor, current_scale - 1,
          shape, brush, blend);
      };

      const int subblock_size = ggo::pow(scale_factor, current_scale - 1);
      process_blocks(block_rect, subblock_size, subblock_size, paint_subblock);
      break;
    }
  }

  /////////////////////////////////////////////////////////////////////
  template <pixel_sampling sampling, typename image_t, typename shape_t, typename brush_t, typename blend_t>
  void paint_multi_scale(image_t & image,
    const shape_t & shape,
    int scale_factor, int first_scale,
    brush_t brush, blend_t blend,
    const ggo::rect_int & clipping)
  {
    using data_t = typename shape_t::data_t;

    // Bounding rect.
    const auto shape_bounding_rect_data = shape.get_bounding_rect();
    if (shape_bounding_rect_data._width == 0.f || shape_bounding_rect_data._height == 0.f)
    {
      return;
    }

    // Clip.
    ggo::rect<data_t> shape_bounding_rect(shape_bounding_rect_data);
    rect_int shape_pixel_rect = discretize(shape_bounding_rect.data());
    if (shape_pixel_rect.clip(image.width(), image.height()) == false || shape_pixel_rect.clip(clipping) == false)
    {
      return;
    }

    // Process blocks.
    auto process_rect = [&](const ggo::rect_int & block_rect)
    {
      paint_block_single_t<sampling>(image, 
        block_rect,
        scale_factor, first_scale,
        shape, brush, blend);
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
  template <pixel_sampling sampling, typename image_t, typename paint_shape_t>
  void paint_block_multi_t(image_t & image, const ggo::rect_int & block_rect,
    int scale_factor, int current_scale,
    const std::vector<const paint_shape_t *> & paint_shapes)
  {
    using data_t = typename paint_shape_t::data_t;

    GGO_ASSERT(current_scale >= 0);

    // Check for shapes intersecting the current block.
    auto block_rect_data = to_continuous<data_t>(block_rect);

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
      case ggo::rect_intersection::unknown:
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
        auto pixel_color = image.read_pixel(x, y);

        for (const auto * paint_shape : current_block_paint_shapes)
        {
          pixel_color = paint_shape->paint(x, y, pixel_color);
        }

        image.write_pixel(x, y, pixel_color);
      });

      return;
    }

    // The current block is only a pixel: sample shapes.
    if (block_rect.is_one() == true)
    {
      const auto bkgd_color = image.read_pixel(block_rect.left(), block_rect.bottom());

      ggo::color_accumulator<typename std::remove_const<decltype(bkgd_color)>::type> acc;
      ggo::sampler<sampling>::template sample_pixel<data_t>(block_rect.left(), block_rect.bottom(), [&](data_t x_f, data_t y_f)
      {
        auto sample_color = bkgd_color;

        for (const auto * paint_shape : current_block_paint_shapes)
        {
          if (paint_shape->is_point_inside({ x_f, y_f }) == true)
          {
            sample_color = paint_shape->paint(block_rect.left(), block_rect.bottom(), sample_color);
          }
        }

        acc.add(sample_color);
      });

      const auto pixel_color = acc.template div<ggo::sampler<sampling>::samples_count>();
      image.write_pixel(block_rect.left(), block_rect.bottom(), pixel_color);

      return;
    }

    // General case: recursion.
    auto paint_subblock = [&](const ggo::rect_int & block_rect)
    {
      paint_block_multi_t<sampling>(image, block_rect, scale_factor, current_scale - 1, current_block_paint_shapes);
    };

    int subblock_size = ggo::pow(scale_factor, current_scale - 1);
    process_blocks(block_rect, subblock_size, subblock_size, paint_subblock);
  }

  /////////////////////////////////////////////////////////////////////
  template <pixel_sampling sampling, typename image_t, typename color_t, typename data_t>
  void paint_multi_scale(image_t & image,
    const scene2d<color_t, data_t> & scene,
    int scale_factor, int first_scale,
    const ggo::rect_int & clipping)
  {
    // Clip.
    ggo::rect_int safe_clipping(clipping);
    if (safe_clipping.clip(image.size()) == false)
    {
      return;
    }

    // Retrieve shapes that are not clipped away.
    const ggo::rect_data<data_t> clipping_rect_data = to_continuous<data_t>(safe_clipping);
    std::optional<ggo::rect_data<data_t>> bounding_rect_data;

    std::vector<const paint_shape_abc<color_t, data_t> *> clipped_paint_shapes;
    for (const auto & paint_shape : scene)
    {
      const ggo::rect_data<data_t> cur_rect_data = paint_shape.get_bounding_rect();
      if (ggo::test_intersection(cur_rect_data, clipping_rect_data) == true)
      {
        clipped_paint_shapes.push_back(&paint_shape);

        if (bounding_rect_data)
        {
          bounding_rect_data = ggo::get_union(*bounding_rect_data, cur_rect_data);
        }
        else
        {
          bounding_rect_data = cur_rect_data;
        }
      }
    }

    if (clipped_paint_shapes.empty() == true)
    {
      return;
    }

    ggo::rect_int bounding_pixel_rect = discretize(*bounding_rect_data);
    if (bounding_pixel_rect.clip(safe_clipping) == false)
    {
      return;
    }

    // Process blocks.
    auto process_block = [&](const ggo::rect_int & block_rect)
    {
      paint_block_multi_t<sampling>(image, block_rect, scale_factor, first_scale, clipped_paint_shapes);
    };

    int block_size = ggo::pow(scale_factor, first_scale);
    process_blocks(bounding_pixel_rect, block_size, block_size, process_block);
  }
}

