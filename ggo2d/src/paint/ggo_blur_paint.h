#ifndef __GGO_BLUR_PAINT__
#define __GGO_BLUR_PAINT__

#include <vector>
#include <ggo_vec.h>

namespace ggo
{
  enum class blur_samples_type
  {
    disc_12_samples,
    disc_32_samples,
    disc_52_samples,
  };

  template <blur_samples_type samples_type>
  struct blur_samples {};

  template <>
  struct blur_samples<blur_samples_type::disc_12_samples>
  {
    template <typename real_t>
    static std::vector<ggo::pos2<real_t>> get_samples(const real_t r)
    {
      const real_t r3 = r / 3;

      return{
        { r3, r },{ r3, r3 },{ r , r3 },
        { -r3, r },{ -r3, r3 },{ -r , r3 },
        { r3, -r },{ r3, -r3 },{ r , -r3 },
        { -r3, -r },{ -r3, -r3 },{ -r , -r3 } };
    }
  };

  template <>
  struct blur_samples<blur_samples_type::disc_32_samples>
  {
    template <typename real_t>
    static std::vector<ggo::pos2<real_t>> get_samples(const real_t r)
    {
      const real_t r1_5 = r / 5;
      const real_t r3_5 = 3 * r / 5;

      return{
        { r1_5, r1_5 },{ r1_5, r3_5 },{ r1_5, r },{ r3_5, r1_5 },{ r3_5, r3_5 },{ r3_5, r },{ r, r1_5 },{ r, r3_5 },
        { -r1_5, r1_5 },{ -r1_5, r3_5 },{ -r1_5, r },{ -r3_5, r1_5 },{ -r3_5, r3_5 },{ -r3_5, r },{ -r, r1_5 },{ -r, r3_5 },
        { r1_5, -r1_5 },{ r1_5, -r3_5 },{ r1_5, -r },{ r3_5, -r1_5 },{ r3_5, -r3_5 },{ r3_5, -r },{ r, -r1_5 },{ r, -r3_5 },
        { -r1_5, -r1_5 },{ -r1_5, -r3_5 },{ -r1_5, -r },{ -r3_5, -r1_5 },{ -r3_5, -r3_5 },{ -r3_5, -r },{ -r, -r1_5 },{ -r, -r3_5 } };
    }
  };

  template <>
  struct blur_samples<blur_samples_type::disc_52_samples>
  {
    template <typename real_t>
    static std::vector<ggo::pos2<real_t>> get_samples(const real_t r)
    {
      const real_t r1_7 = r / 7;
      const real_t r3_7 = 3 * r / 7;
      const real_t r5_7 = 5 * r / 7;

      return{
        { r1_7, r1_7 },{ r1_7, r3_7 },{ r1_7,r5_7 },{ r1_7, r },
        { r3_7, r1_7 },{ r3_7, r3_7 },{ r3_7,r5_7 },{ r3_7, r },
        { r5_7, r1_7 },{ r5_7, r3_7 },{ r5_7,r5_7 },
        { r, r1_7 },{ r, r3_7 },
        { -r1_7, r1_7 },{ -r1_7, r3_7 },{ -r1_7,r5_7 },{ -r1_7, r },
        { -r3_7, r1_7 },{ -r3_7, r3_7 },{ -r3_7,r5_7 },{ -r3_7, r },
        { -r5_7, r1_7 },{ -r5_7, r3_7 },{ -r5_7,r5_7 },
        { -r, r1_7 },{ -r, r3_7 },
        { r1_7, -r1_7 },{ r1_7, -r3_7 },{ r1_7,-r5_7 },{ r1_7, -r },
        { r3_7, -r1_7 },{ r3_7, -r3_7 },{ r3_7,-r5_7 },{ r3_7, -r },
        { r5_7, -r1_7 },{ r5_7, -r3_7 },{ r5_7,-r5_7 },
        { r, -r1_7 },{ r, -r3_7 },
        { -r1_7, -r1_7 },{ -r1_7, -r3_7 },{ -r1_7,-r5_7 },{ -r1_7, -r },
        { -r3_7, -r1_7 },{ -r3_7, -r3_7 },{ -r3_7,-r5_7 },{ -r3_7, -r },
        { -r5_7, -r1_7 },{ -r5_7, -r3_7 },{ -r5_7,-r5_7 },
        { -r, -r1_7 },{ -r, -r3_7 } };
    }
  };

  template <blur_samples_type samples_type, typename shape_t, typename paint_pixel_t>
  void paint_blur_shape(const shape_t & shape, int width, int height, typename shape_t::type blur_radius, paint_pixel_t paint_pixel)
  {
    using real_t = typename shape_t::type;
    static_assert(std::is_floating_point<real_t>::value, "expecting floating point type");

    // Bounding rect.
    const auto shape_bounding_rect_data = shape.get_bounding_rect();
    ggo::rect<real_t> shape_bounding_rect(shape_bounding_rect_data);
    shape_bounding_rect.inflate(blur_radius);

    // Clip.
    pixel_rect shape_pixel_rect(shape_bounding_rect);
    if (shape_pixel_rect.crop(width, height) == false)
    {
      return;
    }

    // Process.
    const auto disc_samples = blur_samples<samples_type>::get_samples(blur_radius);
    shape_pixel_rect.for_each_pixel([&](int x, int y)
    {
      const real_t x_fp = static_cast<real_t>(x);
      const real_t y_fp = static_cast<real_t>(y);

      int count = 0;

      for (const auto & sample : disc_samples)
      {
        const real_t sample_x = x_fp + sample.x();
        const real_t sample_y = y_fp + sample.y();

        if (shape.is_point_inside(sample_x, sample_y) == true)
        {
          ++count;
        }
      }

      paint_pixel(x, y, count, static_cast<int>(disc_samples.size()));
    });
  }
}

#endif

