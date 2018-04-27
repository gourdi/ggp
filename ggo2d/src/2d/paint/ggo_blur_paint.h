#ifndef __GGO_BLUR_PAINT__
#define __GGO_BLUR_PAINT__

#include <vector>
#include <kernel/ggo_vec.h>
#include <kernel/math/ggo_coordinates_conversions.h>

namespace ggo
{
  template <typename shape_t, typename paint_pixel_t>
  void paint_blur_shape(const shape_t & shape, int width, int height, typename shape_t::data_t blur_radius, typename shape_t::data_t sample_step, paint_pixel_t paint_pixel)
  {
    using real_t = typename shape_t::data_t;
    static_assert(std::is_floating_point<real_t>::value);

    // Bounding rect.
    const auto shape_bounding_rect_data = shape.get_bounding_rect();
    ggo::rect<real_t> shape_bounding_rect(shape_bounding_rect_data);
    shape_bounding_rect.inflate(blur_radius);

    // Clip.
    rect_int shape_pixel_rect = from_math_to_pixel_exclusive(shape_bounding_rect.data());
    if (shape_pixel_rect.clip(width, height) == false)
    {
      return;
    }

    // Process.
    real_t squared_radius = blur_radius * blur_radius;
    shape_pixel_rect.for_each_pixel([&](int x, int y)
    {
      auto center = from_pixel_to_math<real_t>({ x, y });

      int inside = 0;
      int total = 0;

      for (real_t dx = sample_step / 2; dx < blur_radius; dx += sample_step)
      {
        for (real_t dy = sample_step / 2; dy < blur_radius; dy += sample_step)
        {
          if (dx * dx + dy * dy < squared_radius)
          {
            auto sample = [&](const auto & p)
            {
              if (shape.is_point_inside(p) == true)
              {
                ++inside;
              }
            };

            sample(center + ggo::pos2<real_t>(dx, dy));
            sample(center + ggo::pos2<real_t>(-dx, dy));
            sample(center + ggo::pos2<real_t>(dx, -dy));
            sample(center + ggo::pos2<real_t>(-dx, -dy));

            total += 4;
          }
        }
      }

      paint_pixel(x, y, inside, total);
    });
  }
}

#endif

