#pragma once

#include <kernel/ggo_rect_int.h>
#include <kernel/ggo_ease.h>
#include <kernel/math/ggo_discretization.h>
#include <2d/blend/ggo_alpha_blend.h>
#include <2d/blend/ggo_opacity_blend.h>

namespace ggo
{
  template <typename image_t, typename shape_t, typename color_t>
  void paint_glow(image_t & image,
    const shape_t & shape, typename shape_t::scalar_t radius, typename shape_t::scalar_t opacity,
    const color_t & color)
  {
    auto bounding_rect = shape.get_bounding_rect();
    bounding_rect = extend(bounding_rect, radius);

    ggo::rect_int pixel_rect = discretize(bounding_rect);
    if (pixel_rect.clip(ggo::rect_int::from_size(image.size())) == false)
    {
      return;
    }

    auto scan = ggo::pixels_scan_for(image);

    for (auto [x, y] : scan(pixel_rect))
    {
      auto p = ggo::get_pixel_center<typename shape_t::data_t>({ x, y });
      auto dist = shape.dist_to_point(p);
      if (dist < radius)
      {
        auto glow_opacity = 1 - ggo::ease_inout(dist / radius);
        opacity_blender<typename image_t::color_t, alpha_blender<typename image_t::color_t, color_t>> blender(opacity * glow_opacity);

        auto bkgd_color = image.read_pixel(x, y);
        auto pixel_color = blender(bkgd_color, color);
        image.write_pixel(x, y, pixel_color);
      }
    }
  }
}

