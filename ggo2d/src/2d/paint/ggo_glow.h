#ifndef __GGO_2D_PAINT_GLOW__
#define __GGO_2D_PAINT_GLOW__

#include <kernel/ggo_rect_int.h>
#include <kernel/ggo_ease.h>
#include <kernel/math/ggo_coordinates_conversions.h>
#include <2d/paint/ggo_blend.h>

namespace ggo
{
  template <typename image_t, typename shape_t>
  void paint_glow(image_t & image,
    const shape_t & shape, typename shape_t::data_t radius, typename shape_t::data_t opacity,
    typename image_t::color_t color)
  {
    auto bounding_rect = shape.get_bounding_rect();
    bounding_rect = extend(bounding_rect, radius);

    ggo::rect_int pixel_rect = from_continuous_to_discrete_exclusive(bounding_rect);
    if (pixel_rect.clip(ggo::rect_int::from_size(image.size())) == false)
    {
      return;
    }

    pixel_rect.for_each_pixel([&](int x, int y)
    {
      auto p = ggo::from_discrete_to_continuous<typename shape_t::data_t>({ x, y });
      auto dist = shape.dist_to_point(p);
      if (dist < radius)
      {
        auto glow_opacity = 1 - ggo::ease_inout(dist / radius);
        auto bkgd_color = image.read_pixel(x, y);
        auto pixel_color = alpha_blend(bkgd_color, color, opacity * glow_opacity);
        image.write_pixel(x, y, pixel_color);
      }
    });
  }
}

#endif
