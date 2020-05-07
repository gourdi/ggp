#if 0

#ifndef __GGO_FILL_COLOR_CURVE__
#define __GGO_FILL_COLOR_CURVE__

#include <kernel/math/interpolation/ggo_curve.h>

namespace ggo
{
  template <typename image_t, typename curve_t>
  void fill_color_curve(image_t & image, const curve_t & curve)
  {
    using floating_point_color_t = typename color_traits<image_t::color_t>::floating_point_color_t;
    using floating_point_t = typename color_traits<floating_point_color_t>::sample_t;

    const int w = image.width();
    const int h = image.height();
    const floating_point_t inv_h = 1 / floating_point_t(h);

    for (int y = 0; y < h; ++y)
    {
      const auto c = convert_color_to<image_t::color_t>(curve.evaluate(y * inv_h));

      for (int x = 0; x < w; ++x)
      {
        image.write_pixel(x, y, c);
      }
    }
  }

  template <ggo::lines_order memory_lines_order, ggo::pixel_type pixel_type, typename curve_t>
  void fill_color_curve(void * buffer, int width, int height, int line_byte_step, const curve_t & curve)
  {
    ggo::image_view_t<memory_lines_order, pixel_type> image(buffer, { width, height }, line_byte_step);

    fill_color_curve(image, curve);
  }
}

#endif
#endif
