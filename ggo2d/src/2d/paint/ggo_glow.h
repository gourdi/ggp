#ifndef __GGO_2D_PAINT_GLOW__
#define __GGO_2D_PAINT_GLOW__

#include <kernel/ggo_rect_int.h>
#include <kernel/ggo_ease.h>
#include <kernel/math/ggo_coordinates_conversions.h>
#include <2d/ggo_image_format.h>
#include <2d/paint/ggo_blend.h>

namespace ggo
{
  template <ggo::image_format format, typename shape_t>
  void paint_glow(void * buffer, int width, int height, int line_byte_step, 
    const shape_t & shape, typename shape_t::data_t radius, typename shape_t::data_t opacity,
    typename ggo::image_format_traits<format>::color_t color)
  {
    auto bounding_rect = shape.get_bounding_rect();
    bounding_rect = extend(bounding_rect, radius);

    ggo::rect_int pixel_rect = from_math_to_pixel_exclusive(bounding_rect);
    if (pixel_rect.clip(ggo::rect_int::from_width_height(width, height)) == false)
    {
      return;
    }

    pixel_rect.for_each_pixel([&](int x, int y)
    {
      auto p = ggo::from_pixel_to_math<typename shape_t::data_t>({ x, y });
      auto dist = shape.dist_to_point(p);
      if (dist < radius)
      {
        auto glow_opacity = 1 - ggo::ease_inout(dist / radius);
        auto ptr = get_pixel_ptr<format>(buffer, x, y, height, line_byte_step);
        auto bkgd_color = ggo::read_pixel<format>(ptr);
        auto pixel_color = alpha_blend(bkgd_color, color, opacity * glow_opacity);
        ggo::write_pixel<format>(ptr, pixel_color);
      }
    });
  }
}

#endif
