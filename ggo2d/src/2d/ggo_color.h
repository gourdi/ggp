#ifndef __GGO_COLOR__
#define __GGO_COLOR__

#include <stdint.h>
#include <kernel/ggo_kernel.h>
#include <kernel/ggo_vec.h>
#include <sstream>
#include <string>

// Aliases.
namespace ggo
{
  using color_8u = color<uint8_t>;
  using color_16u = color<uint16_t>;
  using color_32u = color<uint32_t>;
  using color_32f = color<float>;
  using alpha_color_8u = alpha_color<uint8_t>;
  using alpha_color_16u = alpha_color<uint16_t>;
  using alpha_color_32u = alpha_color<uint32_t>;
  using alpha_color_32f = alpha_color<float>;
}

/////////////////////////////////////////////////////////////////////
// Color traits
namespace ggo
{
  template <typename color_t>
  struct color_traits {};

  template <>
  struct color_traits<uint8_t>
  {
    using floating_point_color_t = float;
    using no_alpha_color_t = uint8_t;
    using sample_t = uint8_t;
    static constexpr uint8_t max() { return 0xff; }
    static constexpr int samples_count = 1;
    static constexpr bool has_alpha = false;
  };

  template <>
  struct color_traits<uint16_t>
  {
    using floating_point_color_t = float;
    using no_alpha_color_t = uint16_t;
    using sample_t = uint16_t;
    static constexpr uint16_t max() { return 0xffff; }
    static constexpr int samples_count = 1;
    static constexpr bool has_alpha = false;
  };

  template <>
  struct color_traits<uint32_t>
  {
    using floating_point_color_t = float;
    using no_alpha_color_t = uint32_t;
    using sample_t = uint32_t;
    static constexpr uint32_t max() { return 0xffffffff; }
    static constexpr int samples_count = 1;
    static constexpr bool has_alpha = false;
  };

  template <>
  struct color_traits<float>
  {
    using floating_point_color_t = float;
    using no_alpha_color_t = float;
    using sample_t = float;
    static constexpr float max() { return 1.f; }
    static constexpr int samples_count = 1;
    static constexpr bool has_alpha = false;
  };

  template <>
  struct color_traits<ggo::color_8u>
  {
    using floating_point_color_t = ggo::color_32f;
    using no_alpha_color_t = ggo::color_8u;
    using alpha_color_t = ggo::alpha_color_8u;
    using sample_t = uint8_t;
    static constexpr uint8_t max() { return 0xff; }
    static constexpr int samples_count = 3;
    static constexpr bool has_alpha = false;
  };

  template <>
  struct color_traits<ggo::color_16u>
  {
    using floating_point_color_t = ggo::color_32f;
    using no_alpha_color_t = ggo::color_16u;
    using alpha_color_t = ggo::alpha_color_16u;
    using sample_t = uint16_t;
    static constexpr uint16_t max() { return 0xffff; }
    static constexpr int samples_count = 3;
    static constexpr bool has_alpha = false;
  };

  template <>
  struct color_traits<ggo::color_32u>
  {
    using floating_point_color_t = ggo::color_32f;
    using no_alpha_color_t = ggo::color_32u;
    using alpha_color_t = ggo::alpha_color_32u;
    using sample_t = uint32_t;
    static constexpr uint32_t max() { return 0xffffffff; }
    static constexpr int samples_count = 3;
    static constexpr bool has_alpha = false;
  };

  template <>
  struct color_traits<ggo::color_32f>
  {
    using floating_point_color_t = ggo::color_32f;
    using no_alpha_color_t = ggo::color_32f;
    using alpha_color_t = ggo::alpha_color_32f;
    using sample_t = float;
    static constexpr float max() { return 1.f; }
    static constexpr int samples_count = 3;
    static constexpr bool has_alpha = false;
  };

  template <>
  struct color_traits<ggo::alpha_color_8u>
  {
    using floating_point_color_t = ggo::alpha_color_32f;
    using no_alpha_color_t = ggo::color_8u;
    using alpha_color_t = ggo::alpha_color_8u;
    using sample_t = uint8_t;
    static constexpr uint8_t max() { return 0xff; }
    static constexpr int samples_count = 4;
    static constexpr bool has_alpha = true;
  };

  template <>
  struct color_traits<ggo::alpha_color_32u>
  {
    using floating_point_color_t = ggo::alpha_color_32f;
    using no_alpha_color_t = ggo::color_32u;
    using alpha_color_t = ggo::alpha_color_32u;
    using sample_t = uint32_t;
    static constexpr uint32_t max() { return 0xffffffff; }
    static constexpr int samples_count = 4;
    static constexpr bool has_alpha = true;
  };

  template <>
  struct color_traits<ggo::alpha_color_32f>
  {
    using floating_point_color_t = ggo::alpha_color_32f;
    using no_alpha_color_t = ggo::color_32f;
    using alpha_color_t = ggo::alpha_color_32f;
    using sample_t = float;
    static constexpr float max() { return 1.f; }
    static constexpr int samples_count = 4;
    static constexpr bool has_alpha = true;
  };
}

// The following traits makes it possible to have a floating point color type of a given precision.
namespace ggo
{
  template <typename color_t, typename floating_point_t>
  struct floating_point_color_traits {};

  template <>
  struct floating_point_color_traits<uint8_t, float>
  {
    using floating_point_color_t = float;
  };

  template <>
  struct floating_point_color_traits<float, float>
  {
    using floating_point_color_t = float;
  };

  template <>
  struct floating_point_color_traits<ggo::color_8u, float>
  {
    using floating_point_color_t = ggo::color_32f;
  };

  template <>
  struct floating_point_color_traits<ggo::color_32f, float>
  {
    using floating_point_color_t = ggo::color_32f;
  };
}

/////////////////////////////////////////////////////////////////////
// Color conversion
// Warning: don't use ggo::to<> because uint8_t <=> float conversion won't work 
// since float is normalized betwen 0 and 1.
namespace ggo
{
  // Default case : return input color.
  template <typename color_out_t, typename color_in_t>
  color_out_t convert_color_to(const color_in_t & c)
  {
    // Automatically convert from y to rgb.
    if constexpr(std::is_same<color_traits<color_in_t>::sample_t, color_traits<color_out_t>::sample_t>::value == true &&
                 color_traits<color_in_t>::samples_count == 1 && color_traits<color_out_t>::samples_count == 3)
    {
      return color_out_t(c, c, c);
    }
    // Automatically convert from y to rgba.
    else if constexpr(std::is_same<color_traits<color_in_t>::sample_t, color_traits<color_out_t>::sample_t>::value == true &&
                      color_traits<color_in_t>::samples_count == 1 && color_traits<color_out_t>::samples_count == 4)
    {
      return color_out_t(c, c, c, color_traits<color_in_t>::max());
    }
    // Automatically convert from rgb to rgba.
    else if constexpr(std::is_same<color_traits<color_in_t>::sample_t, color_traits<color_out_t>::sample_t>::value == true &&
                      color_traits<color_in_t>::samples_count == 3 && color_traits<color_out_t>::samples_count == 4)
    {
      return color_out_t(c.r(), c.g(), c.b(), color_traits<color_in_t>::max());
    }
    // Automatically convert from rgba to rgb by removing the alpha channel.
    else if constexpr(std::is_same<color_traits<color_in_t>::sample_t, color_traits<color_out_t>::sample_t>::value == true &&
                      color_traits<color_in_t>::samples_count == 4 && color_traits<color_out_t>::samples_count == 3)
    {
      return color_out_t(c.r(), c.g(), c.b());
    }
    else
    {
      static_assert(std::is_same<color_in_t, color_out_t>::value);
      return c;
    }
  }

  // y 8u => y 16u
  template <> inline uint16_t convert_color_to<uint16_t, uint8_t>(const uint8_t & c)
  {
    return (uint16_t(c) << 8) | c;
  }

  // y 8u => y 32f
  template <> inline float convert_color_to<float, uint8_t>(const uint8_t & c)
  {
    return c / float(std::numeric_limits<uint8_t>::max());
  }

  // y 8u => rgb 16u
  template <> inline ggo::color_16u convert_color_to<ggo::color_16u, uint8_t>(const uint8_t & c)
  {
    uint16_t gray = (uint16_t(c) << 8) | c;
    return ggo::color_16u(gray, gray, gray);
  }

  // y 8u => rgb 32f
  template <> inline ggo::color_32f convert_color_to<ggo::color_32f, uint8_t>(const uint8_t & c)
  {
    float gray = c / float(std::numeric_limits<uint8_t>::max());
    return ggo::color_32f(gray, gray, gray);
  }

  // y 16u => y 8u
  template <> inline uint8_t convert_color_to<uint8_t, uint16_t>(const uint16_t & c)
  {
    return static_cast<uint8_t>(c >> 8);
  }

  // y 16u => y 32f
  template <> inline float convert_color_to<float, uint16_t>(const uint16_t & c)
  {
    return c / float(std::numeric_limits<uint16_t>::max());
  }

  // y 16u => rgb 8u
  template <> inline ggo::color_8u convert_color_to<ggo::color_8u, uint16_t>(const uint16_t & c)
  {
    uint8_t gray = static_cast<uint8_t>(c >> 8);
    return ggo::color_8u(gray, gray, gray);
  }

  // y 16u => rgba 8u
  template <> inline ggo::alpha_color_8u convert_color_to<ggo::alpha_color_8u, uint16_t>(const uint16_t & c)
  {
    uint8_t gray = static_cast<uint8_t>(c >> 8);
    return ggo::alpha_color_8u(gray, gray, gray, 0xff);
  }

  // y 16u => rgb 32f
  template <> inline ggo::color_32f convert_color_to<ggo::color_32f, uint16_t>(const uint16_t & c)
  {
    float gray = c / float(std::numeric_limits<uint16_t>::max());
    return ggo::color_32f(gray, gray, gray);
  }

  // y 32f => y 8u
  template <> inline uint8_t convert_color_to<uint8_t, float>(const float & c)
  {
    return static_cast<uint8_t>(std::numeric_limits<uint8_t>::max() * ggo::clamp(c, 0.f, 1.f) + 0.5f);
  }

  // y 32f => y 16u
  template <> inline uint16_t convert_color_to<uint16_t, float>(const float & c)
  {
    return static_cast<uint16_t>(std::numeric_limits<uint16_t>::max() * ggo::clamp(c, 0.f, 1.f) + 0.5f);
  }

  // y 32f => rgb 8u
  template <> inline ggo::color_8u convert_color_to<ggo::color_8u, float>(const float & c)
  {
    uint8_t gray = static_cast<uint8_t>(std::numeric_limits<uint8_t>::max() * ggo::clamp(c, 0.f, 1.f) + 0.5f);
    return ggo::color_8u(gray, gray, gray);
  }

  // y 32f => rgba 8u
  template <> inline ggo::alpha_color_8u convert_color_to<ggo::alpha_color_8u, float>(const float & c)
  {
    uint8_t gray = static_cast<uint8_t>(std::numeric_limits<uint8_t>::max() * ggo::clamp(c, 0.f, 1.f) + 0.5f);
    return ggo::alpha_color_8u(gray, gray, gray, 0xff);
  }

  // y 32f => rgb 16u
  template <> inline ggo::color_16u convert_color_to<ggo::color_16u, float>(const float & c)
  {
    uint16_t gray = static_cast<uint16_t>(std::numeric_limits<uint16_t>::max() * ggo::clamp(c, 0.f, 1.f) + 0.5f);
    return ggo::color_16u(gray, gray, gray);
  }

  // y 32f => rgb 32f
  template <> inline ggo::color_32f convert_color_to<ggo::color_32f, float>(const float & c)
  {
    return ggo::color_32f(c, c, c);
  }

  // rgb 8u => y 8u
  template <> inline uint8_t convert_color_to<uint8_t, ggo::color_8u>(const ggo::color_8u & c)
  {
    return (c.r() + c.g() + c.b()) / 3;
  }

  // rgb 8u => y 16u
  template <> inline uint16_t convert_color_to<uint16_t, ggo::color_8u>(const color_8u & c)
  {
    uint16_t gray = (c.r() + c.g() + c.b()) / 3;
    return (gray << 8) | gray;
  }

  // rgb 8u => y 32f
  template <> inline float convert_color_to<float, ggo::color_8u>(const ggo::color_8u & c)
  {
    return (c.r() + c.g() + c.b()) / (3.f * std::numeric_limits<uint8_t>::max());
  }  

  // rgb 8u => rgb 16u
  template <> inline ggo::color_16u convert_color_to<ggo::color_16u, ggo::color_8u>(const color_8u & c)
  {
    return ggo::color_16u((uint16_t(c.r()) << 8) | c.r(), (uint16_t(c.g()) << 8) | c.g(), (uint16_t(c.b()) << 8) | c.b());
  }

  // rgb 8u => rgb 32f
  template <> inline ggo::color_32f convert_color_to<ggo::color_32f, ggo::color_8u>(const ggo::color_8u & c)
  {
    constexpr float scale = 1.f / std::numeric_limits<uint8_t>::max();
    return ggo::color_32f(scale * c.r(), scale * c.g(), scale * c.b());
  }

  // rgb 16u => y 8u
  template <> inline uint8_t convert_color_to<uint8_t, ggo::color_16u>(const ggo::color_16u & c)
  {
    return ggo::fixed_point_div<8>((c.r() + c.g() + c.b()) / 3);
  }

  // rgb 16u => y 16u
  template <> inline uint16_t convert_color_to<uint16_t, ggo::color_16u>(const ggo::color_16u & c)
  {
    return (c.r() + c.g() + c.b()) / 3;
  }

  // rgb 16u => y 32f
  template <> inline float convert_color_to<float, ggo::color_16u>(const ggo::color_16u & c)
  {
    return (c.r() + c.g() + c.b()) / (3.f * std::numeric_limits<uint16_t>::max());
  }

  // rgb 16u => rgb 8u
  template <> inline ggo::color_8u convert_color_to<ggo::color_8u, ggo::color_16u>(const color_16u & c)
  {
    return ggo::color_8u(c.r() >> 8, c.g() >> 8, c.b() >> 8);
  }

  // rgb 16u => rgb 32f
  template <> inline ggo::color_32f convert_color_to<ggo::color_32f, ggo::color_16u>(const color_16u & c)
  {
    auto conv = [](uint16_t v) { return float(v) / std::numeric_limits<uint16_t>::max(); };
    return ggo::color_32f(conv(c.r()), conv(c.g()), conv(c.b()));
  }

  // rgb 16u => rgba 8u
  template <> inline ggo::alpha_color_8u convert_color_to<ggo::alpha_color_8u, ggo::color_16u>(const color_16u & c)
  {
    return ggo::alpha_color_8u(
      static_cast<uint8_t>(ggo::fixed_point_div<8>(c.r())), 
      static_cast<uint8_t>(ggo::fixed_point_div<8>(c.g())),
      static_cast<uint8_t>(ggo::fixed_point_div<8>(c.b())),
      0xff);
  }

  // rgb 32f => y 8u
  template <> inline uint8_t convert_color_to<uint8_t, ggo::color_32f>(const ggo::color_32f & c)
  {
    return ggo::clamp_and_round_to<uint8_t>(std::numeric_limits<uint8_t>::max() * (c.r() + c.g() + c.b()) / 3.f);
  }

  // rgb 32f => y 16u
  template <> inline uint16_t convert_color_to<uint16_t, ggo::color_32f>(const ggo::color_32f & c)
  {
    float gray = (c.r() + c.g() + c.b()) / 3.f;
    return ggo::clamp_and_round_to<uint16_t>(std::numeric_limits<uint16_t>::max() * gray);
  }

  // rgb 32f => y 32f
  template <> inline float convert_color_to<float, ggo::color_32f>(const ggo::color_32f & c)
  {
    return (c.r() + c.g() + c.b()) / 3.f;
  }

  // rgb 32f => rgb 8u
  template <> inline ggo::color_8u convert_color_to<ggo::color_8u, ggo::color_32f>(const ggo::color_32f & c)
  {
    auto conv = [](float v) { return ggo::clamp_and_round_to<uint8_t>(std::numeric_limits<uint8_t>::max() * v); };
    return { conv(c.r()), conv(c.g()), conv(c.b()) };
  }

  // rgb 32f => rgb 16u
  template <> inline ggo::color_16u convert_color_to<ggo::color_16u, ggo::color_32f>(const ggo::color_32f & c)
  {
    return ggo::color_16u(ggo::clamp_and_round_to<uint16_t>(c.r()), ggo::clamp_and_round_to<uint16_t>(c.g()), ggo::clamp_and_round_to<uint16_t>(c.b()));
  }

  // rgb 32f => rgba 8u
  template <> inline ggo::alpha_color_8u convert_color_to<ggo::alpha_color_8u, ggo::color_32f>(const ggo::color_32f & c)
  {
    auto conv = [](float v) { return ggo::clamp_and_round_to<uint8_t>(std::numeric_limits<uint8_t>::max() * v); };
    return { conv(c.r()), conv(c.g()), conv(c.b()), 0xff };
  }

  // rgba 8u => y 8u
  template <> inline uint8_t convert_color_to<uint8_t, ggo::alpha_color_8u>(const ggo::alpha_color_8u & c)
  {
    return (c.r() + c.g() + c.b()) / 3;
  }

  // rgba 8u => y 16u
  template <> inline uint16_t convert_color_to<uint16_t, ggo::alpha_color_8u>(const ggo::alpha_color_8u & c)
  {
    uint16_t gray = (c.r() + c.g() + c.b()) / 3;
    return (gray << 8) | gray;
  }

  // rgba 8u => y 32f
  template <> inline float convert_color_to<float, ggo::alpha_color_8u>(const ggo::alpha_color_8u & c)
  {
    return (c.r() + c.g() + c.b()) / (3.f * std::numeric_limits<uint8_t>::max());
  }

  // rgba 8u => rgb 16u
  template <> inline ggo::color_16u convert_color_to<ggo::color_16u, ggo::alpha_color_8u>(const ggo::alpha_color_8u & c)
  {
    return ggo::color_16u((c.r() << 8) | c.r(), (c.g() << 8) | c.g(), (c.b() << 8) | c.b());
  }

  // rgba 8u => rgb 32f
  template <> inline ggo::color_32f convert_color_to<ggo::color_32f, ggo::alpha_color_8u>(const ggo::alpha_color_8u & c)
  {
    float scale = 1.f / std::numeric_limits<uint8_t>::max();
    return ggo::color_32f(scale * c.r(), scale * c.g(), scale * c.b());
  }

  // rgba 8u => rgba 32f
  template <> inline ggo::alpha_color_32f convert_color_to<ggo::alpha_color_32f, ggo::alpha_color_8u>(const ggo::alpha_color_8u & c)
  {
    constexpr float scale = 1.f / std::numeric_limits<uint8_t>::max();
    return ggo::alpha_color_32f(scale * c.r(), scale * c.g(), scale * c.b(), scale * c.a());
  }

  // rgba 32f => rgba 8u
  template <> inline ggo::alpha_color_8u convert_color_to<ggo::alpha_color_8u, ggo::alpha_color_32f>(const ggo::alpha_color_32f & c)
  {
    auto conv = [](float v) { return ggo::clamp_and_round_to<uint8_t>(std::numeric_limits<uint8_t>::max() * v); };
    return { conv(c.r()), conv(c.g()), conv(c.b()), conv(c.a()) };
  }
}

/////////////////////////////////////////////////////////////////////
// Static variables.
namespace ggo
{
  template <typename color_t> color_t white()
  { 
    const auto v = color_traits<color_t>::max();
    return color_t(v, v, v);
  }

  template <typename color_t> color_t gray()
  { 
    using sample_t = typename color_traits<color_t>::sample_t;
    const sample_t v = color_traits<color_t>::max() / 2;
    return color_t(v, v, v);
  }

  template <typename color_t> color_t black()
  { 
    using sample_t = typename color_traits<color_t>::sample_t;
    return color_t(sample_t(0), sample_t(0), sample_t(0));
  }

  template <typename color_t> color_t red()
  { 
    using sample_t = typename color_traits<color_t>::sample_t;
    const auto v = color_traits<color_t>::max();
    return color_t(v, sample_t(0), sample_t(0));
  }

  template <typename color_t> color_t green()
  { 
    using sample_t = typename color_traits<color_t>::sample_t;
    const auto v = color_traits<color_t>::max();
    return color_t(sample_t(0), v, sample_t(0));
  }

  template <typename color_t> color_t blue()
  {
    using sample_t = typename color_traits<color_t>::sample_t;
    const auto v = color_traits<color_t>::max();
    return color_t(sample_t(0), sample_t(0), v);
  }

  template <typename color_t> color_t cyan()
  {
    using sample_t = typename color_traits<color_t>::sample_t;
    const auto v = color_traits<color_t>::max();
    return color_t(sample_t(0), v, v);
  }

  template <typename color_t> color_t magenta()
  {
    using sample_t = typename color_traits<color_t>::sample_t;
    const auto v = color_traits<color_t>::max();
    return color_t(v, sample_t(0), v);
  }

  template <typename color_t> color_t yellow()
  {
    using sample_t = typename color_traits<color_t>::sample_t;
    const auto v = color_traits<color_t>::max();
    return color_t(v, v, sample_t(0));
  }

  template <typename color_t> color_t orange()
  {
    using sample_t = typename color_traits<color_t>::sample_t;
    const auto v = color_traits<color_t>::max();
    return color_t(v, sample_t(v / 2), sample_t(0));
  }

  inline ggo::color_8u white_8u()     { return ggo::white<ggo::color_8u>(); }
  inline ggo::color_8u gray_8u()      { return ggo::gray<ggo::color_8u>(); }
  inline ggo::color_8u black_8u()     { return ggo::black<ggo::color_8u>(); }
  inline ggo::color_8u red_8u()       { return ggo::red<ggo::color_8u>(); }
  inline ggo::color_8u green_8u()     { return ggo::green<ggo::color_8u>(); }
  inline ggo::color_8u blue_8u()      { return ggo::blue<ggo::color_8u>(); }
  inline ggo::color_8u cyan_8u()      { return ggo::cyan<ggo::color_8u>(); }
  inline ggo::color_8u magenta_8u()   { return ggo::magenta<ggo::color_8u>(); }
  inline ggo::color_8u yellow_8u()    { return ggo::yellow<ggo::color_8u>(); }
  inline ggo::color_8u orange_8u()    { return ggo::orange<ggo::color_8u>(); }

  inline ggo::color_32f white_32f()   { return ggo::white<ggo::color_32f>(); }
  inline ggo::color_32f gray_32f()    { return ggo::gray<ggo::color_32f>(); }
  inline ggo::color_32f black_32f()   { return ggo::black<ggo::color_32f>(); }
  inline ggo::color_32f red_32f()     { return ggo::red<ggo::color_32f>(); }
  inline ggo::color_32f green_32f()   { return ggo::green<ggo::color_32f>(); }
  inline ggo::color_32f blue_32f()    { return ggo::blue<ggo::color_32f>(); }
  inline ggo::color_32f cyan_32f()    { return ggo::cyan<ggo::color_32f>(); }
  inline ggo::color_32f magenta_32f() { return ggo::magenta<ggo::color_32f>(); }
  inline ggo::color_32f yellow_32f()  { return ggo::yellow<ggo::color_32f>(); }
  inline ggo::color_32f orange_32f()  { return ggo::orange<ggo::color_32f>(); }
}

/////////////////////////////////////////////////////////////////////
// Color accumulator.
namespace ggo
{
  template <typename color_t>
  struct accumulator
  {
  };

  template <>
  struct accumulator<uint8_t>
  {
    int acc = 0;
    void add(uint8_t c) { acc += c; }
    template <int count> uint8_t div() const { return (acc + count / 2) / count; }
  };

  template <>
  struct accumulator<color_8u>
  {
    int r = 0; int g = 0; int b = 0;
    void add(const color_8u & c) { r += c.r(); g += c.g(); b += c.b(); }
    template <int count> color_8u div() const {
      return color_8u(
        static_cast<uint8_t>((r + count / 2) / count),
        static_cast<uint8_t>((g + count / 2) / count),
        static_cast<uint8_t>((b + count / 2) / count));
    }
  };

  template <>
  struct accumulator<float>
  {
    float acc = 0.f;
    void add(float c) { acc += c; }
    template <int count> float div() const { return acc / count; }
  };

  template <>
  struct accumulator<color_32f>
  {
    float r = 0.f; float g = 0.f; float b = 0.f;
    void add(const color_32f & c) { r += c.r(); g += c.g(); b += c.b(); }
    template <int count> color_32f div() const { return color_32f(r / count, g / count, b / count); }
  };
}

/////////////////////////////////////////////////////////////////////
// Fixed point division for color.
namespace ggo
{
  template <int bit_shift, typename data_t>
  ggo::color<data_t> fixed_point_div(const ggo::color<data_t> & c)
  {
    using sample_t = typename ggo::color_traits<color<data_t>>::sample_t;

    static_assert(bit_shift > 1, "invalid bit shift");
    static_assert(std::is_integral<sample_t>::value && std::is_unsigned<sample_t>::value, "expected unsigned integral sample type");

    return ggo::color<data_t>(
      ggo::fixed_point_div<bit_shift>(c.r()),
      ggo::fixed_point_div<bit_shift>(c.g()),
      ggo::fixed_point_div<bit_shift>(c.b()));
  }
}

/////////////////////////////////////////////////////////////////////
// Linear interpolation.
namespace ggo
{
  template <typename fract_t>
  uint8_t linerp(uint8_t color_a, uint8_t color_b, const fract_t & weight_a)
  {
    constexpr uint32_t one = 1 << fract_t::_log2_den;

    const uint32_t weight_b = one - weight_a._num;
    
    return ggo::fixed_point_div<fract_t::_log2_den>(weight_a._num * color_a + weight_b * color_b);
  }

  template <typename fract_t>
  color_8u linerp(color_8u color_a, color_8u color_b, const fract_t & weight_a)
  {
    constexpr uint32_t one = 1 << fract_t::_log2_den;

    const uint32_t weight_b = one - weight_a._num;

    return {
      static_cast<uint8_t>(ggo::fixed_point_div<fract_t::_log2_den>(weight_a._num * color_a.r() + weight_b * color_b.r())),
      static_cast<uint8_t>(ggo::fixed_point_div<fract_t::_log2_den>(weight_a._num * color_a.g() + weight_b * color_b.g())),
      static_cast<uint8_t>(ggo::fixed_point_div<fract_t::_log2_den>(weight_a._num * color_a.b() + weight_b * color_b.b())) };
  }

  template <typename fract_t>
  alpha_color_8u linerp(alpha_color_8u color_a, alpha_color_8u color_b, const fract_t & weight_a)
  {
    uint32_t w_a = ggo::fixed_point_div<fract_t::_log2_den>(weight_a._num * color_a.a());
    uint32_t w_b = ggo::round_div((0xff - color_b.a()) * color_b.a(), 0xff);

    uint32_t a = w_a + w_b;
    if (a == 0)
    {
      return { 0, 0, 0, 0 };
    }

    uint32_t r = ggo::round_div(w_a * color_a.r() + w_b * color_b.r(), a);
    uint32_t g = ggo::round_div(w_a * color_a.g() + w_b * color_b.g(), a);
    uint32_t b = ggo::round_div(w_a * color_a.b() + w_b * color_b.b(), a);

    return { static_cast<uint8_t>(r), static_cast<uint8_t>(g), static_cast<uint8_t>(b), static_cast<uint8_t>(a) };
  }

  template <typename fract_t>
  float linerp(float color_a, float color_b, const fract_t & weight_a)
  {
    const float weight_a_32f = weight_a.to<float>();
    const float weight_b_32f = 1.f - weight_a_32f;

    return weight_a_32f * color_a + weight_b_32f * color_b;
  }

  template <typename fract_t>
  color_32f linerp(const color_32f & color_a, const color_32f & color_b, const fract_t & weight_a)
  {
    const float weight_a_32f = weight_a.to<float>();
    const float weight_b_32f = 1.f - weight_a_32f;

    return {
      weight_a_32f * color_a.r() + weight_b_32f * color_b.r(),
      weight_a_32f * color_a.g() + weight_b_32f * color_b.g(),
      weight_a_32f * color_a.b() + weight_b_32f * color_b.b() };
  }
}

/////////////////////////////////////////////////////////////////////
// HSV
namespace ggo
{
  // Reference: http://en.wikipedia.org/wiki/HSL_color_space
  template <typename data_t>
  void hsv2rgb(data_t h, data_t s, data_t v, data_t & r, data_t & g, data_t & b)
  {
    static_assert(std::is_floating_point<data_t>::value);

    h = std::fmod(h, data_t(1));
    if (h < 0)
    {
      h += 1;
    }
    s = ggo::clamp<data_t>(s, 0, 1);
    v = ggo::clamp<data_t>(v, 0, 1);

    int		  h_i = ggo::clamp(int(h * 6), 0, 5);
    data_t	f = h * 6 - std::floor(h * 6);
    data_t	p = v * (1 - s);
    data_t	q = v * (1 - f * s);
    data_t	t = v * (1 - (1 - f) * s);

    switch (h_i)
    {
    case 0:
      r = v;
      g = t;
      b = p;
      break;
    case 1:
      r = q;
      g = v;
      b = p;
      break;
    case 2:
      r = p;
      g = v;
      b = t;
      break;
    case 3:
      r = p;
      g = q;
      b = v;
      break;
    case 4:
      r = t;
      g = p;
      b = v;
      break;
    case 5:
      r = v;
      g = p;
      b = q;
      break;
    default:
      GGO_FAIL();
      break;
    }
  }

  template <typename color_t>
  color_t from_hsv(
    typename color_traits<typename color_traits<color_t>::floating_point_color_t>::sample_t hue,
    typename color_traits<typename color_traits<color_t>::floating_point_color_t>::sample_t sat,
    typename color_traits<typename color_traits<color_t>::floating_point_color_t>::sample_t val)
  {
    using floating_point_color_t = typename color_traits<color_t>::floating_point_color_t;
    using sample_t = typename color_traits<floating_point_color_t>::sample_t;

    static_assert(std::is_floating_point<decltype(hue)>::value, "expecting floating point type");
    static_assert(std::is_floating_point<decltype(sat)>::value, "expecting floating point type");
    static_assert(std::is_floating_point<decltype(val)>::value, "expecting floating point type");
    static_assert(std::is_floating_point<sample_t>::value, "expecting floating point type");

    sample_t r, g, b;
    ggo::hsv2rgb(hue, sat, val, r, g, b);
    floating_point_color_t c(r, g, b);

    return convert_color_to<color_t>(c);
  }
}

/////////////////////////////////////////////////////////////////////
// Strings
namespace ggo
{
  inline std::string to_hex(const ggo::color_8u & c)
  {
    auto convert = [](uint8_t v)
    {
      switch (v)
      {
      case 0x00: return '0';
      case 0x01: return '1';
      case 0x02: return '2';
      case 0x03: return '3';
      case 0x04: return '4';
      case 0x05: return '5';
      case 0x06: return '6';
      case 0x07: return '7';
      case 0x08: return '8';
      case 0x09: return '9';
      case 0x0A: return 'A';
      case 0x0B: return 'B';
      case 0x0C: return 'C';
      case 0x0D: return 'D';
      case 0x0E: return 'E';
      case 0x0F: return 'F';
      default: GGO_FAIL(); return '\0';
      }
    };

    std::ostringstream oss;
    oss << convert((c.r() & 0xF0) >> 4) << convert(c.r() & 0x0F);
    oss << convert((c.g() & 0xF0) >> 4) << convert(c.g() & 0x0F);
    oss << convert((c.b() & 0xF0) >> 4) << convert(c.b() & 0x0F);
    return oss.str();
  }

  inline ggo::color_8u from_hex(const std::string & s)
  {
    if (s.size() != 6)
    {
      throw std::runtime_error(std::string("cannot convert string '") + s + "' to color: expecting 6 characeters");
    }

    auto convert = [](char c)
    {
      switch (c)
      {
      case '0': return 0x00;
      case '1': return 0x01;
      case '2': return 0x02;
      case '3': return 0x03;
      case '4': return 0x04;
      case '5': return 0x05;
      case '6': return 0x06;
      case '7': return 0x07;
      case '8': return 0x08;
      case '9': return 0x09;
      case 'A': return 0x0A;
      case 'B': return 0x0B;
      case 'C': return 0x0C;
      case 'D': return 0x0D;
      case 'E': return 0x0E;
      case 'F': return 0x0F ;
      default: throw std::runtime_error(std::string("invalid character in string '") + c + "'");
      }
    };

    unsigned char r = (convert(s[0]) << 4) + convert(s[1]);
    unsigned char g = (convert(s[2]) << 4) + convert(s[3]);
    unsigned char b = (convert(s[4]) << 4) + convert(s[5]);

    return { r, g, b };
  }
}

#endif
