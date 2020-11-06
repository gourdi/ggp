#pragma once

#include <2d/ggo_color.h>
#include <map>

namespace ggo
{
  template <typename image_t, typename scalar_t, typename color_t>
  void fill_color_curve(image_t & image, const std::map<scalar_t, color_t> & curve)
  {
    using sample_t = typename color_traits<color_t>::sample_t;

    static_assert(color_traits<image_t::color_t>::color_space == color_traits<color_t>::color_space);
    static_assert(std::is_floating_point_v<sample_t>);
    static_assert(std::is_same_v<sample_t, scalar_t>);

    const int w = image.width();
    const int h = image.height();
    const scalar_t inv_h = 1 / scalar_t(h - 1);

    for (int y = 0; y < h; ++y)
    {
      const auto c1 = linear_interpolation1d(curve.begin(), curve.end(), y * inv_h);
      const auto c2 = convert_color_to<image_t::color_t>(c1);

      for (int x = 0; x < w; ++x)
      {
        image.write_pixel(x, y, c2);
      }
    }
  }
}

