#pragma once

#include <vector>
#include <kernel/ggo_kernel.h>
#include <kernel/ggo_rect_int.h>
#include <kernel/scan/ggo_scan.h>
#include <kernel/math/ggo_pixel_sampling.h>
#include <kernel/math/shapes_2d/ggo_shapes2d.h>
#include <2d/ggo_image.h>
#include <2d/paint/ggo_layer.h>
#include <2d/paint/ggo_paint_canvas.h>
#include <2d/blend/ggo_alpha_blend.h>
#include <2d/blend/ggo_opacity_blend.h>

namespace ggo
{
  /////////////////////////////////////////////////////////////////////
  template <pixel_sampling sampling, typename shape_t, typename read_pixel_t, typename write_pixel_t, typename brush_t, typename blend_t, typename tiles_scan_t>
  void paint_layer_t(const ggo::rect_int & tile_rect,
    const int scale_factor, const int current_scale,
    const shape_t & shape,
    read_pixel_t && read_pixel, write_pixel_t && write_pixel, brush_t && brush, blend_t && blend, tiles_scan_t && tiles_scan)
  {
    GGO_ASSERT(current_scale >= 0);

    using scalar_t = typename shape_t::scalar_t;

    // Check for shape intersecting the current tile.
    const ggo::rect_data<scalar_t> tile_rect_data = to_continuous<scalar_t>(tile_rect);

    const ggo::rect_intersection intersection = shape.get_rect_intersection(tile_rect_data);

    switch (intersection)
    {
    case ggo::rect_intersection::disjoints:
      return;

    case ggo::rect_intersection::rect_in_shape:
      tile_rect.for_each_pixel([&](int x, int y)
      {
        write_pixel(x, y, blend(read_pixel(x, y), brush(x, y)));
      });
      break;

    case ggo::rect_intersection::partial_overlap:
    case ggo::rect_intersection::shape_in_rect:
    case ggo::rect_intersection::unknown:
      // The current tile is only a pixel => stop the recursion and sample the pixel.
      if (tile_rect.is_one() == true)
      {
        // Sampling.
        using sampler_t = ggo::sampler<sampling>;
        using fract_t = ggo::log2_fract<sampler_t::samples_count_log2>;

        fract_t fract;
        sampler_t::template sample_pixel<scalar_t>(tile_rect.left(), tile_rect.bottom(), [&](scalar_t x_f, scalar_t y_f)
        {
          if (shape.is_point_inside({ x_f, y_f }) == true)
          {
            ++fract._num;
          }
        });
        GGO_ASSERT(fract._num <= fract._den);

        // Blending.
        int x = tile_rect.left();
        int y = tile_rect.bottom();
        auto bkgd_color   = read_pixel(x, y);
        auto pixel_color  = linerp(blend(bkgd_color, brush(x, y)), bkgd_color, fract);

        write_pixel(x, y, pixel_color);

        return;
      }

      // Recursion.
      auto paint_subtile = [&](const ggo::rect_int & tile_rect)
      {
        paint_layer_t<sampling>(tile_rect,
          scale_factor, current_scale - 1,
          shape, read_pixel, write_pixel, brush, blend, tiles_scan);
      };

      const int subtile_size = ggo::pow(scale_factor, current_scale - 1);
      tiles_scan(tile_rect, subtile_size, subtile_size, paint_subtile);
      break;
    }
  }

  /////////////////////////////////////////////////////////////////////
  template <pixel_sampling sampling, typename shape_t, typename read_pixel_t, typename write_pixel_t, typename brush_t, typename blend_t, typename tiles_scan_t>
  void paint_layer(int width, int height, const shape_t & shape,
    int scale_factor, int first_scale,
    read_pixel_t && read_pixel, write_pixel_t && write_pixel, brush_t && brush, blend_t && blend, tiles_scan_t && tiles_scan,
    const ggo::rect_int & clipping)
  {
    using scalar_t = typename shape_t::scalar_t;

    // Bounding rect.
    const auto shape_bounding_rect_data = shape.get_bounding_rect();
    if (shape_bounding_rect_data._width == 0.f || shape_bounding_rect_data._height == 0.f)
    {
      return;
    }

    // Clip.
    ggo::rect<scalar_t> shape_bounding_rect(shape_bounding_rect_data);
    rect_int shape_pixel_rect = discretize(shape_bounding_rect.data());
    if (shape_pixel_rect.clip(width, height) == false || shape_pixel_rect.clip(clipping) == false)
    {
      return;
    }

    // Process tiles.
    auto paint_tile = [&](const ggo::rect_int & tile_rect)
    {
      paint_layer_t<sampling>(tile_rect, scale_factor, first_scale, shape, read_pixel, write_pixel, brush, blend, tiles_scan);
    };

    int tile_size = ggo::pow(scale_factor, first_scale);
    tiles_scan(shape_pixel_rect, tile_size, tile_size, paint_tile);
  }
}

// Helpers.
namespace ggo
{
  /////////////////////////////////////////////////////////////////////
  template <pixel_sampling sampling, typename image_t, typename shape_t, typename brush_t, typename blend_t>
  void paint(image_t & image, const shape_t & shape, brush_t brush, blend_t blend, const ggo::rect_int & clipping)
  {
    auto read_pixel  = [&](int x, int y)
    { 
      return image.read_pixel(x, y);
    };

    auto write_pixel = [&](int x, int y, auto p)
    {
      image.write_pixel(x, y, p);
    };

    paint_layer<sampling>(image.width(), image.height(), shape, 8, 2, read_pixel, write_pixel, brush, blend, tiles_scan_for(image), clipping);
  }

  /////////////////////////////////////////////////////////////////////
  template <pixel_sampling sampling, typename image_t, typename shape_t, typename brush_t, typename blend_t>
  void paint(image_t & image, const shape_t & shape, brush_t brush, blend_t blend)
  {
    paint<sampling>(image, shape, brush, blend, ggo::rect_int::from_size(image.size()));
  }

  /////////////////////////////////////////////////////////////////////
  template <pixel_sampling sampling, typename image_t, typename shape_t, typename color_t>
  void paint(image_t & image, const shape_t & shape, const color_t & c, float opacity, const ggo::rect_int & clipping)
  {
    solid_color_brush<color_t> brush(c);
    opacity_blender<typename image_t::color_t, alpha_blender<typename image_t::color_t, color_t>> blender(opacity);

    paint<sampling>(image, shape, brush, blender, clipping);
  }

  /////////////////////////////////////////////////////////////////////
  template <pixel_sampling sampling, typename image_t, typename shape_t, typename color_t>
  void paint(image_t & image, const shape_t & shape, const color_t & c, float opacity)
  {
    paint<sampling>(image, shape, c, opacity, ggo::rect_int::from_size(image.size()));
  }

  /////////////////////////////////////////////////////////////////////
  template <pixel_sampling sampling, typename image_t, typename shape_t, typename color_t>
  void paint(image_t & image, const shape_t & shape, const color_t & c, const ggo::rect_int & clipping)
  {
    solid_color_brush<color_t> brush(c);
    alpha_blender<typename image_t::color_t, color_t> blender;

    paint<sampling>(image, shape, brush, blender, clipping);
  }

  /////////////////////////////////////////////////////////////////////
  template <pixel_sampling sampling, typename image_t, typename shape_t, typename color_t>
  void paint(image_t & image, const shape_t & shape, const color_t & c)
  {
    paint<sampling>(image, shape, c, ggo::rect_int::from_size(image.size()));
  }
}

