#ifndef __GGO_SAMPLING_PAINT__
#define __GGO_SAMPLING_PAINT__

/////////////////////////////////////////////////////////////////////
// Sampling structs.
namespace ggo
{
  enum sampling
  {
    sampling_1,
    sampling_2x2,
    sampling_4x4,
    sampling_8x8,
    sampling_16x16
  };

  template <sampling smp>
  struct sampler { };

  template <>
  struct sampler<sampling_1>
  {
    template <typename real_t, typename func_t>
    static void sample_pixel(int x, int y, func_t func)
    {
      static_assert(std::is_floating_point<real_t>::value, "expecting floating point type");
      func(real_t(x), real_t(y));
    }

    static const int samples_count = 1;
  };

  template <>
  struct sampler<sampling_2x2>
  {
    template <typename real_t, typename func_t>
    static void sample_pixel(int x, int y, func_t func)
    {
      static_assert(std::is_floating_point<real_t>::value, "expecting floating point type");
      const real_t x_f(static_cast<real_t>(x));
      const real_t y_f(static_cast<real_t>(y));

      func(x_f + real_t(0.25), y_f + real_t(0.25));
      func(x_f + real_t(0.25), y_f - real_t(0.25));

      func(x_f - real_t(0.25), y_f + real_t(0.25));
      func(x_f - real_t(0.25), y_f - real_t(0.25));
    }

    static const int samples_count = 4;
  };

  template <>
  struct sampler<sampling_4x4>
  {
    template <typename real_t, typename func_t>
    static void sample_pixel(int x, int y, func_t func)
    {
      static_assert(std::is_floating_point<real_t>::value, "expecting floating point type");
      const real_t x_f(static_cast<real_t>(x));
      const real_t y_f(static_cast<real_t>(y));

      func(x_f - real_t(0.375), y_f - real_t(0.375));
      func(x_f - real_t(0.375), y_f - real_t(0.125));
      func(x_f - real_t(0.375), y_f + real_t(0.125));
      func(x_f - real_t(0.375), y_f + real_t(0.375));

      func(x_f - real_t(0.125), y_f - real_t(0.375));
      func(x_f - real_t(0.125), y_f - real_t(0.125));
      func(x_f - real_t(0.125), y_f + real_t(0.125));
      func(x_f - real_t(0.125), y_f + real_t(0.375));

      func(x_f + real_t(0.125), y_f - real_t(0.375));
      func(x_f + real_t(0.125), y_f - real_t(0.125));
      func(x_f + real_t(0.125), y_f + real_t(0.125));
      func(x_f + real_t(0.125), y_f + real_t(0.375));

      func(x_f + real_t(0.375), y_f - real_t(0.375));
      func(x_f + real_t(0.375), y_f - real_t(0.125));
      func(x_f + real_t(0.375), y_f + real_t(0.125));
      func(x_f + real_t(0.375), y_f + real_t(0.375));
    }

    static const int samples_count = 16;
  };

  template <>
  struct sampler<sampling_8x8>
  {
    template <typename real_t, typename func_t>
    static void sample_pixel(int x, int y, func_t func)
    {
      static_assert(std::is_floating_point<real_t>::value, "expecting floating point type");
      const real_t x_f(static_cast<real_t>(x));
      const real_t y_f(static_cast<real_t>(y));

      for (int offset_y = -7; offset_y <= 7; offset_y += 2)
      {
        for (int offset_x = -7; offset_x <= 7; offset_x += 2)
        {
          func(x_f + offset_x / real_t(16), y_f + offset_y / real_t(16));
        }
      }
    }

    static const int samples_count = 64;
  };

  template <>
  struct sampler<sampling_16x16>
  {
    template <typename real_t, typename func_t>
    static void sample_pixel(int x, int y, func_t func)
    {
      static_assert(std::is_floating_point<real_t>::value, "expecting floating point type");
      const real_t x_f(static_cast<real_t>(x));
      const real_t y_f(static_cast<real_t>(y));

      for (int offset_y = -15; offset_y <= 15; offset_y += 2)
      {
        for (int offset_x = -15; offset_x <= 15; offset_x += 2)
        {
          func(x_f + offset_x / real_t(32), y_f + offset_y / real_t(32));
        }
      }
    }

    static const int samples_count = 256;
  };
}

#if 0
/////////////////////////////////////////////////////////////////////
// Multi-scale paint with sampling.
namespace ggo
{
  template <sampling smp, typename color_t, typename real_t, typename layer_it_t>
  void paint(int width, int height, layer_it_t layer_begin_it, layer_it_t layer_end_it)
  {
    const int scale_factor = 8;
    const int first_scale = 2;

    // Function that compute pixel color by sampling shapes.
    auto sample_pixel = [&](int x, int y, layer_it_t layer_begin_it, layer_it_t layer_end_it)
    {
      ggo::accumulator<color_t> accumulator;
      const color_t bkgd_color = get_pixel(x, y, height);

      sampler<smp>::sample_pixel<real_t>(x, y, [&](real_t x_f, real_t y_f)
      {
        color_t sample_color(bkgd_color);
        for (auto it = layer_begin_it; it != layer_end_it; ++it)
        {
          if (it->is_point_inside(x_f, y_f) == true)
          {
            // ergreret
          }
        }
        accumulator.add(sample_color);
      });

      color_t pixel = accumulator.div<sampler<smp>::samples_count>();
      //set_pixel(x, y, pixel);
    };

    // Function that compute pixel color without sampling shapes.
    auto paint_pixel = [&](int x, int y, layer_it_t layer_begin_it, layer_it_t layer_end_it)
    {
      ggo::accumulator<color_t> accumulator;
      const color_t bkgd_color = get_pixel(x, y, height);

      sampler<smp>::sample_pixel<real_t>(x, y, [&](real_t x_f, real_t y_f)
      {
        color_t sample_color(bkgd_color);
        for (auto it = layer_begin_it; it != layer_end_it; ++it)
        {
          // zegdfgdf
        }
        accumulator.add(sample_color);
      });

      color_t pixel = accumulator.div<sampler<smp>::samples_count>();
      //set_pixel(x, y, pixel);
    };

    // Function that fill a block of pixels.
    auto paint_block = [&](const ggo::pixel_rect & block_rect)
    {
      //block_rect.for_each_pixel(const pixel_func & f) const
    };

    // Call the multi-scale paint algorithm.
    paint_shapes<layer_item>(
      width, height,
      layer_begin_it, layer_end_it
      scale_factor, first_scale,
      paint_block, paint_pixel);
  }
}

#endif

#endif

