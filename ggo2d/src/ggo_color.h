#ifndef __GGO_COLOR__
#define __GGO_COLOR__

#include <stdint.h>
#include <ggo_kernel.h>

namespace ggo
{
    // Reference: http://en.wikipedia.org/wiki/HSL_color_space
  template <typename data_t>
  void hsv2rgb(data_t h, data_t s, data_t v, data_t & r, data_t & g, data_t & b);

  template <typename data_t>
  class color
  {
  public:

    using sample_t = data_t;

  public:

    data_t	_r = 0;
    data_t	_g = 0;
    data_t	_b = 0;

  public:

              color() = default;
              color(data_t v) { _r = v; _g = v; _b = v; }
              color(data_t r, data_t g, data_t b) { _r = r; _g = g; _b = b; }
              color(const color<data_t> & c) { _r = c._r; _g = c._g; _b = c._b; }

    void		  operator+=(const color<data_t> & c) { _r += c._r; _g += c._g; _b += c._b; }
    void		  operator-=(const color<data_t> & c) { _r -= c._r; _g -= c._g; _b -= c._b; }
    void		  operator*=(const color<data_t> & c) { _r *= c._r; _g *= c._g; _b *= c._b; }
    void		  operator*=(data_t v) { _r *= v; _g *= v; _b *= v; }
    void		  operator/=(data_t v) { _r /= v; _g /= v; _b /= v; }

    bool      is_black() const { return _r == 0 && _g == 0 && _b == 0; }

    color &   operator=(const color & c) = default;
    bool      operator==(const color & c) const { return _r == c._r && _g == c._g && _b == c._b; }
    bool      operator!=(const color & c) const { return _r != c._r || _g != c._g || _b != c._b; }

    static data_t max() { return std::is_floating_point<data_t>() ? 1 : std::numeric_limits<data_t>::max(); }

  public:

    static const color white;
    static const color gray;
    static const color black;
    static const color red;
    static const color green;
    static const color blue;
    static const color cyan;
    static const color magenta;
    static const color yellow;
    static const color orange;
  };
}

namespace ggo
{
  using color_8u = color<uint8_t>;
  using color_16u = color<uint16_t>;
  using color_32u = color<uint32_t>;
  using color_32f = color<float>;
}

namespace ggo
{
  template <typename data_t>
  std::ostream & operator<<(std::ostream & os, const ggo::color<data_t> & c)
  {
    os << c._r << ' ' << c._g << ' ' << c._b;
    return os;
  }

  template <typename data_t>
  std::istream & operator >> (std::istream & is, ggo::color<data_t> & c)
  {
    is >> c._r >> c._g >> c._b;
    return is;
  }
  
  template <typename data_t>
  ggo::color<data_t> operator-(const ggo::color<data_t> & c)
  {
  return ggo::color<data_t>(-c._r, -c._g, -c._b);
  }

  template <typename data_t>
  ggo::color<data_t> operator+(const ggo::color<data_t> & c1, const ggo::color<data_t> & c2)
  {
  return ggo::color<data_t>(c1._r + c2._r, c1._g + c2._g, c1._b + c2._b);
  }

  template <typename data_t>
  ggo::color<data_t> operator-(const ggo::color<data_t> & c1, const ggo::color<data_t> & c2)
  {
  return ggo::color<data_t>(c1._r - c2._r, c1._g - c2._g, c1._b - c2._b);
  }

  template <typename data_t>
  ggo::color<data_t> operator*(const ggo::color<data_t> & c1, const ggo::color<data_t> & c2)
  {
  return ggo::color<data_t>(c1._r * c2._r, c1._g * c2._g, c1._b * c2._b);
  }

  template <typename data_t>
  ggo::color<data_t> operator*(const ggo::color<data_t> & c, data_t k)
  {
  return ggo::color<data_t>(k * c._r, k * c._g, k * c._b);
  }

  template <typename data_t>
  ggo::color<data_t> operator*(data_t k, const ggo::color<data_t> & c)
  {
  return ggo::color<data_t>(k * c._r, k * c._g, k * c._b);
  }

  template <typename data_t>
  ggo::color<data_t> operator/(const ggo::color<data_t> & c, data_t k)
  {
    return ggo::color<data_t>(c._r / k, c._g / k, c._b / k);
  }

  template <int bit_shift, typename data_t>
  ggo::color<data_t> fixed_point_div(const ggo::color<data_t> & c)
  {
    using sample_t = typename ggo::color<data_t>::sample_t;

    static_assert(bit_shift > 1, "invalid bit shift");
    static_assert(std::is_integral<sample_t>::value && std::is_unsigned<sample_t>::value, "expected unsigned integral sample type");

    return ggo::color<data_t>(
      ggo::fixed_point_div<bit_shift>(c._r),
      ggo::fixed_point_div<bit_shift>(c._g),
      ggo::fixed_point_div<bit_shift>(c._b));
  }
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
    using floating_point_t = ggo::color_32f;
    using sample_t = uint8_t;
  };

  template <>
  struct color_traits<float>
  {
    using floating_point_t = float;
    using sample_t = float;
  };

  template <>
  struct color_traits<ggo::color_8u>
  {
    using floating_point_t = ggo::color_32f;
    using sample_t = uint8_t;
  };

  template <>
  struct color_traits<ggo::color_32f>
  {
    using floating_point_t = ggo::color_32f;
    using sample_t = float;
  };
}

/////////////////////////////////////////////////////////////////////
// Color conversion
// Warning: don't use ggo::to<> because uint8_t <=> float conversion won't work 
// since float is normalized betwen 0 and 1.
namespace ggo
{
  template <typename color_out_t, typename color_in_t>
  color_out_t convert_color_to(const color_in_t & c)
  {
    static_assert(false, "missing specialization");
  }

  // rgb 32f => rgb 8u
  template <> inline ggo::color_8u convert_color_to<ggo::color_8u, ggo::color_32f>(const ggo::color_32f & c)
  {
    return ggo::color_8u(ggo::to<uint8_t>(255.f * c._r), ggo::to<uint8_t>(255.f * c._g), ggo::to<uint8_t>(255.f * c._b));
  }

  // rgb 8u <=> rgb 8u
  template <> inline ggo::color_8u convert_color_to<ggo::color_8u, ggo::color_8u>(const ggo::color_8u & c)
  {
    return c;
  }

  // rgb 8u => rgb 32f
  template <> inline ggo::color_32f convert_color_to<ggo::color_32f, ggo::color_8u>(const ggo::color_8u & c)
  {
    return ggo::color_32f(c._r / 255.f, c._g / 255.f, c._b / 255.f);
  }

  // y 8u => rgb 8u
  template <> inline ggo::color_8u convert_color_to<ggo::color_8u, uint8_t>(const uint8_t & c)
  {
    return ggo::color_8u(c, c, c);
  }

  // y 32f => rgb 8u
  template <> inline ggo::color_8u convert_color_to<ggo::color_8u, float>(const float & c)
  {
    uint8_t gray = static_cast<uint8_t>(255.f * ggo::clamp(c, 0.f, 1.f) + 0.5f);

    return ggo::color_8u(gray, gray, gray);
  }
}

/////////////////////////////////////////////////////////////////////
// Static variables.
namespace ggo
{
  template <typename data_t> const color<data_t> color<data_t>::white(color<data_t>::max(), color<data_t>::max(), color<data_t>::max());
  template <typename data_t> const color<data_t> color<data_t>::gray(color<data_t>::max() / 2, color<data_t>::max() / 2, color<data_t>::max() / 2);
  template <typename data_t> const color<data_t> color<data_t>::black(0, 0, 0);
  template <typename data_t> const color<data_t> color<data_t>::red(color<data_t>::max(), 0, 0);
  template <typename data_t> const color<data_t> color<data_t>::green(0, color<data_t>::max(), 0);
  template <typename data_t> const color<data_t> color<data_t>::blue(0, 0, color<data_t>::max());
  template <typename data_t> const color<data_t> color<data_t>::cyan(0, color<data_t>::max(), color<data_t>::max());
  template <typename data_t> const color<data_t> color<data_t>::magenta(color<data_t>::max(), 0, color<data_t>::max());
  template <typename data_t> const color<data_t> color<data_t>::yellow(color<data_t>::max(), color<data_t>::max(), 0);
  template <typename data_t> const color<data_t> color<data_t>::orange(color<data_t>::max(), color<data_t>::max() / 2, 0);
}

/////////////////////////////////////////////////////////////////////
// Color accumulator.
namespace ggo
{
  template <typename color_t>
  struct accumulator {};

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
    void add(const color_8u & c) { r += c._r; g += c._g; b += c._b; }
    template <int count> color_8u div() const { return color_8u((r + count / 2) / count, (g + count / 2) / count, (b + count / 2) / count); }
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
    void add(const color_32f & c) { r += c._r; g += c._g; b += c._b; }
    template <int count> color_32f div() const { return color_32f(r / count, g / count, b / count); }
  };
}

/////////////////////////////////////////////////////////////////////
// Implementation
namespace ggo
{
  /////////////////////////////////////////////////////////////////////
  template <typename color_t>
  color_t from_hsv(
    typename color_traits<typename color_traits<color_t>::floating_point_t>::sample_t hue,
    typename color_traits<typename color_traits<color_t>::floating_point_t>::sample_t sat,
    typename color_traits<typename color_traits<color_t>::floating_point_t>::sample_t val)
  {
    using floating_point_color_t = color_traits<color_t>::floating_point_t;
    using real_t = color_traits<floating_point_color_t>::sample_t;

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

/////////////////////////////////////////////////////////////////////
// Implementation.
namespace ggo
{
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
}

#endif
