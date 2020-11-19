#pragma once

#include <vector>
#include <kernel/ggo_rect_int.h>
#include <kernel/math/ggo_discretization.h>
#include <kernel/math/ggo_pixel_sampling.h>
#include <2d/paint/ggo_canvas.h>

namespace ggo
{
  /////////////////////////////////////////////////////////////////////
  template <pixel_sampling sampling, typename paint_shape_t, typename read_pixel_t, typename write_pixel_t, typename tiles_scan_t>
  void paint_canvas_t(const ggo::rect_int & tile_rect,
    const int scale_factor, const int current_scale,
    const std::vector<const paint_shape_t *> & paint_shapes,
    read_pixel_t && read_pixel, write_pixel_t && write_pixel, const tiles_scan_t & tiles_scan)
  {
    using scalar_t = typename paint_shape_t::scalar_t;

    GGO_ASSERT(current_scale >= 0);

    // Check for shapes intersecting the current tile.
    auto tile_rect_data = to_continuous<scalar_t>(tile_rect);

    bool tile_inside_all_shapes = true;

    std::vector<const paint_shape_t *> current_tile_paint_shapes;
    for (const auto * paint_shape : paint_shapes)
    {
      ggo::rect_intersection intersection = paint_shape->get_rect_intersection(tile_rect_data);

      switch (intersection)
      {
      case ggo::rect_intersection::disjoints:
        break;
      case ggo::rect_intersection::rect_in_shape:
        current_tile_paint_shapes.push_back(paint_shape);
        break;
      case ggo::rect_intersection::partial_overlap:
      case ggo::rect_intersection::shape_in_rect:
      case ggo::rect_intersection::unknown:
        tile_inside_all_shapes = false;
        current_tile_paint_shapes.push_back(paint_shape);
        break;
      }
    }

    // Check if the current tile intersects some shapes.
    if (current_tile_paint_shapes.empty() == true)
    {
      return;
    }

    // If the current tile is inside all shapes: no need to sample.
    if (tile_inside_all_shapes == true)
    {
      tile_rect.for_each_pixel([&](int x, int y)
      {
        auto pixel_color = read_pixel(x, y);

        for (const auto * paint_shape : current_tile_paint_shapes)
        {
          pixel_color = paint_shape->paint(x, y, pixel_color);
        }

        write_pixel(x, y, pixel_color);
      });

      return;
    }

    // The current tile is only a pixel: sample shapes.
    if (tile_rect.is_one() == true)
    {
      const auto bkgd_color = read_pixel(tile_rect.left(), tile_rect.bottom());

      ggo::color_accumulator<typename std::remove_const<decltype(bkgd_color)>::type> acc;
      ggo::sampler<sampling>::template sample_pixel<scalar_t>(tile_rect.left(), tile_rect.bottom(), [&](scalar_t x_f, scalar_t y_f)
      {
        auto sample_color = bkgd_color;

        for (const auto * paint_shape : current_tile_paint_shapes)
        {
          if (paint_shape->is_point_inside({ x_f, y_f }) == true)
          {
            sample_color = paint_shape->paint(tile_rect.left(), tile_rect.bottom(), sample_color);
          }
        }

        acc.add(sample_color);
      });

      const auto pixel_color = acc.template div<ggo::sampler<sampling>::samples_count>();
      write_pixel(tile_rect.left(), tile_rect.bottom(), pixel_color);
    }
    else
    {
      // General case: recursion.
      auto paint_subtile = [&](const ggo::rect_int & tile_rect)
      {
        paint_canvas_t<sampling>(tile_rect, scale_factor, current_scale - 1, current_tile_paint_shapes, read_pixel, write_pixel, tiles_scan);
      };

      int subtile_size = ggo::pow(scale_factor, current_scale - 1);
      tiles_scan(tile_rect, subtile_size, subtile_size, paint_subtile);
    }
  }

  /////////////////////////////////////////////////////////////////////
  template <pixel_sampling sampling, typename color_t, typename scalar_t, typename read_pixel_t, typename write_pixel_t, typename tiles_scan_t>
  void paint_canvas(int width, int height,
    const canvas<color_t, scalar_t> & canvas,
    int scale_factor, int first_scale,
    read_pixel_t && read_pixel, write_pixel_t && write_pixel, const tiles_scan_t & tiles_scan,
    const ggo::rect_int & clipping)
  {
    // Clip.
    ggo::rect_int safe_clipping(clipping);
    if (safe_clipping.clip(width, height) == false)
    {
      return;
    }

    // Retrieve shapes that are not clipped away.
    const ggo::rect_data<scalar_t> clipping_rect_data = to_continuous<scalar_t>(safe_clipping);
    std::optional<ggo::rect_data<scalar_t>> bounding_rect_data;

    std::vector<const layer<color_t, scalar_t> *> clipped_layers;
    for (const auto & layer : canvas)
    {
      const ggo::rect_data<scalar_t> cur_rect_data = layer.get_bounding_rect();
      if (ggo::test_intersection(cur_rect_data, clipping_rect_data) == true)
      {
        clipped_layers.push_back(&layer);

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

    if (clipped_layers.empty() == true)
    {
      return;
    }

    ggo::rect_int bounding_pixel_rect = discretize(*bounding_rect_data);
    if (bounding_pixel_rect.clip(safe_clipping) == false)
    {
      return;
    }

    // Process tiles.
    auto process_rect = [&](const ggo::rect_int & tile_rect)
    {
      paint_canvas_t<sampling>(tile_rect, scale_factor, first_scale, clipped_layers, read_pixel, write_pixel, tiles_scan);
    };

    int tile_size = ggo::pow(scale_factor, first_scale);
    tiles_scan(bounding_pixel_rect, tile_size, tile_size, process_rect);
  }
}

// Helpers.
namespace ggo
{
  /////////////////////////////////////////////////////////////////////
  template <pixel_sampling sampling, typename image_t, typename color_t, typename scalar_t>
  void paint(image_t & image, const canvas<color_t, scalar_t> & canvas, const ggo::rect_int & clipping)
  {
    auto read_pixel  = [&](int x, int y) { return image.read_pixel(x, y); };
    auto write_pixel = [&](int x, int y, auto p) { image.write_pixel(x, y, p); };

    paint_canvas<sampling>(image.width(), image.height(), canvas, 8, 2, read_pixel, write_pixel, tiles_scan_for(image), clipping);
  }

  /////////////////////////////////////////////////////////////////////
  template <pixel_sampling sampling, typename image_t, typename color_t, typename scalar_t>
  void paint(image_t & image, const canvas<color_t, scalar_t> & canvas)
  {
    paint<sampling>(image, canvas, ggo::rect_int::from_size(image.size()));
  }
}


