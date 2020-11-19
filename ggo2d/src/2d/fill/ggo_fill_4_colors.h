#pragma once

#include <2d/ggo_image.h>
#include <2d/ggo_color.h>

namespace ggo
{
  template <typename image_t>
  void fill_4_colors(image_t & image,
    const typename image_t::color_t & c1,
    const typename image_t::color_t & c2,
    const typename image_t::color_t & c3,
    const typename image_t::color_t & c4,
    std::optional<ggo::rect_int> clipping = {})
  {
    using floating_point_color_t = typename color_traits<typename image_t::color_t>::floating_point_color_t;
    using floating_point_t = typename color_traits<floating_point_color_t>::sample_t;

    static_assert(std::is_floating_point_v<floating_point_t>);

    const int w = image.width();
    const int h = image.height();
    const floating_point_t inv_w = 1 / static_cast<floating_point_t>(image.width() - 1);
    const floating_point_t inv_h = 1 / static_cast<floating_point_t>(image.height() - 1);
    const floating_point_color_t c1_fp = convert_color_to<floating_point_color_t>(c1);
    const floating_point_color_t c2_fp = convert_color_to<floating_point_color_t>(c2);
    const floating_point_color_t c3_fp = convert_color_to<floating_point_color_t>(c3);
    const floating_point_color_t c4_fp = convert_color_to<floating_point_color_t>(c4);

    if (!clipping)
    {
      clipping = ggo::rect_int::from_size(image.size());
    }

    for (int y = clipping->bottom(); y <= clipping->top(); ++y)
    {
      auto color5 = (static_cast<floating_point_t>(y) * c1_fp + static_cast<floating_point_t>(h - y - 1) * c2_fp) * inv_h;
      auto color6 = (static_cast<floating_point_t>(y) * c3_fp + static_cast<floating_point_t>(h - y - 1) * c4_fp) * inv_h;

      for (int x = clipping->left(); x <= clipping->right(); ++x)
      {
        auto color7 = (static_cast<floating_point_t>(x) * color5 + static_cast<floating_point_t>(w - x - 1) * color6) * inv_w;
        image.write_pixel(x, y, ggo::convert_color_to<typename image_t::color_t>(color7));
      }
    }
  }
}

