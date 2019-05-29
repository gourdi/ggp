#ifndef __GGO_PIXEL_SAMPLING__
#define __GGO_PIXEL_SAMPLING__

/////////////////////////////////////////////////////////////////////
// Sampling structs.
namespace ggo
{
  enum pixel_sampling
  {
    sampling_1,
    sampling_2x2,
    sampling_4x4,
    sampling_8x8,
    sampling_16x16
  };
  
  template <pixel_sampling sampling>
  struct sampler { };

  template <>
  struct sampler<sampling_1>
  {
    template <typename real_t, typename func_t>
    static void sample_pixel(int x, int y, func_t func)
    {
      static_assert(std::is_floating_point<real_t>::value, "expecting floating point type");
      func(real_t(x) + real_t(0.5), real_t(y) + real_t(0.5));
    }

    static constexpr int samples_count = 1;
    static constexpr int samples_count_log2 = 0;
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
      func(x_f + real_t(0.25), y_f + real_t(0.75));

      func(x_f + real_t(0.75), y_f + real_t(0.25));
      func(x_f + real_t(0.75), y_f + real_t(0.75));
    }

    static constexpr int samples_count = 4;
    static constexpr int samples_count_log2 = 2;
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

      func(x_f + real_t(0.125), y_f + real_t(0.125));
      func(x_f + real_t(0.125), y_f + real_t(0.375));
      func(x_f + real_t(0.125), y_f + real_t(0.625));
      func(x_f + real_t(0.125), y_f + real_t(0.875));

      func(x_f + real_t(0.375), y_f + real_t(0.125));
      func(x_f + real_t(0.375), y_f + real_t(0.375));
      func(x_f + real_t(0.375), y_f + real_t(0.625));
      func(x_f + real_t(0.375), y_f + real_t(0.875));

      func(x_f + real_t(0.625), y_f + real_t(0.125));
      func(x_f + real_t(0.625), y_f + real_t(0.375));
      func(x_f + real_t(0.625), y_f + real_t(0.625));
      func(x_f + real_t(0.625), y_f + real_t(0.875));

      func(x_f + real_t(0.875), y_f + real_t(0.125));
      func(x_f + real_t(0.875), y_f + real_t(0.375));
      func(x_f + real_t(0.875), y_f + real_t(0.625));
      func(x_f + real_t(0.875), y_f + real_t(0.875));
    }

    static constexpr int samples_count = 16;
    static constexpr int samples_count_log2 = 4;
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

      for (int offset_y = 1; offset_y <= 15; offset_y += 2)
      {
        for (int offset_x = 1; offset_x <= 15; offset_x += 2)
        {
          func(x_f + offset_x / real_t(16), y_f + offset_y / real_t(16));
        }
      }
    }

    static constexpr int samples_count = 64;
    static constexpr int samples_count_log2 = 6;
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

      for (int offset_y = 1; offset_y <= 31; offset_y += 2)
      {
        for (int offset_x = 1; offset_x <= 31; offset_x += 2)
        {
          func(x_f + offset_x / real_t(32), y_f + offset_y / real_t(32));
        }
      }
    }

    static constexpr int samples_count = 256;
    static constexpr int samples_count_log2 = 8;
  };
}

#endif

