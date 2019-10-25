#ifndef __GGO_PIXEL_SAMPLING__
#define __GGO_PIXEL_SAMPLING__

#include <kernel/math/ggo_coordinates_conversions.h>

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
      static_assert(std::is_floating_point<real_t>::value);
      auto p = from_discrete_to_continuous<real_t>({ x, y });
      
      func(p.x(), p.y());
    }

    static constexpr uint32_t samples_count = 1;
    static constexpr uint32_t samples_count_log2 = 0;
  };

  template <>
  struct sampler<sampling_2x2>
  {
    template <typename real_t, typename func_t>
    static void sample_pixel(int x, int y, func_t func)
    {
      static_assert(std::is_floating_point<real_t>::value);
      auto p = from_discrete_to_continuous<real_t>({ x, y });
      
      func(p.x() - real_t(0.25), p.y() - real_t(0.25));
      func(p.x() + real_t(0.25), p.y() - real_t(0.25));
      func(p.x() - real_t(0.25), p.y() + real_t(0.25));
      func(p.x() + real_t(0.25), p.y() + real_t(0.25));
    }

    static constexpr uint32_t samples_count = 4;
    static constexpr uint32_t samples_count_log2 = 2;
  };

  template <>
  struct sampler<sampling_4x4>
  {
    template <typename real_t, typename func_t>
    static void sample_pixel(int x, int y, func_t func)
    {
      static_assert(std::is_floating_point<real_t>::value);
      auto p = from_discrete_to_continuous<real_t>({ x, y });

      func(p.x() - real_t(0.375), p.y() - real_t(0.375));
      func(p.x() - real_t(0.375), p.y() - real_t(0.125));
      func(p.x() - real_t(0.375), p.y() + real_t(0.125));
      func(p.x() - real_t(0.375), p.y() + real_t(0.375));

      func(p.x() - real_t(0.125), p.y() - real_t(0.375));
      func(p.x() - real_t(0.125), p.y() - real_t(0.125));
      func(p.x() - real_t(0.125), p.y() + real_t(0.125));
      func(p.x() - real_t(0.125), p.y() + real_t(0.375));

      func(p.x() + real_t(0.125), p.y() - real_t(0.375));
      func(p.x() + real_t(0.125), p.y() - real_t(0.125));
      func(p.x() + real_t(0.125), p.y() + real_t(0.125));
      func(p.x() + real_t(0.125), p.y() + real_t(0.375));

      func(p.x() + real_t(0.375), p.y() - real_t(0.375));
      func(p.x() + real_t(0.375), p.y() - real_t(0.125));
      func(p.x() + real_t(0.375), p.y() + real_t(0.125));
      func(p.x() + real_t(0.375), p.y() + real_t(0.375));
    }

    static constexpr uint32_t samples_count = 16;
    static constexpr uint32_t samples_count_log2 = 4;
  };

  template <>
  struct sampler<sampling_8x8>
  {
    template <typename real_t, typename func_t>
    static void sample_pixel(int x, int y, func_t func)
    {
      static_assert(std::is_floating_point<real_t>::value);
      auto p = from_discrete_to_continuous<real_t>({ x, y });

      for (int offset_y = -7; offset_y <= 7; offset_y += 2)
      {
        for (int offset_x = -7; offset_x <= 7; offset_x += 2)
        {
          func(p.x() + offset_x / real_t(16), p.y() + offset_y / real_t(16));
        }
      }
    }

    static constexpr uint32_t samples_count = 64;
    static constexpr uint32_t samples_count_log2 = 6;
  };

  template <>
  struct sampler<sampling_16x16>
  {
    template <typename real_t, typename func_t>
    static void sample_pixel(int x, int y, func_t func)
    {
      static_assert(std::is_floating_point<real_t>::value);
      auto p = from_discrete_to_continuous<real_t>({ x, y });

      for (int offset_y = -15; offset_y <= 15; offset_y += 2)
      {
        for (int offset_x = -15; offset_x <= 15; offset_x += 2)
        {
          func(p.x() + offset_x / real_t(32), p.y() + offset_y / real_t(32));
        }
      }
    }

    static constexpr uint32_t samples_count = 256;
    static constexpr uint32_t samples_count_log2 = 8;
  };
}

#endif

