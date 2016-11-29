#ifndef __GGO_COLOR__
#define __GGO_COLOR__

#include <stdint.h>
#include <ggo_kernel.h>
#include <ggo_vec.h>

// Aliases.
namespace ggo
{
  using color_8u = color<uint8_t>;
  using color_16u = color<uint16_t>;
  using color_32u = color<uint32_t>;
  using color_32f = color<float>;
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
    using floating_point_t = float;
    using sample_t = uint8_t;
    static uint8_t max() { return 0xff; }
    static const int _samples_count = 1;
  };

  template <>
  struct color_traits<float>
  {
    using floating_point_t = float;
    using sample_t = float;
    static float max() { return 1.f; }
    static const int _samples_count = 1;
  };

  template <>
  struct color_traits<ggo::color_8u>
  {
    using floating_point_t = ggo::color_32f;
    using sample_t = uint8_t;
    static uint8_t max() { return 0xff; }
    static const int _samples_count = 3;
  };

  template <>
  struct color_traits<ggo::color_32u>
  {
    using floating_point_t = ggo::color_32f;
    using sample_t = uint32_t;
    static uint32_t max() { return 0xffffffff; }
    static const int _samples_count = 3;
  };

  template <>
  struct color_traits<ggo::color_32f>
  {
    using floating_point_t = ggo::color_32f;
    using sample_t = float;
    static float max() { return 1.f; }
    static const int _samples_count = 3;
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
    static_assert(std::is_same<color_in_t, color_out_t>::value, "expecting same color type");
    return c;
  }

  // rgb 32f => rgb 8u
  template <> inline ggo::color_8u convert_color_to<ggo::color_8u, ggo::color_32f>(const ggo::color_32f & c)
  {
    return ggo::color_8u(ggo::to<uint8_t>(255.f * c.r()), ggo::to<uint8_t>(255.f * c.g()), ggo::to<uint8_t>(255.f * c.b()));
  }

  // rgb 8u => rgb 32f
  template <> inline ggo::color_32f convert_color_to<ggo::color_32f, ggo::color_8u>(const ggo::color_8u & c)
  {
    return ggo::color_32f(c.r() / 255.f, c.g() / 255.f, c.b() / 255.f);
  }

  // y 8u => rgb 8u
  template <> inline ggo::color_8u convert_color_to<ggo::color_8u, uint8_t>(const uint8_t & c)
  {
    return ggo::color_8u(c, c, c);
  }

  // y 8u => y 32f
  template <> inline float convert_color_to<float, uint8_t>(const uint8_t & c)
  {
    return c / 255.f;
  }

  // y 32f => rgb 8u
  template <> inline ggo::color_8u convert_color_to<ggo::color_8u, float>(const float & c)
  {
    uint8_t gray = static_cast<uint8_t>(255.f * ggo::clamp(c, 0.f, 1.f) + 0.5f);

    return ggo::color_8u(gray, gray, gray);
  }

  // y 32f => y 8u
  template <> inline uint8_t convert_color_to<uint8_t, float>(const float & c)
  {
    return static_cast<uint8_t>(255.f * ggo::clamp(c, 0.f, 1.f) + 0.5f);
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

  inline ggo::color_8u white_8u()    { return ggo::white<ggo::color_8u>(); }
  inline ggo::color_8u gray_8u()     { return ggo::gray<ggo::color_8u>(); }
  inline ggo::color_8u black_8u()    { return ggo::black<ggo::color_8u>(); }
  inline ggo::color_8u red_8u()      { return ggo::red<ggo::color_8u>(); }
  inline ggo::color_8u green_8u()    { return ggo::green<ggo::color_8u>(); }
  inline ggo::color_8u blue_8u()     { return ggo::blue<ggo::color_8u>(); }
  inline ggo::color_8u cyan_8u()     { return ggo::cyan<ggo::color_8u>(); }
  inline ggo::color_8u magenta_8u()  { return ggo::magenta<ggo::color_8u>(); }
  inline ggo::color_8u yellow_8u()   { return ggo::yellow<ggo::color_8u>(); }
  inline ggo::color_8u orange_8u()   { return ggo::orange<ggo::color_8u>(); }
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
// HSV
namespace ggo
{
  // Reference: http://en.wikipedia.org/wiki/HSL_color_space
  template <typename data_t>
  void hsv2rgb(data_t h, data_t s, data_t v, data_t & r, data_t & g, data_t & b)
  {
    static_assert(std::is_floating_point<data_t>::value, "expected floating point type");

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
    typename color_traits<typename color_traits<color_t>::floating_point_t>::sample_t hue,
    typename color_traits<typename color_traits<color_t>::floating_point_t>::sample_t sat,
    typename color_traits<typename color_traits<color_t>::floating_point_t>::sample_t val)
  {
    using floating_point_color_t = typename color_traits<color_t>::floating_point_t;
    using real_t = typename color_traits<floating_point_color_t>::sample_t;

    static_assert(std::is_floating_point<decltype(hue)>::value, "expecting floating point type");
    static_assert(std::is_floating_point<decltype(sat)>::value, "expecting floating point type");
    static_assert(std::is_floating_point<decltype(val)>::value, "expecting floating point type");
    static_assert(std::is_floating_point<real_t>::value, "expecting floating point type");

    real_t r, g, b;
    ggo::hsv2rgb(hue, sat, val, r, g, b);
    floating_point_color_t c(r, g, b);

    return convert_color_to<color_t>(c);
  }
}

#endif
