#ifndef __GGO_COLOR__
#define __GGO_COLOR__

#include <stdint.h>
#include <ggo_kernel.h>
#include <iostream>

namespace ggo
{
    // Reference: http://en.wikipedia.org/wiki/HSL_color_space
  template <typename data_t>
  void hsv2rgb(data_t h, data_t s, data_t v, data_t & r, data_t & g, data_t & b);

  template <typename data_t>
  class color
  {
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
    void		  operator-=(const color & c) { _r -= c._r; _g -= c._g; _b -= c._b; }
/*    void		  operator*=(const color & c) { _r *= c._r; _g *= c._g; _b *= c._b; }
    void		  operator*=(float v) { _r *= v; _g *= v; _b *= v; }
    void		  operator/=(float v) { _r /= v; _g /= v; _b /= v; }
*/

/*
    uint8_t 	r8() const { return ggo::to<uint8_t>(0xFF*_r); }
    uint8_t 	g8() const { return ggo::to<uint8_t>(0xFF*_g); }
    uint8_t 	b8() const { return ggo::to<uint8_t>(0xFF*_b); }

    uint16_t 	r16() const { return ggo::to<uint16_t>(0xFFFF*_r); }
    uint16_t 	g16() const { return ggo::to<uint16_t>(0xFFFF*_g); }
    uint16_t 	b16() const { return ggo::to<uint16_t>(0xFFFF*_b); }

    uint32_t 	r32() const { return ggo::to<uint32_t>(0xFFFFFFFF*_r); }
    uint32_t 	g32() const { return ggo::to<uint32_t>(0xFFFFFFFF*_g); }
    uint32_t 	b32() const { return ggo::to<uint32_t>(0xFFFFFFFF*_b); }
*/
    //color_8u  color_8u() const { return{ r8(), g8(), b8() }; }

   // void 		  randomize();

    /*float		  max() const { return ggo::max(_r, _g, _b); }
    float		  min() const { return ggo::min(_r, _g, _b); }
    float		  mean() const { return (_r + _g + _b) / 3; }
    */
    bool      is_black() const { return _r == 0 && _g == 0 && _b == 0; }

    color &   operator=(const color & c) = default;
    bool      operator==(const color & c) const { return _r == c._r && _g == c._g && _b == c._b; }
    bool      operator!=(const color & c) const { return _r != c._r || _g != c._g || _b != c._b; }

  public:

    static data_t   max() { return std::is_floating_point<data_t>() ? 1 : std::numeric_limits<data_t>::max(); }

    static  color	  from_hsv(float hue, float saturation, float value);
    static  color	  get_random();

  public:

    static const color WHITE;
    static const color GRAY;
    static const color BLACK;
    static const color RED;
    static const color GREEN;
    static const color BLUE;
    static const color CYAN;
    static const color MAGENTA;
    static const color YELLOW;
    static const color ORANGE;
  };
}

//namespace ggo
//{
//  template <>
//  inline ggo::color to<ggo::color, float>(float v) { return ggo::color(v); }
//}

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
  /*
  inline ggo::color operator-(const ggo::color & c)
  {
  return ggo::color(-c._r, -c._g, -c._b);
  }*/

  template <typename data_t>
  ggo::color<data_t> operator+(const ggo::color<data_t> & c1, const ggo::color<data_t> & c2)
  {
  return ggo::color<data_t>(c1._r + c2._r, c1._g + c2._g, c1._b + c2._b);
  }

  /*
  inline ggo::color operator-(const ggo::color & c1, const ggo::color & c2)
  {
  return ggo::color(c1._r - c2._r, c1._g - c2._g, c1._b - c2._b);
  }

  inline ggo::color operator*(const ggo::color & c1, const ggo::color & c2)
  {
  return ggo::color(c1._r * c2._r, c1._g * c2._g, c1._b * c2._b);
  }*/

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

  /*inline ggo::color operator/(const ggo::color & c, float k)
  {
  return ggo::color(c._r / k, c._g / k, c._b / k);
  }*/
}





/////////////////////////////////////////////////////////////////////
// Static variables.
namespace ggo
{
  template <typename data_t> const color<data_t> color<data_t>::WHITE(color<data_t>::max(), color<data_t>::max(), color<data_t>::max());
  template <typename data_t> const color<data_t> color<data_t>::GRAY(color<data_t>::max() / 2, color<data_t>::max() / 2, color<data_t>::max() / 2);
  template <typename data_t> const color<data_t> color<data_t>::BLACK(0, 0, 0);
  template <typename data_t> const color<data_t> color<data_t>::RED(color<data_t>::max(), 0, 0);
  template <typename data_t> const color<data_t> color<data_t>::GREEN(0, color<data_t>::max(), 0);
  template <typename data_t> const color<data_t> color<data_t>::BLUE(0, 0, color<data_t>::max());
  template <typename data_t> const color<data_t> color<data_t>::CYAN(0, color<data_t>::max(), color<data_t>::max());
  template <typename data_t> const color<data_t> color<data_t>::MAGENTA(color<data_t>::max(), 0, color<data_t>::max());
  template <typename data_t> const color<data_t> color<data_t>::YELLOW(color<data_t>::max(), color<data_t>::max(), 0);
  template <typename data_t> const color<data_t> color<data_t>::ORANGE(color<data_t>::max(), color<data_t>::max() / 2, 0);
}

/////////////////////////////////////////////////////////////////////
// Implementation
namespace ggo
{
  /////////////////////////////////////////////////////////////////////
  template <typename data_t>
  color<data_t> color<data_t>::from_hsv(float hue, float saturation, float value)
  {
    float r, g, b;
    ggo::hsv2rgb(hue, saturation, value, r, g, b);

    if (std::is_integral<data_t>::value)
    {
      return color(r, g, b);
    }
    else
    {
      return color(
        ggo::to<data_t>(r * std::numeric_limits<data_t>::max()),
        ggo::to<data_t>(g * std::numeric_limits<data_t>::max()),
        ggo::to<data_t>(b * std::numeric_limits<data_t>::max()));
    }
  }

  /////////////////////////////////////////////////////////////////////
  template <typename data_t>
  color<data_t> color<data_t>::get_random()
  {
    return color(
      ggo::rand<data_t>(0, ggo::color<data_t>::max()),
      ggo::rand<data_t>(0, ggo::color<data_t>::max()),
      ggo::rand<data_t>(0, ggo::color<data_t>::max()));
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

  template <> inline ggo::color_8u convert_color_to<ggo::color_8u, ggo::color_32f>(const ggo::color_32f & c)
  {
    return ggo::color_8u(ggo::to<uint8_t>(255.f * c._r), ggo::to<uint8_t>(255.f * c._g), ggo::to<uint8_t>(255.f * c._b));
  }

  template <> inline ggo::color_8u convert_color_to<ggo::color_8u, ggo::color_8u>(const ggo::color_8u & c)
  {
    return c;
  }

  template <> inline ggo::color_8u convert_color_to<ggo::color_8u, uint8_t>(const uint8_t & c)
  {
    return ggo::color_8u(c, c, c);
  }

  template <> inline ggo::color_8u convert_color_to<ggo::color_8u, float>(const float & c)
  {
    uint8_t gray = static_cast<uint8_t>(255.f * ggo::clamp(c, 0.f, 1.f) + 0.5f);

    return ggo::color_8u(gray, gray, gray);
  }
}

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

#endif
